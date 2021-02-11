//
// Created by chastikov on 10.02.2021.
//

#ifndef BLOCKCHAIN_TRANSACTION_H
#define BLOCKCHAIN_TRANSACTION_H

// c++ headers
#include <string>

struct Transac{
  std::string client_from;
  std::string client_to ;
  uint32_t sum;
  Transac() : client_from(), client_to(), sum(0) {}
  explicit Transac(const std::string c_f,
                   const std::string c_t,
                   uint32_t s) :
      client_from(c_f),
      client_to(c_t),
      sum(s) {}
};

#endif  // BLOCKCHAIN_TRANSACTION_H
