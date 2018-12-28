#pragma once

#include "grefab/flakysbrain/brain/neuron.h"
#include "grefab/flakysbrain/brain/types.h"

struct event {
    explicit event(timestamp when);
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

struct maintenance_event : public event {
    explicit maintenance_event(timestamp when) : event(when) {}
    ~maintenance_event() override = default;

    void action(brain* b, timestamp now) override;

    constexpr static double timeout_ = 10000;
};

struct periodic_event : public event {
    periodic_event(timestamp when, duration period, std::function<void(brain* b, timestamp now)> f);
    ~periodic_event() override = default;

    void action(brain* b, timestamp now) override;

private:
    duration period_;
    std::function<void(brain* b, timestamp now)> f_;
};
