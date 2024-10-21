#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include <types.hpp>
#include <bb.hpp>

class DeclInstr;

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

  size_t argn() { return args_.size(); }

  Instruction* arg(size_t n) { return args_[n]; }

  void AddArg(Instruction* a) {
    args_.push_back(a);
  }

  std::string name() { return name_; }

  DataType type() { return type_; }

 private:
  std::string name_;
  DataType type_;

  std::vector<DeclInstr*> args_;

  std::vector<BasicBlock*> bb_;
};

#endif // FUNC_HPP_
