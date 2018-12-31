#include "Service.h"

Service::Service() {}

grpc::Status Service::get_snapshot(grpc::ServerContext* context, const brain::SnapshotRequest* request, brain::Snapshot* response) {
    return grpc::Status::OK;
}
