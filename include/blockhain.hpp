//
// Created by chastikov on 18.01.2021.
//

#ifndef BLOCKCHAIN_BLOCKHAIN_HPP
#define BLOCKCHAIN_BLOCKHAIN_HPP

// c++ header
#include <list>
#include <iterator>
#include <string>
#include <shared_mutex>

// PicoSHA2
#include <picosha2.h>



struct Transac {
  std::string client_from;
  std::string client_to ;
  uint32_t sum;
  Transac() : client_from(), client_to(), sum(0) {}
  explicit Transac(const std::string c_f, const std::string c_t, uint32_t s) :
      client_from(c_f),
      client_to(c_t),
      sum(s) {}

};

struct Block {
  unsigned position;
  std::string block_hash;
  std::string transaction_hash;
  std::string prev_block_hash;

  Block(unsigned i, std::string b_h, std::string t_h, std::string p_h) :
                        position(i), block_hash(b_h),
                        transaction_hash(t_h), prev_block_hash(p_h) {}
};


struct BlockChain {
  std::list <Block> block_chain;

  BlockChain() = default;

  BlockChain(const BlockChain& b) : block_chain(b.block_chain) {}

  BlockChain& operator= (const BlockChain& b) {
    if (this != &b) {
      block_chain = b.block_chain;
    }
    return *this;
  }

  //Подумать, может быть можно сделать конструктор по умролчанию для первой операции
  void add_block(const Transac& info, std::shared_mutex& mutex_){
    std::string transaction_hash =
        picosha2::hash256_hex_string(info.client_from +
                                     info.client_to +
                                     std::to_string(info.sum));
    std::string block_hash =
        picosha2::hash256_hex_string(std::to_string(block_chain.size() + 1) +
                                     transaction_hash);
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (block_chain.size() != 0) {
      std::list<Block>::iterator it = block_chain.end();
      block_chain.emplace_back(block_chain.size() + 1,
                               block_hash,
                               transaction_hash,
                               (--it)->block_hash);
    } else {
      block_chain.emplace_back(0, block_hash, transaction_hash, "");
    }
  }
};
#endif  // BLOCKCHAIN_BLOCKHAIN_HPP
