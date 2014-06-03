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

const Symbol& Reader::Intern(std::string name)
{
  ToUpper(name);
  return Intern(Symbol(name));
}

const Symbol& Reader::Intern(const Symbol& symbol)
{
  auto pair = symbols_.emplace(symbol.name(), symbol);
  return pair.first->second;
}

void Reader::Init()
{

  // At the moment, this is pointless since the Symbols are copied/moved into
  // the symbols_ map. We could just as well wait for the reader to read NIL or
  // T and only intern them then. Symbol equality is implemented by delegating
  // to string= on Symbol::name, so this copying-intern doesn't break anything.
  // Once the allocator/gc are implemented, we should instead intern pointers to
  // symbols so we can implement Symbol equality as pointer equality.
  Intern(kNil);
  Intern(kT);
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
  return Intern(lexer_.current_token());
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
