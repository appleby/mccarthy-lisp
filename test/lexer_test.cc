#include "lexer.h"
#include "gtest/gtest.h"

TEST(LexerTest, Eof)
{
  mclisp::Lexer lexer("");
  ASSERT_EQ(mclisp::kEof, lexer.nextToken());
}

TEST(LexerTest, Cons)
{
  mclisp::Lexer lexer("(hello)");
  ASSERT_EQ(mclisp::kCons, lexer.nextToken());
}
