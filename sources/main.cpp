// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

// c++ headers
#include <iostream>
#include <memory>
#include <string>

// private headers
#include "Mainer.hpp"

// JSON
#include <nlohmann/json.hpp>

using grpc::Server;
using grpc::ServerBuilder;


void RunServer(const std::string& name, const unsigned time) {
  std::string server_address("0.0.0.0:9090");
  Mainer service(name, time);

  std::cout << "Your password: " << service.GetPassword() << std::endl
            << "Please do not lose it, otherwise "
               "you will lose access to the wallet."
            << std::endl;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  std::string path;
  if (argc >= 2) {
    path = argv[1];
  } else {
    throw std::invalid_argument{"Path is not avaible"};
  }

  std::ifstream file{path};
  nlohmann::json data;
  file >> data;
  file.close();

  RunServer(data.at("name").get<std::string>(),
      data.at("time").get<uint32_t>());
  return 0;
}


