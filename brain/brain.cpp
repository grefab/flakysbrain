#include "grefab/flakysbrain/brain/brain.h"

void brain::run(bool with_maintenance) {
    if (with_maintenance) {
        add_event(std::make_shared<maintenance_event>(maintenance_event::timeout_));
    }

    while (!events_.empty()) {
        auto e = events_.top();
        events_.pop();

        timestamp now = e->when_;
        e->action(this, now);

        perf_();
    }
}

void brain::kill() {
    // Clear event queue soon.
    add_maintenance_action([](brain* b, timestamp now) {
        while (!b->events_.empty()) {
            b->events_.pop();
        }
    });
}

void brain::add_event(event_ptr e) {
    events_.push(e);
}

neuron_ptr brain::add_neuron(neuron_ptr n) {
    neurons_.insert(n);
    return n;
}

void brain::remove_neuron(neuron_ptr n) {
    // Remove all connections from this neuron, so that if it is still part of
    // the event queue, it does not fire anywhere upon event execution.
    n->connections_.clear();

    neurons_.erase(n);

    // Remove all connections to the to-be-deleted neuron.
    for (auto& neuron : neurons_) {
        for (auto c_it = neuron->connections_.begin(); c_it != neuron->connections_.end();) {
            if ((*c_it)->target_ == n) {
                neuron->connections_.erase(c_it++);
            } else {
                ++c_it;
            }
        }
    }
}

void brain::add_connection(neuron_ptr n, connection_ptr c) {
    n->connections_.insert(c);
}

void brain::remove_connection(neuron_ptr n, connection_ptr c) {
    n->connections_.erase(c);
}

void brain::add_maintenance_action(std::function<void(brain* b, timestamp now)> maintenance_action) {
    std::lock_guard<std::recursive_mutex> lock(m_);
    maintenance_actions_.emplace_back(maintenance_action);
}

/// Make sure that timestamps do not get out of hand, i.e. reach numeric limits. Therefore once in a while every
/// timestamp is reduced by the current time, effectively becoming zero. Events have to be adapted accordingly.
void brain::maintenance(timestamp now) {
    // Work through other maintenance actions
    {
        std::lock_guard<std::recursive_mutex> lock(m_);
        for (auto& f : maintenance_actions_) {
            f(this, now);
        }
        maintenance_actions_.clear();
    }

    // Reset timestamps of all our neurons.
    for (auto& neuron : neurons_) {
        neuron->last_pulse_received_timestamp_ -= now;
        neuron->last_fired_timestamp_ -= now;
    }

    // Make a new event queue with events' due dates shifted accordingly.
    std::priority_queue<event_ptr, std::deque<event_ptr>, decltype(compare)> new_events{compare};
    while (!events_.empty()) {
        auto new_event = events_.top();
        new_event->when_ -= now;
        new_events.push(new_event);
        events_.pop();
    }
    events_ = new_events;

    // Repeat this soon if there are other things to do.
    if (!events_.empty()) {
        add_event(std::make_shared<maintenance_event>(maintenance_event::timeout_));
    }
}
