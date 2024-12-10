#include <type.hpp>
#include <instr.hpp>
#include <func.hpp>
#include <builder.hpp>
#include <cfg.hpp>

int main() {
  Builder build;
  
  auto test = build.Func(U64, "looptest");
  auto n = build.Arg(test, U64);

  build.SetFunc(test);
  auto entry = build.Entry();
  auto a = build.BB();
  auto b = build.BB();
  auto c = build.BB();
  auto d = build.BB();
  auto e = build.BB();
  auto f = build.BB();
  auto g = build.BB();
  auto h = build.BB();
  auto o = build.BB();
  auto r = build.BB();
  auto k = build.BB();
  auto t = build.BB();
  auto v = build.BB();
  auto x = build.BB();
  auto q = build.BB();
  auto w = build.BB();
  auto i = build.BB();
  auto l = build.BB();
  auto end = build.BB();


  entry->SetTrueSucc(a);
  a->AddPrec(entry);
  a->SetTrueSucc(b);
  b->AddPrec(a);
  b->SetTrueSucc(c);
  c->AddPrec(b);
  c->SetTrueSucc(r);
  r->AddPrec(c);
  r->SetTrueSucc(b);
  b->AddPrec(r);
  c->SetFalseSucc(d);
  d->AddPrec(c);
  d->SetTrueSucc(e);
  e->AddPrec(d);
  e->SetTrueSucc(l);
  l->AddPrec(e);
  l->SetTrueSucc(i);
  i->AddPrec(l);
  i->SetTrueSucc(a);
  a->AddPrec(i);
  e->SetFalseSucc(f);
  f->AddPrec(e);
  f->SetTrueSucc(g);
  g->AddPrec(f);
  g->SetTrueSucc(q);
  q->AddPrec(g);
  q->SetTrueSucc(w);
  w->AddPrec(q);
  w->SetTrueSucc(a);
  a->AddPrec(w);
  f->SetFalseSucc(h);
  h->AddPrec(f);
  h->SetTrueSucc(o);
  o->AddPrec(h);
  o->SetTrueSucc(e);
  e->AddPrec(o);
  h->SetFalseSucc(t);
  t->AddPrec(h);
  t->SetTrueSucc(v);
  v->AddPrec(t);
  v->SetFalseSucc(x);
  x->AddPrec(v);
  x->SetTrueSucc(end);
  end->AddPrec(x);
  v->SetTrueSucc(k);
  k->AddPrec(v);
  k->SetTrueSucc(a);
  a->AddPrec(k);


  ControlFlow* cfg = test->cfg();
  cfg->DumpGraph("loop.gv");
  cfg->SetDom();
  cfg->DumpDomTree("testloop.gv");
  cfg->SetIdom();
  cfg->DumpIdomTree("testiloop.gv");


  cfg->FindLoop();

  return 0;
}
