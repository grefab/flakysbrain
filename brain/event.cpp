#include "brain/event.h"

#include "brain/brain.h"

event::event(timestamp when) : when_(when) {}

void neuronal_event::action(brain* b, timestamp now) {
    target_->apply_pulse(pulse_, now, b);
}

periodic_event::periodic_event(timestamp when, duration period, std::function<void(brain* b, timestamp now)> f)
    : event(when), period_(period), f_(std::move(f)) {}

void periodic_event::action(brain* b, timestamp now) {
    f_(b, now);
    b->add_event(std::make_shared<periodic_event>(now + period_, period_, f_));
}
