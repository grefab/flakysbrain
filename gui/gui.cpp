//
// Created by grefab on 28.12.18.
//

#include "gui.h"
#include <iostream>

gui::gui(brain* b) : brain_(b) {
    std::cout << "Starting GUI" << std::endl;
    auto request_display_data = [this](brain* b, timestamp now) { abs_time_ += now; };

    thread_ = std::thread([this, request_display_data]() {
        while (!close_thread_) {
            brain_->add_maintenance_action(request_display_data);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

gui::~gui() {
    std::cout << "Closing GUI" << std::endl;
    close_thread_ = true;
    thread_.join();
    std::cout << "GUI Closed" << std::endl;
}
