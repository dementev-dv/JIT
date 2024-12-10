#include <type.hpp>
#include <bb.hpp>
#include <func.hpp>
#include <instr.hpp>
#include <cfg.hpp>
#include <loop.hpp>

#include <iterator>
// #include <iostream>

const char* str(DataType type) {
  switch(type) {
    case VOID:  return "void";
    case BOOL:  return "bool";
    case I8:    return "i8";
    case U8:    return "u8";
    case I16:   return "i16";
    case U16:   return "u16";
    case I32:   return "i32";
    case U32:   return "u32";
    case I64:   return "i64";
    case U64:   return "u64";
    default:    return "UNKNOWN";
  }
  return "UNKNOWN";
}

void ArithmeticInstr::Dump(std::ofstream& out) {
  out << str(type());
  out << " _" << id() << " = ";
  out << "_" << op1_->id();
  switch(ari_) {
    case ADD: out << " + "; break;
    case SUB: out << " - "; break;
    case MUL: out << " * "; break;
    case DIV: out << " / "; break;
    default: out << " unknown arithmetic op "; break;
  }
  out << "_" << op2_->id();
}

void CompareInstr::Dump(std::ofstream& out) {
  out << str(type());
  out << " _" << id() << " = ";
  out << "_" << op1_->id();
  switch(cmp_) {
    case EQ: out << " == "; break;
    case NE: out << " != "; break;
    case LT: out << " < "; break;
    case LE: out << " <= "; break;
    case GT: out << " > "; break;
    case GE: out << " >= "; break;
  }
  out << "_" << op2_->id();
}

void GotoInstr::Dump(std::ofstream& out) {
  out << "goto [bb_" << dst_->id() << "]";
}

void GotoCondInstr::Dump(std::ofstream& out) {
  out << "if _" << cond_->id();
  out << " goto [bb_" << dst1_->id() << "]";
  out << " else goto [bb_" << dst2_->id() << "]";
}

void CallInstr::Dump(std::ofstream& out) {
  out << callee_->name() << "(";
  for (size_t i = 0; i < args_.size(); i ++) {
    out << "_" << args_[i]->id();
    if (i != args_.size() - 1) out << ", ";
  }
  out << ")";
}

void PhiInstr::Dump(std::ofstream& out) {
  out << str(type()) << " _" << id() << " = phi <";
  for (size_t i = 0; i < args_.size(); i++) {
    out << "bb_" << args_[i].first->id() << ": _" << args_[i].second->id();
    if (i != args_.size() - 1)
      out << ", ";
  }
}

void MovInstr::Dump(std::ofstream& out) {
  out << str(type()) << " _" << id() << " = " << data_;
}

void CastInstr::Dump(std::ofstream& out) {
  out << str(type()) << " _" << id() << " = ";
  out << str(type()) << " _" << op_->id();
}

void DeclInstr::Dump(std::ofstream& out) {
  out << "param " << str(type()) << " _" << id();
}

void ControlFlow::DumpIR(const char* path) {
  std::ofstream out(path);
  out << func_->name() << ":\n";
  for (size_t i = 0; i < func_->argn(); i++) {
    out << "\t";
    func_->arg(i)->Dump(out);
    out << "\n";
  }
  out << "\n";
  entry_->Dump(out);
    out << "\n";
  for (size_t i = 0; i < bb_.size(); i ++) {
    bb_[i]->Dump(out);
    out << "\n";
  }
  out << "\n";
}

void ControlFlow::DumpGraph(const char* path) {
  std::ofstream out(path);
  out << "digraph " << func_->name() << " {\n";
  out << "\tpeek" << entry_ << "[label = \"" << entry_->id() << "\"]\n";
  if (entry_->GetTrueSucc())
    out << "\t\tpeek" << entry_ << " -> peek" << entry_->GetTrueSucc() << "\n";
  if (entry_->GetFalseSucc())
    out << "\t\tpeek" << entry_ << " -> peek" << entry_->GetFalseSucc() << "\n";

  for (size_t i = 0; i < bb_.size(); i++) {
    out << "\tpeek" << bb_[i] << "[label = \"" << bb_[i]->id() << "\"]\n";
    if (bb_[i]->GetTrueSucc())
      out << "\t\tpeek" << bb_[i] << " -> peek" << bb_[i]->GetTrueSucc() << "\n";
    if (bb_[i]->GetFalseSucc())
      out << "\t\tpeek" << bb_[i] << " -> peek" << bb_[i]->GetFalseSucc() << "\n";
  }
  out << "}\n";
}

void ControlFlow::DumpDomTree(const char* path) {
  std::ofstream out(path);
  out << "digraph DomTree_" << func_->name() << " {\n";
  out << "\tpeek" << entry_ << "[label = \"" << entry_->id() << "\"]\n";
  Set& sub = entry_->Sub();
  for (SetIt it = sub.begin(); it != sub.end(); it++) {
    out << "\t\tpeek" << entry_ << " -> peek" << *it << "\n";
  }
  for (size_t i = 0; i < bb_.size(); i++) {
    sub = bb_[i]->Sub();
    out << "\tpeek" << bb_[i] << "[label = \"" << bb_[i]->id() << "\"]\n";
    for (SetIt it = sub.begin(); it != sub.end(); it++) {
      out << "\t\tpeek" << bb_[i] << " -> peek" << *it << "\n";
    }
  }
  out << "}\n";
}

void ControlFlow::DumpIdomTree(const char* path) {
  std::ofstream out(path);
  out << "digraph IdomTree_" << func_->name() << " {\n";
  out << "\tpeek" << entry_ << "[label = " << entry_->id() << "]\n";

  for (size_t i = 0; i < bb_.size(); i++) {
    out << "\tpeek" << bb_[i] << "[label = " << bb_[i]->id() << "]\n";
    if (bb_[i]->GetIdom())
      out << "\t\tpeek" << bb_[i]->GetIdom() << " -> peek" << bb_[i] << "\n";
  }
  out << "}\n";
}

void BasicBlock::Dump(std::ofstream& out) {
  // std::cout << "bb" << id_ << std::endl;
  // std::cout << phi_.size() << " phis" << std::endl;
  // std::cout << instr_.size() << " instr" << std::endl;
  out << "<bb_" << id_ << ">\n";

  for (std::list<PhiInstr*>::iterator phi = phi_.begin(); phi != phi_.end(); phi++) {
    out << "\t";
    (*phi)->Dump(out);
    out << "\n";
  }

  for (std::list<Instruction*>::iterator ins = instr_.begin(); ins != instr_.end(); ins++) {
    out << "\t";
    (*ins)->Dump(out);
    out << "\n";
  }
}

void ControlFlow::DumpLoopTree(const char *path) {
  std::ofstream out(path);
  out << "digraph LoopTree_" << func_->name() << " {\n";
  for (size_t i = 0; i < loop_.size(); i ++) {
    out << "\tpeek" << loop_[i] << "[label = \"" << loop_[i]->id() << "\"]\n";
    if (loop_[i]->Outer())
      out << "\t\tpeek" << loop_[i]->Outer() << " -> peek" << loop_[i] << "\n";
  }
  out << "}\n";
}
