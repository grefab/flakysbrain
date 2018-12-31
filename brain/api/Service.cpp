#include "grefab/flakysbrain/brain/api/Service.h"

Service::Service() {}

grpc::Status Service::get_snapshot(grpc::ServerContext* context, const brain_api::SnapshotRequest* request, brain_api::Snapshot* response) {
    return grpc::Status::OK;
}
