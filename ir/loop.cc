#include <loop.hpp>
#include <iostream>

class RPO {
 public:
  RPO(BasicBlock* e)
    : entry_(e),
      count_(Reachable(e).size()),
      res_(count_) {
    // std::cout << "rpo from " << entry_->id() << std::endl;
  }

  void DFS(BasicBlock* bb) {
    if (!bb) return;
    bb->SetMarker(true);

    if (bb->GetTrueSucc()) {
      if (!bb->GetTrueSucc()->marker())
        DFS(bb->GetTrueSucc());
    }

    if (bb->GetFalseSucc()) {
      if (!bb->GetFalseSucc()->marker())
        DFS(bb->GetFalseSucc());
    }
    // std::cout << "Add bb " << bb->id() << " on pos " << count_ << std::endl;
    res_[--count_] = bb;
  }

  std::vector<BasicBlock*> Run() {
    DFS(entry_);
    for (size_t i = 0; i < res_.size(); i ++) {
      res_[i]->SetMarker(false);
    }
    return res_;
  }

 private:
  size_t count_;
  std::vector<BasicBlock*> res_;
  BasicBlock* entry_;
};

void ControlFlow::FindLoop() {
  // SetDomSlow();
  loop_travel(entry_);
  for (size_t i = 0; i < bb_.size(); i++) {
    bb_[i]->mark_black(false);
  }

  RPO order(entry_);
  auto rpo = order.Run();
  for (size_t i = 0; i < rpo.size(); i ++) {
    // std::cout << rpo[i]->id() << " is in rpo " << i << std::endl;
    if (rpo[i]->GetLoop() != nullptr && rpo[i]->GetLoop()->Header() == rpo[i]) {
      // std::cout << i << " " << rpo[i]->id() << " is header for " << rpo[i]->GetLoop()->id() << std::endl;
      rpo[i]->GetLoop()->build();
    }
  }

  ASSERT(entry_->GetLoop() == nullptr);
  Loop* root = new Loop(entry_, loopid_++);
  loop_.push_back(root);
  entry_->SetLoop(root);

  for (size_t i = 0; i < loop_.size(); i++) {
    if (loop_[i]->Outer() == nullptr)
      loop_[i]->SetOuter(root);
  }

  for (size_t i = 0; i < bb_.size(); i++) {
    if (bb_[i]->GetLoop() == nullptr) {
      root->AddBlock(bb_[i]);
      bb_[i]->SetLoop(root);
    }
  }

  std::cout << entry_->id() << " ";
  if (entry_->GetLoop() != nullptr) {
    std::cout << entry_->GetLoop()->id() << std::endl;
  }
  else std::cout << "null\n";

  for (size_t i = 0; i < bb_.size(); i++) {
    std::cout << bb_[i]->id() << " ";
    if (bb_[i]->GetLoop() != nullptr) {
      std::cout << bb_[i]->GetLoop()->id() << std::endl;
    }
    else std::cout << "null\n";
  }

  // for (size_t i = 0; i < loop_.size(); i++) {
  //   BasicBlock* h = loop_[i]->Header();
  //   for (size_t j = 0; j < h->nprecs(); h++) {

  //   }
  // }
}

void ControlFlow::loop_travel(BasicBlock* bb) {
  if (!bb || bb->black()) return;
  bb->mark_gray(true);
  bb->mark_black(true);

  // std::cout << "traveling " << bb->id() << "\n";

  if (bb->GetLoop() != nullptr) {
    // std::cout << "got loop\n";
    loop_travel(bb->GetTrueSucc());
    loop_travel(bb->GetFalseSucc());
    return;
  }

  analyze_edge(bb, bb->GetTrueSucc());
  analyze_edge(bb, bb->GetFalseSucc());
  
  bb->mark_gray(false);
}

void ControlFlow::analyze_edge(BasicBlock* src, BasicBlock* dst) {
  // std::cout << "src = " << src->id() << std::endl;
  if (!dst || !src) return;
  if (dst == nullptr || src == nullptr) return;
  // std::cout << "dst = " << dst->id() << std::endl;
  // if (dst->black()) return;
  if (!dst->gray()) { loop_travel(dst); return; }

  // std::cout << "analyze edge " << src->id() << " -> " << dst->id() << "\n";

  Loop* loop = dst->GetLoop();
  if (loop == nullptr) {
    // std::cout << "new loop " << loopid_ << " " << dst << std::endl;
    loop = new Loop(dst, loopid_++);
    loop_.push_back(loop);
    dst->SetLoop(loop);
  }
  if (!dst->Subordinate(src)) {
    // std::cout << dst->id() << " not dominate on " << src->id() << std::endl;
    loop->MarkIrreducible();
    // src->SetLoop(loop);
    // return;
  }
  loop->AddLatch(src);
  src->SetLoop(loop);
}

void Loop::build() {
  if (!reducible_) {
    std::cout << "irreducible loop " << id_ << std::endl;
    return;
  }
  // std::cout << "build loop " << id_ << std::endl;
  RPO rpo(header_);
  auto vec = rpo.Run();

  for (size_t i = 0; i < vec.size(); i ++) {
    if (!header_->Subordinate(vec[i])) continue;
    if (!Reachable(vec[i]).contains(header_)) continue;
    if (vec[i]->GetLoop() != nullptr) {
      if (vec[i]->GetLoop()->Outer() == nullptr && vec[i]->GetLoop() != this)
        vec[i]->GetLoop()->SetOuter(this);
      continue;
    }
    // std::cout << "Add bb " << vec[i]->id() << std::endl;
    AddBlock(vec[i]);
    vec[i]->SetLoop(this);
  }
}
