//
// Created by chastikov on 10.02.2021.
//

// private files
#include "Mainer.hpp"


Mainer::Mainer() : db(), b_c() {
  //Создаём, если не существует
  db.CreateDataBase();

  std::cout << "Please input your login: " << std::endl;
  std::cin >> name;

  //Вставляем пользлвателя
  std::string password =
      db.InsertPersonDataBase(name, sh_mutex);
  std::cout << "Your password: " << password << std::endl
            << "Please do not lose it, otherwise "
               "you will lose access to the wallet."
            << std::endl;
  //Проверка на хакерство
  std::thread check(&Mainer::HackerProtection, this);
  check.detach();
}

grpc::Status Mainer::Transaction(grpc::ServerContext* context,
                         const blockchain::TransactionRequest* request,
                         blockchain::TransactionResponse* response) {
    Transac transac_(request->client_from(),
                    request->client_to(),
                    request->sum());
    if (db.ChecksTransac(transac_, sh_mutex) == ERROR){
      return grpc::Status::CANCELLED;
    }
    // Транзакция Клиент-клиенту
    Status s = db.TransactionDataBase(transac_, sh_mutex);
    if (s == OK) {
      b_c.add_block(transac_, sh_mutex);
    } else {
      *response->mutable_answer() = "Data Base was messed up";
      return grpc::Status::CANCELLED;
    }

    // Вознаграждение за созданный блок
    s = db.AddTokens(1, name, sh_mutex);
    if (s == OK) {
      b_c.add_block(Transac("", name, 1), sh_mutex);
    } else {
      *response->mutable_answer() = "Data Base was messed up";
      return grpc::Status::CANCELLED;
    }
    return grpc::Status::OK;
}

grpc::Status Mainer::Registration(grpc::ServerContext* context,
                          const blockchain::RegistrationRequest* request,
                          blockchain::RegistrationResponse* response) {
  std::string login = request->name();
  std::string password =
      db.InsertPersonDataBase(login, sh_mutex);
  *response->mutable_password() = password;
  if (password == "NAME_ERROR"){
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status Mainer::Authorization(grpc::ServerContext* context,
                           const blockchain::AuthorizationRequest* request,
                           blockchain::AuthorizationResponse* response) {
  std::string login = request->name();
  std::string password = request->password();
  *response->mutable_answer() =
      db.Authorization(login, password, sh_mutex);
  return grpc::Status::OK;
}


//Подумать как лучше
grpc::Status Mainer::AddMoney(grpc::ServerContext* context,
                      const blockchain::AddMoneyRequest* request,
                      blockchain::AddMoneyResponse* response){
  std::string login = request->name();
  uint32_t sum = request->sum();

  // Увеличение суммы
  Status s = db.AddTokens(sum, login, sh_mutex);
  if (s == OK) {
    b_c.add_block(Transac("", login, sum), sh_mutex);
  } else {
    *response->mutable_answer() = "Data Base was messed up";
    return grpc::Status::CANCELLED;
  }

  // Вознаграждение за созданный блок
  s = db.AddTokens(1, name, sh_mutex);
  if (s == OK) {
    b_c.add_block(Transac("", name, 1), sh_mutex);
  } else {
    *response->mutable_answer() = "Data Base was messed up";
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}


grpc::Status Mainer::InfoBalance(grpc::ServerContext* context,
                        const blockchain::InfoBalanceRequest* request,
                        blockchain::InfoBalanceResponse* response){
  std::string login = request->name();
  std::string str = db.Balance(login, sh_mutex); //shared
  *response->mutable_sum() = str;
  return grpc::Status::OK;
}

void Mainer::HackerProtection() {
  while (true) {
    auto it = b_c.block_chain.cbegin();
    std::string l_hash = it->block_hash;
    ++it;
    for (; it != b_c.block_chain.cend(); ++it) {
      if (l_hash != it->prev_block_hash) {
        std::cerr << "Someone is trying to change the block system!!!";
        exit(2);
      } else {
        l_hash = it->block_hash;
      }
    }
    std::this_thread::sleep_for(std::chrono::minutes(5));
  }
}
