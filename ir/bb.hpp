#ifndef BB_HPP_
#define BB_HPP_

#include <type.hpp>

#include <list>
#include <iterator>
#include <fstream>
#include <unordered_set>

class Function;

class Instruction;

class PhiInstr;

class BasicBlock;

class Loop;

using Set = typename std::unordered_set<BasicBlock*>;
using SetIt = typename std::unordered_set<BasicBlock*>::iterator;

class BasicBlock {
 public:
  BasicBlock(size_t id, Function* f) 
    : id_(id),
      f_(f) { }

  ~BasicBlock() {
    for (std::list<Instruction*>::iterator ins = instr_.begin(); ins != instr_.end(); ins++) {
      delete *ins;
    }
    for (std::list<PhiInstr*>::iterator phi = phi_.begin(); phi != phi_.end(); phi++) {
      delete *phi;
    }
  }

  size_t id() { return id_; }

  void AddInstr(Instruction* i) { instr_.push_back(i); }

  void AddPhi(PhiInstr* i) { phi_.push_back(i); }

  void SetTrueSucc(BasicBlock* bb) { trueSucc_ = bb; }

  void SetFalseSucc(BasicBlock* bb) { falseSucc_ = bb; }

  void AddPrec(BasicBlock* bb) { prec_.push_back(bb); }

  void AddDom(BasicBlock* bb) { if (!dom_.contains(bb)) dom_.insert(bb); }

  void AddSub(BasicBlock* bb) { if (!sub_.contains(bb)) sub_.insert(bb); }

  void SetIdom(BasicBlock* bb) { idom_ = bb; }

  BasicBlock* GetTrueSucc() { return trueSucc_; }

  BasicBlock* GetFalseSucc() { return falseSucc_; }

  BasicBlock* GetPrec(size_t n) { return prec_[n]; }

  BasicBlock* GetIdom() { return idom_; }

  size_t nprecs() { return prec_.size(); }

  // size_t nsubs() { return sub_.size(); }

  // size_t ndoms() { return dom_.size(); }

  Function* func() { return f_; }

  void Dump(std::ofstream& out);

  void SetMarker(bool m) { marker_ = m; }

  bool marker() { return marker_; }

  bool Dominate(BasicBlock* bb) { return dom_.contains(bb); }

  bool Subordinate(BasicBlock* bb) { return sub_.contains(bb); }

  Set& Dom() { return dom_; }

  Set& Sub() { return sub_; }

 private:
  size_t id_;
  Function* f_;

  std::list<Instruction*> instr_;
  std::list<PhiInstr*> phi_;

  std::vector<BasicBlock*> prec_;
  BasicBlock* trueSucc_{nullptr};
  BasicBlock* falseSucc_{nullptr};

  bool marker_{false};
  Set dom_;
  Set sub_;
  BasicBlock* idom_{nullptr};

  Loop* loop_{nullptr};
  bool black_{false};
  bool grey_{false};
  bool green_{false};
};

#endif // BB_HPP_
