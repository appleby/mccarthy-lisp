#ifndef READER_H_
#define READER_H_

#include <istream>
#include <list>
#include <map>
#include <set>
#include <string>

#include "cons.h"
#include "error.h"
#include "lexer.h"
#include "utils.h"

namespace mclisp
{

class ReadError: public Error
{
  public:
    explicit ReadError(const char* file, const char* func, int line,
                       const std::string& what_arg):
      Error(file, func, line, what_arg) {};

    explicit ReadError(const char* file, const char* func, int line,
                       const char* what_arg):
      Error(file, func, line, std::string(what_arg)) {};

  protected:
    virtual const char* prefix() const noexcept
    { return "Read Error:"; }
};

#define READ_ERROR(msg) THROW(ReadError, (msg))

class BadTokenError: public ReadError
{
  public:
    explicit BadTokenError(const char* file, const char* func, int line,
                           Token actual, Token expected):
      ReadError(file, func, line,
                "expected: " + std::to_string(expected)
                + ", but found: " + std::to_string(actual)) {};

    explicit BadTokenError(const char* file, const char* func, int line,
                           Token actual, std::set<Token> expected):
      ReadError(file, func, line,
                + ": expected: " + container_to_string<>(expected)
                + ", but found: " + std::to_string(actual)) {};

  protected:
    virtual const char* prefix() const noexcept
    { return "Bad Token Error:"; }
};

#define BAD_TOKEN_ERROR(actual, expected) THROW(BadTokenError, actual, expected)

class Reader
{
  private:
    bool initialized_;
    Lexer lexer_;
    std::map<std::string, ConsCell *> symbols_;

    void AcceptToken(Token token);
    Token AcceptTokens(std::set<Token> tokens);
    ConsCell* ReadCons();
    ConsCell* ReadSymbol();
    ConsCell* ReadQuotation();
    void Init();  // Intern NIL and T.

  public:
    explicit Reader(std::istream& in=std::cin) : initialized_(false), lexer_(in)
    { Init(); };
    explicit Reader(const std::string& in) : initialized_(false), lexer_(in)
    { Init(); };

    ConsCell* Intern(const std::string& name);
    ConsCell* Intern(ConsCell* symbol);
    ConsCell* Read();
};

} // namespace mclisp
#endif // READER_H_
