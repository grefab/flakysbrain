#include <iostream>
#include "brain.h"

int main() {
    std::cout << "Running brain." << std::endl;
    {
        brain b;

        // Sensors
        auto eye = b.add_neuron(std::make_shared<neuron>(1, 0.5));

        // Grey matter
        auto n = b.add_neuron(std::make_shared<neuron>(1, 0.5));
        b.add_connection(eye, std::make_shared<connection>(n, 1, 1));
        for (int i = 0; i < 100; ++i) {
            auto n2 = b.add_neuron(std::make_shared<neuron>(1, 0.5));
            b.add_connection(n, std::make_shared<connection>(n2, 1, 1));
            n = n2;
        }

        // Actuators
        auto motor = b.add_neuron(std::make_shared<neuron>(1, 0.5));
        b.add_connection(n, std::make_shared<connection>(motor, 1, 1));

        b.add_event(std::make_shared<periodic_event>(0, 100, [&eye](brain* b, timestamp now) {
            eye->apply_pulse(1, now, b);
        }));

        b.run();
    }
    std::cout << "Brain is dead." << std::endl;

    return 0;
}
