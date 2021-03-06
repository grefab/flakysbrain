//
// Created by Gregor on 27.02.2016.
//

#include "grefab/flakysbrain/brain/neuron.h"
#include "grefab/flakysbrain/brain/brain.h"

void neuron::apply_pulse(pulse p, timestamp now, brain* b) {
    // Modify our action potential in respect of time passed since last events.
    potential_ = decayed(now - last_pulse_received_timestamp_, potential_);

    // Apply what's left of our pulse after a potential cool down phase after firing.
    potential_ += cooled_down(now - last_fired_timestamp_, p);
    if (potential_ > bias_) {
        fire(now, b);
        potential_ = 0;
    }

    last_pulse_received_timestamp_ = now;
}

void neuron::fire(timestamp now, brain* b) {
    on_fire_(now, power_);

    // Add events in the future for all our connections.
    for (auto const& c : connections_) {
        b->add_event(std::make_shared<neuronal_event>(now + c->distance_, c->target_, power_ * c->weight_));
    }

    last_fired_timestamp_ = now;
}

double neuron::decayed(duration time_passed, double last_potential) const {
    // TODO(grefab): implement
    return last_potential;
}

double neuron::cooled_down(duration time_passed, pulse p) const {
    // TODO(grefab): implement
    return p;
}
