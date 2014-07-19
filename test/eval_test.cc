#include "eval.h"
#include "reader.h"

#include <map>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

namespace
{
using namespace mclisp;

class EvalTest :
  public ::testing::TestWithParam< std::tuple<const char*, const char*> >
{
  private:
    static std::map<std::string, ConsCell*> objects_;
    static bool objects_initialized_;

  protected:
    EvalTest()
    {
      if (!objects_initialized_)
      {
        ConsCell *foo = reader::Intern("FOO");
        ConsCell *bar = reader::Intern("BAR");

        objects_.emplace("t", kT);
        objects_.emplace("nil", kNil);
        objects_.emplace("foo", foo);
        objects_.emplace("bar", bar);
        objects_.emplace("foobar", Cons(foo, bar));
        objects_.emplace("barfoo", Cons(bar, foo));
        objects_.emplace("foo_list", List(foo));
        objects_.emplace("bar_list", List(bar));
        objects_.emplace("foobar_list", List(foo, bar));
        objects_initialized_ = true;
      }

      expr_ = std::get<0>(GetParam());
      expect_ = objects_[std::string(std::get<1>(GetParam()))];
    }

    std::string expr_;
    ConsCell* expect_;
};

std::map<std::string, ConsCell*> EvalTest::objects_;
bool EvalTest::objects_initialized_ = false;

#define TUP std::make_tuple
INSTANTIATE_TEST_CASE_P(
 Eval, EvalTest,
 ::testing::Values(
  TUP("t", "t"),
  TUP("nil", "nil"),

  TUP("(quote, foo)", "foo"),
  TUP("(quote, (foo . bar))", "foobar"),
  TUP("(quote, (foo, bar))", "foobar_list"),

  TUP("(atom, t)", "t"),
  TUP("(atom, nil)", "t"),
  TUP("(atom, 'foo)", "t"),
  TUP("(atom, 'foo bar)", "t"),
  TUP("(atom, 'apple pie number 3)", "t"),
  TUP("(atom, '(foo))", "nil"),
  TUP("(atom, '(foo, bar))", "nil"),
  TUP("(atom, '(foo . bar))", "nil"),

  TUP("(eq, t, t)", "t"),
  TUP("(eq, nil, nil)", "t"),
  TUP("(eq, 'foo, 'foo)", "t"),
  TUP("(eq, t, nil)", "nil"),
  TUP("(eq, 'foo, 'bar)", "nil"),
  TUP("(eq, 'foo, '(foo))", "nil"),
  TUP("(eq, '(foo), '(foo))", "nil"),

  TUP("(cons, 'foo, 'bar)", "foobar"),
  TUP("(cons, 'foo, '(bar))", "foobar_list"),

  TUP("(car, '(foo, bar))", "foo"),
  TUP("(car, '((foo, bar), foo))", "foobar_list"),

  TUP("(cdr, '(foo, bar))", "bar_list"),
  TUP("(cdr, '((foo, bar), foo))", "foo_list"),

  TUP("(cond)", "nil"),
  TUP("(cond, (nil, t))", "nil"),
  TUP("(cond, (nil, unbound))", "nil"),
  TUP("(cond, (nil, t), (t, 'foo))", "foo"),
  TUP("(cond, (nil, t), ((atom, 'foo), 'foo))", "foo")));
#undef TUP

ConsCell* EvalString(const std::string& expr)
{
  Reader reader(expr);
  return Eval(reader.Read());
}

TEST_P(EvalTest, TestExpr)
{
  EXPECT_EQ(*expect_, *EvalString(expr_));
}

TEST(EvalTest, MalformedCondThrowsTypeError)
{
  EXPECT_THROW(EvalString("(cond, t)"), TypeError);
}

TEST(EvalTest, UnboundSymbol)
{
  EXPECT_THROW(EvalString("unbound"), env::UnboundSymbolError);
}
} // namespace
