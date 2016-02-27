//
// Created by Gregor on 27.02.2016.
//

#include <iostream>
#include "brain.h"

void brain::run() {
    while (events_.size() > 0) {
        auto e = events_.top();

        timestamp now = e.when_;
        e.target_->apply_pulse(e.pulse_, now, this);

        events_.pop();

        std::cout << "executed event at " << now << ". remaining size: " << events_.size() << std::endl;
    }
}

void brain::add_event(event const& e) {
    events_.push(e);
}

neuron_ptr brain::add_neuron(neuron_ptr n) {
    neurons_.insert(n);
    return n;
}

void brain::remove_neuron(neuron_ptr n) {
    neurons_.erase(n);
}

void brain::add_connection(neuron_ptr n, connection_ptr c) {
    n->connections_.insert(c);
}

void brain::remove_connection(neuron_ptr n, connection_ptr c) {
    n->connections_.erase(c);
}
