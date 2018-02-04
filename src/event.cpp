#include "event.h"
#include <utility>
#include "brain.h"
#include "types.h"

event::event(timestamp when) :
        when_(when) {
}

neuronal_event::neuronal_event(timestamp when, neuron_ptr target, pulse pulse) :
        event(when),
        target_(std::move(target)),
        pulse_(pulse) {

}

void neuronal_event::action(brain* b, timestamp now) {
    target_->apply_pulse(pulse_, now, b);
}

maintenance_event::maintenance_event(timestamp when) :
        event(when) {

}

void maintenance_event::action(brain* b, timestamp now) {
    b->maintenance(now);
}
