#pragma once

#include <chrono>
#include <cstdint>

class performance_measure {
public:
    void operator()();

private:
    uint64_t counter_ = 0;
    uint64_t batch_size_ = 1;
    std::chrono::time_point<std::chrono::steady_clock> last_real_time_ = std::chrono::steady_clock::now();
};
