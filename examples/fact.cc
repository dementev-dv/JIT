#include <types.hpp>
#include <instr.hpp>
#include <func.hpp>
#include <builder.hpp>

int main() {
  Builder build;
  
  auto fact = build.Func(U64, "factorial", 1);
  auto n = build.Arg(U64);
  fact->AddArg(n);

  auto bb_init = build.BB(fact);
  auto bb_cond = build.BB(fact);
  auto bb_body = build.BB(fact);
  auto bb_end  = build.BB(fact);

  auto f0  = build.Mov(U64, 1);
  auto i0  = build.Mov(U64, 1);
  auto inc = build.Mov(U64, 1);

  bb_init->AddInstr(f0);
  bb_init->AddInstr(i0);
  bb_init->AddInstr(inc);

  auto f1 = build.Phi();
  auto i1 = build.Phi();
  auto cond = build.Cmp(LE, i1, n);
  auto jmp0 = build.GotoCond(cond, bb_body, bb_end);

  bb_cond->AddInstr(f1);
  bb_cond->AddInstr(i1);
  bb_cond->AddInstr(cond);
  bb_cond->AddInstr(jmp0);

  auto f2 = build.Mul(f1, i1);
  auto i2 = build.Add(i1, inc);
  auto jmp1 = build.Goto(bb_cond);

  bb_body->AddInstr(f2);
  bb_body->AddInstr(i2);
  bb_body->AddInstr(jmp1);

  auto ret = build.Ret(i1);

  bb_end->AddInstr(f);
  bb_end->AddInstr(ret);

  f1->AddPhiArg(bb_init, f0);
  f1->AddPhiArg(bb_body, f2);

  i1->AddPhiArg(bb_init, i0);
  i1->AddPhiArg(bb_body, i2);

  build.Dump("graph.gv");
}