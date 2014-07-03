#include <iostream>

#include "cons.h"
#include "eval.h"
#include "init.h"
#include "reader.h"

namespace
{

class Repl
{
  std::istream& in_;
  std::ostream& out_;
  std::string prompt_ = "mclisp> ";
  mclisp::Reader reader_;

  public:
    explicit Repl(std::istream& in=std::cin, std::ostream& out=std::cout):
      in_(in), out_(out), reader_(in_) {};

    int loop();
};

int Repl::loop()
{
  std::ostringstream oss;
  while (true)
  {
    out_ << prompt_;
    oss.str("");
    oss.clear();
    mclisp::ConsCell *exp = reader_.Read();
    oss << *exp;

    if (oss.str() == "QUIT" || oss.str() == "(QUIT . NIL)")
      // TODO implement real (quit) function.
      break;

    mclisp::ConsCell *value = Eval(exp);
    out_ << *value << std::endl;
  }
  return 0;
}


} //end namespace

int main()
{
  Repl repl;
  mclisp::InitLisp();
  return repl.loop();
}
