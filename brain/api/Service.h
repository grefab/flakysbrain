#pragma once

#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include "grefab/flakysbrain/brain/api/api.grpc.pb.h"

class Service final : public brain_api::Api::Service {
public:
    Service();

public:
    grpc::Status get_snapshot(grpc::ServerContext* context, const brain_api::SnapshotRequest* request, brain_api::Snapshot* response) override;
};
