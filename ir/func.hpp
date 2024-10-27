#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include <type.hpp>
#include <bb.hpp>
#include <cfg.hpp>

class DeclInstr;

class Function final {
 public:
  Function(DataType type, const char* name)
    : name_(name),
      type_(type),
      args_(0),
      cfg_(nullptr) { }

  ~Function() {
    for (size_t i = 0; i < args_.size(); i++) {
      delete args_[i];
    }
    delete cfg_;
  }

  void SetCFG(ControlFlow* cfg) { cfg_ = cfg; }

  size_t argn() { return args_.size(); }

  DeclInstr* arg(size_t n) { return args_[n]; }

  void AddArg(DeclInstr* a) {
    args_.push_back(a);
  }

  std::string name() { return name_; }

  DataType type() { return type_; }

  ControlFlow* cfg() { return cfg_; }

 private:
  std::string name_;
  DataType type_;

  std::vector<DeclInstr*> args_;

  ControlFlow* cfg_;
};

#endif // FUNC_HPP_
