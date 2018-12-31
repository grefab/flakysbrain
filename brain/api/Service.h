#pragma once

#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include "grefab/flakysbrain/brain/api/api.grpc.pb.h"

class Service final : public brain::Api::Service {
public:
    Service();

public:
    grpc::Status get_snapshot(grpc::ServerContext* context, const brain::SnapshotRequest* request, brain::Snapshot* response) override;
};
