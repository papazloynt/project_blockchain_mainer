// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

#include <grpcpp/grpcpp.h>

/*#include <google/protobuf/repeated_field.h>*/
#include <blockchain.grpc.pb.h>
#include <shared_mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include<thread>
#include <sqlite3.h>


struct Transac {
  std::string c_from;
  std::string c_to ;
  uint32_t sum;
  Transac() : c_from(), c_to(), sum(0) {}
  explicit Transac(const blockchain::TransactionRequest* request) :
                            c_from(request->req().client_from()),
                            c_to(request->req().client_to()),
                            sum(stoi(request->req().client_from())) {}
};

class Mainer : public blockchain::Blockchain::Service {
 public:
  Mainer();
 private:
   grpc::Status Transaction(grpc::ServerContext* context,
                      const blockchain::TransactionRequest* request,
                      blockchain::TransactionResponse* response) override {
    Transac transac_(request);

  }

};

#endif  // INCLUDE_SUGGESTIONS_HPP_
