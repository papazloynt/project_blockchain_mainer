// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

// private headers
#include "blockhain.hpp"

// c++ headers
#include <string>
#include<thread>
#include <random>
#include <shared_mutex>

// SQLite3
#include <sqlite3.h>

// PicoSHA2
#include <picosha2.h>

// gRPC
#include <grpcpp/grpcpp.h>
/*#include <google/protobuf/repeated_field.h>*/
#include <blockchain.grpc.pb.h>

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
  std::cout << "Please input your login: " << std::endl;
  std::cin >> name;

  thread_local std::mt19937 gen(std::random_device{}());
  std::string randomString = std::to_string(gen());
  std::string password =
          picosha2::hash256_hex_string(randomString + name);

  std::string sql_request = "INSERT INTO information (name, password, sum) "
      "VALUES ("
      "'" + name + "',"
      " '" + password + "',"
      " 0);";
  int rc = sqlite3_exec(db, sql_request.c_str(),
                    NULL, NULL, &err);

  if ( rc != SQLITE_OK) {
    std::cout<< "error:" << err <<std::endl;
  }
}

class Mainer : public blockchain::Blockchain::Service {
 public:
  Mainer() : db(nullptr), err(nullptr) {
    //Открываем
    sqlite3_open("Data.db", &db);
    //Создаём, если не существует
    CreateDataBase(db, err);
    //Вставляем пользлвателя
    InsertPerson(db, err);
    //Закрываем, необходимо
    sqlite3_close(db);
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
