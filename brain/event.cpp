#include "brain/event.h"

#include "brain/brain.h"

void neuronal_event::action(brain* b, timestamp now) {
    target_->apply_pulse(pulse_, now, b);
}

//

periodic_event::periodic_event(timestamp when, duration period, std::function<bool(brain* b, timestamp now)> f)
    : event(when), period_(period), f_(std::move(f)) {}

void periodic_event::action(brain* b, timestamp now) {
    bool repeat = f_(b, now);
    if (repeat) {
        b->add_event(std::make_shared<periodic_event>(b->last_executed_event_ts() + period_, period_, f_));
    }
}
