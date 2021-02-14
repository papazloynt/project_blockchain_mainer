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

// убрать std::cin в main - DONE
// Имя таблицы - константа - DONE
// Shared_mutex  на два - DONE
// убрать unlock - DONE
// DBconnection - Repo - DONE
// убрать exit в клиенте и сервере - DONE
// Декремент у листа - DONE
// Параметр 5 минут вынести в конфигуратор (передача в мейн) - DONE
// Вознаграждение в транзакцию. - DONE
// Как можно улучшить свой проект переход на https - DONE
// Шифрование базы sqlite3 c++ - DONE

enum Status {
  OK,
  ERROR
};

class DBRepo{
private:
  sqlite3* db;
  char* err;
  std::shared_mutex mutex_;

public:
 //Устанавливаем соединение с БД
 DBRepo() : db(nullptr), err(nullptr) {
   //обработать вероятность ошибки
    sqlite3_open("DataBase.db", &db);
  }
  //Закрываем соединение с БД
  virtual ~DBRepo(){
    sqlite3_close(db);
  }
  //Создаём таблицу
  void CreateDataBase();

  //Вставаляем пользователя
  std::string InsertPersonDataBase(const std::string name);
  //Проверка
  Status ChecksTransac(const Transac& tr);

  //Транзакция
  Status TransactionDataBase(const Transac& tr);

  //Добавление токенов на баланс
  Status AddTokens(const unsigned s, const std::string& name);

  //Авторизация
  std::string Authorization(const std::string& login,
                            const std::string& password);

  // Сумма баланса
  std::string Balance(const std::string& login);
};

#endif  // INCLUDE_DBCONNECTION_HPP_
