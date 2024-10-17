#ifndef TYPES_HPP_
#define TYPES_HPP_

enum DataType {
  VOID,
  BOOL,
  I8,
  U8,
  I16,
  U16,
  I32,
  U32,
  I64,
  U64
};

/* Check if t1 instr can be casted to t2 */

bool familiar(DataType t1, DataType t2) {
  if (t1 == VOID || t2 == VOID)
    return false;
  return true;
}

#endif // TYPES_HPP_
