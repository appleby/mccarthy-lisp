#include "eval.h"
#include "env.h"
#include "mclisp_test.h"

#include <iostream>
#include <map>
#include <sstream>
#include <tuple>

namespace
{
using namespace mclisp;

class EvalTest :
  public mclisp::testing::Test,
  public ::testing::WithParamInterface<std::tuple<const char*, const char*>>
{
  private:
    static std::map<std::string, ConsCell*> objects_;
    static bool objects_initialized_;

  protected:
    EvalTest()
    {
      if (!objects_initialized_)
      {
        objects_.emplace("t", kT);
        objects_.emplace("nil", kNil);
        objects_.emplace("foo", foo_);
        objects_.emplace("bar", bar_);
        objects_.emplace("foobar", foobar_);
        objects_.emplace("foo_list", foo_list_);
        objects_.emplace("bar_list", bar_list_);
        objects_.emplace("foobar_list", foobar_list_);
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
  TUP("(eq, (), nil)", "t"),
  TUP("(eq, (  ), nil)", "t"),
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
  TUP("(cond, (nil, t), ((atom, 'foo), 'foo))", "foo"),

  TUP("(defun, foo, nil, 'foo)", "foo"),
  TUP("(foo)", "foo"),
  TUP("(defun, foo, (x), x)", "foo"),
  TUP("(foo, 'bar)", "bar"),
  TUP("(defun, foo, (), 'foo)", "foo"),
  TUP("(foo)", "foo"),
  TUP("(defun, foo, args, args)", "foo"),
  TUP("(foo)", "nil"),
  TUP("(foo, 'bar)", "bar_list"),
  TUP("(foo, 'foo, 'bar)", "foobar_list"),

  TUP("(load, 'test/loadtest)", "nil"),
  TUP("(loadtest)", "foo"),

  // Note: label shadows the binding created by defun.
  TUP("((label, foo,"
      "   (lambda, (x),"
      "      (cond, ((atom, x), x),"
      "             (t, (foo, (car, x)))))),"
      " '((foo, bar), bar))",
      "foo"),

  TUP("((lambda, nil, 'foo))", "foo"),
  TUP("((lambda, (x), x), 'bar)", "bar"),
  TUP("((lambda, (x, y), (cons, x, y)), 'foo, 'bar)", "foobar")
  ));
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

TEST(EvalTest, Print)
{
  // This test is handled separately because of the need to capture std::cerr.
  std::ostringstream oss;
  std::streambuf* old = std::cerr.rdbuf(oss.rdbuf());
  EvalString("(print, '(foo, bar))");
  std::cerr.rdbuf(old);
  EXPECT_EQ("(FOO, BAR)\n", oss.str());
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
