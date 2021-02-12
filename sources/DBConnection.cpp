//
// Created by chastikov on 10.02.2021.
//


// private files
#include "DBConnection.hpp"

// c++ headers
#include <random>

// PicoSHA2
#include <picosha2.h>

void DBConnection::CreateDataBase(){
  std::cout << "Please input name of database:" << std::endl;
  std::cin >> name_table;
  std::string sql_request = "CREATE TABLE IF NOT EXISTS " + name_table +
                            "(name TEXT NOT NULL,"
                            "password TEXT NOT NULL,"
                            "sum INT,"
                            "PRIMARY KEY (name));";

  //Создаём таблицу, имя - единственное, повторяться не может
  int rc = sqlite3_exec(db, sql_request.c_str(),
                        NULL, NULL, &err);
  if ( rc != SQLITE_OK) {
    std::cout << "error:" << err << std::endl;
    exit(1);
  }
  else {
    std::cout << "Table created successfully" << std::endl;
  }
}

std::string DBConnection::InsertPersonDataBase(const std::string name,
                                               std::shared_mutex& mutex_) {
  thread_local std::mt19937 gen(std::random_device{}());
  std::string randomString = std::to_string(gen());
  std::string password =
      picosha2::hash256_hex_string(randomString + name);

  std::string sql_request = "INSERT INTO information (name, password, sum) "
                            "VALUES ("
                            "'" + name + "',"
                            " '" + password + "',"
                            " 0);";

  std::unique_lock<std::shared_mutex> lock(mutex_);
  int rc = sqlite3_exec(db, sql_request.c_str(),
                        NULL, NULL, &err);
  lock.unlock();

  if ( rc != SQLITE_OK) {
    std::cout << "error:" << err << std::endl;
    password = "NAME_ERROR";
  }
  return password;
}


Status DBConnection::TransactionDataBase(const Transac& tr, std::shared_mutex& mutex_) {
  //А что если такого человека нету?

  //У кого минус
  std::string sql_minus = "UPDATE information SET "
                          "sum=sum-" + std::to_string(tr.sum) +
                          " WHERE name='" + tr.client_from +"'";

  //У кого плюс
  std::string sql_plus = "UPDATE information SET "
                         "sum=sum+" + std::to_string(tr.sum) +
                         " WHERE name='" + tr.client_to +"'";
  char* err_h = nullptr;

  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_minus.c_str(), NULL, NULL, &err);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err_h);
  lock.unlock();

  if (err != nullptr && err_h != nullptr) {
    return OK;
  }
  return ERROR;
}

Status DBConnection::AddTokens(const unsigned s, const std::string& name, std::shared_mutex& mutex_) {
  std::string sql_plus =
      "UPDATE information SET "
      "sum=sum+" +
      std::to_string(s) + " WHERE name='" + name + "'";

  std::unique_lock<std::shared_mutex> lock(mutex_);
  sqlite3_exec(db, sql_plus.c_str(), NULL, NULL, &err);
  lock.unlock();

  if (err != nullptr) {
    return OK;
  }
  return ERROR;
}

int callback_auth(void* NotUsed, int args, char** argv, char** azColName){
  return 1;
}

// shared_ptr сделать в execute
std::string DBConnection::Authorization(const std::string& login,
                          const std::string& password,
                          std::shared_mutex& mutex_){
  std::string sql_request = "SELECT name, sum FROM information WHERE "
                           "name='" + login + "' and "
                           "password='" + password + "'";
  std::shared_lock<std::shared_mutex> lock(mutex_);
  int rc = sqlite3_exec(db, sql_request.c_str(), callback_auth, NULL, NULL);
  lock.unlock();
  if (rc == 0) {
     return "Wrong login or password! Please try again.";
  } else {
    return "Welcome to the system " + login;
  }
}


int callback_balance(void* sum, int args, char** argv, char** azColName) {
  std::string str = argv[1];
  int *intPtr = static_cast<int*>(sum);
  *intPtr = std::stoi(str);
  return 0;
}

std::string DBConnection::Balance(const std::string& login, std::shared_mutex& mutex_){

    std::string sql_request = "SELECT name, sum FROM "
                              + name_table + " where "
                              "name='" + login + "'";
    int sum;

    std::shared_lock<std::shared_mutex> lock(mutex_);
    int rc = sqlite3_exec(db, sql_request.c_str(), callback_balance,
                 &sum, &err);
    lock.unlock();
    if (rc != SQLITE_OK) {
      return err;
    }
    return std::to_string(sum);
}

