#include <iostream>

namespace
{

class Repl
{
  std::istream& in_;
  std::ostream& out_;
  std::string prompt_ = "mclisp> ";

  public:
    Repl(std::istream& in=std::cin, std::ostream& out=std::cout) : in_(in), out_(out) {};
    int loop();
};

int Repl::loop()
{
  std::string val;

  while (val != "quit")
  {
    out_ << prompt_;
    in_ >> val;
    out_ << val << std::endl;
  }
  return 0;
}


}; //end namespace

int main(int argc, const char **argv)
{
  Repl repl;
  return repl.loop();
}
