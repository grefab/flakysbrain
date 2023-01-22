#include "brain/brain.h"
#include <spdlog/spdlog.h>

void brain::run(bool with_maintenance) {
  if (with_maintenance) {
    add_event(
      std::make_shared<periodic_event>(100, 100, [](brain* b, timestamp now) {
        b->maintenance(now);
        if (b->events_in_queue() == 0) {
          // no need to maintenance if there are no more other events.
          // this happens when closing the brain
          spdlog::info("empty brain. stopping maintenance");
          return false;
        }
        return true;
      }));
  }

  while (!events_.empty()) {
    auto e = events_.top();
    events_.pop();

    timestamp now = e->when_;
    last_executed_event_ts_ = now;
    e->action(this, now);

    perf_();
  }
}

void brain::kill() {
  // Clear event queue soon.
  add_maintenance_action([](brain* b, timestamp now) {
    spdlog::info("deleting brain's {} events", b->events_in_queue());
    while (!b->events_.empty()) {
      b->events_.pop();
    }
  });
}

void brain::add_event(event_ptr e) {
  events_.push(e);
}

void brain::add_maintenance_action(
  std::function<void(brain* b, timestamp now)> maintenance_action) {
  std::lock_guard<std::recursive_mutex> lock(maintenance_actions_mutex_);
  maintenance_actions_.emplace_back(std::move(maintenance_action));
}

/// Make sure that timestamps do not get out of hand, i.e. reach numeric limits.
/// Therefore once in a while every timestamp is reduced by the current time,
/// effectively becoming zero. Events have to be adapted accordingly.
void brain::maintenance(timestamp now) {
  // Work through other maintenance actions
  {
    std::lock_guard<std::recursive_mutex> lock(maintenance_actions_mutex_);
    for (auto& f : maintenance_actions_) {
      f(this, now);
    }
    maintenance_actions_.clear();
  }

  // Reset timestamps of all our neurons.
  for (auto& neuron : brain_mass_->neurons_) {
    neuron->last_pulse_received_timestamp_ -= now;
    neuron->last_fired_timestamp_ -= now;
  }

  // Make a new event queue with events' due dates shifted accordingly.
  std::priority_queue<event_ptr, std::deque<event_ptr>, decltype(compare)>
    new_events{compare};
  while (!events_.empty()) {
    auto new_event = events_.top();
    new_event->when_ -= now;
    new_events.push(new_event);
    events_.pop();
  }
  events_ = new_events;
  last_executed_event_ts_ = 0;
}
