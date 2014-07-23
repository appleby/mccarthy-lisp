#include <iostream>
#include <limits>

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
      // C++ doesn't seem to provide a way to discard all remaining input from
      // cin. cin.peek() will block rather than return EOF when the buffer is
      // empty, and both cin.readsome() and cin.sync() appear to be no-ops.
      // cin.ignore() isn't a perfect solution, since the expression we're
      // trying to ignore may span multiple lines, but probably 90% of repl
      // expressions are one-liners.
      in_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}


} //end namespace

int main(int argc, char const *argv[])
{
  mclisp::InitLisp();
  mclisp::LoadFile("mclisp.lisp");

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
      mclisp::LoadFile(argv[i]);

    return 0;
  }

  Repl repl;
  return repl.loop();
}
