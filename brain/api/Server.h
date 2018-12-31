#pragma once

#include <grpc++/server.h>
#include <memory>
#include "Service.h"

class Server {
public:
    explicit Server(std::string address_) : address_(std::move(address_)) {}

public:
    void run(Service* service);
    void wait();

private:
    std::string address_;
    std::unique_ptr<grpc::Server> server_;
};
