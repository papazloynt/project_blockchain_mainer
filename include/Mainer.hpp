// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_MAINER_HPP_
#define INCLUDE_MAINER_HPP_

// private headers
#include "DBConnection.hpp"
#include "Transac_struct.hpp"
#include "blockchain.hpp"

// c++ headers
#include <chrono>
#include <string>

// gRPC
#include <grpcpp/grpcpp.h>
#include <blockchain.grpc.pb.h>

class Mainer : public blockchain::Blockchain::Service {
 public:
  Mainer();
  std::shared_mutex sh_mutex;
  std::string name;

 private:
  DBConnection db;
  BlockChain b_c;

   grpc::Status Transaction(grpc::ServerContext* context,
                      const blockchain::TransactionRequest* request,
                      blockchain::TransactionResponse* response) override;

   grpc::Status Registration(grpc::ServerContext* context,
                        const blockchain::RegistrationRequest* request,
                        blockchain::RegistrationResponse* response) override;

  grpc::Status Authorization(grpc::ServerContext* context,
                        const blockchain::AuthorizationRequest* request,
                        blockchain::AuthorizationResponse* response) override;

  grpc::Status AddMoney(grpc::ServerContext* context,
                        const blockchain::AddMoneyRequest* request,
                        blockchain::AddMoneyResponse* response) override;

  grpc::Status InfoBalance(grpc::ServerContext* context,
                        const blockchain::InfoBalanceRequest* request,
                        blockchain::InfoBalanceResponse* response) override;
  //Функция проверки на взлом
  void HackerProtection();
};

#endif  // INCLUDE_MAINER_HPP_
