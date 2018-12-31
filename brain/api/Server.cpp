#include "grefab/flakysbrain/brain/api/Server.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server_builder.h>
#include <iostream>

void Server::run(Service* service) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << address_ << std::endl;
}

void Server::wait() {
    server_->Wait();
    std::cout << "Server closed" << address_ << std::endl;
}
