//
// Created by grefab on 28.12.18.
//

#include "grefab/flakysbrain/brain/brain_runner.h"
#include <iostream>

void brain_runner::run() {
    std::cout << "Starting runner" << std::endl;
    thread_ = std::thread([this]() { brain_.run(); });
}

brain_runner::~brain_runner() {
    std::cout << "Closing runner" << std::endl;
    brain_.kill();
    close_thread_ = true;
    if (thread_.joinable()) {
        thread_.join();
    }
    std::cout << "Runner closed" << std::endl;
}

brain_runner::brain_runner() {
    // Sensors
    auto eye = brain_.add_neuron(std::make_shared<neuron>(1, 0.5));

    // Grey matter
    auto n = brain_.add_neuron(std::make_shared<neuron>(1, 0.5));
    brain_.add_connection(eye, std::make_shared<connection>(n, 1, 1));
    for (int i = 0; i < 10; ++i) {
        auto n2 = brain_.add_neuron(std::make_shared<neuron>(1, 0.5));
        brain_.add_connection(n, std::make_shared<connection>(n2, 1, 1));
        n = n2;
    }

    // Actuators
    auto motor = brain_.add_neuron(std::make_shared<neuron>(1, 0.5));
    brain_.add_connection(n, std::make_shared<connection>(motor, 1, 1));
    motor->on_fire_ = [](timestamp now, pulse power) {
        /// std::cout << now << "motor activated: " << power << std::endl;
    };

    // Initial event
    brain_.add_event(
        std::make_shared<periodic_event>(0, 100, [eye](brain* b, timestamp now) { eye->apply_pulse(1, now, b); }));
}
