#include <fstream>
#include <iostream>

#include "eval.h"
#include "load.h"
#include "reader.h"
#include "utils.h"

namespace mclisp
{

void LoadFile(const std::string& fname)
{
  std::ifstream ifs(fname);
  Reader reader(ifs);

  std::cerr << "Loading " << fname << "... ";

  while (true)
  {
    ConsCell *exp = reader.Read();

    if (ShouldQuit(exp))
      break;

    Eval(exp);
  }

  std::cerr << "done." << std::endl;
}

} // mclisp
