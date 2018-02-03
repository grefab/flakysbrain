//
// Created by Gregor on 27.02.2016.
//

#include <iostream>
#include "brain.h"

void brain::run() {
    add_event({100, [this](timestamp now) {
        maintenance(now);
    }});

    while (!events_.empty()) {
        auto& e = events_.top();

        timestamp now = e.when_;
        e.action(now);
        events_.pop();

        perf_();
    }
}

void brain::add_event(event&& e) {
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

void brain::maintenance(timestamp now) {
    for (auto& neuron : neurons_) {
        neuron->last_pulse_received_timestamp_ -= now;
        neuron->last_fired_timestamp_ -= now;
    }

    std::priority_queue<event, std::deque<event>, decltype(compare)> new_events{compare};
    while (!events_.empty()) {
        auto new_event = events_.top();
        new_event.when_ -= now;
        new_events.push(new_event);
        events_.pop();
    }
    events_ = new_events;

    add_event({100, [this](timestamp now) {
        maintenance(now);
    }});
}
