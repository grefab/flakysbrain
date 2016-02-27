//
// Created by Gregor on 27.02.2016.
//

#ifndef FLAKYSBRAIN_EVENT_H
#define FLAKYSBRAIN_EVENT_H

#include "types.h"
#include "neuron.h"

struct event {
    neuron_ptr target_;
    timestamp when_;
    pulse pulse_;

};

#endif //FLAKYSBRAIN_EVENT_H
