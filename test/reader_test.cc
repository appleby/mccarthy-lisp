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
  EXPECT_EQ(Symbol("SYMBOL"), reader.Read());
}

TEST(ReaderTest, ReadNil)
{
  Reader reader("NIL");
  EXPECT_EQ(kNil, reader.Read());
}

TEST(ReaderTest, ReadT)
{
  Reader reader("T");
  EXPECT_EQ(kT, reader.Read());
}

TEST(ReaderTest, ReadCons)
{
  Reader reader("(A . B)");
  Symbol A("A");
  Symbol B("B");
  Cons C1(A, B);
  EXPECT_EQ(C1, reader.Read());
}

TEST(ReaderTest, ReadNestedCons)
{
  Reader reader("((A . B) . (C . D))");
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");
  Symbol D("D");
  Cons C1(A, B);
  Cons C2(C, D);
  Cons C3(C1, C2);
  EXPECT_EQ(C3, reader.Read());
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

TEST(ReaderTest, ReadBadConsNoCar)
{
  Reader reader("( . B)");
  EXPECT_THROW(reader.Read(), ReadError);
}
