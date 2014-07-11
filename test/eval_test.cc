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
  ConsCell* C1 = Cons(foo, bar);
  EXPECT_EQ(*C1, *Eval(exp));
}

TEST(EvalTest, QuotedList)
{
  Reader reader("(quote, (foo, bar))");
  ConsCell* exp = reader.Read();
  ConsCell* foo = reader.Intern("foo");
  ConsCell* bar = reader.Intern("bar");
  ConsCell* C1 = Cons(bar, kNil);
  ConsCell* C2 = Cons(foo, C1);
  EXPECT_EQ(*C2, *Eval(exp));
}

class EvalAtomTest :
  public ::testing::TestWithParam< std::tuple<std::string, ConsCell *> >
{
  protected:
    EvalAtomTest():
      expr_(std::get<0>(GetParam())),
      expected_(std::get<1>(GetParam()))
    {}

    std::string expr_;
    ConsCell *expected_;
};

using ::testing::Combine;
using ::testing::Values;

auto Symbols = Values("nil", "t", "'foo", "'foo bar", "'apple pie number 3");
auto Lists = Values("nil", "'(foo)", "'(foo, bar)", "'(foo . bar)");
auto Conses = Values("'(foo)", "'(foo, bar)", "'(foo . bar)");

INSTANTIATE_TEST_CASE_P(SymbolsAreAtoms, EvalAtomTest, Combine(Symbols, Values(kT)));
INSTANTIATE_TEST_CASE_P(ConsessAreNotAtoms, EvalAtomTest, Combine(Conses, Values(kNil)));

TEST_P(EvalAtomTest, Atom)
{
  Reader reader("(atom, " + expr_ + ")");
  ConsCell* exp = reader.Read();
  EXPECT_EQ(*expected_, *Eval(exp));
}
