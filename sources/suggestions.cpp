// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com
#include "suggestions.hpp"


//В неймспейсе suggest для неявного присваивания методом get
namespace suggest {
void from_json(const nlohmann::json& j,
               suggest::Suggest_answer& s) {
  s.set_text(j.at("name").get<std::string>());
  s.set_position(j.at("cost").get<uint32_t>());
}
}

bool compare(const suggest::Suggest_answer& a,
             const suggest::Suggest_answer& b ){
  return a.position() < b.position();
}


grpc::Status Suggest_Service_Answer::Answer(grpc::ServerContext* context,
                                            const suggest::SuggestRequest* request,
                                            suggest::SuggestResponse* response) {

  google::protobuf::RepeatedPtrField<suggest::Suggest_answer> answ;
  std::shared_lock<std::shared_mutex> lock(mutex_);

  for(const auto& el : arr_val) {
    if (request->input() == el.at("id").get<std::string>())
      answ.Add(el.get<suggest::Suggest_answer>());

  }
  lock.unlock();
  std::sort(answ.begin(), answ.end(), compare);
  for(int i = 0; i < answ.size(); i++){
    answ[i].set_position(i+1);
  }
  *response->mutable_sugg() = answ;
  return grpc::Status::OK;
}


void Suggest_Service_Answer::funnc_for_listening(){
  std::ifstream file;
  while (true){
    file.open("/home/chastikov/go/src/github.com/iu8-31-cpp-2020/lab07-papazloynt/sources/suggestion.json");
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
