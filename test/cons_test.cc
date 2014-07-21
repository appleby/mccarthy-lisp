#include "cons.h"
#include "mclisp_test.h"

namespace
{
using namespace mclisp;

class ConsTest : public mclisp::testing::Test {};

TEST_F(ConsTest, FromBool)
{
  EXPECT_EQ(*kT, *FromBool(true));
  EXPECT_EQ(*kNil, *FromBool(false));
}

TEST_F(ConsTest, Atom)
{
  EXPECT_TRUE(Atom(foo_));
  EXPECT_FALSE(Atom(foobar_));
}

TEST_F(ConsTest, Symbolp)
{
  EXPECT_TRUE(Symbolp(foo_));
  EXPECT_FALSE(Symbolp(foobar_));
}

TEST_F(ConsTest, Consp)
{
  EXPECT_TRUE(Consp(foobar_));
  EXPECT_FALSE(Consp(foo_));
}

TEST_F(ConsTest, Listp)
{
  EXPECT_TRUE(Listp(kNil));
  EXPECT_TRUE(Listp(foobar_));
  EXPECT_TRUE(Listp(Cons(foo_, kNil)));
  EXPECT_FALSE(Listp(foo_));
}

TEST_F(ConsTest, Eq)
{
  EXPECT_TRUE(Eq(kT, kT));
  EXPECT_TRUE(Eq(kNil, kNil));
  EXPECT_TRUE(Eq(foo_, foo_));

  EXPECT_FALSE(Eq(foo_, bar_));
  EXPECT_FALSE(Eq(foo_, foobar_));
  EXPECT_FALSE(Eq(foobar_, barfoo_));

  ConsCell* foobar2 = Cons(foo_, bar_);
  EXPECT_FALSE(Eq(foobar_, foobar2));
}

TEST_F(ConsTest, Null)
{
  EXPECT_TRUE(Null(kNil));

  EXPECT_FALSE(Null(kT));
  EXPECT_FALSE(Null(foo_));
  EXPECT_FALSE(Null(foobar_));
  EXPECT_FALSE(Null(Cons(kNil, kNil)));
}

TEST_F(ConsTest, Car)
{
  EXPECT_EQ(*foo_, *Car(foobar_));
  EXPECT_EQ(*bar_, *Car(barfoo_));
  EXPECT_EQ(*foobar_, *Car(foobarbarfoo_));
  EXPECT_EQ(*kNil, *Car(kNil));
}

TEST_F(ConsTest, Cdr)
{
  EXPECT_EQ(*foo_, *Cdr(barfoo_));
  EXPECT_EQ(*bar_, *Cdr(foobar_));
  EXPECT_EQ(*barfoo_, *Cdr(foobarbarfoo_));
  EXPECT_EQ(*kNil, *Cdr(kNil));
}

TEST_F(ConsTest, Caar)
{
  EXPECT_EQ(*foo_, *Caar(foobarbarfoo_));
}

TEST_F(ConsTest, Cadr)
{
  EXPECT_EQ(*bar_, *Cadr(foobarbarfoo_));
}

TEST_F(ConsTest, Cadar)
{
  EXPECT_EQ(*bar_, *Cadar(Cons(foobarbarfoo_, kNil)));
}

TEST_F(ConsTest, Caddr)
{
  EXPECT_EQ(*foo_, *Caddr(List(bar_, kT, foo_)));
}

TEST_F(ConsTest, Cdddr)
{
  EXPECT_EQ(*List(foo_), *Cdddr(List(kT, bar_, kT, foo_)));
}

TEST_F(ConsTest, Caddar)
{
  EXPECT_EQ(*foo_, *Caddar(List(List(kT, kT, foo_))));
}

TEST_F(ConsTest, CarRequiresListp)
{
  EXPECT_THROW(Car(foo_), TypeError);
}

TEST_F(ConsTest, CdrRequiresListp)
{
  EXPECT_THROW(Cdr(foo_), TypeError);
}

TEST_F(ConsTest, Alist)
{
  ConsCell* alist = kNil;
  alist = Acons(foo_, foobar_, alist);
  alist = Acons(bar_, foo_, alist);
  alist = Acons(kNil, kNil, alist);

  EXPECT_EQ(*foobar_, *Assoc(foo_, alist));
  EXPECT_EQ(*foo_, *Assoc(bar_, alist));
  EXPECT_EQ(*kNil, *Assoc(kNil, alist));
  EXPECT_EQ(nullptr, Assoc(kT, alist));

  EXPECT_THROW(Assoc(foo_, foo_), Error);
  EXPECT_THROW(Assoc(foo_, foobar_), Error);
}

TEST_F(ConsTest, CopyAlist)
{
  ConsCell *a1, *a2;
  a1 = Acons(foo_, foobar_, kNil);
  a1 = Acons(bar_, barfoo_, a1);

  a2 = CopyAlist(a1);
  a2 = Acons(kT, kT, a2);

  EXPECT_EQ(*foobar_, *Assoc(foo_, a1));
  EXPECT_EQ(*foobar_, *Assoc(foo_, a2));
  EXPECT_EQ(*barfoo_, *Assoc(bar_, a1));
  EXPECT_EQ(*barfoo_, *Assoc(bar_, a2));

  EXPECT_EQ(nullptr, Assoc(kT, a1));
  EXPECT_EQ(*kT, *Assoc(kT, a2));

  EXPECT_THROW(CopyAlist(foo_), Error);
  EXPECT_THROW(CopyAlist(foobar_), Error);
}

TEST_F(ConsTest, CopyList)
{
  ConsCell *L1, *L2, *c;

  // Can copy proper lists.
  L1 = List(foobar_, foo_, bar_);
  L2 = CopyList(L1);

  EXPECT_EQ(*foobar_, *Car(L1));
  EXPECT_EQ(*foobar_, *Car(L2));

  // Only list structure is copied (shallow copy).
  EXPECT_EQ(*foo_, *Cadr(L1));
  EXPECT_EQ(*foo_, *Cadr(L2));
  c = Cdr(L1);
  c->car = bar_;
  EXPECT_EQ(*bar_, *Cadr(L1));
  EXPECT_EQ(*foo_, *Cadr(L2));

  EXPECT_EQ(*foo_, *Caar(L1));
  EXPECT_EQ(*foo_, *Caar(L2));
  c = Car(L1);
  c->car = bar_;
  EXPECT_EQ(*bar_, *Caar(L1));
  EXPECT_EQ(*bar_, *Caar(L2));

  // Can copy dotted lists.
  c = CopyList(foobar_);
  EXPECT_EQ(*foobar_, *c);

  // Throws exception if passed a symbol.
  EXPECT_THROW(CopyList(foo_), Error);
}

TEST_F(ConsTest, List)
{
  ConsCell *L1, *L2;

  EXPECT_EQ(*kNil, *List());

  L1 = List(foo_);
  L2 = Cons(foo_, kNil);
  EXPECT_EQ(*L2, *L1);

  L1 = List(foo_, bar_);
  L2 = Cons(foo_, Cons(bar_, kNil));
  EXPECT_EQ(*L2, *L1);
}

TEST_F(ConsTest, Length)
{
  EXPECT_EQ(0, Length(List()));
  EXPECT_EQ(1, Length(foo_list_));
  EXPECT_EQ(2, Length(foobar_list_));
  EXPECT_EQ(5, Length(List(foo_, bar_, foobar_, kT, foo_list_)));

  EXPECT_THROW(Length(foo_), TypeError);
}

TEST_F(ConsTest, Append)
{
  ConsCell *L1 = List(foo_, bar_);
  ConsCell *L2 = List(bar_, foo_);

  EXPECT_EQ(*kNil, *Append(kNil, kNil));
  EXPECT_EQ(*L1, *Append(L1, kNil));
  EXPECT_EQ(*L2, *Append(kNil, L2));
  EXPECT_EQ(*List(foo_, bar_, bar_, foo_), *Append(L1, L2));

  EXPECT_THROW(Append(foo_, L2), TypeError);
  EXPECT_THROW(Append(L1, bar_), TypeError);
}

TEST_F(ConsTest, Pair)
{
  EXPECT_EQ(*kNil, *Pair(kNil, kNil));

  ConsCell *L1 = List(foo_);
  ConsCell *L2 = List(bar_);
  ConsCell *expect = Acons(foo_, bar_, kNil);
  EXPECT_EQ(*expect, *Pair(L1, L2));

  L1 = List(foo_, bar_);
  L2 = List(bar_, foo_);
  expect = Acons(foo_, bar_, Acons(bar_, foo_, kNil));
  EXPECT_EQ(*expect, *Pair(L1, L2));

  EXPECT_THROW(Pair(foo_, L2), TypeError);
  EXPECT_THROW(Pair(L1, bar_), TypeError);

  // Both lists must be proper lists.
  EXPECT_THROW(Pair(L1, foobar_), TypeError);

  // Lists must be the same length.
  EXPECT_THROW(Pair(L1, List(foo_)), Error);
}

TEST_F(ConsTest, SymbolName)
{
  EXPECT_EQ("FOO", SymbolName(foo_));
  EXPECT_EQ("BAR", SymbolName(bar_));
  EXPECT_EQ("X", SymbolName(MakeSymbol("X")));

  ConsCell* spacesym = MakeSymbol("a long symbol name with spaces");
  EXPECT_EQ("a long symbol name with spaces", SymbolName(spacesym));

  ConsCell* nospacesym = MakeSymbol("alongsymbolnamewithoutspaces");
  EXPECT_EQ("alongsymbolnamewithoutspaces", SymbolName(nospacesym));
}

class ConsPrintTest : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P(
 ToString, ConsPrintTest,
 ::testing::Values(
  "FOO",
  "(FOO)",
  "(FOO, BAR)",
  "(FOO . BAR)",
  "(FOO, BAR, FOO)",
  "(FOO, BAR . FOO)",
  "((FOO))",
  "((FOO), BAR)",
  "((FOO) . BAR)",
  "(FOO, (BAR))",
  "((FOO), BAR, FOO)",
  "((FOO), BAR . FOO)",
  "(FOO, (BAR), FOO)",
  "(FOO, (BAR) . FOO)",
  "(FOO, BAR, (FOO))",
  "((FOO, BAR))",
  "((FOO . BAR))",
  "((FOO, BAR), BAR)",
  "((FOO, BAR) . BAR)",
  "((FOO . BAR), BAR)",
  "((FOO . BAR) . BAR)",
  "(FOO, (BAR, FOO))",
  "(FOO, (BAR . FOO))",
  "((FOO, BAR), BAR, FOO)",
  "((FOO . BAR), BAR, FOO)",
  "((FOO, BAR), BAR . FOO)",
  "((FOO . BAR), BAR . FOO)",
  "(FOO, (BAR, FOO), FOO)",
  "(FOO, (BAR . BAR), FOO)",
  "(FOO, (BAR, FOO) . FOO)",
  "(FOO, (BAR . FOO) . FOO)",
  "(FOO, BAR, (FOO, BAR))",
  "(FOO, BAR, (FOO . BAR))",
  "((FOO, BAR), (BAR, FOO))",
  "((FOO . BAR), (BAR . FOO))",
  "((FOO, BAR), (BAR, FOO), (FOO, BAR))"
  ));

TEST_P(ConsPrintTest, ToString)
{
  Reader reader(GetParam());
  EXPECT_EQ(GetParam(), ToString(reader.Read()));
}
} // namespace
