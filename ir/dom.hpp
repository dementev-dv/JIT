#ifndef DOM_HPP_
#define DOM_HPP_

#include <cfg.hpp>

#include <unordered_set>

using Set = typename std::unordered_set<BasicBlock*>;

Set Intersect(Set& s1, Set& s2);
Set Substract(Set& s1, Set& s2);
Set Reachable(BasicBlock* bb);

#endif // DOM_HPP_
