#ifndef READER_H_
#define READER_H_

#include <istream>
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
                           const std::string& actual, Token expected):
      ReadError(file, func, line, ConstructWhat(actual, expected)) {}

    explicit BadTokenError(const char* file, const char* func, int line,
                           const std::string& actual, std::set<Token> expected):
      ReadError(file, func, line, ConstructWhat(actual, expected)) {}

  protected:
    const std::string ConstructWhat(const std::string& actual, Token expected);
    const std::string ConstructWhat(const std::string& actual,
                                    std::set<Token> expected);

    virtual const char* prefix() const noexcept
    { return "Bad Token Error:"; }
};

#define BAD_TOKEN_ERROR(actual, expected) THROW(BadTokenError, actual, expected)

class Reader
{
  private:
    Lexer lexer_;

    void AcceptToken(Token token);
    Token AcceptTokens(std::set<Token> tokens);
    ConsCell* ReadCons();
    ConsCell* ReadSymbol();
    ConsCell* ReadQuotation();

  public:
    explicit Reader(std::istream& in=std::cin) : lexer_(in) {}
    explicit Reader(const std::string& in) : lexer_(in) {}

    ConsCell* Read();
};

namespace reader
{

void Init();
ConsCell* Intern(ConsCell* symbol);
ConsCell* Intern(const std::string& name);

} // namespace reader
} // namespace mclisp
#endif // READER_H_
