#pragma  once

#include "types.h"
#include "neuron.h"

struct event {
    neuron_ptr target_;
    timestamp when_;
    pulse pulse_;

};
