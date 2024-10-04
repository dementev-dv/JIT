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
    DECL,
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

class DeclInstr final : public Instruction {
 public:
  DeclInstr(size_t bb, DataType type)
    : code_(DECL),
      bb_(bb),
      type_(type) {
  }
};

class Function;

class CallInstr final : public Instruction {
 public:
  CallInstr(size_t bb, DataType type, Function* f)
    : code_(CALL),
      bb_(bb),
      type_(type), 
      callee_(f)


 private:
  std::vector<Instruction*> args_;
  Function* callee_;

}



#endif // INSTR_HPP_
