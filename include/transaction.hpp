// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_SUGGESTIONS_HPP_
#define INCLUDE_SUGGESTIONS_HPP_

// private headers
#include "blockhain.hpp"

// c++ headers
#include <iostream>
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
#include <blockchain.grpc.pb.h>



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

void InsertPersonDataBase(sqlite3* db, char* err) {
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
    std::cout << "error:" << err << std::endl;
  }
}

void TransactionDataBase(sqlite3* db, char* err, const Transac& tr, std::shared_mutex& mutex_) {
  //А что если такого человека нету?
  //Авторизация на стороне клиента
  //Проверка баланса на стороне клиента

  sqlite3_open("Data.db", &db);

  //У кого минус
  std::string sql_minus = "UPDATE information SET "
      "sum=sum-" + std::to_string(tr.sum) +
      " WHERE name='" + tr.client_from +"'";

  //У кого плюс
  std::string sql_plus = "UPDATE information SET "
                          "sum=sum+" + std::to_string(tr.sum) +
                          " WHERE name='" + tr.client_to +"'";


  //Исключения если что не так
  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_minus.c_str(), NULL, NULL, NULL);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, NULL);
  lock.unlock();

  sqlite3_close(db);
}

class Mainer : public blockchain::Blockchain::Service {
 public:

  Mainer() : db(nullptr), err(nullptr), block_chain() {
    //Открываем
    sqlite3_open("Data.db", &db);
    //Создаём, если не существует
    CreateDataBase(db, err);
    //Вставляем пользлвателя
    InsertPersonDataBase(db, err);
    //Закрываем, т.к. необходимо
    sqlite3_close(db);
  }
  sqlite3* db;
  char* err;

 private:
   std::shared_mutex sh_mutex;
   //ещё раз обратить внимание на конструктор по умолчанию, который будет проверять Мошенников
   BlockChain block_chain;

   grpc::Status Transaction(grpc::ServerContext* context,
                      const blockchain::TransactionRequest* request,
                      blockchain::TransactionResponse* response) override {

     Transac transac_(request->req().client_from(),
                     request->req().client_to(),
                     request->req().sum());

    //Многопоточность сначала - транзакция,
    // окончательное подтверждение операции - добавление её в цепь блоков
    block_chain.add_block(transac_, sh_mutex);
    TransactionDataBase(db, err, transac_, sh_mutex);
    //Добавление токена Майнеру
     return grpc::Status::OK;
  }

  //Добавить получение информации о балансе, скорее всего реализация у клиента

};

#endif  // INCLUDE_SUGGESTIONS_HPP_
