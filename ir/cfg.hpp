#ifndef CFG_HPP_
#define CFG_HPP_

#include <type.hpp>
#include <bb.hpp>
#include <assert.hpp>

#include <fstream>
#include <queue>

class Function;

class Loop;

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
  void SetIdom();
  void FindLoop();

  void DumpIR(const char* path);
  void DumpGraph(const char* path);
  void DumpDomTree(const char* path);
  void DumpIdomTree(const char* path);

 private:
  void travel(BasicBlock* bb, std::vector<BasicBlock*>& vec);
  void find_idom(BasicBlock* bb);

  std::vector<BasicBlock*> DFS() {
    std::vector<BasicBlock*> vec;
    travel(entry_, vec);
    entry_->SetMarker(false);
    for (size_t i = 0; i < bb_.size(); i++) {
      bb_[i]->SetMarker(false);
    }
    return vec;
  }

  std::vector<BasicBlock*> BFS() {
    std::queue<BasicBlock*> q;
    q.push(entry_);
    std::vector<BasicBlock*> vec;
    while (!q.empty()) {
      BasicBlock* curr = q.front();
      q.pop();
      if (curr->marker())
        continue;
      curr->SetMarker(true);
      vec.push_back(curr);
      BasicBlock* succ0 = curr->GetTrueSucc();
      BasicBlock* succ1 = curr->GetFalseSucc();

      if (succ0 && !succ0->marker()) q.push(succ0);
      if (succ1 && !succ1->marker()) q.push(succ1);
    }

    for (size_t i = 0; i < vec.size(); i++) {
      vec[i]->SetMarker(false);
    }

    return vec;
  }

  BasicBlock* entry_{nullptr};
  std::vector<BasicBlock*> bb_;
  std::vector<Loop*> loop_;
  size_t bbid_{0};
  size_t insid_{0};
  Function* func_;
};

#endif // CFG_HPP_
