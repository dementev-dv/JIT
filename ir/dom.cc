#include <dom.hpp>
#include <cfg.hpp>


Set Intersect(Set& s1, Set& s2) {
  Set inter;
  for (SetIt it = s1.begin(); it != s1.end(); it++) {
    if (s2.contains(*it))
      inter.insert(*it);
  }
  return inter;
}

Set Substract(Set& s1, Set& s2) {
  Set sub;
  for (SetIt it = s1.begin(); it != s1.end(); it++) {
    if (!s2.contains(*it))
      sub.insert(*it);
  }
  return sub;
}

bool Differ(Set& s1, Set& s2) {
  for (SetIt it = s1.begin(); it != s1.end(); it++) {
    if (!s2.contains(*it))
      return true;
  }
  for (SetIt it = s2.begin(); it != s2.end(); it++) {
    if (!s1.contains(*it))
      return true;
  }
  return false;
}

Set Reachable(BasicBlock* bb) {
  Set res(0);
  if (bb->marker()) return res;

  res.insert(bb);
  bb->SetMarker(true);
  BasicBlock* succ0 = bb->GetTrueSucc();
  BasicBlock* succ1 = bb->GetFalseSucc();

  if (succ0) res.merge(Reachable(succ0));
  if (succ1) res.merge(Reachable(succ1));
  bb->SetMarker(false);
  return res;
}

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
  Set cand = Reachable(entry_);

  if (bb->nprecs() == 1) {
    bb->SetIdom(bb->GetPrec(0));
  } else {
    for (size_t i = 0; i < bb->nprecs(); i++) {
      Set& dom = bb->GetPrec(i)->Dom();
      cand = Intersect(cand, bb->GetPrec(i)->Dom());
    }
    ASSERT(!cand.empty());
    size_t n = 0;
    BasicBlock* closest;
    for (SetIt it = cand.begin(); it != cand.end(); it++) {
      if ((*it)->Dom().size() > n) {
        n = (*it)->Dom().size();
        closest = *it;
      }
    }
    bb->SetIdom(closest);
  }

  find_idom(bb->GetTrueSucc());
  find_idom(bb->GetFalseSucc());
}
