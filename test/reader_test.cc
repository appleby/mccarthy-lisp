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

TEST(ReaderTest, ReadBadConsNoOpenParen)
{
  Reader reader("A . B)");
  Symbol A("A");
  EXPECT_EQ(A, reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
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

TEST(ReaderTest, ReadBadConsTooManyDots)
{
  Reader reader("(A . B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadConsNoCar)
{
  Reader reader("( . B)");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadConsNoCdr)
{
  Reader reader("(A . )");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadNestedConsNoOpenParen)
{
  Reader reader("(A . B . C))");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadNestedConsNoCloseParen)
{
  Reader reader("(A . (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, Read1ItemList)
{
  Reader reader("(A)");
  Symbol A("A");
  Cons C1(A, kNil);
  EXPECT_EQ(C1, reader.Read());
}

TEST(ReaderTest, Read2ItemList)
{
  Reader reader("(A, B)");
  Symbol A("A");
  Symbol B("B");
  Cons C1(B, kNil);
  Cons C2(A, C1);
  EXPECT_EQ(C2, reader.Read());
}

TEST(ReaderTest, Read3ItemList)
{
  Reader reader("(A, B, C)");
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");
  Cons C1(C, kNil);
  Cons C2(B, C1);
  Cons C3(A, C2);
  EXPECT_EQ(C3, reader.Read());
}

TEST(ReaderTest, ReadDottedList)
{
  Reader reader("(A, B . C)");
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");
  Cons C1(B, C);
  Cons C2(A, C1);
  EXPECT_EQ(C2, reader.Read());
}

TEST(ReaderTest, ReadNestedLists)
{
  Reader reader("((A . B), (C, D))");
  Symbol A("A");
  Symbol B("B");
  Symbol C("C");
  Symbol D("D");
  Cons C1(A, B);
  Cons C2(D, kNil);
  Cons C3(C, C2);
  Cons C4(C3, kNil);
  Cons C5(C1, C4);
  EXPECT_EQ(C5, reader.Read());
}

TEST(ReaderTest, ReadBadListCommaFollowsDot)
{
  Reader reader("(A . B, C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadListCommaFollowsDot2)
{
  Reader reader("(A, B . C, D)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadListNoCar)
{
  Reader reader("( , B)");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadListNoCdr)
{
  Reader reader("(A, )");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadListNoOpenParen)
{
  Reader reader("A, B)");
  Symbol A("A");
  EXPECT_EQ(A, reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadListNoCloseParen)
{
  Reader reader("(A, B");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST(ReaderTest, ReadBadNestedListNoOpenParen)
{
  Reader reader("(A, B . C))");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadNestedListNoCloseParen)
{
  Reader reader("(A, (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}
