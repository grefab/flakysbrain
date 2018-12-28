//
// Created by grefab on 28.12.18.
//

#pragma once

#include <atomic>
#include <thread>
#include "grefab/flakysbrain/brain/brain.h"

class gui {
public:
    explicit gui(brain* b);
    virtual ~gui();

private:
    brain* brain_;
    timestamp abs_time_ = 0;

    std::thread thread_;
    std::atomic_bool close_thread_{false};
};
