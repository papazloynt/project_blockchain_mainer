// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com
#include "suggestions.hpp"

grpc::Status Suggest_Service_Answer::Answer(grpc::ServerContext* context,
                                  const suggest::SuggestRequest* request,
                                  suggest::SuggestResponse* response) {
  google::protobuf::RepeatedPtrField<suggest::SuggestAnswer> answ;
  std::shared_lock<std::shared_mutex> lock(mutex_);

  for (const auto& el : arr_val) {
    if (request->input() == el.at("id").get<std::string>())
      answ.Add(el.get<suggest::SuggestAnswer>());
  }
  lock.unlock();
  std::sort(answ.begin(), answ.end(), compare);
  for (int i = 0; i < answ.size(); i++){
    answ[i].set_position(i+1);
  }
  *response->mutable_sugg() = answ;
  return grpc::Status::OK;
}

[[noreturn]] void Suggest_Service_Answer::funnc_for_listening(){
  std::ifstream file;
  while (true){
    file.open("/home/chastikov/go/src/github.com/iu8-31-cpp-2020/lab07-papazloynt/sources/suggestion.json"); //Полный путь прописать
    std::unique_lock<std::shared_mutex> lock(mutex_);
    arr_val = nlohmann::json::parse(file);
    lock.unlock();
    file.close();
    std::cout << "sleeping for 15 min" << std::endl;
    std::this_thread::sleep_for(std::chrono::minutes(15));
  }
}

Suggest_Service_Answer::Suggest_Service_Answer() {
  std::thread listen(&Suggest_Service_Answer::funnc_for_listening, this);
  listen.detach();
}
