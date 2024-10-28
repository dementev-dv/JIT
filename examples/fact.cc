#include <type.hpp>
#include <instr.hpp>
#include <func.hpp>
#include <builder.hpp>
#include <cfg.hpp>

int main() {
  Builder build;
  
  auto fact = build.Func(U64, "factorial");
  auto n = build.Arg(fact, U64);

  build.SetFunc(fact);
  auto bb_init = build.Entry();
  auto bb_cond = build.BB();
  auto bb_body = build.BB();
  auto bb_end  = build.BB();

  build.SetBB(bb_init);
  auto f0  = build.Mov(U64, 1);
  auto i0  = build.Mov(U64, 1);
  auto inc = build.Mov(U64, 1);
  build.Goto(bb_cond);

  build.SetBB(bb_cond);
  auto f1 = build.Phi(U64);
  auto i1 = build.Phi(U64);
  auto cond = build.Cmp(LE, i1, n);
  build.GotoCond(cond, bb_body, bb_end);
  
  build.SetBB(bb_body);
  auto f2 = build.Mul(f1, i1);
  auto i2 = build.Add(i1, inc);
  build.Goto(bb_cond);
  
  build.SetBB(bb_end);
  build.Ret(i1);

  build.AddPhiArg(f1, bb_init, f0);
  build.AddPhiArg(f1, bb_body, f2);

  build.AddPhiArg(i1, bb_init, i0);
  build.AddPhiArg(i1, bb_body, i2);

  ControlFlow* cfg = fact->cfg();
  cfg->DumpGraph("fact.gv");
  cfg->DumpIR("fact.txt");

  return 0;
}