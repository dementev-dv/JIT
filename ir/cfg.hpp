#ifndef CFG_HPP_
#define CFG_HPP_

#include <type.hpp>
#include <instr.hpp>
#include <bb.hpp>
#include <assert.hpp>

#include <fstream>

class Function;

class ControlFlow final {
 public:
  ControlFlow(Function* f) : func_(f) { }
  ~ControlFlow() = default;

  void AddBB(BasicBlock* bb) { bb_.push_back(bb); }

  void SetEntry(BasicBlock* e) { entry_ = e; }

  size_t bbid() { return bbid_++; }

  size_t insid() { return insid_++; }

  Function* func() { return func_; }

  void DumpIR(const char* path);
  void DumpGraph(const char* path);

 private:
  BasicBlock* entry_{nullptr};
  std::vector<BasicBlock*> bb_;
  size_t bbid_{0};
  size_t insid_{0};
  Function* func_;
};

#endif // CFG_HPP_
