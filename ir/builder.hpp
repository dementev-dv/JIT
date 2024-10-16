#ifndef BUILDER_HPP_
#define BUILDER_HPP_

class Builder final {
 public:
  Builder();

  Function* Func(DataType type, const char* name);
  Function* Func(DataType type, const char* name,
      std::initializer_list<DataType> args);
  Function* Func(DataType type, const char* name,
      std::vector<DataType> args);

  BasicBlock* BB(Function* f);

  Instruction* Add(Instruction* op1, Instruction* op2);
  Instruction* Sub(Instruction* op1, Instruction* op2);
  Instruction* Mul(Instruction* op1, Instruction* op2);
  Instruction* Div(Instruction* op1, Instruction* op2);

  Instruction* Cmp(CmpCode code, Instruction* op1, Instruction* op2);

  Instruction* Goto(BasicBlock* dst);
  Instruction* Goto();

  Instruction* GotoCond(Instruction* cond, BasicBlock* dst1, BasicBlock* dst2);  
  Instruction* GotoCond(Instruction* cond);

  Instruction* Mov(int64_t data);

  Instruction* Phi();

  Instruction* Call(Function* f, std::initializer_list<Instruction*> args);
  Instruction* Call(Function* f, std::vector<Instruction*> args);

  Instruction* Cast(DataType type, Instruction* op);

  Instruction* Ret(Instruction* op);

 private:
  Instruction* Ari(Instruction* op1, Instruction* op2);


  std::vector<Function*> func_;

  std::vector<BasicBlock*> bb_;
  size_t id_;
};

BasicBlock* Builder::BB(Function* f) {
    BasicBlock bb = new BasicBlock(f, id_++);
    bb_.push_back(bb);
    f->AddBB(bb);
    return bb;
}

Function* Builder::Func(DataType type, const char* name) {
  Function* f = new Funnction(type, name);
  func_.push_back(f);
  return f;
}

Function* Builder::Func(DataType type, const char* name,
    std::initializer_list<DataType> args) {
  Function* f = new Funnction(type, name, args);
  func_.push_back(f);
  return f;
}

Function* Builder::Func(DataType type, const char* name,
    std::vector<DataType> args) {
  Function* f = new Funnction(type, name, args);
  func_.push_back(f);
  return f;
}

Instruction* Builder::Ari(AriCode code, Instruction* op1, Instruction* op2) {
  assert(op1->type() == op2->type());
  assert(op1->type() != VOID)
  return (Instruction*) new ArithmeticInstr(code, op1, op2, op1.type());
}

Instruction* Builder::Add(Instruction* op1, Instruction* op2) {
  return Ari(ADD, op1, op2);
}

Instruction* Builder::Sub(Instruction* op1, Instruction* op2) {
  return Ari(SUB, op1, op2);
}

Instruction* Builder::Mul(Instruction* op1, Instruction* op2) {
  return Ari(MUL, op1, op2);
}

Instruction* Builder::Div(Instruction* op1, Instruction* op2) {
  return Ari(DIV, op1, op2);
}

Instruction* Builder::Cmp(CmpCode code, Instruction* op1, Instruction* op2) {
  assert(op1->type() == op2->type());
  assert(op1->type() != VOID);
  return (Instruction*) new CompareInstr(code, op1, op2);
}

Instruction* Builder::Goto(BasicVlock* dst) {
  return (Instruction*) new GotoInstr(dst);
}

void Builder::GotoCondInstr(Instruction* cond, BasicBlock* dst1, BasicBlock* bb2) {
  return (Instruction*) new GotoCondInstr(cond, dst1, dst2);
}

Instruction* Builder::Phi(DataType type) {
  return (Instruction*) new PhiInstr(type);
}

Instruction* Builder::Call(Function* f, std::vector<Instruction*> args) {
  assert(args.size() == f->argn());
  for (size_t arg = 0; arg < args.size(); arg++) {
    assert(args[arg]->type() == f->arg(arg)->type());
  }
  return (Instruction*) new CallInstr(f->type(), f, args);
}

Instruction* Builder::Call(Function* f, std::initializer_list<Instruction*> args) {
  assert(args.size() == f->argn());
  for (size_t arg = 0; arg < args.size(); arg++) {
    assert(args[arg]->type() == f->arg(arg)->type());
  }
  return (Instruction*) new CallInstr(f->type(), f, args);
}

Instruction* Builder::Call(Function* f) {
  assert(f.argn() == 0);
  return (Instruction*) new CallInstr(f->type(), f);
}

Instruction* Builder::Ret(Instruction* op) {
  return (Instruction*) new ReturnInstr(op->type(), op);
}

Instruction* Builder::Ret() {
  return (Instruction*) new ReturnInstr(VOID, NULL);
}

Instruction* Builder::Mov(DataType type, int64_t val) {
  return (Instruction*) new MovInstr(type, val);
}

Instruction* Builder::Cast(DataType type, Instruction* i) {
  assert(familiar(t1, t2));
  return (Instruction*) new CastInstr(type, i);
}

#endif // BUILDER_HPP_
