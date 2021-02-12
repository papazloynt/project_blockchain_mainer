// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_DBCONNECTION_HPP_
#define INCLUDE_DBCONNECTION_HPP_

// private files
#include "Transac_struct.hpp"

// c++ headers
#include <iostream>
#include <string>
#include <thread>
#include <shared_mutex>

// SQLite3
#include <sqlite3.h>

enum Status {
  OK,
  ERROR
};

class DBConnection{
private:
  sqlite3* db;
  char* err;

public:
  std::string name_table;

 //Устанавливаем соединение с БД
 DBConnection() : db(nullptr), err(nullptr) {
    sqlite3_open("DataBase.db", &db);
  }

  //Закрываем соединение с БД
  virtual ~DBConnection(){
    sqlite3_close(db);
  }
  //Создаём таблицу
  void CreateDataBase();

  //Вставаляем пользователя
  std::string InsertPersonDataBase(const std::string name,
                                   std::shared_mutex& mutex_);
  //Проверка
  Status ChecksTransac(const Transac& tr,
                       std::shared_mutex& mutex_);

  //Транзакция
  Status TransactionDataBase(const Transac& tr,
                             std::shared_mutex& mutex_);

  //Добавление токенов на баланс
  Status AddTokens(const unsigned s, const std::string& name,
                   std::shared_mutex& mutex_);

  //Авторизация
  std::string Authorization(const std::string& login,
                            const std::string& password,
                            std::shared_mutex& mutex_);

  // Сумма баланса
  std::string Balance(const std::string& login,
                      std::shared_mutex& mutex_);
};

#endif  // INCLUDE_DBCONNECTION_HPP_
