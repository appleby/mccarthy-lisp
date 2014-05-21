#ifndef MCLISP_LEXER_H_
#define MCLISP_LEXER_H_

#include <iostream>

namespace mclisp
{

class Lexer
{
  std::istream& in_;

  public:
    explicit Lexer(std::istream& in=std::cin) : in_(in) {};
    int nextToken();
};

}; // namespace mclisp
#endif // MCLISP_LEXER_H_
