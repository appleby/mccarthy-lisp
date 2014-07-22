#include "reader.h"
#include "mclisp_test.h"

namespace
{
using namespace mclisp;

class ReaderTest: public mclisp::testing::Test
{
  protected:
    ReaderTest()
    {
      A = reader::Intern("A");
      B = reader::Intern("B");
      C = reader::Intern("C");
      D = reader::Intern("D");
      quote_ = BUILTIN(QUOTE);
    }

    ConsCell *A;
    ConsCell *B;
    ConsCell *C;
    ConsCell *D;
    ConsCell* quote_;
};

TEST_F(ReaderTest, DefaultCtor)
{
  Reader reader("");
  EXPECT_EQ(*BUILTIN(EOF), *reader.Read());
}

TEST_F(ReaderTest, ReadNil)
{
  Reader reader("NIL");
  EXPECT_EQ(*kNil, *reader.Read());
}

TEST_F(ReaderTest, ReadNilSyntax)
{
  Reader reader("()");
  EXPECT_EQ(*kNil, *reader.Read());
}

TEST_F(ReaderTest, ReadT)
{
  Reader reader("T");
  EXPECT_EQ(*kT, *reader.Read());
}

TEST_F(ReaderTest, ReadSymbol)
{
  Reader reader("symbol");
  EXPECT_EQ(*reader::Intern("symbol"), *reader.Read());
}

TEST_F(ReaderTest, ReadSymbolWithSpaces)
{
  Reader reader("APPLE PIE NUMBER 3");
  EXPECT_EQ(*reader::Intern("APPLE PIE NUMBER 3"), *reader.Read());
}

TEST_F(ReaderTest, ReadUpcasesSymbols)
{
  Reader reader("uPpErCaSe");
  EXPECT_EQ(*reader::Intern("UPPERCASE"), *reader.Read());
}

TEST_F(ReaderTest, ReadQuotedSymbol)
{
  Reader reader("'foo");
  EXPECT_EQ(*List(quote_, foo_), *reader.Read());
}

TEST_F(ReaderTest, ReadBadSymbolNoEarMuffs)
{
  Reader reader("*bad*");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadSymbolNoHyphens)
{
  Reader reader("bad-symbol");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadComment)
{
  Reader reader(";;; Look, a comment\nfoo ; Another comment");
  EXPECT_EQ(*foo_, *reader.Read());
  EXPECT_EQ(*BUILTIN(EOF), *reader.Read());
}

TEST_F(ReaderTest, ReadCons)
{
  Reader reader("(A . B)");
  EXPECT_EQ(*Cons(A, B), *reader.Read());
}

TEST_F(ReaderTest, ReadNestedCons)
{
  Reader reader("((A . B) . (C . D))");
  EXPECT_EQ(*Cons(Cons(A, B), Cons(C, D)), *reader.Read());
}

TEST_F(ReaderTest, ReadBadConsNoOpenParen)
{
  Reader reader("A . B)");
  EXPECT_EQ(*A, *reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadConsNoCloseParen)
{
  Reader reader("(A . B");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadConsTooManyDots)
{
  Reader reader("(A . B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadConsNoCar)
{
  Reader reader("( . B)");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadConsNoCdr)
{
  Reader reader("(A . )");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadNestedConsNoOpenParen)
{
  Reader reader("(A . B . C))");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadNestedConsNoCloseParen)
{
  Reader reader("(A . (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, Read1ItemList)
{
  Reader reader("(A)");
  EXPECT_EQ(*List(A), *reader.Read());
}

TEST_F(ReaderTest, Read2ItemList)
{
  Reader reader("(A, B)");
  EXPECT_EQ(*List(A, B), *reader.Read());
}

TEST_F(ReaderTest, Read3ItemList)
{
  Reader reader("(A, B, C)");
  EXPECT_EQ(*List(A, B, C), *reader.Read());
}

TEST_F(ReaderTest, ReadDottedList)
{
  Reader reader("(A, B . C)");
  EXPECT_EQ(*Cons(A, Cons(B, C)), *reader.Read());
}

TEST_F(ReaderTest, ReadNestedLists)
{
  Reader reader("((A . B), (C, D))");
  EXPECT_EQ(*List(Cons(A, B), List(C, D)), *reader.Read());
}

TEST_F(ReaderTest, ReadQuotedList)
{
  Reader reader("'(foo, bar)");
  EXPECT_EQ(*List(quote_, List(foo_, bar_)), *reader.Read());
}

TEST_F(ReaderTest, ReadQuotedDottedList)
{
  Reader reader("'(foo . bar)");
  EXPECT_EQ(*List(quote_, Cons(foo_, bar_)), *reader.Read());
}

TEST_F(ReaderTest, ReadBadListCommaFollowsDot)
{
  Reader reader("(A . B, C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadListCommaFollowsDot2)
{
  Reader reader("(A, B . C, D)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadListNoCar)
{
  Reader reader("( , B)");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadListNoCdr)
{
  Reader reader("(A, )");
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadListNoOpenParen)
{
  Reader reader("A, B)");
  EXPECT_EQ(*A, *reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadListNoCloseParen)
{
  Reader reader("(A, B");
  EXPECT_THROW(reader.Read(), BadTokenError);
}

TEST_F(ReaderTest, ReadBadNestedListNoOpenParen)
{
  Reader reader("(A, B . C))");
  EXPECT_EQ(*Cons(A, Cons(B, C)), *reader.Read());
  EXPECT_THROW(reader.Read(), ReadError);
}

TEST_F(ReaderTest, ReadBadNestedListNoCloseParen)
{
  Reader reader("(A, (B . C)");
  EXPECT_THROW(reader.Read(), BadTokenError);
}
} // namespace
