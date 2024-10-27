#ifndef BUILDER_HPP_
#define BUILDER_HPP_

#include <type.hpp>
#include <instr.hpp>
#include <bb.hpp>
#include <func.hpp>
#include <assert.hpp>

#include <vector>

class Builder final {
 public:
  Builder() = default;

  Function* Func(DataType type, const char* name);
  DeclInstr* Arg(Function* f, DataType type);

  BasicBlock* BB();
  BasicBlock* Entry();

  ArithmeticInstr* Add(Instruction* op1, Instruction* op2);
  ArithmeticInstr* Sub(Instruction* op1, Instruction* op2);
  ArithmeticInstr* Mul(Instruction* op1, Instruction* op2);
  ArithmeticInstr* Div(Instruction* op1, Instruction* op2);

  CompareInstr* Cmp(CmpCode code, Instruction* op1, Instruction* op2);

  GotoInstr* Goto(BasicBlock* dst);
  GotoInstr* Goto();

  GotoCondInstr* GotoCond(Instruction* cond, BasicBlock* dst1, BasicBlock* dst2);  
  GotoCondInstr* GotoCond(Instruction* cond);

  MovInstr* Mov(DataType type, int64_t data);

  PhiInstr* Phi(DataType type);

  void AddPhiArg(PhiInstr* phi, BasicBlock* bb, Instruction* i);

  CallInstr* Call(Function* f, std::initializer_list<Instruction*> args);
  CallInstr* Call(Function* f, std::vector<Instruction*> args);
  CallInstr* Call(Function* f);

  CastInstr* Cast(DataType type, Instruction* op);

  ReturnInstr* Ret(Instruction* op);
  ReturnInstr* Ret();

  void SetBB(BasicBlock* bb) { curr_ = bb; }
  void SetFunc(Function* f) { cfg_ = f->cfg(); }

  ControlFlow* CFG() { return cfg_; }

 private:
  ArithmeticInstr* Ari(AriCode code, Instruction* op1, Instruction* op2);

  BasicBlock* curr_;
  ControlFlow* cfg_;
};

#endif // BUILDER_HPP_
