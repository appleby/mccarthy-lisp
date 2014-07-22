#include <algorithm>
#include <cctype>
#include <cstdio>
#include <limits>

#include "error.h"
#include "lexer.h"

namespace
{

inline bool IsDelim(int c)
{
  static const std::string delims("(.,;)\n");
  return c == EOF || delims.find(c) != std::string::npos;
}

inline void RTrim(std::string& s, const std::string ws=" \n\r\t")
{
  s.erase(s.find_last_not_of(ws) + 1);
}

} // namespace

namespace mclisp
{

std::ostream& operator<<(std::ostream& os, Token token)
{
  switch (token)
  {
    case kComma: return os << ",";
    case kDot: return os << ".";
    case kOpenParen: return os << "(";
    case kCloseParen: return os << ")";
    case kQuote: return os << "'";
    case kNumber: return os << "Number";
    case kSymbol: return os << "Symbol";
    case kBadToken: return os << "BadToken";
    case kEofToken: return os << "EOF";
    default: ERROR("Unhandled Token: " + std::to_string(token));
  }
  // Not reached.
}

std::string TokenToString(Token token)
{
  std::ostringstream oss;
  oss << token;
  return oss.str();
}

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

  return std::all_of(
   start, end,
   [](char c) { return c == ' ' || c == '/' || isalnum(c); }
  );
}

Token Lexer::nextToken()
{
  char c = '\0';

  current_token_.clear();
  in_ >> std::skipws >> c;
  in_ >> std::noskipws;

  if (in_.eof())
    return kEofToken;

  current_token_ = c;

  if (c == '.') return kDot;
  if (c == ',') return kComma;
  if (c == '(') return kOpenParen;
  if (c == ')') return kCloseParen;
  if (c == '\'') return kQuote;
  if (c == ';')
  {
    in_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return nextToken();
  }

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
