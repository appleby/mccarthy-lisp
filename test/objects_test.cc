#include "objects.h"
#include "gtest/gtest.h"

using namespace mclisp;

TEST(ObjectTest, NilIsFalse)
{
  EXPECT_FALSE(mclisp::kNil);
}

TEST(ObjectTest, TisTrue)
{
  EXPECT_TRUE(mclisp::kT);
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
  Symbol A("A");
  Symbol B("B");
  EXPECT_NE(A, B);
}
