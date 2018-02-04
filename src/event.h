#pragma  once

#include "types.h"
#include "neuron.h"

struct event {

    explicit event(timestamp when);

    virtual void action(brain* b, timestamp now) = 0;

    timestamp when_;

};

using event_ptr = std::shared_ptr<event>;

struct neuronal_event : public event {
    neuronal_event(timestamp when, neuron_ptr target, pulse pulse);

    void action(brain* b, timestamp now) override;

private:
    neuron_ptr target_;
    pulse pulse_;

};

struct maintenance_event : public event {
    explicit maintenance_event(timestamp when);

    void action(brain* b, timestamp now) override;

};

struct periodic_event : public event {
     periodic_event(timestamp when, duration period, std::function<void(brain* b, timestamp now)> f);

    void action(brain* b, timestamp now) override;

private:
    duration period_;
    std::function<void(brain* b, timestamp now)> f_;

};
