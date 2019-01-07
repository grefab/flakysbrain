#include "grefab/flakysbrain/brain/api/Service.h"
#include <future>

grpc::Status Service::get_snapshot(grpc::ServerContext* context, const brain_api::SnapshotRequest* request, brain_api::Snapshot* response) {
    // Request data
    std::promise<timestamp> promise;
    auto request_display_data = [&promise](brain* b, timestamp now) {
        promise.set_value(now);

        // Slow down brain
        //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    brain_->add_maintenance_action(request_display_data);

    {
        auto now = promise.get_future().get();
        //        std::lock_guard<std::mutex> lock(display_data_.mutex_);
        //        display_data_.monotonic_now_ += now;
    }

    return grpc::Status::OK;
}
