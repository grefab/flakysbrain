#pragma once

#include "brain/neuron.h"
#include "brain/types.h"

struct event {
    explicit event(timestamp when) : when_(when){};
    virtual ~event() = default;
    virtual void action(brain* b, timestamp now) = 0;
    timestamp when_;
};

using event_ptr = std::shared_ptr<event>;

struct neuronal_event : public event {
    neuronal_event(timestamp when, neuron_ptr target, pulse pulse)
        : event(when), target_(std::move(target)), pulse_(pulse) {}
    ~neuronal_event() override = default;
    void action(brain* b, timestamp now) override;

private:
    neuron_ptr target_;
    pulse pulse_;
};

//

struct periodic_event : public event {
    // f returns if periodic event pushing should continue
    periodic_event(timestamp when, duration period, std::function<bool(brain* b, timestamp now)> f);
    ~periodic_event() override = default;
    void action(brain* b, timestamp now) override;

private:
    duration period_;
    std::function<bool(brain* b, timestamp now)> f_;
};
