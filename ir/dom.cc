#include <dom.hpp>
#include <set.hpp>
#include <iostream>

void ControlFlow::travel(BasicBlock* bb, std::vector<BasicBlock*>& vec) {
  ASSERT(bb);
  if (bb->marker())
    return;
  bb->SetMarker(true);
  vec.push_back(bb);
  BasicBlock* succ0 = bb->GetTrueSucc();
  BasicBlock* succ1 = bb->GetFalseSucc();

  if (succ0) travel(succ0, vec);
  if (succ1) travel(succ1, vec);
}

void ControlFlow::SetDomSlow() {
  std::vector<BasicBlock*> all = DFS();
  Set r = Reachable(entry_);

  for (size_t i = 0; i < all.size(); i++) {
    all[i]->SetMarker(true);
    Set s = Reachable(entry_);
    all[i]->SetMarker(false);
    Set sub = Substract(r, s);
    for (SetIt it = sub.begin(); it != sub.end(); it++) {
      all[i]->AddSub(*it);
      (*it)->AddDom(all[i]);
    }
  }
}

void ControlFlow::SetDom() {
  Set all = Reachable(entry_);
  std::unordered_map<BasicBlock*, Set> dom;
  dom.emplace(entry_, Set{entry_});

  for (size_t i = 0; i < bb_.size(); i++) {
    dom.emplace(bb_[i], all);
  }

  bool changed = true;
  while (changed) {
    changed = false;
    for (size_t i = 0; i < bb_.size(); i++) {
      Set tmp = all;
      for (size_t j = 0; j < bb_[i]->nprecs(); j++) {
        tmp = Intersect(tmp, dom[bb_[i]->GetPrec(j)]);
        tmp.insert(bb_[i]);
      }
      if (Differ(tmp, dom[bb_[i]])) {
        dom[bb_[i]] = tmp;
        changed = true;
      }
    }
  }

  for (size_t i = 0; i < bb_.size(); i++) {
    for (SetIt it = dom[bb_[i]].begin(); it != dom[bb_[i]].end(); it++) {
      bb_[i]->AddDom(*it);
      (*it)->AddSub(bb_[i]);
    }
  }
}

void ControlFlow::SetIdom() {
  find_idom(entry_->GetTrueSucc());
  find_idom(entry_->GetFalseSucc());
}

void ControlFlow::find_idom(BasicBlock* bb) {
  if (!bb) return;
  if (bb->GetIdom() != nullptr) return;

  if (bb->Dom().size() == 1) {
    // std::cout << "idom(" << bb->id() << ") = " << *(bb->Dom().begin());
    bb->SetIdom(*(bb->Dom().begin()));
  } else {
    Set cand = bb->Dom();
    size_t n = 0;
    BasicBlock* closest = nullptr;
    for (SetIt it = cand.begin(); it != cand.end(); it++) {
      if (*it == bb)
        continue;
      // std::cout << "cand(" << bb->id() << ") = " << (*it)->id() << std::endl;
      if ((*it)->Dom().size() >= n) {
        n = (*it)->Dom().size();
        closest = *it;
      }
    }
    bb->SetIdom(closest);
  }

  find_idom(bb->GetTrueSucc());
  find_idom(bb->GetFalseSucc());
}
