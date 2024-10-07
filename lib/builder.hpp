#ifndef BUILDER_HPP_
#define BUILDER_HPP_

class Builder final {
 public:
  Builder();

  Function* Func(DataType type, const char* name) {
    Function* f = new Funnction(type, name);
    func_.push_back(f);
    return f;
  }

  Function* Func(DataType type, const char* name,
      std::initializer_list<DataType> args) {
    Function* f = new Funnction(type, name, args);
    func_.push_back(f);
    return f;
  }

  Function* Func(DataType type, const char* name,
      std::vector<DataType> args) {
    Function* f = new Funnction(type, name, args);
    func_.push_back(f);
    return f;
  }

  BasicBlock* BB(Function* f) {
    BasicBlock bb = new BasicBlock(f, id_++);
    bb_.push_back(bb);
    Func.AddBB(bb);
    return bb;
  }

  Instruction* Add(size_t bb, Instruction* op1, Instruction* op2);
  Instruction* Sub(size_t bb, Instruction* op1, Instruction* op2);
  Instruction* Mul(size_t bb, Instruction* op1, Instruction* op2);
  Instruction* Div(size_t bb, Instruction* op1, Instruction* op2);

  Instruction* Cmp(size_t bb, CmpCode code, Instruction* op1, Instruction* op2);

  Instruction* Goto(size_t bb, size_t dst);
  Instruction* Goto(size_t bb);

  Instruction* GotoCond(size_t bb, Instruction* cond,
     size_t dst1, size_t dst2);  
  Instruction* GotoCond(size_t bb, Instruction* cond);

  Instruction* Mov(size_t bb, int64_t data);

  Instruction* Phi(size_t bb);

  Instruction* Call(size_t bb, Function* f, std::initializer_list<Instruction*> args);
  Instruction* Call(size_t bb, Function* f, std::vector<Instruction*> args);

  Instruction* Cast(size_t bb, DataType type, Instruction* op);

  Instruction* Ret(size_t bb, Instruction* op);

 private:
  Instruction* Ari(size_t bb, Instruction* op1, Instruction* op2);


  std::vector<Function*> func_;

  std::vector<BasicBlock*> bb_;
  size_t id_;
};

Instruction* Builder::Ari(size_t bbid, AriCode code, Instruction* op1, Instruction* op2) {
  assert(op1.type() == op2.type());
  assert(op1.type() != VOID)
  ArithmeticInstr* a = new ArithmeticInstr(bbid, code, op1, op2, op1.type());
  Instruction* i = (Instruction*) a;
  bb_[bbid].AddInstr(i);
  return i;
}

Instruction* Builder::Add(size_t bbid, Instruction* op1, Instruction* op2) {
  return Ari(bbid, ADD, op1, op2);
}

Instruction* Builder::Sub(size_t bbid, Instruction* op1, Instruction* op2) {
  return Ari(bbid, SUB, op1, op2);
}

Instruction* Builder::Mul(size_t bbid, Instruction* op1, Instruction* op2) {
  return Ari(bbid, MUL, op1, op2);
}

Instruction* Builder::Div(size_t bbid, Instruction* op1, Instruction* op2) {
  return Ari(bbid, DIV, op1, op2);
}

Instruction* Builder::Cmp(size_t bbid, CmpCode code, Instruction* op1, Instruction* op2) {
  assert(op1.type() == op2.type());
  assert(op1.type() != VOID);
  CompareInstr* c = new CompareInstr(bbid, code, op1, op2);
  Instruction* i = (Instruction*) c;
  bb_[bbid].AddInstr(i);
  return i;
}

void Builder::Goto(size_t bbid, size_t dst) {
  GotoInstr* g = new GotoInstr(bbid, dst);
  Instruction* i = (Instruction*) g;
  bb_[bbid].AddInstr(i);
  bb_[bbid].AddSucc(dst);
  bb_[dst].AddPrec(bbid);
}

void Builder::GotoCondInstr(size_t bbid, Instruction* cond, size_t dst1, size_t bb2) {
  GotoCondInstr* g = new GotoCondInstr(bbid, cond, dst1, dst2);
  Instruction* i = (Instruction*) g;
  bb_[bbid].AddInstr(i);
  bb_[bbid].AddSucc(dst1);
  bb_[bbid].AddSucc(dst2);
  bb_[dst1].AddPrec(bbid);
  bb_[dst2].AddPrec(bbid);
}

Instruction* Builder::Phi(size_t bbid, DataType type) {
  PhiInstr* p = new PhiInstr(bbid, type);
  Instruction* i = (Instruction*) p;
  bb_[bbid].AddPhi(i);
}

Instruction* Builder::Call(size_t bbid, Function* f, std::vector<Instruction*> args) {
  assert(args.size() == f.argn());
  for (size_t arg = 0; arg < args.size(); arg++) {
    assert(args[arg].type() == f.arg(arg).type());
  }
  CallInstr* c = new CallInstr(bbid, f.type(), f, args);
  Instruction* i = (Instruction*) c;
  bb_[bbid].AddInstr(i);
  return i;
}

Instruction* Builder::Call(size_t bbid, Function* f, std::initializer_list<Instruction*> args) {
  assert(args.size() == f.argn());
  for (size_t arg = 0; arg < args.size(); arg++) {
    assert(args[arg].type() == f.arg(arg).type());
  }
  CallInstr* c = new CallInstr(bbid, f.type(), f, args);
  Instruction* i = (Instruction*) c;
  bb_[bbid].AddInstr(i);
  return i;
}

Instruction* Builder::Call(size_t bbid, Function* f) {
  CallInstr* c = new CallInstr(bbid, f.type(), f);
  Instruction* i = (Instruction*) c;
  bb_[bbid].AddInstr(i);
  return i;
}

Instruction* Builder::Ret(size_t bbid, Instruction* op) {
  ReturnInstr* r = new ReturnInstr(bbid, op.type(), op);
  Instruction* i = (Instruction*) r;
  bb_[bbid].AddInstr(i);
  return i;
}

Instruction* Builder::Ret(size_t bbid) {
  ReturnInstr* r = new ReturnInstr(bbid, VOID, NULL);
  Instruction* i = (Instruction*) r;
  bb_[bbid].AddInstr(i);
  return i;
}

#endif // BUILDER_HPP_
