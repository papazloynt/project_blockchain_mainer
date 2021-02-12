// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com
#ifndef INCLUDE_TRANSAC_STRUCT_HPP_
#define INCLUDE_TRANSAC_STRUCT_HPP_

// c++ headers
#include <string>

struct Transac{
  std::string client_from;
  std::string client_to;
  uint32_t sum;
  Transac() : client_from(), client_to(), sum(0) {}
  explicit Transac(const std::string c_f,
                   const std::string c_t,
                   uint32_t s) :
                    client_from(c_f),
                    client_to(c_t),
                    sum(s) {}
};

#endif  // INCLUDE_TRANSAC_STRUCT_HPP_
