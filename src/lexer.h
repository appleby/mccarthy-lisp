#ifndef MCLISP_LEXER_H_
#define MCLISP_LEXER_H_

#include <iostream>
#include <sstream>

namespace mclisp
{

enum Token { kBadToken, kSymbol, kNumber, kCons, kEof };

class Lexer
{
  std::istringstream ss_;
  std::istream& in_;

  public:
    explicit Lexer(std::istream& in=std::cin) : in_(in) {};
    explicit Lexer(const std::string& in) : ss_(in), in_(ss_) {};
    Token nextToken();
};

}; // namespace mclisp
#endif // MCLISP_LEXER_H_
