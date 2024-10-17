#ifndef BB_HPP_
#define BB_HPP_

#include <types.hpp>
#include <instr.hpp>

#include <list>
#include <iterator>

class Function;

class BasicBlock {
  using InstrIt = typename std::list<Instruction*>::iterator;

 public:
  BasicBlock(size_t id, Function* f) 
    : id_(id),
      f_(f) {
    first_ = instr_.begin();
  }

  size_t id() { return id_; }

  void AddInstr(Instruction* i) { instr_.push_back(i); }
  void AddPhi(Instruction* i) { instr_.insert(i, first_); }
  void InsertInstr(Instruction* i, Instruction* next) {
    InsrtIt it = instr_.begin();
    while(it != instr_.end() && *it != i)
      it++;
    instr_.insert(i, it);
  }

 private:
  size_t id_;
  Function* f_;

  std::list<Instruction*> instr_;
  InstrIt first_;   // Phis are before first_ instruction

  std::vector<size_t> prec_:
  std::vector<size_t> succ_;
};

#endif // BB_HPP_
