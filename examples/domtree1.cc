#include <type.hpp>
#include <instr.hpp>
#include <func.hpp>
#include <builder.hpp>
#include <cfg.hpp>

int main() {
  Builder build;
  
  auto test = build.Func(U64, "test");
  auto n = build.Arg(test, U64);

  build.SetFunc(test);
  auto a = build.Entry();
  auto b = build.BB();
  auto c = build.BB();
  auto d = build.BB();
  auto e = build.BB();
  auto f = build.BB();
  auto g = build.BB();

  a->SetTrueSucc(b);
  b->AddPrec(a);
  b->SetTrueSucc(c);
  c->AddPrec(b);
  b->SetFalseSucc(f);
  f->AddPrec(b);
  c->SetTrueSucc(d);
  d->AddPrec(c);
  e->SetTrueSucc(d);
  d->AddPrec(e);
  g->SetTrueSucc(d);
  d->AddPrec(g);
  f->SetTrueSucc(e);
  e->AddPrec(f);
  f->SetFalseSucc(g);
  g->AddPrec(f);

  ControlFlow* cfg = test->cfg();
  cfg->SetDom();
  cfg->SetIdom();
  cfg->DumpGraph("test.gv");
  // cfg->SetDom();
  cfg->SetDom();
  cfg->DumpDomTree("testdom.gv");
  cfg->DumpIdomTree("testidom.gv");

  cfg->FindLoop();

  return 0;
}
