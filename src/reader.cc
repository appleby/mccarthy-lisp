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

void Reader::AcceptToken(Token expected_token)
{
  Token token = lexer_.nextToken();
  if (token != expected_token)
    BAD_TOKEN_ERROR(token, expected_token);
}

Token Reader::AcceptTokens(std::set<Token> tokens)
{
  Token token = lexer_.nextToken();

  if (tokens.find(token) == tokens.end())
    BAD_TOKEN_ERROR(token, tokens);

  return token;
}

void Reader::Init()
{
  
  if (initialized_)
    return;

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
  ConsCell* quote = BUILTIN(QUOTE);
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
    case kEofToken: return BUILTIN(EOF);
    default:
      READ_ERROR("Expected start of Cons or Symbol, found: "
                 + lexer_.current_token());
  }
}

} // namespace mclisp
