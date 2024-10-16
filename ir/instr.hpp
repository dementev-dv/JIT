#ifndef INSTR_HPP_
#define INSTR_HPP_

#include <types.hpp>

#include <vector>
#include <initializer_list>

class BasicBlock;

enum OpCode {
  ARI,
  CMP,
  JMP,
  JMPC,
  RET,
  CALL,
  CAST,
  MOV,
  PHI,
  DECL
};

class Instruction {
 public:
  Instruction(DataType type, OpCode code)
    : type_(type),
      code_(code),
      use_(0) { }

  void AddUse(Instruction* i) { use_.push_back(i); }

  DataType type() { return type_; }

 private:
  DataType type_;
  OpCode code_;
  std::vector<Instruction*> use_;
};

enum AriCode {
  ADD,
  SUB,
  MUL,
  DIV
};

class ArithmeticInstr final : public Instruction {
 public:
  ArithmeticInstr(AriCode code, DataType type,
       Instruction* op1, Instruction* op2)
    : Instruction(type, ARI),
      ari_(code),
      op1_(op1),
      op2_(op2) { }

 private:
  AriCode ari_;
  Instruction* op1_;
  Instruction* op2_;
};

enum CmpCode {
  EQ,
  NE,
  LT,
  LE,
  GT,
  GE
};

class CompareInstr final : public Instruction {
 public:
  CompareInstr(CmpCode code,
       Instruction* op1, Instruction* op2)
    : Instruction(BOOL, CMP),
      cmp_(code),
      op1_(op1),
      op2_(op2) { }

 private:
  CmpCode cmp_;
  Instruction* op1_;
  Instruction* op2_;
};

class GotoInstr final : public Instruction {
 public:
  GotoInstr(BasicBlock* dst)
    : Instruction(VOID, JMP),
      dst_(dst) {
  }

  GotoInstr()
    : Instruction(VOID, JMP),
      dst_(NULL) {
  }

  void SetDst(BasicBlock* dst) { dst_ = dst; }

 private:
  BasicBlock* dst_;
};

class GotoCondInstr final : public Instruction {
 public:
  GotoCondInstr(Instruction* cond,
       BasicBlock* dst1, BasicBlock* dst2)
    : Instruction(VOID, JMPC),
      dst1_(dst1),
      dst2_(dst2),
      cond_(cond) { }

  GotoCondInstr(Instruction* cond)
    : Instruction(VOID, JMPC),
      dst1_(NULL),
      dst2_(NULL),
      cond_(cond) { }

  void SetDst1(BasicBlock* dst) { dst1_ = dst; }
  void SetDst2(BasicBlock* dst) { dst2_ = dst; }

 private:
  BasicBlock* dst1_;
  BasicBlock* dst2_;
  Instruction* cond_;
};

class ReturnInstr final : public Instruction {
 public:
  ReturnInstr(DataType type, Instruction* op)
    : Instruction(type, RET),
      op_(op) { }

 private:
  Instruction* op_;
};

class Function;

class CallInstr final : public Instruction {
 public:
  CallInstr(DataType type, Function* f)
    : Instruction(type, CALL),
      callee_(f),
      args_(0) { }

 private:
  Function* callee_;
  std::vector<Instruction*> args_;
};

class PhiInstr final : public Instruction {
  using PhiArg = typename std::pair<BasicBlock*, Instruction*>;

 public:
  PhiInstr(DataType type)
    : Instruction(type, PHI),
      args_(0) {
  }

  void AddPhiArg(BasicBlock* bb, Instruction* op) {
    PhiArg arg = std::pair<BasicBlock*, Instruction*>(bb, op);
    args_.push_back(arg);
  }

 private:
  std::vector<PhiArg> args_;
};

class MovInstr final : public Instruction {
 public:
  MovInstr(DataType type, int64_t data)
    : Instruction(type, MOV),
      data_(data) {
  }

 private:
  int64_t data_;
};

class CastInstr final : public Instruction {
 public:
  CastInstr(DataType type,  Instruction* op)
    : Instruction(type, CAST),
      op_(op) { }

 private:
  Instruction* op_;
};

#endif // INSTR_HPP_
