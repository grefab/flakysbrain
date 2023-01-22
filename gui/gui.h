//
// Created by grefab on 28.12.18.
//

#pragma once

#include <atomic>
#include <thread>

#include "brain/api/api.grpc.pb.h"
#include "brain/brain.h"

class gui {
public:
    explicit gui(brain* b);
    virtual ~gui();

    void connect_to(std::string const& address);
    void wait();

    struct DisplayData {
        timestamp monotonic_now_ = 0;

        std::mutex mutex_;
    } display_data_;

private:
    brain* brain_;

    std::thread gui_thread_;
    std::thread collect_thread_;
    std::atomic_bool close_thread_{false};
};
