#include <iostream>

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
  while (oss.str() != "QUIT" && oss.str() != "(QUIT . NIL)")
  {
    out_ << prompt_;
    oss.str("");
    oss.clear();
    const mclisp::ConsCell *exp = reader_.Read();
    oss << *exp;
    out_ << *exp << std::endl;
  }
  return 0;
}


}; //end namespace

int main()
{
  Repl repl;
  return repl.loop();
}
