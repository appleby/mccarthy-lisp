#include "cons.h"

#include "gtest/gtest.h"

#include "init.h"
#include "error.h"
#include "reader.h"

using namespace mclisp;

class ConsTest : public ::testing::Test
{
  protected:
    ConsTest()
    {
      foosym_ = MakeSymbol("FOO");
      barsym_ = MakeSymbol("BAR");
      foobar_ = Cons(foosym_, barsym_);
      barfoo_ = Cons(barsym_, foosym_);
      foobarbarfoo_ = Cons(foobar_, barfoo_);
    }
    ConsCell* foosym_;
    ConsCell* barsym_;
    ConsCell* foobar_;
    ConsCell* barfoo_;
    ConsCell* foobarbarfoo_;
};

TEST_F(ConsTest, FromBool)
{
  EXPECT_EQ(*kT, *FromBool(true));
  EXPECT_EQ(*kNil, *FromBool(false));
}

TEST_F(ConsTest, Atom)
{
  EXPECT_TRUE(Atom(foosym_));
  EXPECT_FALSE(Atom(foobar_));
}

TEST_F(ConsTest, Symbolp)
{
  EXPECT_TRUE(Symbolp(foosym_));
  EXPECT_FALSE(Symbolp(foobar_));
}

TEST_F(ConsTest, Consp)
{
  EXPECT_TRUE(Consp(foobar_));
  EXPECT_FALSE(Consp(foosym_));
}

TEST_F(ConsTest, Eq)
{
  EXPECT_TRUE(Eq(kT, kT));
  EXPECT_TRUE(Eq(kNil, kNil));
  EXPECT_TRUE(Eq(foosym_, foosym_));

  EXPECT_FALSE(Eq(foosym_, barsym_));
  EXPECT_FALSE(Eq(foosym_, foobar_));
  EXPECT_FALSE(Eq(foobar_, barfoo_));

  ConsCell* foobar2 = Cons(foosym_, barsym_);
  EXPECT_FALSE(Eq(foobar_, foobar2));
}

TEST_F(ConsTest, Null)
{
  EXPECT_TRUE(Null(kNil));

  EXPECT_FALSE(Null(kT));
  EXPECT_FALSE(Null(foosym_));
  EXPECT_FALSE(Null(foobar_));
  EXPECT_FALSE(Null(Cons(kNil, kNil)));
}

TEST_F(ConsTest, Car)
{
  EXPECT_EQ(*foosym_, *Car(foobar_));
  EXPECT_EQ(*barsym_, *Car(barfoo_));
  EXPECT_EQ(*foobar_, *Car(foobarbarfoo_));
  EXPECT_EQ(*kNil, *Car(kNil));
}

TEST_F(ConsTest, Cdr)
{
  EXPECT_EQ(*foosym_, *Cdr(barfoo_));
  EXPECT_EQ(*barsym_, *Cdr(foobar_));
  EXPECT_EQ(*barfoo_, *Cdr(foobarbarfoo_));
  EXPECT_EQ(*kNil, *Cdr(kNil));
}

TEST_F(ConsTest, Caar)
{
  EXPECT_EQ(*foosym_, *Caar(foobarbarfoo_));
}

TEST_F(ConsTest, Cadr)
{
  EXPECT_EQ(*barsym_, *Cadr(foobarbarfoo_));
}

TEST_F(ConsTest, Cadar)
{
  EXPECT_EQ(*barsym_, *Cadar(Cons(foobarbarfoo_, kNil)));
}

TEST_F(ConsTest, CarRequiresListp)
{
  EXPECT_THROW(Car(foosym_), TypeError);
}

TEST_F(ConsTest, CdrRequiresListp)
{
  EXPECT_THROW(Cdr(foosym_), TypeError);
}

TEST_F(ConsTest, Alist)
{
  ConsCell* alist = kNil;
  alist = Acons(foosym_, foobar_, alist);
  alist = Acons(barsym_, foosym_, alist);
  alist = Acons(kNil, kNil, alist);

  EXPECT_EQ(*foobar_, *Assoc(foosym_, alist));
  EXPECT_EQ(*foosym_, *Assoc(barsym_, alist));
  EXPECT_EQ(*kNil, *Assoc(kNil, alist));
  EXPECT_EQ(nullptr, Assoc(kT, alist));

  EXPECT_THROW(Assoc(foosym_, foosym_), Error);
  EXPECT_THROW(Assoc(foosym_, foobar_), Error);
}

TEST_F(ConsTest, CopyAlist)
{
  ConsCell *a1, *a2;
  a1 = Acons(foosym_, foobar_, kNil);
  a1 = Acons(barsym_, barfoo_, a1);

  a2 = CopyAlist(a1);
  a2 = Acons(kT, kT, a2);

  EXPECT_EQ(*foobar_, *Assoc(foosym_, a1));
  EXPECT_EQ(*foobar_, *Assoc(foosym_, a2));
  EXPECT_EQ(*barfoo_, *Assoc(barsym_, a1));
  EXPECT_EQ(*barfoo_, *Assoc(barsym_, a2));

  EXPECT_EQ(nullptr, Assoc(kT, a1));
  EXPECT_EQ(*kT, *Assoc(kT, a2));

  EXPECT_THROW(CopyAlist(foosym_), Error);
  EXPECT_THROW(CopyAlist(foobar_), Error);
}

TEST_F(ConsTest, CopyList)
{
  ConsCell *L1, *L2, *c;

  // Can copy proper lists.
  L1 = List(foobar_, foosym_, barsym_);
  L2 = CopyList(L1);

  EXPECT_EQ(*foobar_, *Car(L1));
  EXPECT_EQ(*foobar_, *Car(L2));

  // Only list structure is copied (shallow copy).
  EXPECT_EQ(*foosym_, *Cadr(L1));
  EXPECT_EQ(*foosym_, *Cadr(L2));
  c = Cdr(L1);
  c->car = barsym_;
  EXPECT_EQ(*barsym_, *Cadr(L1));
  EXPECT_EQ(*foosym_, *Cadr(L2));

  EXPECT_EQ(*foosym_, *Caar(L1));
  EXPECT_EQ(*foosym_, *Caar(L2));
  c = Car(L1);
  c->car = barsym_;
  EXPECT_EQ(*barsym_, *Caar(L1));
  EXPECT_EQ(*barsym_, *Caar(L2));

  // Can copy dotted lists.
  c = CopyList(foobar_);
  EXPECT_EQ(*foobar_, *c);

  // Throws exception if passed a symbol.
  EXPECT_THROW(CopyList(foosym_), Error);
}

TEST_F(ConsTest, List)
{
  ConsCell *L1, *L2;

  EXPECT_EQ(*kNil, *List());

  L1 = List(foosym_);
  L2 = Cons(foosym_, kNil);
  EXPECT_EQ(*L2, *L1);

  L1 = List(foosym_, barsym_);
  L2 = Cons(foosym_, Cons(barsym_, kNil));
  EXPECT_EQ(*L2, *L1);
}

TEST_F(ConsTest, SymbolName)
{
  EXPECT_EQ("FOO", SymbolName(foosym_));
  EXPECT_EQ("BAR", SymbolName(barsym_));
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
