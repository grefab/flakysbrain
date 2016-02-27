#include <iostream>
#include "brain.h"

int main() {
    std::cout << "Running brain." << std::endl;
    {
        brain b;

        auto n1 = b.add_neuron(neuron_ptr(new neuron(1, 0.5)));
        auto n2 = b.add_neuron(neuron_ptr(new neuron(1, 0.5)));
        b.add_connection(n1, connection_ptr(new connection(n2, 1, 1)));
        b.add_connection(n2, connection_ptr(new connection(n1, 1, 1)));
        n1->apply_pulse(1, 0, &b);

        b.run();
    }
    std::cout << "Brain is dead." << std::endl;

    return 0;
}
