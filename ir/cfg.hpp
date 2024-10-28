#ifndef CFG_HPP_
#define CFG_HPP_

#include <type.hpp>
#include <bb.hpp>
#include <assert.hpp>

#include <fstream>
#include <queue>

class Function;

class ControlFlow final {
 public:
  ControlFlow(Function* f) : func_(f) { }
  ~ControlFlow() {
    delete entry_;
    for (size_t i = 0; i < bb_.size(); i++) {
      delete bb_[i];
    }
  }

  void AddBB(BasicBlock* bb) { bb_.push_back(bb); }

  void SetEntry(BasicBlock* e) { entry_ = e; }

  size_t bbid() { return bbid_++; }

  size_t insid() { return insid_++; }

  Function* func() { return func_; }

  void SetDomSlow();
  void SetDom();

  void DumpIR(const char* path);
  void DumpGraph(const char* path);
  void DumpDomTree(const char* path);
  void DumpIdomTree(const char* path);

 private:
  BasicBlock* entry_{nullptr};
  std::vector<BasicBlock*> bb_;
  size_t bbid_{0};
  size_t insid_{0};
  Function* func_;
};

class DFS final {
 public:
  DFS(BasicBlock* bb) : entry_(bb) { }

  std::vector<BasicBlock*> Run() {
    travel(entry_);
    return vec_;
  }

 private:
  void travel(BasicBlock* bb);

  BasicBlock* entry_;
  std::vector<BasicBlock*> vec_;
};

class BFS final {
 public:
  BFS(BasicBlock* bb) : entry_(bb) { }

  std::vector<BasicBlock*> Run() {
    std::queue<BasicBlock*> q;
    q.push(entry_);
    while (!q.empty()) {
      BasicBlock* curr = q.front();
      q.pop();
      if (curr->marker())
        continue;
      curr->SetMarker(true);
      vec_.push_back(curr);
      BasicBlock* succ0 = curr->GetTrueSucc();
      BasicBlock* succ1 = curr->GetFalseSucc();

      if (succ0 && !succ0->marker()) q.push(succ0);
      if (succ1 && !succ1->marker()) q.push(succ1);
    }

    for (size_t i = 0; i < vec_.size(); i++) {
      vec_[i]->SetMarker(false);
    }

    return vec_;
  }

 private:
  void travel(BasicBlock* bb);

  BasicBlock* entry_;
  std::vector<BasicBlock*> vec_;
};

#endif // CFG_HPP_
