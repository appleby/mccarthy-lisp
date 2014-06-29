#include <algorithm>
#include <cctype>
#include <cstdarg>

#include "reader.h"

namespace
{

// The parameter is mutated, and therefore must be passed by value.
std::string ToUpper(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
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

Token Reader::AcceptTokens(std::set<Token> tokens)
{
  Token token = lexer_.nextToken();

  if (tokens.find(token) == tokens.end())
    throw BadTokenError(token, tokens);

  return token;
}

void Reader::Init()
{
  if (kNil == nullptr)
  {
    kNil = Intern("NIL");
    HackToFixNil();
  }
  else
    Intern(kNil);

  if (kT == nullptr)
    kT = Intern("T");
  else
    Intern(kT);
}

const ConsCell* Reader::Intern(const std::string& name)
{
  return Intern(MakeSymbol(ToUpper(name)));
}

const ConsCell* Reader::Intern(const ConsCell* symbol)
{
  auto pair = symbols_.emplace(SymbolName(symbol), symbol);
  return pair.first->second;
}

const ConsCell* Reader::ReadCons()
{
  const ConsCell* car = Read();
  const ConsCell* cdr = nullptr;
  Token token = AcceptTokens({kDot, kComma, kCloseParen});

  if (token == kDot)
  {
    cdr = Read();
    AcceptToken(kCloseParen);
  }
  else if (token == kComma)
  {
    cdr = ReadCons();
  }
  else if (token == kCloseParen)
    cdr = kNil;

  return MakeCons(car, cdr);
}

const ConsCell* Reader::ReadSymbol()
{
  return Intern(lexer_.current_token());
}

const ConsCell* Reader::Read()
{
  Token token = lexer_.nextToken();
  switch (token)
  {
    case kSymbol: return ReadSymbol();
    case kOpenParen: return ReadCons();
    case kEof: return kNil; // TODO: return Eof object.
    default:
      throw ReadError("Expected start of Cons or Symbol, found: "
                      + std::to_string(token));
  }
}

} // namespace mclisp
