#include "brain/api/Service.h"
#include <future>

grpc::Status Service::getSnapshot(grpc::ServerContext* context,
                                  const google::protobuf::Empty* request,
                                  brain_api::Snapshot* response) {
  std::promise<brain_api::Snapshot> promise;
  auto request_display_data = [&promise](brain* b, timestamp now) {
    promise.set_value(b->brain_mass_->makeSnapshot());
  };
  brain_->add_maintenance_action(request_display_data);
  *response = promise.get_future().get();
  return grpc::Status::OK;
}
