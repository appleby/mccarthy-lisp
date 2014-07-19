#ifndef MCLISP_TEST_H_
#define MCLISP_TEST_H_

#include "gtest/gtest.h"

#include "cons.h"
#include "error.h"
#include "reader.h"

namespace mclisp
{
namespace testing
{
class Test : public ::testing::Test
{
  protected:
    Test()
    {
      foo_ = reader::Intern("FOO");
      bar_ = reader::Intern("BAR");
      foobar_ = Cons(foo_, bar_);
      barfoo_ = Cons(bar_, foo_);
      foobarbarfoo_ = Cons(foobar_, barfoo_);
      foo_list_ = List(foo_);
      bar_list_ = List(bar_);
      foobar_list_ = List(foo_, bar_);
    }
    ConsCell* foo_;
    ConsCell* bar_;
    ConsCell* foobar_;
    ConsCell* barfoo_;
    ConsCell* foobarbarfoo_;
    ConsCell* foo_list_;
    ConsCell* bar_list_;
    ConsCell* foobar_list_;
};
} // namespace testing
} // namespace mclisp
#endif // MCLISP_TEST_H_
