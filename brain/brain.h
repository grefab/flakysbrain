#pragma once

#include <deque>
#include <functional>
#include <mutex>
#include <queue>
#include <unordered_set>
#include "brain/event.h"
#include "brain/neuron.h"
#include "brain/performance_measure.h"

class brain {
  public:
  // Runs until no events are in the queue anymore.
  void run(bool with_maintenance = true);

  // Empties event queue, causing main loop to end.
  void kill();

  // Is called by neuron. Adds an event to the event queue.
  void add_event(event_ptr e);

  // Fiddle with neurons.
  neuron_ptr add_neuron(neuron_ptr n);
  void remove_neuron(neuron_ptr n);

  void add_connection(neuron_ptr n, connection_ptr c);
  void remove_connection(neuron_ptr n, connection_ptr c);

  // Monitoring methods
  std::unordered_set<neuron_ptr> const& neurons() { return neurons_; }
  [[nodiscard]] size_t events_in_queue() const { return events_.size(); }
  [[nodiscard]] timestamp last_executed_event_ts() const {
    return last_executed_event_ts_;
  }

  // Maintenance
  void add_maintenance_action(
    std::function<void(brain* b, timestamp now)> maintenance_action);

  private:
  // "Normalizies" brain, i.e. makes sure that there origin of time for all
  // thing concerned with time (neurons, events) is now.
  void maintenance(timestamp now);

  // Contains all neurons in this brain. Mainly used for bookkeeping.
  std::unordered_set<neuron_ptr> neurons_;

  // A priority queue that places small timestamps first. Therefore, we can
  // easily access the next event that shall happen.
  std::function<bool(event_ptr const&, event_ptr const&)> compare =
    [](event_ptr const& a, event_ptr const& b) { return a->when_ > b->when_; };
  std::priority_queue<event_ptr, std::deque<event_ptr>, decltype(compare)>
    events_{compare};
  timestamp last_executed_event_ts_ = 0;

  // Enqueued actions for interacting with the brain
  std::recursive_mutex m_;
  std::vector<std::function<void(brain* b, timestamp now)>> maintenance_actions_;

  performance_measure perf_;
};
