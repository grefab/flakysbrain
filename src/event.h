#pragma  once

#include "types.h"
#include "neuron.h"

struct event {
    timestamp when_;
    std::function<void(timestamp now)> action;

};
