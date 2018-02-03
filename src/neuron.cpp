//
// Created by Gregor on 27.02.2016.
//

#include "neuron.h"
#include "brain.h"

void neuron::apply_pulse(pulse p, timestamp now, brain* b) {
    // Modify our action potential in respect of time passed since last events.
    potential_ = decay(now - last_pulse_received_timestamp_, potential_);

    // Apply what's left of our pulse after a potential cool down phase after firing.
    potential_ += cool_down(now - last_fired_timestamp_, p);
    if (potential_ > bias_) {
        fire(now, b);
        potential_ = 0;
    }

    last_pulse_received_timestamp_ = now;
}

void neuron::fire(timestamp now, brain* b) {
    // Add events in the future for all our connections.
    for (auto const& c : connections_) {
        auto pulse = power_ * c->weight_;
        auto target = c->target_;

        event e;
        e.when_ = now + c->distance_;
        e.action = [target, pulse, b](timestamp now) {
            target->apply_pulse(pulse, now, b);
        };

        b->add_event(std::move(e));
    }

    last_fired_timestamp_ = now;
}

double neuron::decay(duration time_passed, double last_potential) const {
    // TODO(grefab): implement
    return last_potential;
}

double neuron::cool_down(duration time_passed, pulse p) const {
    // TODO(grefab): implement
    return p;
}
