#include "lexer.h"
#include "gtest/gtest.h"

using namespace mclisp;

TEST(LexerTest, DefaultCtor)
{
  Lexer lexer;
  EXPECT_EQ("", lexer.current_token());
}

TEST(LexerTest, EmptyString)
{
  Lexer lexer("");
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

TEST(LexerTest, AllWhiteSpace)
{
  Lexer lexer("    ");
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

TEST(LexerTest, Delimiters)
{
  Lexer lexer("(,.)");
  EXPECT_EQ(kOpenParen, lexer.nextToken());
  EXPECT_EQ("(", lexer.current_token());

  EXPECT_EQ(kComma, lexer.nextToken());
  EXPECT_EQ(",", lexer.current_token());

  EXPECT_EQ(kDot, lexer.nextToken());
  EXPECT_EQ(".", lexer.current_token());

  EXPECT_EQ(kCloseParen, lexer.nextToken());
  EXPECT_EQ(")", lexer.current_token());

  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

TEST(LexerTest, TrimTrailingWhiteSpace)
{
  std::string strval("TRIM TRAILING");
  Lexer lexer(strval + "    ");
  EXPECT_EQ(kSymbol, lexer.nextToken());
  EXPECT_EQ(strval, lexer.current_token());
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

class LexerIntTest : public ::testing::TestWithParam<int> {};

INSTANTIATE_TEST_CASE_P(NegativeTenToTen, LexerIntTest,
                        ::testing::Range(-10, 11));

TEST_P(LexerIntTest, Integers)
{
  const std::string strval(std::to_string(GetParam()));
  Lexer lexer(strval);
  EXPECT_EQ(kNumber, lexer.nextToken());
  EXPECT_EQ(strval, lexer.current_token());
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

class LexerSymbolTest : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P(
 ValidSymbols, LexerSymbolTest,
 ::testing::Values("car", "CAR", "a", "two  space", "apple pie number 3"));

TEST_P(LexerSymbolTest, ValidSymbols)
{
  Lexer lexer(GetParam());
  EXPECT_EQ(kSymbol, lexer.nextToken());
  EXPECT_EQ(GetParam(), lexer.current_token());
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}

class LexerBadTokenTest : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P(
 BadTokens, LexerBadTokenTest,
 ::testing::Values("+", "*special*", "no-good", "no_good", "bad?", "no/good"));

TEST_P(LexerBadTokenTest, BadTokens)
{
  Lexer lexer(GetParam());
  EXPECT_EQ(kBadToken, lexer.nextToken());
  EXPECT_EQ(GetParam(), lexer.current_token());
  EXPECT_EQ(kEofToken, lexer.nextToken());
  EXPECT_EQ("", lexer.current_token());
}
