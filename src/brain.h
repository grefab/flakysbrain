#pragma  once

#include <unordered_set>
#include <functional>
#include <queue>
#include <deque>
#include "neuron.h"
#include "event.h"
#include "performance_measure.h"

class brain {
    friend struct maintenance_event;

public:
    // Runs until no events are in the queue anymore.
    void run();

    // Is called by neuron. Adds an event to the event queue.
    void add_event(event::ptr e);

    // Fiddle with neurons.
    neuron_ptr add_neuron(neuron_ptr n);
    void remove_neuron(neuron_ptr n);

    void add_connection(neuron_ptr n, connection_ptr c);
    void remove_connection(neuron_ptr n, connection_ptr c);

    // Monitoring methods
    std::unordered_set<neuron_ptr> const& neurons() { return neurons_; }
    size_t events_in_queue() const { return events_.size(); }

private:
    // "Normalizies" brain, i.e. makes sure that there origin of time for all thing concerned with time
    // (neurons, events) is now.
    void maintenance(timestamp now);

    // Contains all neurons in this brain. Mainly used for bookkeeping.
    std::unordered_set<neuron_ptr> neurons_;

    // A priority queue that places small timestamps first. Therefore we can easily
    // access the next event that shall happen.
    std::function<bool(event::ptr const&, event::ptr const&)> compare = [](event::ptr const& a, event::ptr const& b) {
        return a->when_ > b->when_;
    };
    std::priority_queue<event::ptr, std::deque<event::ptr>, decltype(compare)> events_{compare};

    performance_measure perf_;

};
