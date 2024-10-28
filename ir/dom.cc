#include <dom.hpp>
#include <cfg.hpp>
#include <iostream>


using SetIt = typename std::unordered_set<BasicBlock*>::iterator;

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

void DFS::travel(BasicBlock* bb) {
  ASSERT(bb);
  if (bb->marker())
    return;
  bb->SetMarker(true);
  vec_.push_back(bb);
  BasicBlock* succ0 = bb->GetTrueSucc();
  BasicBlock* succ1 = bb->GetFalseSucc();

  if (succ0) travel(succ0);
  if (succ1) travel(succ1);
  bb->SetMarker(false);
}

void ControlFlow::SetDomSlow() {
  DFS dfs(entry_);
  std::vector<BasicBlock*> all = dfs.Run();
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
      // std::cout << (*it)->id() << " dominates " << bb_[i]->id() << std::endl;
      (*it)->AddSub(bb_[i]);
    }
  }
}
