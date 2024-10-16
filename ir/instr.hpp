#ifndef INSTR_HPP_
#define INSTR_HPP_

class Instruction {
 public:
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

  void AddUse(Instruction* i) { use_.push_back(i); }

 private:
  DataType type_;
  OpCode code_;
  std::vector<Instruction*> use_;
};

class ArithmeticInstr final : public Instruction {
 public:
  enum AriCode {
    ADD,
    SUB,
    MUL,
    DIV
  };

  ArithmeticInstr(AriCode code, , DataType type
       Instruction* op1, Instruction* op2)
    : ari_(code),
      op1_(op1),
      op2_(op2),
      code_(ARI),
      type_(type) {
    op1->AddUse((Instruction*) this);
    op2->AddUse((Instruction*) this);
  }

 private:
  AriCode ari_;
  Instruction* op1_;
  Instruction* op2_;
};

class CompareInstr final : public Instruction {
 public:
  enum CmpCode {
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE
  };

  CompareInstr(CmpCode code,
       Instriction* op1, Instruction* op2)
    : cmp_(code),
      op1_(op1),
      op2_(op2),
      code_(CMP),
      type_(BOOL) {
    op1->AddUse((Instruction*) this);
    op2->AddUse((Instruction*) this);
  }

 private:
  CmpCode cmp_;
  Instruction* op1_;
  Instruction* op2_;
};

class GotoInstr final : public Instruction {
 public:
  GotoInstr(BasicBlock* dst)
    : code_(JMP),
      type_(VOID),
      dst_(dst) {
  }

  GotoInstr()
    : code_(JMP),
      type_(VOID),
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
    : code_(JMPC),
      type_(void),
      dst1_(dst1),
      dst2_(dst),
      cond_(cond) {
    cond->AddUse((instruction*) this);
  }

  GotoCondInstr(Instruction* cond)
    : code_(JMPC),
      type_(void),
      dst1_(NULL),
      dst2_(NULL),
      cond_(cond) {
    cond->AddUse((instruction*) this);
  }

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
    : code_(RET),
      op_(op),
      type_(type) {
    if (op_ && type != VOID)
      op->AddUse((Instruction*) this);
  }

 private:
  Instruction* op_;
};

class Function;

class CallInstr final : public Instruction {
 public:
  CallInstr(DataType type, Function* f,
     std::vector<Instruction*> args)
    : code_(CALL),
      type_(type), 
      callee_(f) 
      args_(args) {
    for (const Instruction* i : args_)
      i->AddUse((Instruction*) this);
  }

  CallInstr(DataType type, Function* f,
     std::initializer_list<Instruction*> args)
    : code_(CALL),
      type_(type), 
      callee_(f) 
      args_(args) {
    for (const Instruction* i : args_)
      i->AddUse((Instruction*) this);
  }

  CallInstr(DataType type, Function* f)
    : type_(type),
      callee_(f),
      args_(0) {
  }

 private:
  std::vector<Instruction*> args_;
  Function* callee_;
};

class PhiInstr final : public Instruction {
  using PhiArg = std::pair<BasicBlock*, Instruction*>;

 public:
  PhiInstr(DataType type)
    : code_(PHI),
      type_(type),
      args_(0) {
  }

  void AddPhiArg(BasicBlock* bb, Instruction* op) {
    assert(type_ == op->type());
    PhiArg arg = std::pair<BasicBlock*, Instruction*>(bb, op);
    args_.push_back(arg);
    op->AddUse((Instruction*) this);
  }

 private:
  std::vector<PhiArg> args_;
};

class MovIntInstr final : public Instruction {
 public:
  MovIntInstr(DataType type, int64_t data)
    : code_(MOV),
      type_(type),
      data_(data) {
  }

 private:
  int64_t data_;
};

class CastInstr final : public Instruction {
 public:
  CastInstr(DataType type,  Instruction* op)
    : code_(CAST),
      type_(type),
      op_(op) {
    op->AddUse((Instruction*) this);
  }

 private:
  Instruction* op_;
};

#endif // INSTR_HPP_
