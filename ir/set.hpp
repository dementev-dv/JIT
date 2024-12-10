#ifndef SET_HPP_
#define SET_HPP_

#include <unordered_set>
#include <iterator>

class BasicBlock;

using Set = typename std::unordered_set<BasicBlock*>;
using SetIt = typename std::unordered_set<BasicBlock*>::iterator;

Set Intersect(Set& s1, Set& s2);
Set Substract(Set& s1, Set& s2);
Set Reachable(BasicBlock* bb);
bool Differ(Set& s1, Set& s2);

#endif // SET_HPP_
