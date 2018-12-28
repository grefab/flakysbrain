#include <iostream>
#include "grefab/flakysbrain/brain/brain_runner.h"
#include "grefab/flakysbrain/gui/gui.h"

int main() {
    std::cout << "Running brain." << std::endl;
    {
        brain_runner runner;
        runner.run();
        {
            gui g(&runner.brain_);
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }
    std::cout << "Brain is dead." << std::endl;

    return 0;
}
