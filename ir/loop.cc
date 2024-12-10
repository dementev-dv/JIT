#include <loop.hpp>
#include <iostream>

void ControlFlow::FindLoop() {
  // SetDomSlow();
  loop_travel(entry_);
  for (size_t i = 0; i < bb_.size(); i++) {
    bb_[i]->mark_black(false);
  }

  for (size_t i = 0; i < loop_.size(); i++) {
    loop_[i]->build();
  }

  for (size_t i = 0; i < bb_.size(); i++) {
    std::cout << bb_[i]->id() << " ";
    if (bb_[i]->GetLoop()) std::cout << bb_[i]->GetLoop() << "\n";
    else std::cout << "null\n";
  }
}

void ControlFlow::loop_travel(BasicBlock* bb) {
  if (!bb || bb->black()) return;
  bb->mark_gray(true);
  bb->mark_black(true);

  // std::cout << "traveling " << bb->id() << "\n";

  if (bb->GetLoop()) {
    std::cout << "got loop\n";
    loop_travel(bb->GetTrueSucc());
    loop_travel(bb->GetFalseSucc());
    return;
  }

  analyze_edge(bb, bb->GetTrueSucc());
  analyze_edge(bb, bb->GetFalseSucc());
  
  bb->mark_gray(false);
}

void ControlFlow::analyze_edge(BasicBlock* src, BasicBlock* dst) {
  if (!dst || !src) return;
  if (dst->black()) return;
  if (!dst->gray()) { loop_travel(dst); return; }

  // std::cout << "analyze edge " << src->id() << " -> " << dst->id() << "\n";

  Loop* loop = dst->GetLoop();
  if (!loop) {
    loop = new Loop(dst, loopid_++);
    loop_.push_back(loop);
    dst->SetLoop(loop);
  }
  if (!dst->Dominate(src))
    loop->MarkIrreducible();
  loop->AddLatch(src);
  loop->AddBlock(src);
  src->SetLoop(loop);
}

void Loop::build() {
  Set blocks = Reachable(header_);
  for (SetIt it = blocks.begin(); it != blocks.end(); it++) {
    for (SetIt jt = latch_.begin(); jt != latch_.end(); jt++) {
      if (Reachable(*it).contains(*jt)) {
        block_.push_back(*it);
        break;
      }
    }
  }

  for (size_t i = 0; i < blocks.size(); i++) {
    BasicBlock* succ0 = block_[i]->GetTrueSucc();
    BasicBlock* succ1 = block_[i]->GetFalseSucc();
    if (succ0 && succ0->GetLoop() != this) {
      AddExit(succ0);
      // if (succ0->GetLoop()) {
      //   succ0->GetLoop()->AddInner(this);
      //   SetOuter(succ0->GetLoop());
      // }
    }
    if (succ1 && succ1->GetLoop() != this)
      AddExit(succ1);
  }
}
