#include "reader.h"
#include "gtest/gtest.h"
#include "cons.h"

using namespace mclisp;

TEST(ReaderTest, DefaultCtor)
{
  Reader reader("");
  EXPECT_NE(kT, nullptr);
  EXPECT_NE(kNil, nullptr);
  EXPECT_EQ(kNil, reader.Read());
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

TEST(ReaderTest, ReadSymbol)
{
  Reader reader("symbol");
  EXPECT_EQ(reader.Intern("symbol"), reader.Read());
}

TEST(ReaderTest, ReadSymbolWithSpaces)
{
  Reader reader("APPLE PIE NUMBER 3");
  EXPECT_EQ(reader.Intern("APPLE PIE NUMBER 3"), reader.Read());
}

TEST(ReaderTest, ReadUpcasesSymbols)
{
  Reader reader("uPpErCaSe");
  EXPECT_EQ(reader.Intern("UPPERCASE"), reader.Read());
}

TEST(ReaderTest, ReadBadSymbolNoEarMuffs)
{
  Reader reader("*bad*");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadSymbolNoHyphens)
{
  Reader reader("bad-symbol");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadCons)
{
  Reader reader("(A . B)");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C1 = MakeCons(A, B);
  EXPECT_EQ(*C1, *reader.Read());
}

TEST(ReaderTest, ReadNestedCons)
{
  Reader reader("((A . B) . (C . D))");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C = reader.Intern("C");
  ConsCell* D = reader.Intern("D");
  ConsCell* C1 = MakeCons(A, B);
  ConsCell* C2 = MakeCons(C, D);
  ConsCell* C3 = MakeCons(C1, C2);
  EXPECT_EQ(*C3, *reader.Read());
}

TEST(ReaderTest, ReadBadConsNoOpenParen)
{
  Reader reader("A . B)");
  ConsCell* A = reader.Intern("A");
  EXPECT_EQ(A, reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadConsNoCloseParen)
{
  Reader reader("(A . B");
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
  ConsCell* A = reader.Intern("A");
  ConsCell* C1 = MakeCons(A, kNil);
  EXPECT_EQ(*C1, *reader.Read());
}

TEST(ReaderTest, Read2ItemList)
{
  Reader reader("(A, B)");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C1 = MakeCons(B, kNil);
  ConsCell* C2 = MakeCons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
}

TEST(ReaderTest, Read3ItemList)
{
  Reader reader("(A, B, C)");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C = reader.Intern("C");
  ConsCell* C1 = MakeCons(C, kNil);
  ConsCell* C2 = MakeCons(B, C1);
  ConsCell* C3 = MakeCons(A, C2);
  EXPECT_EQ(*C3, *reader.Read());
}

TEST(ReaderTest, ReadDottedList)
{
  Reader reader("(A, B . C)");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C = reader.Intern("C");
  ConsCell* C1 = MakeCons(B, C);
  ConsCell* C2 = MakeCons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
}

TEST(ReaderTest, ReadNestedLists)
{
  Reader reader("((A . B), (C, D))");
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C = reader.Intern("C");
  ConsCell* D = reader.Intern("D");
  ConsCell* C1 = MakeCons(A, B);
  ConsCell* C2 = MakeCons(D, kNil);
  ConsCell* C3 = MakeCons(C, C2);
  ConsCell* C4 = MakeCons(C3, kNil);
  ConsCell* C5 = MakeCons(C1, C4);
  EXPECT_EQ(*C5, *reader.Read());
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
  ConsCell* A = reader.Intern("A");
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
  ConsCell* A = reader.Intern("A");
  ConsCell* B = reader.Intern("B");
  ConsCell* C = reader.Intern("C");
  ConsCell* C1 = MakeCons(B, C);
  ConsCell* C2 = MakeCons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadNestedListNoCloseParen)
{
  Reader reader("(A, (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}
