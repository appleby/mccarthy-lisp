#include <map>

#include "reader.h"
#include "init.h"
#include "utils.h"

namespace
{

std::map<std::string, mclisp::ConsCell *> g_interned_symbols;

} // namespace

namespace mclisp
{

namespace reader
{
void Init()
{
  static bool initialized = false;
  
  if (initialized)
    return;

  for (auto it : g_builtin_symbols)
    Intern(it.second);

  initialized = true;
}

ConsCell* Intern(ConsCell* symbol)
{
  auto pair = g_interned_symbols.emplace(SymbolName(symbol), symbol);
  return pair.first->second;
}

ConsCell* Intern(const std::string& name)
{
  return Intern(MakeSymbol(ToUpper(name)));
}
} // namespace reader

const std::string BadTokenError::ConstructWhat(const std::string& actual,
                                               Token expected)
{
  return ("expected: " + TokenToString(expected) +
          ", but found: " + actual);
}

const std::string BadTokenError::ConstructWhat(const std::string& actual,
                                               std::set<Token> expected)
{
  return ("expected one of: " + ContainerToString<>(expected) +
          ", but found: " + actual);
}

void Reader::AcceptToken(Token expected_token)
{
  Token token = lexer_.nextToken();
  if (token != expected_token)
    BAD_TOKEN_ERROR(lexer_.current_token(), expected_token);
}

Token Reader::AcceptTokens(std::set<Token> tokens)
{
  Token token = lexer_.nextToken();

  if (tokens.find(token) == tokens.end())
    BAD_TOKEN_ERROR(lexer_.current_token(), tokens);

  return token;
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
  return reader::Intern(lexer_.current_token());
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
    case kNilToken: return kNil;
    default:
      READ_ERROR("Expected start of Cons or Symbol, found: "
                 + lexer_.current_token());
  }
}

} // namespace mclisp
