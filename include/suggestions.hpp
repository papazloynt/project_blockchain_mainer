// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

#include <grpcpp/grpcpp.h>

#include <google/protobuf/repeated_field.h>
#include <blockchain.grpc.pb.h>
#include <shared_mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include<thread>


class Suggest_Service_Answer : public blockchain::Blockchain::Service {
 private:
  std::shared_mutex mutex_;
  nlohmann::json arr_val;

  grpc::Status Answer(grpc::ServerContext* context,
                      const suggest::SuggestRequest* request,
                      suggest::SuggestResponse* response) override;

  [[noreturn]] void funnc_for_listening();

 public:
  Suggest_Service_Answer();
};

#endif  // INCLUDE_SUGGESTIONS_HPP_
