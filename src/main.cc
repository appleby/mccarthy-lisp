#include <iostream>

#include "cons.h"
#include "error.h"
#include "eval.h"
#include "init.h"
#include "load.h"
#include "reader.h"
#include "utils.h"

namespace
{

class Repl
{
  std::istream& in_;
  std::ostream& out_;
  std::string prompt_ = "mclisp> ";
  mclisp::Reader reader_;

  public:
    explicit Repl(std::istream& in=std::cin, std::ostream& out=std::cerr):
      in_(in), out_(out), reader_(in_) {};

    int loop();
};

int Repl::loop()
{
  while (true)
  {
    out_ << prompt_;
    try
    {
      mclisp::ConsCell *exp = reader_.Read();

      if (mclisp::ShouldQuit(exp))
        break;

      mclisp::ConsCell *value = Eval(exp);
      out_ << *value << std::endl;
    }
    catch (mclisp::Error& e)
    {
      out_ << e.what() << std::endl;
    }
  }
  return 0;
}


} //end namespace

int main()
{
  // InitLisp must be called before the Reader object is initialized.
  mclisp::InitLisp();
  mclisp::LoadFile("mclisp.lisp");

  Repl repl;
  return repl.loop();
}
