#include <builder.hpp>

void Builder::AddPhiArg(PhiInstr* phi, BasicBlock* bb, Instruction* i) {
    ASSERT((phi->type() == i->type()));
    i->AddUse(phi);
    phi->AddPhiArg(std::pair<BasicBlock*, Instruction*>(bb, i));
}

Function* Builder::Func(DataType type, const char* name) {
  Function* f = new Function(type, name);
  f->SetCFG(new ControlFlow(f));
  return f;
}

BasicBlock* Builder::BB() {
  BasicBlock* bb = new BasicBlock(cfg_->bbid(), cfg_->func());
  cfg_->AddBB(bb);
  return bb;
}

BasicBlock* Builder::Entry() {
  BasicBlock* bb = new BasicBlock(cfg_->bbid(), cfg_->func());
  cfg_->SetEntry(bb);
  return bb;
}

DeclInstr* Builder::Arg(Function* f, DataType type) {
  DeclInstr* i = new DeclInstr(cfg_->insid(), type);
  f->AddArg(i);
  return i;
}

ArithmeticInstr* Builder::Ari(AriCode code, Instruction* op1, Instruction* op2) {
  ArithmeticInstr* i = new ArithmeticInstr(cfg_->insid(), code, op1->type(), op1, op2);
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
  CompareInstr* i = new CompareInstr(cfg_->insid(), code, op1, op2);
  op1->AddUse(i);
  op2->AddUse(i);
  curr_->AddInstr(i);
  return i;
}

GotoInstr* Builder::Goto(BasicBlock* dst) {
  GotoInstr* i = new GotoInstr(cfg_->insid(), dst);
  curr_->AddInstr(i);
  curr_->SetTrueSucc(dst);
  dst->AddPrec(curr_);
  return i;
}

GotoCondInstr* Builder::GotoCond(Instruction* cond, BasicBlock* dst1, BasicBlock* dst2) {
  GotoCondInstr* i = new GotoCondInstr(cfg_->insid(), cond, dst1, dst2);
  curr_->AddInstr(i);
  curr_->SetTrueSucc(dst1);
  curr_->SetFalseSucc(dst2);
  dst1->AddPrec(curr_);
  dst2->AddPrec(curr_);
  return i;
}

PhiInstr* Builder::Phi(DataType type) {
  PhiInstr* i = new PhiInstr(cfg_->insid(), type);
  curr_->AddPhi(i);
  return i;
}

CallInstr* Builder::Call(Function* f, std::vector<Instruction*> args) {
  CallInstr * i = new CallInstr(cfg_->insid(), f->type(), f);

  for (size_t arg = 0; arg < args.size(); arg++) {
    i->AddArg(args[arg]);
    args[arg]->AddUse(i);
  }

  curr_->AddInstr(i);
  return i;
}

CallInstr* Builder::Call(Function* f, std::initializer_list<Instruction*> args) {
  std::vector<Instruction*> a{args};
  CallInstr * i = new CallInstr(cfg_->insid(), f->type(), f);

  for (size_t arg = 0; arg < a.size(); arg++) {
    i->AddArg(a[arg]);
  }

  curr_->AddInstr(i);
  return i;
}

CallInstr* Builder::Call(Function* f) {
  CallInstr* i = new CallInstr(cfg_->insid(), f->type(), f);
  curr_->AddInstr(i);
  return i;
}

ReturnInstr* Builder::Ret(Instruction* op) {
  ReturnInstr* i = new ReturnInstr(cfg_->insid(), op->type(), op);
  op->AddUse(i);
  curr_->AddInstr(i);
  return i;
}

ReturnInstr* Builder::Ret() {
  ReturnInstr* i = new ReturnInstr(cfg_->insid(), VOID, NULL);
  curr_->AddInstr(i);
  return i;
}

MovInstr* Builder::Mov(DataType type, int64_t val) {
  MovInstr* i = new MovInstr(cfg_->insid(), type, val);
  curr_->AddInstr(i);
  return i;
}

CastInstr* Builder::Cast(DataType type, Instruction* op) {
  CastInstr* i = new CastInstr(cfg_->insid(), type, op);
  curr_->AddInstr(i);
  op->AddUse(i);
  return i;
}