#include "reader.h"
#include "gtest/gtest.h"
#include "objects.h"

using namespace mclisp;

TEST(ReaderTest, DefaultCtor)
{
  Reader reader("");
  EXPECT_EQ(kNil, reader.Read());
}

TEST(ReaderTest, ReadSymbol)
{
  Reader reader("symbol");
  EXPECT_EQ(Symbol("symbol"), reader.Read());
}

TEST(ReaderTest, ReadCons)
{
  Reader reader("(A . B)");
  Symbol A("A");
  Symbol B("B");
  Cons C1(A, B);
  EXPECT_EQ(C1, reader.Read());
}

TEST(ReaderTest, ReadBadConsNoCloseParen)
{
  Reader reader("(A . B");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadConsNoDot)
{
  Reader reader("(A B)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}
