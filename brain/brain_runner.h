//
// Created by grefab on 28.12.18.
//

#pragma once

#include <atomic>
#include <thread>
#include "grefab/flakysbrain/brain/brain.h"

class brain_runner {
public:
    brain_runner();
    virtual ~brain_runner();

    void run();

    brain brain_;
    std::thread thread_;
    std::atomic_bool close_thread_{false};
};