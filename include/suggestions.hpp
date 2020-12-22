// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

#include <grpcpp/grpcpp.h>

#include <google/protobuf/repeated_field.h>
#include <suggest.grpc.pb.h>
#include <shared_mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include<thread>

//В неймспейсе suggest для неявного присваивания методом get
namespace suggest {
void from_json(const nlohmann::json &j, suggest::Suggest_answer& s);
}

bool compare(const suggest::Suggest_answer& a,
             const suggest::Suggest_answer& b);

class Suggest_Service_Answer : public suggest::Suggest::Service {
 private:
  std::shared_mutex mutex_;
  nlohmann::json arr_val;

  grpc::Status Answer(grpc::ServerContext* context,
                      const suggest::SuggestRequest* request,
                      suggest::SuggestResponse* response) override;

  void funnc_for_listening();

 public:
  Suggest_Service_Answer();
};

#endif  // INCLUDE_SUGGESTIONS_HPP_
