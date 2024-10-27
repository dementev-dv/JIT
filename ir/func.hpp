#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include <types.hpp>
#include <bb.hpp>

class DeclInstr;

class Function final {
 public:
  Function(DataType type, const char* name)
    : name_(name),
      type_(type),
      args_(0),
      entry_(nullptr) { }

  void SetEntry(BasicBlock* bb) { entry_ = bb; }

  size_t argn() { return args_.size(); }

  DeclInstr* arg(size_t n) { return args_[n]; }

  void AddArg(DeclInstr* a) {
    args_.push_back(a);
  }

  std::string name() { return name_; }

  DataType type() { return type_; }

 private:
  std::string name_;
  DataType type_;

  std::vector<DeclInstr*> args_;

  BasicBlock* entry_;
};

#endif // FUNC_HPP_
