#include <type.hpp>

bool familiar(DataType t1, DataType t2) {
  if (t1 == VOID || t2 == VOID)
    return false;
  return true;
}
