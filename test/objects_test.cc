#include "objects.h"
#include "gtest/gtest.h"

using namespace mclisp;

TEST(ObjectTest, NilIsFalse)
{
  // Explicit bool cast is required here. See
  // https://code.google.com/p/googletest/issues/detail?id=429
  EXPECT_FALSE(bool(mclisp::kNil));
}

TEST(ObjectTest, TisTrue)
{
  // Explicit bool cast is required here. See
  // https://code.google.com/p/googletest/issues/detail?id=429
  EXPECT_TRUE(bool(mclisp::kT));
}

TEST(ObjectTest, NotNilIsTrue)
{
  EXPECT_TRUE(!mclisp::kNil);
}

TEST(ObjectTest, NotTisFalse)
{
  EXPECT_FALSE(!mclisp::kT);
}

TEST(ObjectTest, NilEqNil)
{
  EXPECT_EQ(mclisp::kNil, mclisp::kNil);
}

TEST(ObjectTest, TEqT)
{
  EXPECT_EQ(mclisp::kT, mclisp::kT);
}

TEST(ObjectTest, SymbolEqual)
{
  Symbol A("A");
  Symbol B("A");
  EXPECT_EQ(A, A);
  EXPECT_EQ(A, B);
}

TEST(ObjectTest, SymbolNotEqual)
{
  Symbol a("a");
  Symbol A("A");
  Symbol B("B");
  EXPECT_NE(A, a);
  EXPECT_NE(A, B);
}

TEST(ObjectTest, SymbolRelationalOps)
{
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");

  EXPECT_LE(A, A);
  EXPECT_LE(A, B);
  EXPECT_LE(A, C);
  EXPECT_LE(B, B);
  EXPECT_LE(B, C);
  EXPECT_LE(C, C);

  EXPECT_LT(A, B);
  EXPECT_LT(A, C);
  EXPECT_LT(B, C);

  EXPECT_GE(C, C);
  EXPECT_GE(C, B);
  EXPECT_GE(C, A);
  EXPECT_GE(B, B);
  EXPECT_GE(B, A);
  EXPECT_GE(A, A);

  EXPECT_GT(C, B);
  EXPECT_GT(C, A);
  EXPECT_GT(B, A);
}

TEST(ObjectTest, ConsEqual)
{
  Symbol A1("A"), A2("A");
  Symbol B1("B"), B2("B");

  Cons C1(A1, B1);
  Cons C2(A2, B2);
  EXPECT_EQ(C1, C1);
  EXPECT_EQ(C1, C2);

  Cons C3(C1, C1);
  Cons C4(C2, C2);
  EXPECT_EQ(C3, C3);
  EXPECT_EQ(C3, C4);
}

TEST(ObjectTest, ConsNotEqual)
{
  Symbol A("A");
  Symbol B("B");

  Cons C1(A, A);
  Cons C2(A, B);
  Cons C3(B, A);
  EXPECT_NE(C1, C2);
  EXPECT_NE(C1, C3);

  Cons C4(C1, C2);
  Cons C5(C1, C3);
  Cons C6(C3, C2);
  EXPECT_NE(C1, C2);
  EXPECT_NE(C1, C3);
}

TEST(ObjectTest, ConsRelationalOps)
{
  Symbol A("A");
  Symbol B("B");

  Cons C1(A, A);
  Cons C2(A, B);
  Cons C3(B, A);

  EXPECT_LE(C1, C1);
  EXPECT_LE(C1, C2);
  EXPECT_LE(C1, C3);
  EXPECT_LE(C2, C2);
  EXPECT_LE(C2, C3);
  EXPECT_LE(C3, C3);
  EXPECT_LT(C1, C2);
  EXPECT_LT(C1, C3);
  EXPECT_LT(C2, C3);
  EXPECT_GE(C3, C3);
  EXPECT_GE(C3, C2);
  EXPECT_GE(C3, C1);
  EXPECT_GE(C2, C2);
  EXPECT_GE(C2, C1);
  EXPECT_GE(C1, C1);
  EXPECT_GT(C3, C2);
  EXPECT_GT(C3, C1);
  EXPECT_GT(C2, C1);

  Cons C4(C1, C2);
  Cons C5(C1, C3);

  EXPECT_LT(C4, C5);
  EXPECT_LE(C4, C4);
  EXPECT_LE(C4, C5);
  EXPECT_LE(C5, C5);
  EXPECT_GE(C5, C5);
  EXPECT_GE(C5, C4);
  EXPECT_GT(C5, C4);

  // Wacky. Maybe these should throw instead?
  EXPECT_FALSE(C1 == C4);
  EXPECT_FALSE(C1 < C4);
  EXPECT_FALSE(C1 > C4);
}

class ObjectPrintSymbolsTest : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P(
 PrintSymbols, ObjectPrintSymbolsTest,
 ::testing::Values("A", "some symbol", "Another 1"));

TEST_P(ObjectPrintSymbolsTest, PrintSymbol)
{
  std::ostringstream oss;
  Symbol symbol(GetParam());
  oss << symbol;
  EXPECT_EQ(GetParam(), oss.str());
}

TEST(ObjectTest, PrintConses)
{
  std::ostringstream ossC1;
  std::ostringstream ossC3;
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");
  Symbol D("D");
  Cons C1(A, B);
  Cons C2(C, D);
  Cons C3(C1, C2);
  ossC1 << C1;
  ossC3 << C3;
  EXPECT_EQ("(A . B)", ossC1.str());
  EXPECT_EQ("((A . B) . (C . D))", ossC3.str());
}

TEST(ObjectTest, PrintNil)
{
  std::ostringstream oss;
  oss << kNil;
  EXPECT_EQ("NIL", oss.str());
}

TEST(ObjectTest, PrintT)
{
  std::ostringstream oss;
  oss << kT;
  EXPECT_EQ("T", oss.str());
}
