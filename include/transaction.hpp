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
#include <chrono>

// SQLite3
#include <sqlite3.h>

// PicoSHA2
#include <picosha2.h>

// gRPC
#include <grpcpp/grpcpp.h>
#include <blockchain.grpc.pb.h>



void CreateDataBase(sqlite3* db){
  char* err;
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

std::string InsertPersonDataBase(sqlite3* db) {
  std::string name;
  std::cout << "Please input your login: " << std::endl;
  std::cin >> name;

  thread_local std::mt19937 gen(std::random_device{}());
  std::string randomString = std::to_string(gen());
  std::string password =
          picosha2::hash256_hex_string(randomString + name);

  char* err;
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
  return name;
}

std::string TransactionDataBase(sqlite3* db, const Transac& tr, std::shared_mutex& mutex_) {
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


  char* err1 = nullptr;
  char* err2 = nullptr;

  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_minus.c_str(), NULL, NULL, &err1);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err2);
  lock.unlock();

  sqlite3_close(db);

  if (err1 != nullptr && err2 != nullptr) {
    return "OK";
  }
    return "ERROR";


}

std::string AddTokens(sqlite3* db, const unsigned s, const std::string& name, std::shared_mutex& mutex_) {
  sqlite3_open("Data.db", &db);

  std::string sql_plus = "UPDATE information SET "
                         "sum=sum+" + std::to_string(s) +
                         " WHERE name='" + name +"'";
  char* err;
  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err);
  lock.unlock();

  sqlite3_close(db);
  if (err != nullptr) {
    return "OK";
  }
  return "ERROR";
}

class Mainer : public blockchain::Blockchain::Service {
 public:

  Mainer() : db(nullptr), b_c() {
    //Открываем
    sqlite3_open("Data.db", &db);
    //Создаём, если не существует
    CreateDataBase(db);
    //Вставляем пользлвателя
    name = InsertPersonDataBase(db);
    //Закрываем, т.к. необходимо
    sqlite3_close(db);

    //Проверка на хакерство
    std::thread check(&Mainer::HackerProtection, this);
    check.detach();
  }
  sqlite3* db;
  std::string name;

 private:
   std::shared_mutex sh_mutex;
   BlockChain b_c;

   grpc::Status Transaction(grpc::ServerContext* context,
                      const blockchain::TransactionRequest* request,
                      blockchain::TransactionResponse* response) override {

     Transac transac_(request->req().client_from(),
                     request->req().client_to(),
                     request->req().sum());

     //Избавиться от одинакого кода!
     //Транзакция Клиент-клиенту
    std:: string status = TransactionDataBase(db, transac_, sh_mutex);
    if (status == "OK") {
      b_c.add_block(transac_, sh_mutex);
    } else {
      *response->mutable_answer() = "Data Base was messed up";
      return grpc::Status::CANCELLED;
    }

    //Вознаграждение
    status = AddTokens(db, 1, name, sh_mutex);
     if (status == "OK") {
       b_c.add_block( Transac("", name, 1), sh_mutex);
     } else {
       *response->mutable_answer() = "Data Base was messed up";
       return grpc::Status::CANCELLED;
     }

     return grpc::Status::OK;
  }

  void HackerProtection() {
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

  //Избавиться от повторения кода и говнокода
  //Разбить всё на файлы
  //Добавить получение информации о балансе, скорее всего реализация у клиента
  //Добавть в прото Добавление денег на баланс
  //У клиента небольшой интерфейс сделать

};

#endif  // INCLUDE_SUGGESTIONS_HPP_
