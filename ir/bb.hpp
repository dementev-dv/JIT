#ifndef BB_HPP_
#define BB_HPP_

#include <types.hpp>

#include <list>
#include <iterator>

class Function;

class Instruction;

class PhiInstr;

class BasicBlock {
 public:
  BasicBlock(size_t id, Function* f) 
    : id_(id),
      f_(f) { }

  size_t id() { return id_; }

  void AddInstr(Instruction* i) { instr_.push_back(i); }

  void AddPhi(PhiInstr* i) { phi_.push_back(i); }

  void SetTrueSucc(BasicBlock* bb) { trueSucc_ = bb; }

  void SetFalseSucc(BasicBlock* bb) { falseSucc_ = bb; }

  void AddPrec(BasicBlock* bb) { prec_.push_back(bb); }

  BasicBlock* GetTrueSucc() { return trueSucc_; }

  BasicBlock* GetFalseSucc() { return falseSucc_; }

  BasicBlock* GetPrec(size_t n) { return prec_[n]; }

  size_t nprecs() { return prec_.size(); }

  Function* func() { return f_; }

 private:
  size_t id_;
  Function* f_;

  std::list<Instruction*> instr_;
  std::list<PhiInstr*> phi_;

  std::vector<BasicBlock*> prec_;
  BasicBlock* trueSucc_;
  BasicBlock* falseSucc_;
};

#endif // BB_HPP_
