#ifndef READER_H_
#define READER_H_

#include <istream>
#include <list>
#include <map>
#include <stdexcept>
#include <string>

#include "lexer.h"
#include "objects.h"

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
};

class Reader
{
  private:
    Lexer lexer_;
    std::map<std::string, Symbol> symbols_;
    std::list<Cons> conses_;

    void AcceptToken(Token token);
    const Cons& ReadCons();
    const Symbol& ReadSymbol();
    const Symbol& Intern(const std::string& name);
    const Symbol& Intern(const Symbol& symbol);
    void Init();  // Intern NIL and T.

  public:
    explicit Reader(std::istream& in=std::cin) : lexer_(in) { Init(); };
    explicit Reader(const std::string& in) : lexer_(in) { Init(); };

    const Sexp& Read();
};

} // namespace mclisp
#endif // READER_H_
