#include "eval.h"
#include "gtest/gtest.h"
#include "cons.h"
#include "env.h"
#include "reader.h"

#include <iostream>

using namespace mclisp;

TEST(EvalTest, T)
{
  Reader reader("t");
  ConsCell* exp = reader.Read();
  EXPECT_EQ(*kT, *Eval(exp));
}

TEST(EvalTest, NIL)
{
  Reader reader("nil");
  ConsCell* exp = reader.Read();
  EXPECT_EQ(*kNil, *Eval(exp));
}

TEST(EvalTest, UnboundSymbol)
{
  Reader reader("unbound");
  ConsCell* exp = reader.Read();
  EXPECT_THROW(Eval(exp), env::UnboundSymbolError);
}

TEST(EvalTest, QuotedSymbol)
{
  Reader reader("(quote, foo)");
  ConsCell* exp = reader.Read();
  ConsCell* foo = reader.Intern("foo");
  EXPECT_EQ(*foo, *Eval(exp));
}

TEST(EvalTest, QuotedCons)
{
  Reader reader("(quote, (foo . bar))");
  ConsCell* exp = reader.Read();
  ConsCell* foo = reader.Intern("foo");
  ConsCell* bar = reader.Intern("bar");
  ConsCell* C1 = MakeCons(foo, bar);
  EXPECT_EQ(*C1, *Eval(exp));
}

TEST(EvalTest, QuotedList)
{
  Reader reader("(quote, (foo, bar))");
  ConsCell* exp = reader.Read();
  ConsCell* foo = reader.Intern("foo");
  ConsCell* bar = reader.Intern("bar");
  ConsCell* C1 = MakeCons(bar, kNil);
  ConsCell* C2 = MakeCons(foo, C1);
  EXPECT_EQ(*C2, *Eval(exp));
}
