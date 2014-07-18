#include "utils.h"
#include "gtest/gtest.h"

using namespace mclisp;

TEST(UtilsTest, ContainerToString)
{
  std::set<Token> tokens {kComma, kDot, kQuote, kOpenParen, kCloseParen};
  std::string result = ContainerToString(tokens);
  EXPECT_EQ(5, result.length());
  EXPECT_NE(std::string::npos, result.find(","));
  EXPECT_NE(std::string::npos, result.find("."));
  EXPECT_NE(std::string::npos, result.find("'"));
  EXPECT_NE(std::string::npos, result.find("("));
  EXPECT_NE(std::string::npos, result.find(")"));
}

TEST(UtilsTest, ShouldQuit)
{
  ConsCell *quit = MakeSymbol("QUIT");
  EXPECT_TRUE(ShouldQuit(quit));
  EXPECT_TRUE(ShouldQuit(List(quit)));
  EXPECT_TRUE(ShouldQuit(BUILTIN(EOF)));

  ConsCell *exit = MakeSymbol("EXIT");
  EXPECT_FALSE(ShouldQuit(exit));
  EXPECT_FALSE(ShouldQuit(List(exit)));
}

TEST(UtilsTest, ToLower)
{
  std::string FoO("FoO");
  std::string foo = ToLower(FoO);

  EXPECT_EQ("FoO", FoO);
  EXPECT_EQ("foo", foo);
}

TEST(UtilsTest, ToUpper)
{
  std::string fOo("fOo");
  std::string FOO = ToUpper(fOo);

  EXPECT_EQ("fOo", fOo);
  EXPECT_EQ("FOO", FOO);
}
