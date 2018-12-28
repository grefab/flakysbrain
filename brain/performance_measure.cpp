#include "performance_measure.h"
#include <iostream>

void performance_measure::operator()() {
    if (++counter_ >= batch_size_) {
        // Time calculations
        auto current_real_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_real_time - last_real_time_).count();
        auto events_per_second = duration > 0 ? counter_ * 1000 / duration : 0;

        // Log output
        std::cout << "executed " << counter_ << " events in " << duration << "ms, " << events_per_second << "e/s" << std::endl;

        // Make sure we do not log too frequently.
        if (duration < 1000) {
            batch_size_ *= 2;
        } else if (duration > 5000) {
            batch_size_ /= 2;
        }

        counter_ = 0;
        last_real_time_ = current_real_time;
    }
}
