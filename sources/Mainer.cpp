// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

// private files
#include "Mainer.hpp"


Mainer::Mainer(const std::string login, const unsigned time_) : name(login), time(time_),
                                                                password(), db(), b_c()  {
  //Создаём, если не существует
  if (db.CreateDataBase() == OK) {
    //Вставляем пользлвателя
    password =
        db.InsertPersonDataBase(name);

    //Проверка на хакерство
    std::thread check(&Mainer::HackerProtection, this);
    check.detach();
  } else {
    throw std::runtime_error("Please rebut system");
  }
}

grpc::Status Mainer::Transaction(grpc::ServerContext* context,
                         const blockchain::TransactionRequest* request,
                         blockchain::TransactionResponse* response) {
    Transac transac_(request->client_from(),
                    request->client_to(),
                    request->sum());
    if (db.ChecksTransac(transac_) == ERROR){
      return grpc::Status::CANCELLED;
    }
    // Транзакция Клиент-клиенту
    Status s = db.TransactionDataBase(transac_);
    if (s == OK) {
      b_c.add_block(transac_);
    } else {
      *response->mutable_answer() = "Data Base was messed up";
      return grpc::Status::CANCELLED;
    }

  // Вознаграждение за созданный блок
  return RewardMainer();
}

grpc::Status Mainer::Registration(grpc::ServerContext* context,
                          const blockchain::RegistrationRequest* request,
                          blockchain::RegistrationResponse* response) {
  std::string login = request->name();
  std::string pass =
      db.InsertPersonDataBase(login);
  *response->mutable_password() = pass;
  if (pass == "NAME_ERROR"){
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status Mainer::Authorization(grpc::ServerContext* context,
                           const blockchain::AuthorizationRequest* request,
                           blockchain::AuthorizationResponse* response) {
  std::string login = request->name();
  std::string pass = request->password();
  *response->mutable_answer() =
      db.Authorization(login, pass);
  return grpc::Status::OK;
}

grpc::Status Mainer::AddMoney(grpc::ServerContext* context,
                      const blockchain::AddMoneyRequest* request,
                      blockchain::AddMoneyResponse* response){
  std::string login = request->name();
  uint32_t sum = request->sum();

  // Увеличение суммы
  Status s = db.AddTokens(sum, login);
  if (s == OK) {
    b_c.add_block(Transac("", login, sum));
  } else {
    *response->mutable_answer() = "Data Base was messed up";
    return grpc::Status::CANCELLED;
  }

  // Вознаграждение за созданный блок
  return RewardMainer();
}

grpc::Status Mainer::InfoBalance(grpc::ServerContext* context,
                        const blockchain::InfoBalanceRequest* request,
                        blockchain::InfoBalanceResponse* response){
  std::string login = request->name();
  std::string str = db.Balance(login);
  *response->mutable_sum() = str;
  return grpc::Status::OK;
}

grpc::Status Mainer::RewardMainer(){
  int s = db.AddTokens(1, name);
  if (s == OK) {
    b_c.add_block(Transac("", name, 1));
    return grpc::Status::OK;
  }
    return grpc::Status::CANCELLED;
}

std::string Mainer::GetPassword(){
  return password;
}

[[noreturn]] void Mainer::HackerProtection() const {
  while (true) {
    auto it = b_c.block_chain.cbegin();
    std::string l_hash = it->prev_block_hash;
    ++it;
    for (; it != b_c.block_chain.cend(); ++it) {
      if (l_hash != it->block_hash) {
        std::cerr << "Someone is trying to change the block system!!! "
                     "\n Contact support";
      } else {
        l_hash = it->prev_block_hash;
      }
    }
    std::this_thread::sleep_for(std::chrono::minutes(time));
  }
}
