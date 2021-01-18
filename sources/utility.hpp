//
// Created by chastikov on 18.01.2021.
//

#ifndef BLOCKCHAIN_UTILITY_HPP
#define BLOCKCHAIN_UTILITY_HPP

struct Block {
  std::string transaction_hash;
  Block* prev_ptr;

  Block(std::string transaction_hash_, Block* prev_ptr) :
              transaction_hash(transaction_hash_),
               prev_ptr(prev_ptr_) {}
};


struct BlockChain {
 private:
  std::vector <Block> block_chain;

 public:
  BlockChain(const BlockChain& b) : block_chain(b.block_chain) {}

  &BlockChain operator= (const BlockChain& b) {
    if (this != &b) {
      block_chain = b.block_chain;
    }
    return *this;
  }

  void add_block(const InfoTransaction& info){
    std::string transaction_hash_ = picosha2(info);
    if (block_chain.size != 0) {
      //Сделать нормальный допуск до предпоследнего элемента
      block_chain.push_back(transaction_hash_, &block_chain(block_chain.size() - 1));
    } else {
      block_chain.push_back(transaction_hash_, nullptr);
    }
  }

};






#endif  // BLOCKCHAIN_UTILITY_HPP
