// Copyright 2020 Chastikov Alexander cool.chastikov53@gmail.com

#ifndef INCLUDE_BLOCKCHAIN_HPP_
#define INCLUDE_BLOCKCHAIN_HPP_

//local files
#include "Transac_struct.hpp"

// c++ header
#include <list>
#include <iterator>
#include <shared_mutex>
#include <string>

// PicoSHA2
#include <picosha2.h>


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
  std::shared_mutex mutex_;

  BlockChain() {
    // Первый блок - количество токенов,
    // которые получает Майнер за формирование блока
    std::string block_hash =
        picosha2::hash256_hex_string(std::string("first block"));

    std::string transaction_hash =
        picosha2::hash256_hex_string(std::string("1 token"));

    block_chain.emplace_front(0, block_hash, transaction_hash, "");
  }

  BlockChain(const BlockChain& b) : block_chain(b.block_chain) {}

  BlockChain& operator= (const BlockChain& b) {
    if (this != &b) {
      block_chain = b.block_chain;
    }
    return *this;
  }

  void add_block(const Transac& info){
    std::string transaction_hash =
        picosha2::hash256_hex_string(info.client_from +
                                     info.client_to +
                                     std::to_string(info.sum));
    std::string block_hash =
        picosha2::hash256_hex_string(std::to_string(block_chain.size() + 1) +
                                     transaction_hash);
    std::unique_lock<std::shared_mutex> lock(mutex_);

    std::list<Block>::iterator it = block_chain.begin();
    block_chain.emplace_front(block_chain.size(),
                             block_hash,
                             transaction_hash,
                             it->block_hash);
  }
};
#endif  // INCLUDE_BLOCKCHAIN_HPP_
