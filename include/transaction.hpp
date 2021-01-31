// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

// private headers
#include "blockhain.hpp"

#include <grpcpp/grpcpp.h>
/*#include <google/protobuf/repeated_field.h>*/
#include <blockchain.grpc.pb.h>
#include <shared_mutex>
#include <string>
#include <fstream>
#include<thread>
// SQLite3
#include <sqlite3.h>
// picosha
#include <picosha2.h>


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

void CreateDataBase(sqlite3* db,char* err){
  //Создаём таблицу, имя - единственное, повторяться не может
  int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS information("
                            "name TEXT NOT NULL,"
                            "password TEXT NOT NULL,"
                            "sum INT,"
                            "PRIMARY KEY (name));",
                        NULL, NULL, &err);
  if ( rc != SQLITE_OK) {
    std::cout << "error:" << err << std::endl;
    exit(1);
  }
  else {
    std::cout << "Table created successfully" << std::endl;
  }
}

void InsertPerson(sqlite3* db, char* err) {
  std::string name;
  std::string password;
}

class Mainer : public blockchain::Blockchain::Service {
 public:
  Mainer() : db(nullptr), err(nullptr) {
    sqlite3_open("Data.db", &db);
    CreateDataBase(db,err);


  }
  sqlite3* db;
  char* err;
 private:
  std::shared_mutex sh_mutex;
   grpc::Status Transaction(grpc::ServerContext* context,
                      const blockchain::TransactionRequest* request,
                      blockchain::TransactionResponse* response) override {
    Transac transac_(request);

  }

};

#endif  // INCLUDE_SUGGESTIONS_HPP_
