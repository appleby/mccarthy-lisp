#include "lexer.h"

namespace mclisp
{

Token Lexer::nextToken()
{
  char c;
  in_ >> std::skipws >> c;

  if (in_.eof())
    return kEof;

  if (c == '(')
    return kCons;

  return kBadToken;
}

}; // namespace mclisp
