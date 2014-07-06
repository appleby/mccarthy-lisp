#ifndef READER_H_
#define READER_H_

#include <istream>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include "cons.h"
#include "lexer.h"
#include "utils.h"

namespace mclisp
{

class ReadError: public std::logic_error
{
  private:
    static const std::string err_prefix_;

  public:
    explicit ReadError(const std::string& what_arg):
      logic_error(err_prefix_ + what_arg) {};

    explicit ReadError(const char* what_arg):
      logic_error(err_prefix_ + what_arg) {};
};

class BadTokenError: public ReadError
{
  private:
    static const std::string err_prefix_;

  public:
    explicit BadTokenError(Token actual, Token expected) :
      ReadError(err_prefix_ + "expected " + std::to_string(expected) +
                ", but found: " + std::to_string(actual)) {};
    explicit BadTokenError(Token actual, std::set<Token> expected) :
      ReadError(err_prefix_ + "expected " + container_to_string<>(expected) +
                ", but found: " + std::to_string(actual)) {};
};

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
