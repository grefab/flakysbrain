#include <iostream>
#include "brain/runner/brain_runner.h"
#include "gui/gui.h"

int main() {
    std::cout << "Running brain." << std::endl;
    {
        brain_runner runner;
        runner.run();
        gui g(&runner.brain_);

        std::cout << "Waiting for user to close GUI..." << std::endl;
        g.wait();
    }
    std::cout << "Brain is dead." << std::endl;

    return 0;
}
