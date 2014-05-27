#include "reader.h"

namespace mclisp
{

const std::string ReadError::err_prefix_("Read Error: ");
const std::string BadTokenError::err_prefix_("Bad Token: ");

void Reader::AcceptToken(Token expected_token)
{
  Token token = lexer_.nextToken();
  if (token != expected_token)
    throw BadTokenError(token, expected_token);
}

const Cons& Reader::ReadCons()
{
  const Sexp& car = Read();
  AcceptToken(kDot);
  const Sexp& cdr = Read();
  AcceptToken(kCloseParen);
  conses_.emplace_front(car, cdr);
  return conses_.front();
}

const Symbol& Reader::ReadSymbol()
{
  symbols_.emplace_front(lexer_.current_token());
  return symbols_.front();
}

const Sexp& Reader::Read()
{
  Token token = lexer_.nextToken();
  if (token == kSymbol)
    return ReadSymbol();
  if (token == kOpenParen)
    return ReadCons();
  return kNil;
}

} // namespace mclisp
