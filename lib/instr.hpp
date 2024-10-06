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
    PHI
  }; 

  void AddUse(Instruction* i) { use_.push_back(i); }

 private:
  DataType type_;
  OpCode code_;
  size_t bb_;
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

  ArithmeticInstr(size_t bb, AriCode code,
       Instruction* op1, Instruction* op2, DataType type)
    : ari_(code),
      op1_(op1),
      op2_(op2),
      code_(ARI),
      type_(type),
      bb_(bb) {
    op1.AddUse((Instruction*) this);
    op2.AddUse((Instruction*) this);
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

  CompareInstr(size_t bb, CmpCode code,
       Instriction* op1, Instruction* op2)
    : cmp_(code),
      op1_(op1),
      op2_(op2),
      code_(CMP),
      type_(BOOL),
      bb_(bb) {
    op1.AddUse((Instruction*) this);
    op2.AddUse((Instruction*) this);
  }

 private:
  CmpCode cmp_;
  Instruction* op1_;
  Instruction* op2_;
};

class GotoInstr final : public Instruction {
 public:
  GotoInstr(size_t bb, size_t dst)
    : code_(JMP),
      type_(VOID),
      dst_(dst),
      bb_(bb) {
  }

 private:
  size_t dst_;
};

class GotoCondInstr final : public Instruction {
 public:
  GotoCondInstr(size_t bb, Instruction* cond,
       Instruction* dst1, Instruction* dst2)
    : code_(JMPC),
      type_(void),
      bb_(bb),
      dst1_(dst1),
      dst2_(dst),
      cond_(cond) {
    cond.AddUse((instruction*) this);
  }

 private:
  Instruction* dst1_;
  Instruction* dst2_;
  Instruction* cond_;
};

class ReturnInstr final : public Instruction {
 public:
  ReturnInstr(size_t bb, DataType type
       Instruction* op)
    : code_(RET),
      bb_(bb),
      op_(op),
      type_(type) {
    if (op_ && type != VOID)
      op_.AddUse((Instruction*) this);
  }

 private:
  Instruction* op_;
};

class Function;

class CallInstr final : public Instruction {
 public:
  CallInstr(size_t bb, DataType type,
     Function* f, std::vector<Instruction*> args)
    : code_(CALL),
      bb_(bb),
      type_(type), 
      callee_(f) 
      args_(args) {
    for (const Instruction* i : args_)
      i.AddUse((Instruction*) this);
  }

  CallInstr(size_t bb, DataType type,
     Function* f, std::initializer_list<Instruction*> args)
    : code_(CALL),
      bb_(bb),
      type_(type), 
      callee_(f) 
      args_(args) {
    for (const Instruction* i : args_)
      i.AddUse((Instruction*) this);
  }

 private:
  std::vector<Instruction*> args_;
  Function* callee_;
};

class PhiInstr final : public Instruction {
  using PhiArg = std::pair<size_t, Instruction*>;

 public:
  PhiInstr(size_t bb, DataType type,
     std::vector<PhiArg> args)
    : code_(PHI),
      bb_(bb),
      type_(type),
      args_(args) {
    for (const PhiArg a : args_)
      a.second.AddUse((Instruction*) this);
  }

  PhiInstr(size_t bb, DataType type,
     std::initializer_list<PhiArg> args)
    : code_(PHI),
      bb_(bb),
      type_(type),
      args_(args) {
    for (const PhiArg a : args_)
      a.second.AddUse((Instruction*) this);
  }

 private:
  std::vector<PhiArg> args_;
};

class MovIntInstr final : public Instruction {
 public:
  MovIntInstr(size_t bb, DataType type,
     int64_t data)
    : code_(MOV),
      bb_(bb),
      type_(type),
      data_(data) {
  }

 private:
  int64_t data_;
};

class CastInstr final : public Instruction {
 public:
  CastInstr(size_t bb, DataType type,
     Instruction* op)
    : code_(CAST),
      bb_(bb),
      type_(type),
      op_(op) {
    op.AddUse((Instruction*) this);
  }

 private:
  Instruction* op_;
};

#endif // INSTR_HPP_
