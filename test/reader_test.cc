#include "reader.h"
#include "gtest/gtest.h"
#include "cons.h"

using namespace mclisp;

TEST(ReaderTest, DefaultCtor)
{
  Reader reader("");
  EXPECT_EQ(BUILTIN(EOF), reader.Read());
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
  EXPECT_EQ(reader::Intern("symbol"), reader.Read());
}

TEST(ReaderTest, ReadSymbolWithSpaces)
{
  Reader reader("APPLE PIE NUMBER 3");
  EXPECT_EQ(reader::Intern("APPLE PIE NUMBER 3"), reader.Read());
}

TEST(ReaderTest, ReadUpcasesSymbols)
{
  Reader reader("uPpErCaSe");
  EXPECT_EQ(reader::Intern("UPPERCASE"), reader.Read());
}

TEST(ReaderTest, ReadQuotedSymbol)
{
  Reader reader("'foo");
  ConsCell* quote = BUILTIN(QUOTE);
  ConsCell* foo = reader::Intern("foo");
  ConsCell* C1 = List(quote, foo);
  EXPECT_EQ(*C1, *reader.Read());
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
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C1 = Cons(A, B);
  EXPECT_EQ(*C1, *reader.Read());
}

TEST(ReaderTest, ReadNestedCons)
{
  Reader reader("((A . B) . (C . D))");
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C = reader::Intern("C");
  ConsCell* D = reader::Intern("D");
  ConsCell* C1 = Cons(A, B);
  ConsCell* C2 = Cons(C, D);
  ConsCell* C3 = Cons(C1, C2);
  EXPECT_EQ(*C3, *reader.Read());
}

TEST(ReaderTest, ReadBadConsNoOpenParen)
{
  Reader reader("A . B)");
  ConsCell* A = reader::Intern("A");
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
  ConsCell* A = reader::Intern("A");
  ConsCell* C1 = Cons(A, kNil);
  EXPECT_EQ(*C1, *reader.Read());
}

TEST(ReaderTest, Read2ItemList)
{
  Reader reader("(A, B)");
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C1 = Cons(B, kNil);
  ConsCell* C2 = Cons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
}

TEST(ReaderTest, Read3ItemList)
{
  Reader reader("(A, B, C)");
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C = reader::Intern("C");
  ConsCell* C1 = Cons(C, kNil);
  ConsCell* C2 = Cons(B, C1);
  ConsCell* C3 = Cons(A, C2);
  EXPECT_EQ(*C3, *reader.Read());
}

TEST(ReaderTest, ReadDottedList)
{
  Reader reader("(A, B . C)");
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C = reader::Intern("C");
  ConsCell* C1 = Cons(B, C);
  ConsCell* C2 = Cons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
}

TEST(ReaderTest, ReadNestedLists)
{
  Reader reader("((A . B), (C, D))");
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C = reader::Intern("C");
  ConsCell* D = reader::Intern("D");
  ConsCell* C1 = Cons(A, B);
  ConsCell* C2 = Cons(D, kNil);
  ConsCell* C3 = Cons(C, C2);
  ConsCell* C4 = Cons(C3, kNil);
  ConsCell* C5 = Cons(C1, C4);
  EXPECT_EQ(*C5, *reader.Read());
}

TEST(ReaderTest, ReadQuotedList)
{
  Reader reader("'(foo, bar)");
  ConsCell* quote = BUILTIN(QUOTE);
  ConsCell* foo = reader::Intern("foo");
  ConsCell* bar = reader::Intern("bar");
  ConsCell* C1 = List(quote, List(foo, bar));
  EXPECT_EQ(*C1, *reader.Read());
}

TEST(ReaderTest, ReadQuotedDottedList)
{
  Reader reader("'(foo . bar)");
  ConsCell* quote = BUILTIN(QUOTE);
  ConsCell* foo = reader::Intern("foo");
  ConsCell* bar = reader::Intern("bar");
  ConsCell* C1 = List(quote, Cons(foo, bar));
  EXPECT_EQ(*C1, *reader.Read());
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
  ConsCell* A = reader::Intern("A");
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
  ConsCell* A = reader::Intern("A");
  ConsCell* B = reader::Intern("B");
  ConsCell* C = reader::Intern("C");
  ConsCell* C1 = Cons(B, C);
  ConsCell* C2 = Cons(A, C1);
  EXPECT_EQ(*C2, *reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST(ReaderTest, ReadBadNestedListNoCloseParen)
{
  Reader reader("(A, (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}
