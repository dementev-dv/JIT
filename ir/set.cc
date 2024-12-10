#include <set.hpp>
#include <bb.hpp>

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

bool BasicBlock::Dominate(BasicBlock* bb) { return dom_.contains(bb); }
bool BasicBlock::Subordinate(BasicBlock* bb) { return sub_.contains(bb); }

void BasicBlock::AddDom(BasicBlock* bb) { if (!dom_.contains(bb)) dom_.insert(bb); }
void BasicBlock::AddSub(BasicBlock* bb) { if (!sub_.contains(bb)) sub_.insert(bb); }
