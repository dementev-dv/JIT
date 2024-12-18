#ifndef INSTR_HPP_
#define INSTR_HPP_

#include <type.hpp>
#include <bb.hpp>
#include <func.hpp>

#include <fstream>
#include <vector>
#include <string>

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
  Instruction(DataType type, OpCode code, size_t id)
    : type_(type),
      code_(code),
      use_(0),
      id_(id) { }

  void AddUse(Instruction* i) { use_.push_back(i); }

  DataType type() { return type_; }

  size_t id() { return id_; }

  virtual void Dump(std::ofstream& out) = 0;

  virtual ~Instruction() = default;

 private:
  DataType type_;
  OpCode code_;
  std::vector<Instruction*> use_;
  size_t id_;
};

enum AriCode {
  ADD,
  SUB,
  MUL,
  DIV
};

class ArithmeticInstr final : public Instruction {
 public:
  ArithmeticInstr(size_t id, AriCode code, DataType type,
       Instruction* op1, Instruction* op2)
    : Instruction(type, ARI, id),
      ari_(code),
      op1_(op1),
      op2_(op2) { }

  void Dump(std::ofstream& out);

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
  CompareInstr(size_t id, CmpCode code,
       Instruction* op1, Instruction* op2)
    : Instruction(BOOL, CMP, id),
      cmp_(code),
      op1_(op1),
      op2_(op2) { }

  void Dump(std::ofstream& out);

 private:
  CmpCode cmp_;
  Instruction* op1_;
  Instruction* op2_;
};

class GotoInstr final : public Instruction {
 public:
  GotoInstr(size_t id, BasicBlock* dst)
    : Instruction(VOID, JMP, id),
      dst_(dst) {
  }

  GotoInstr(size_t id)
    : Instruction(VOID, JMP, id),
      dst_(NULL) {
  }

  void SetDst(BasicBlock* dst) { dst_ = dst; }

  void Dump(std::ofstream& out);

 private:
  BasicBlock* dst_;
};

class GotoCondInstr final : public Instruction {
 public:
  GotoCondInstr(size_t id, Instruction* cond,
       BasicBlock* dst1, BasicBlock* dst2)
    : Instruction(VOID, JMPC, id),
      dst1_(dst1),
      dst2_(dst2),
      cond_(cond) { }

  GotoCondInstr(size_t id, Instruction* cond)
    : Instruction(VOID, JMPC, id),
      dst1_(NULL),
      dst2_(NULL),
      cond_(cond) { }

  void SetDst1(BasicBlock* dst) { dst1_ = dst; }
  void SetDst2(BasicBlock* dst) { dst2_ = dst; }

  void Dump(std::ofstream& out);

 private:
  BasicBlock* dst1_;
  BasicBlock* dst2_;
  Instruction* cond_;
};

class ReturnInstr final : public Instruction {
 public:
  ReturnInstr(size_t id, DataType type, Instruction* op)
    : Instruction(type, RET, id),
      op_(op) { }

  void Dump(std::ofstream& out) {
    out << "ret _" << op_->id();
  }

 private:
  Instruction* op_;
};

class CallInstr final : public Instruction {
 public:
  CallInstr(size_t id, DataType type, Function* f)
    : Instruction(type, CALL, id),
      callee_(f),
      args_(0) { }

  void AddArg(Instruction* i) { args_.push_back(i); }

  void Dump(std::ofstream& out);

 private:
  Function* callee_;
  std::vector<Instruction*> args_;
};

class PhiInstr final : public Instruction {
  using PhiArg = typename std::pair<BasicBlock*, Instruction*>;

 public:
  PhiInstr(size_t id, DataType type)
    : Instruction(type, PHI, id),
      args_(0) {
  }

  void AddPhiArg(PhiArg arg) {
    args_.push_back(arg);
  }

  void Dump(std::ofstream& out);

 private:
  std::vector<PhiArg> args_;
};

class MovInstr final : public Instruction {
 public:
  MovInstr(size_t id, DataType type, int64_t data)
    : Instruction(type, MOV, id),
      data_(data) {
  }

  void Dump(std::ofstream& out);

 private:
  int64_t data_;
};

class CastInstr final : public Instruction {
 public:
  CastInstr(size_t id, DataType type,  Instruction* op)
    : Instruction(type, CAST, id),
      op_(op) { }

  void Dump(std::ofstream& out);

 private:
  Instruction* op_;
};

class DeclInstr final : public Instruction {
 public:
  DeclInstr(size_t id, DataType type)
    : Instruction(type, DECL, id) {
  }

  void Dump(std::ofstream& out);
};

#endif // INSTR_HPP_
