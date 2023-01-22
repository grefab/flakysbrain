#include "brain/performance_measure.h"

#include <spdlog/spdlog.h>

void performance_measure::operator()() {
    if (++counter_ >= batch_size_) {
        // Time calculations
        auto current_real_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_real_time - last_real_time_).count();
        auto events_per_second = duration > 0 ? counter_ * 1000 / duration : 0;

        // Log output
        spdlog::info("executed {} events in {}ms, {}e/s", counter_, duration, events_per_second);

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
