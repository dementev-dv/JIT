#ifndef ASSERT_HPP_
#define ASSERT_HPP_

#include <iostream>

#define DEBUG 1

#define ASSERT(cond)                      \
  do {                                    \
  if (!(cond)) [[unlikely]] {             \
    std::cout << "assertion failed";      \
    std::cout << " in " << __FUNCTION__;  \
    std::cout << " line" << __LINE__;     \
    std::cout << std::endl;               \
  }                                       \
  }                                       \
  while(0)

#endif // ASSERT_HPP_
