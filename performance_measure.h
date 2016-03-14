//
// Created by Gregor on 27.02.2016.
//

#ifndef FLAKYSBRAIN_PERFORMANCE_MEASURE_H
#define FLAKYSBRAIN_PERFORMANCE_MEASURE_H

#include <c++/cstdint>
#include <c++/chrono>

class performance_measure {
public:
    void operator()();

private:
    uint64_t counter_ = 0;
    uint64_t batch_size_ = 1;
    std::chrono::time_point<std::chrono::_V2::steady_clock> last_real_time_ = std::chrono::_V2::steady_clock::now();

};

#endif //FLAKYSBRAIN_PERFORMANCE_MEASURE_H
