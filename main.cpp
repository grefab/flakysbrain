#include <iostream>
#include "grefab/flakysbrain/brain/brain_runner.h"

int main() {
    std::cout << "Running brain." << std::endl;
    {
        brain_runner runner;
        runner.run();
    }
    std::cout << "Brain is dead." << std::endl;

    return 0;
}
