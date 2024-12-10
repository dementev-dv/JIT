#ifndef LOOP_HPP_
#define LOOP_HPP_

#include <bb.hpp>
#include <cfg.hpp>
#include <set.hpp>

class Loop final {
 public:
  Loop(BasicBlock* h, size_t id) : header_(h), id_(id) { }
  ~Loop() = default;
  
  void AddLatch(BasicBlock* bb) { if (!latch_.contains(bb)) latch_.insert(bb); }

  void AddExit(BasicBlock* bb) { if (!exit_.contains(bb)) exit_.insert(bb); }

  void AddBlock(BasicBlock* bb) { block_.push_back(bb); }

  BasicBlock* Header() { return header_; }

  bool IsLatch(BasicBlock* bb) { return latch_.contains(bb); }

  bool IsExit(BasicBlock* bb) { return exit_.contains(bb); }

  bool Reducible() { return reducible_; }

  size_t id() { return id_; }

  void MarkIrreducible() {
    reducible_ = false;
    if (header_) block_.push_back(header_);    
    header_ = nullptr;
  }
  
  void build();

 private:
  size_t id_;

  BasicBlock* header_;
  Set latch_;
  Set exit_;
  std::vector<BasicBlock*> block_;

  Loop* outer_;
  std::vector<Loop*> inner_;

  bool reducible_{true};
};

#endif // LOOP_HPP_
