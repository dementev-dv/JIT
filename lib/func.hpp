#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include "types.hpp"
#include "instr.hpp"

class DeclInstr final : Instruction {
 public:
  DeclInstr(BasicBlock* bb, DataType type)
    : bb_(bb),
      type_(type) {
  }
}

class Function final {
 public:
  Function(DataType type, const char* name,
      std::initializer_list<DataType> args)
    : name_(name),
      type_(type),
      args_(args) {
  }

  Function(DataType type, const char* name,
      std::initializer_list<DataType> args)
    : name_(name),
      type_(type) {
    for ()
  }

  Function(DataType type, const char* name)
    : name_(name),
      type_(type),
      args_(0) {
  }

  void AddBB(BasicBlock* bb) { bb_.push_back(bb); }

  size_t argn() { return nargs_; }

  DataType arg(size_t n) { return args_[n]; }

 private:
  std::string name_;
  DataType type_;

  size_t nargs_;
  std::vector<Instruction*> args_;

  size_t nbb_;
  std::vector<BasicBlock*> bb_;
}

#endif // FUNC_HPP_
