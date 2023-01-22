#pragma once

#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include "brain/api/proto/api.grpc.pb.h"
#include "brain/brain.h"

class Service final : public brain_api::Api::Service {
  public:
  explicit Service(brain* brain) : brain_(brain) {}

  public:
  grpc::Status get_snapshot(grpc::ServerContext* context,
                            const brain_api::SnapshotRequest* request,
                            brain_api::Snapshot* response) override;

  private:
  brain* brain_;
};
