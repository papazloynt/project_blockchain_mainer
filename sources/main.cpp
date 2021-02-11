// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <iostream>
#include <memory>
#include <string>

#include "Mainer.hpp"

using grpc::Server;
using grpc::ServerBuilder;


void RunServer() {
  std::string server_address("0.0.0.0:9090");
  Mainer service;

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
  RunServer();
  return 0;
}

// Посмотреть не нарушается ли многопоточка
// !!!! Забью на базу для клиента, поэтому: всё через майнера
// Регистрация и вход для клиента - без майнера система не работает!!!! и не подлежит восстановлению
// Транзакция проверки
// Добавление денег
// Вывод информации о счёте
// Клиент
// СI
// Документация
// А если нету того, кто получает деньги?? или сам чувак...
