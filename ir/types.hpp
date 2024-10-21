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

const char* str(DataType type) {
  switch(type) {
    case VOID:  return "void";
    case BOOL:  return "bool";
    case I8:    return "i8";
    case U8:    return "u8";
    case I16:   return "i16";
    case U16:   return "u16";
    case I32:   return "i32";
    case U32:   return "u32";
    case I64:   return "i64";
    case U64:   return "u64";
    default:    return "UNKNOWN";
  }
  return "UNKNOWN";
}

/* Check if t1 instr can be casted to t2 */

bool familiar(DataType t1, DataType t2) {
  if (t1 == VOID || t2 == VOID)
    return false;
  return true;
}

#endif // TYPES_HPP_
