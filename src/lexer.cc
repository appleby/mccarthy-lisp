#include "lexer.h"
#include <algorithm>
#include <cctype>
#include <cstdio>

namespace
{

inline bool IsDelim(int c)
{
  return c == '.' || c == ',' || c == '(' || c == ')' || c == '\n' || c == EOF;
}

inline void RTrim(std::string& s, const std::string ws=" \n\r\t")
{
  s.erase(s.find_last_not_of(ws) + 1);
}

} // namespace

namespace mclisp
{

bool Lexer::CurrentTokenIsValidNumber()
{
  auto start = current_token_.cbegin();
  auto end = current_token_.cend();

  if (*start == '-' || *start == '+')
    start++;

  if (start == end)
    // Require at least one digit.
    return false;

  return std::all_of(start, end, isdigit);
}

bool Lexer::CurrentTokenIsValidSymbol()
{
  auto start = current_token_.cbegin();
  auto end = current_token_.cend();

  if (start == end)
    // Require at least one char.
    return false;

  return std::all_of(start, end,
                     [](char c) { return c == ' ' || isalnum(c); });
}

Token Lexer::nextToken()
{
  char c = '\0';

  current_token_.clear();
  in_ >> std::skipws >> c;
  in_ >> std::noskipws;

  if (in_.eof())
    return kEof;

  current_token_ = c;

  if (c == '.') return kDot;
  if (c == ',') return kComma;
  if (c == '(') return kOpenParen;
  if (c == ')') return kCloseParen;

  while (!IsDelim(in_.peek()))
  {
    in_ >> c;
    current_token_ += c;
  }

  RTrim(current_token_);

  if (CurrentTokenIsValidNumber())
    return kNumber;

  if (CurrentTokenIsValidSymbol())
    return kSymbol;

  return kBadToken;
}

}; // namespace mclisp
