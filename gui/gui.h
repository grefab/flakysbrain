//
// Created by grefab on 28.12.18.
//

#pragma once

#include <atomic>
#include <thread>
#include "brain/api/proto/api.grpc.pb.h"
#include "brain/brain.h"
#include "common/threadsafe/threadsafe.h"

class gui {
  public:
  explicit gui(brain* b);
  virtual ~gui();

  void connect_to(std::string const& address);
  void run();

  isotronic::threadsafe<brain_api::Snapshot> display_data_;

  private:
  brain* brain_;

  std::thread collect_thread_;
  std::atomic_bool close_thread_{false};
};
