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

  void AddBlock(BasicBlock* bb) { if (!block_.contains(bb)) block_.insert(bb); }

  BasicBlock* Header() { return header_; }

  Loop* Outer() { return outer_; }

  void SetOuter(Loop* o) { outer_ = o; }

  bool IsLatch(BasicBlock* bb) { return latch_.contains(bb); }

  bool IsExit(BasicBlock* bb) { return exit_.contains(bb); }

  bool HasBlock(BasicBlock* bb) { return block_.contains(bb); }

  bool Reducible() { return reducible_; }

  size_t id() { return id_; }

  void MarkIrreducible() {
    reducible_ = false;
    if (header_) block_.insert(header_);    
    header_ = nullptr;
  }
  
  void build();

 private:
  BasicBlock* header_;
  size_t id_;

  Set latch_;
  Set exit_;
  Set block_;

  Loop* outer_{nullptr};
  std::vector<Loop*> inner_;

  bool reducible_{true};
};

#endif // LOOP_HPP_
