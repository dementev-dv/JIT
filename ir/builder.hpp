#ifndef BUILDER_HPP_
#define BUILDER_HPP_

#include <types.hpp>
#include <instr.hpp>
#include <bb.hpp>
#include <func.hpp>

class Builder final {
 public:
  Builder() = default;

  Function* Func(DataType type, const char* name);
  DeclInstr* Arg(Function* f, DataType type);

  BasicBlock* BB(Function* f);
  BasicBlock* Entry(Function* f);

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

  void AddPhiArg(PhiInstr* phi, BasicBlock* bb, Instruction* i) {
    i->AddUse(phi);
    phi->AddPhiArg(std::pair<BasicBlock*, Instruction*>(bb, i));
  }

  CallInstr* Call(Function* f, std::initializer_list<Instruction*> args);
  CallInstr* Call(Function* f, std::vector<Instruction*> args);
  CallInstr* Call(Function* f);

  CastInstr* Cast(DataType type, Instruction* op);

  ReturnInstr* Ret(Instruction* op);
  ReturnInstr* Ret();

  void SetBB(BasicBlock* bb) { curr_ = bb; }

 private:
  ArithmeticInstr* Ari(AriCode code, Instruction* op1, Instruction* op2);
  BasicBlock* curr_;
  size_t insid_{0};
  size_t bbid_{0};
};

Function* Builder::Func(DataType type, const char* name) {
  Function* f = new Function(type, name);
  return f;
}

BasicBlock* Builder::BB(Function* f) {
  return new BasicBlock(bbid_++, f);
}

BasicBlock* Builder::Entry(Function* f) {
  BasicBlock* bb = new BasicBlock(bbid_++, f);
  f->SetEntry(bb);
  return bb;
}

DeclInstr* Builder::Arg(Function* f, DataType type) {
  DeclInstr* i = new DeclInstr(insid_++, type);
  f->AddArg(i);
  return i;
}

ArithmeticInstr* Builder::Ari(AriCode code, Instruction* op1, Instruction* op2) {
  ArithmeticInstr* i = new ArithmeticInstr(insid_++, code, op1->type(), op1, op2);
  op1->AddUse(i);
  op2->AddUse(i);
  curr_->AddInstr(i);
  return i;
}

ArithmeticInstr* Builder::Add(Instruction* op1, Instruction* op2) {
  return Ari(ADD, op1, op2);
}

ArithmeticInstr* Builder::Sub(Instruction* op1, Instruction* op2) {
  return Ari(SUB, op1, op2);
}

ArithmeticInstr* Builder::Mul(Instruction* op1, Instruction* op2) {
  return Ari(MUL, op1, op2);
}

ArithmeticInstr* Builder::Div(Instruction* op1, Instruction* op2) {
  return Ari(DIV, op1, op2);
}

CompareInstr* Builder::Cmp(CmpCode code, Instruction* op1, Instruction* op2) {
  CompareInstr* i = new CompareInstr(insid_++, code, op1, op2);
  op1->AddUse(i);
  op2->AddUse(i);
  curr_->AddInstr(i);
  return i;
}

GotoInstr* Builder::Goto(BasicBlock* dst) {
  GotoInstr* i = new GotoInstr(insid_++, dst);
  curr_->AddInstr(i);
  curr_->SetTrueSucc(dst);
  dst->AddPrec(curr_);
  return i;
}

GotoCondInstr* Builder::GotoCond(Instruction* cond, BasicBlock* dst1, BasicBlock* dst2) {
  GotoCondInstr* i = new GotoCondInstr(insid_++, cond, dst1, dst2);
  curr_->SetTrueSucc(dst1);
  curr_->SetFalseSucc(dst2);
  dst1->AddPrec(curr_);
  dst2->AddPrec(curr_);
  return i;
}

PhiInstr* Builder::Phi(DataType type) {
  PhiInstr* i = new PhiInstr(insid_++, type);
  curr_->AddPhi(i);
  return i;
}

CallInstr* Builder::Call(Function* f, std::vector<Instruction*> args) {
  CallInstr * i = new CallInstr(insid_++, f->type(), f);

  for (size_t arg = 0; arg < args.size(); arg++) {
    i->AddArg(args[arg]);
    args[arg]->AddUse(i);
  }

  curr_->AddInstr(i);
  return i;
}

CallInstr* Builder::Call(Function* f, std::initializer_list<Instruction*> args) {
  std::vector<Instruction*> a{args};
  CallInstr * i = new CallInstr(insid_++, f->type(), f);

  for (size_t arg = 0; arg < a.size(); arg++) {
    i->AddArg(a[arg]);
  }

  curr_->AddInstr(i);
  return i;
}

CallInstr* Builder::Call(Function* f) {
  CallInstr* i = new CallInstr(insid_++, f->type(), f);
  curr_->AddInstr(i);
  return i;
}

ReturnInstr* Builder::Ret(Instruction* op) {
  ReturnInstr* i = new ReturnInstr(insid_++, op->type(), op);
  op->AddUse(i);
  curr_->AddInstr(i);
  return i;
}

ReturnInstr* Builder::Ret() {
  ReturnInstr* i = new ReturnInstr(insid_++, VOID, NULL);
  curr_->AddInstr(i);
  return i;
}

MovInstr* Builder::Mov(DataType type, int64_t val) {
  MovInstr* i = new MovInstr(insid_++, type, val);
  curr_->AddInstr(i);
  return i;
}

CastInstr* Builder::Cast(DataType type, Instruction* op) {
  CastInstr* i = new CastInstr(insid_++, type, op);
  curr_->AddInstr(i);
  op->AddUse(i);
  return i;
}

#endif // BUILDER_HPP_
