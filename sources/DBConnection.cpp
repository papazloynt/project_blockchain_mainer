// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com


// private files
#include "DBConnection.hpp"

// c++ headers
#include <random>

// PicoSHA2
#include <picosha2.h>

void DBConnection::CreateDataBase(){
  std::cout << "Please input name of database:" << std::endl;
  std::cin >> name_table;
  std::string sql_request = "CREATE TABLE IF NOT EXISTS " +
                            name_table +
                            "(name TEXT NOT NULL,"
                            "password TEXT NOT NULL,"
                            "sum INT,"
                            "PRIMARY KEY (name));";

  //Создаём таблицу, имя - единственное, повторяться не может
  int rc = sqlite3_exec(db, sql_request.c_str(),
                        NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    std::cout << "error:" << err << std::endl;
    exit(1);
  } else {
    std::cout << "Table created successfully" << std::endl;
  }
}

std::string DBConnection::InsertPersonDataBase(
                                   const std::string name,
                                   std::shared_mutex& mutex_) {
  thread_local std::mt19937 gen(std::random_device{}());
  std::string randomString = std::to_string(gen());
  std::string password =
      picosha2::hash256_hex_string(randomString + name);

  std::string sql_request = "INSERT INTO " + name_table +
                            " (name, password, sum) "
                            "VALUES ("
                            "'" + name + "',"
                            " '" + password + "',"
                            " 0);";

  std::unique_lock<std::shared_mutex> lock(mutex_);
  int rc = sqlite3_exec(db, sql_request.c_str(),
                        NULL, NULL, &err);
  lock.unlock();

  if (rc != SQLITE_OK) {
    std::cout << "error:" << err << std::endl;
    password = "NAME_ERROR";
  }
  return password;
}

int callback_checks(void* NotUsed, int args,
                    char** argv, char** azColName){
  return 1;
}

Status DBConnection::ChecksTransac(const Transac& tr,
                                   std::shared_mutex& mutex_) {
  std::string sql_request_balance =
      "SELECT name, sum FROM " + name_table +
      " WHERE name='" + tr.client_from + "' and "
      "sum>" + std::to_string(tr.sum);

  std::string sql_request_exist =
      "SELECT name FROM " + name_table +
      " WHERE name='" + tr.client_to;

  std::shared_lock<std::shared_mutex> lock(mutex_);
  int rc1 = sqlite3_exec(db, sql_request_balance.c_str(),
                         callback_checks, NULL, NULL);
  int rc2 = sqlite3_exec(db, sql_request_balance.c_str(),
                         callback_checks, NULL, NULL);
  lock.unlock();

  if ((rc1 == rc2) && (rc1 == 0)) {
    return ERROR;
  }
  return OK;
}



Status DBConnection::TransactionDataBase(const Transac& tr,
                                    std::shared_mutex& mutex_) {
  //У кого минус
  std::string sql_minus = "UPDATE " + name_table + " SET "
                          "sum=sum-" + std::to_string(tr.sum) +
                          " WHERE name='" + tr.client_from +"'";

  //У кого плюс
  std::string sql_plus = "UPDATE " + name_table + " SET "
                         "sum=sum+" + std::to_string(tr.sum) +
                         " WHERE name='" + tr.client_to +"'";
  err = nullptr;
  char* err_h = nullptr;

  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_minus.c_str(), NULL, NULL, &err);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err_h);
  lock.unlock();

  if (err != nullptr && err_h != nullptr) {
    return ERROR;
  }
  return OK;
}

Status DBConnection::AddTokens(const unsigned s,
                               const std::string& name,
                               std::shared_mutex& mutex_) {
  std::string sql_plus =
      "UPDATE " + name_table + " SET "
      "sum=sum+" + std::to_string(s) +
      " WHERE name='" + name + "'";
  err = nullptr;
  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err);
  lock.unlock();

  if (err != nullptr) {
    return ERROR;
  }
  return OK;
}

int callback_auth(void* NotUsed, int args,
                  char** argv, char** azColName){
  return 1;
}

std::string DBConnection::Authorization(const std::string& login,
                          const std::string& password,
                          std::shared_mutex& mutex_){
  std::string sql_request = "SELECT name, sum FROM " + name_table +
                            " WHERE name='" + login + "' and "
                           "password='" + password + "'";
  std::shared_lock<std::shared_mutex> lock(mutex_);
  int rc = sqlite3_exec(db, sql_request.c_str(),
                        callback_auth, NULL, NULL);
  lock.unlock();
  if (rc == 0) {
     return "Wrong login or password! Please try again.";
  }
    return "Welcome to the system " + login;
}


int callback_balance(void* sum, int args,
                     char** argv, char** azColName) {
  std::string str = argv[1];
  int *intPtr = static_cast<int*>(sum);
  *intPtr = std::stoi(str);
  return 0;
}

std::string DBConnection::Balance(const std::string& login,
                                  std::shared_mutex& mutex_) {
    std::string sql_request = "SELECT name, sum FROM "
                              + name_table + " where "
                              "name='" + login + "'";
    uint32_t sum = 0;

    std::shared_lock<std::shared_mutex> lock(mutex_);
    int rc = sqlite3_exec(db, sql_request.c_str(),
                          callback_balance, &sum, &err);
    lock.unlock();
    if (rc != SQLITE_OK) {
      return err;
    }
    return std::to_string(sum);
}

