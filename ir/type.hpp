#ifndef TYPE_HPP_
#define TYPE_HPP_

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

bool familiar(DataType t1, DataType t2);

#endif // TYPE_HPP_
