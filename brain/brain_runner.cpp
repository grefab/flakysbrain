//
// Created by grefab on 28.12.18.
//

#include "grefab/flakysbrain/brain/brain_runner.h"
#include <thread>

void brain_runner::run() {
    std::thread t([this]() {
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

        brain_.add_event(
            std::make_shared<periodic_event>(0, 100, [&eye](brain* b, timestamp now) { eye->apply_pulse(1, now, b); }));

        brain_.run();
    });
    t.join();
}
