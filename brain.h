//
// Created by Gregor on 27.02.2016.
//

#ifndef FLAKYSBRAIN_BRAIN_H
#define FLAKYSBRAIN_BRAIN_H

#include <c++/unordered_set>
#include <c++/functional>
#include <c++/queue>
#include <c++/deque>
#include "neuron.h"
#include "event.h"
#include "performance_measure.h"

class brain {
public:
    // Runs until no events are in the queue anymore.
    void run();

    // Is called by neuron. Adds an event to the event queue.
    void add_event(event&& e);

    // Fiddle with neurons.
    neuron_ptr add_neuron(neuron_ptr n);
    void remove_neuron(neuron_ptr n);

    void add_connection(neuron_ptr n, connection_ptr c);
    void remove_connection(neuron_ptr n, connection_ptr c);

private:
    // Contains all neurons in this brain. Mainly used for bookkeeping.
    std::unordered_set<neuron_ptr> neurons_;

    // A priority queue that places small timestamps first. Therefore we can easily
    // access the next event that shall happen.
    std::function<bool(event const&, event const&)> compare = [](event const& a, event const& b) {
        return a.when_ > b.when_;
    };
    std::priority_queue<event, std::deque<event>, decltype(compare)> events_{compare};

    performance_measure perf_;
};

#endif //FLAKYSBRAIN_BRAIN_H
