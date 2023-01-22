//
// Created by grefab on 28.12.18.
//

#pragma once

#include <thread>
#include "brain/api/Server.h"
#include "brain/brain.h"

class brain_runner {
  public:
  brain_runner();
  virtual ~brain_runner();

  void run();

  brain brain_;

  private:
  std::thread run_thread_;
  Service service_{&brain_};
  Server server_{"0.0.0.0"};
};
