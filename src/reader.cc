#include <algorithm>
#include <cctype>
#include <cstdarg>

#include "reader.h"
#include "init.h"

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
  
  if (initialized_)
    return;

  InitLisp();

  for (auto it : g_builtin_symbols)
    Intern(it.second);

  initialized_ = true;
}

ConsCell* Reader::Intern(const std::string& name)
{
  return Intern(MakeSymbol(ToUpper(name)));
}

ConsCell* Reader::Intern(ConsCell* symbol)
{
  auto pair = symbols_.emplace(SymbolName(symbol), symbol);
  return pair.first->second;
}

ConsCell* Reader::ReadCons()
{
  ConsCell* car = Read();
  ConsCell* cdr = nullptr;
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

  return Cons(car, cdr);
}

ConsCell* Reader::ReadSymbol()
{
  return Intern(lexer_.current_token());
}

ConsCell* Reader::ReadQuotation()
{
  ConsCell* quote = g_builtin_symbols["QUOTE"];
  ConsCell* sexp = Read();
  return List(quote, sexp);
}

ConsCell* Reader::Read()
{
  Token token = lexer_.nextToken();
  switch (token)
  {
    case kQuote: return ReadQuotation();
    case kSymbol: return ReadSymbol();
    case kOpenParen: return ReadCons();
    case kEofToken: return g_builtin_symbols["EOF"];
    default:
      throw ReadError("Expected start of Cons or Symbol, found: "
                      + std::to_string(token));
  }
}

} // namespace mclisp
