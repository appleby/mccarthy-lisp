#include <algorithm>
#include <cctype>

#include "reader.h"

namespace
{
void ToUpper(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}
} // namespace

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
  std::string symbolName = lexer_.current_token();
  ToUpper(symbolName);

  // TODO: Intern symbols
  if (symbolName == "NIL")
    return kNil;
  if (symbolName == "T")
    return kT;

  symbols_.emplace_front(symbolName);
  return symbols_.front();
}

const Sexp& Reader::Read()
{
  Token token = lexer_.nextToken();
  switch (token)
  {
    case kSymbol: return ReadSymbol();
    case kOpenParen: return ReadCons();
    case kEof: return kNil;
    default:
      throw ReadError("Expected start of Cons or Symbol, found: "
                      + std::to_string(token));
  }
}

} // namespace mclisp
