#include "eval.h"
#include "gtest/gtest.h"
#include "cons.h"
#include "env.h"
#include "reader.h"

#include <iostream>

// TODO Reduce boilerplate in these tests.

namespace
{
using namespace mclisp;

ConsCell* EvalString(const std::string& expr)
{
  Reader reader(expr);
  return Eval(reader.Read());
}

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

TEST(EvalTest, Eq)
{
  EXPECT_EQ(*kT, *EvalString("(eq, t, t)"));
  EXPECT_EQ(*kT, *EvalString("(eq, nil, nil)"));
  EXPECT_EQ(*kT, *EvalString("(eq, 'foo, 'foo)"));

  EXPECT_EQ(*kNil, *EvalString("(eq, t, nil)"));
  EXPECT_EQ(*kNil, *EvalString("(eq, 'foo, 'bar)"));

  EXPECT_EQ(*kNil, *EvalString("(eq, 'foo, '(foo))"));
  EXPECT_EQ(*kNil, *EvalString("(eq, '(foo), '(foo))"));
}

TEST(EvalTest, Cons)
{
  Reader reader("(cons, 'foo, 'bar)");
  ConsCell *foo = reader.Intern("FOO");
  ConsCell *bar = reader.Intern("BAR");
  ConsCell *cons = Cons(foo, bar);
  EXPECT_EQ(*cons, *Eval(reader.Read()));
}

TEST(EvalTest, Car)
{
  Reader reader("(car, '(foo, bar))");
  ConsCell *foo = reader.Intern("FOO");
  EXPECT_EQ(*foo, *Eval(reader.Read()));
}

TEST(EvalTest, Cdr)
{
  Reader reader("(cdr, '(foo, bar))");
  ConsCell *bar = reader.Intern("BAR");
  ConsCell *cons = List(bar);
  EXPECT_EQ(*cons, *Eval(reader.Read()));
}

TEST(EvalTest, EmptyCondIsNil)
{
  Reader reader("(cond)");
  EXPECT_EQ(*kNil, *Eval(reader.Read()));
}

TEST(EvalTest, NonMatchingCondIsNil)
{
  Reader reader("(cond, (nil, t))");
  EXPECT_EQ(*kNil, *Eval(reader.Read()));
}

TEST(EvalTest, NonMatchingCondConsequentIsNotEvaled)
{
  Reader reader("(cond, (nil, unbound))");
  EXPECT_EQ(*kNil, *Eval(reader.Read()));
}

TEST(EvalTest, MatchingCond)
{
  Reader reader("(cond, (nil, t), (t, 'foo))");
  ConsCell* foo = reader.Intern("FOO");
  EXPECT_EQ(*foo, *Eval(reader.Read()));
}

TEST(EvalTest, MatchingCondWithPredicate)
{
  Reader reader("(cond, (nil, t), ((atom, 'foo), 'foo))");
  ConsCell* foo = reader.Intern("FOO");
  EXPECT_EQ(*foo, *Eval(reader.Read()));
}

TEST(EvalTest, MalformedCondThrowsTypeError)
{
  EXPECT_THROW(EvalString("(cond, t)"), TypeError);
}
} // namespace
