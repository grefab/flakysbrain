#include "brain/api/Server.h"

#include <grpc++/security/server_credentials.h>
#include <grpc++/server_builder.h>
#include <spdlog/spdlog.h>

#include <iostream>

void Server::run(Service* service) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    server_ = builder.BuildAndStart();
    spdlog::info("Server listening on {}", address_);
}

void Server::kill() {
    server_->Shutdown();
}

void Server::wait() {
    server_->Wait();
    spdlog::info("Server closed: {}", address_);
}
