//
// Created by Gregor on 27.02.2016.
//

#ifndef FLAKYSBRAIN_NEURON_H
#define FLAKYSBRAIN_NEURON_H

#include <c++/memory>
#include <c++/set>
#include <c++/limits>
#include <c++/functional>
#include "types.h"

struct brain;
struct connection;
using connection_ptr = std::shared_ptr<connection>;

struct neuron {
    // Constructor
    neuron(pulse power,
           double bias,
           std::set<connection_ptr> connections = std::set<connection_ptr>(),
           double potential = 0,
           timestamp last_pulse_received_timestamp = std::numeric_limits<timestamp>::max(),
           timestamp last_fired_timestamp = std::numeric_limits<timestamp>::max()) :
            power_(power),
            bias_(bias),
            connections_(connections),
            potential_(potential),
            last_pulse_received_timestamp_(last_pulse_received_timestamp),
            last_fired_timestamp_(last_fired_timestamp) { }

    // Is called by brain when an event reaches this neuron.
    void apply_pulse(pulse p, timestamp now, brain* b);

    /* The following parameters shall be modified during learning. */

    // Amount of firing power in [-1, 1]
    pulse power_;

    // If potential exceeds bias, we fire. In (0, inf)
    double bias_;

    // Where and when to fire
    std::set<connection_ptr> connections_;

private:
    // Fires a pulse along the connections.
    void fire(timestamp now, brain* b);

    // Modifies potential depending on time passed since last pulse receive event.
    // Potential shall decay over time until it reaches 0.
    double decay(duration time_passed, double last_potential) const;

    // Modifies pulse depending on time passed since last time fired.
    // Pulse shall be weakened if last event has happened recently.
    double cool_down(duration time_passed, pulse p) const;

private:
    // Internal state
    double potential_;
    timestamp last_pulse_received_timestamp_;
    timestamp last_fired_timestamp_;

};

using neuron_ptr = std::shared_ptr<neuron>;

struct connection {
    // Constructor
    connection(neuron_ptr const& target, duration distance, double weight) :
            target_(target), distance_(distance), weight_(weight) { }

    // Where this event will be applied to.
    neuron_ptr target_;

    // The timely distance to the target, shall be positive.
    duration distance_;

    // Is multiplied to a outgoing pulse, shall be in [0, 1].
    double weight_;

};

#endif //FLAKYSBRAIN_NEURON_H
