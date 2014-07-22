#ifndef MCLISP_LEXER_H_
#define MCLISP_LEXER_H_

#include <iostream>
#include <sstream>
#include <string>

namespace mclisp
{

// Sect. 3.a A Class of Symbolic Expressions
// We shall now define the S-expressions... They are formed by using the special
// characters '.', '(', ')', and an infinite set of distinguishable atomic
// symbols. For atomic symbols, we shall use strings of capital Latin letters
// and digits with single imbedded blanks.
// 
// Note that we slightly relax the constraints on atomic symbols by allowing
// both upper and lowercase letters, and multiple embedded blanks.

// XXX When updating this enum, be sure to also update operator<< to handle new
// or removed Tokens.
enum Token
{
  // Delimiters
  kComma,
  kDot,
  kOpenParen,
  kCloseParen,

  // Syntax
  kQuote,

  // Atoms
  kNumber,
  kSymbol,
  kNilToken,

  // Control
  kBadToken,
  kEofToken
};

std::ostream& operator<<(std::ostream& os, Token token);
std::string TokenToString(Token token);

class Lexer
{
  private:
    std::istringstream ss_;
    std::istream& in_;
    std::string current_token_;

    // True if current_token_ matches /^[+\-]?\d+$/.
    bool CurrentTokenIsValidNumber();

    // True if current_token_ matches /^[:alphanum: ]+$/.
    bool CurrentTokenIsValidSymbol();

  public:
    explicit Lexer(std::istream& in=std::cin) : in_(in) {}
    explicit Lexer(const std::string& in) : ss_(in), in_(ss_) {}

    Token nextToken();

    const std::string& current_token() const { return current_token_; }
};

}; // namespace mclisp
#endif // MCLISP_LEXER_H_
