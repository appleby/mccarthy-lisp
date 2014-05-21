#include "lexer.h"
#include "gtest/gtest.h"

TEST(LexerTest, Dummy)
{
  mclisp::Lexer lexer;
  ASSERT_EQ(0, lexer.nextToken());
}
