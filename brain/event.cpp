#include "event.h"
#include "brain.h"

event::event(timestamp when) : when_(when) {}

neuronal_event::neuronal_event(timestamp when, neuron_ptr target, pulse pulse)
    : event(when), target_(std::move(target)), pulse_(pulse) {}

void neuronal_event::action(brain* b, timestamp now) {
    target_->apply_pulse(pulse_, now, b);
}

maintenance_event::maintenance_event(timestamp when) : event(when) {}

void maintenance_event::action(brain* b, timestamp now) {
    b->maintenance(now);
}

periodic_event::periodic_event(timestamp when, duration period, std::function<void(brain* b, timestamp now)> f)
    : event(when), period_(period), f_(std::move(f)) {}

void periodic_event::action(brain* b, timestamp now) {
    f_(b, now);
    b->add_event(std::make_shared<periodic_event>(now + period_, period_, f_));
}
