#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include <types.hpp>
#include <instr.hpp>

class DeclInstr final : Instruction {
 public:
  DeclInstr(DataType type)
    : Instruction(type, DECL) {
  }
};

class Function final {
 public:
  Function(DataType type, const char* name, size_t n)
    : name_(name),
      type_(type),
      argn_(n),
      args_(0),
      nbb_(0),
      bb_(0) { }

  void AddBB(BasicBlock* bb) { bb_.push_back(bb); }

  size_t argn() { return argn_; }

  Instruction* arg(size_t n) { return args_[n]; }

  void AddArg(Instruction* a) {
    args_.push_back(a);
    assert(args_.size() < argn_);
  }

  DataType type() { return type_; }

 private:
  std::string name_;
  DataType type_;

  size_t argn_;
  std::vector<Instruction*> args_;

  size_t nbb_;
  std::vector<BasicBlock*> bb_;
}

#endif // FUNC_HPP_
