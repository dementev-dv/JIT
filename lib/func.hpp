#ifndef FUNC_HPP_
#define FUNC_HPP_

#include <vector>
#include <string>

#include "types.hpp"
#include "instr.hpp"


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
      type_(type),
      args_(args) {
  }

  Function(DataType type, const char* name)
    : name_(name),
      type_(type),
      args_(0) {
  }

  void AddBB(BasicBlock* bb) { bb_.push_back(bb); }

 private:
  std::string name_;
  DataType type_;
  std::vector<DataType> args_;

  std::vector<BasicBlock*> bb_;
}

#endif // FUNC_HPP_