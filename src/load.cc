#include <fstream>
#include <iostream>

#include "error.h"
#include "eval.h"
#include "load.h"
#include "reader.h"

namespace mclisp
{

void LoadFile(const std::string& fname)
{
  std::ifstream ifs(fname);
  Reader reader(ifs);

  if (!ifs.good())
    ERROR("Failed to open file " + fname);

  std::cerr << "Loading " << fname << "... ";

  while (ifs.good())
  {
    ConsCell *exp = reader.Read();

    if (ShouldQuit(exp))
      break;

    Eval(exp);
  }

  std::cerr << "done." << std::endl;
}

} // mclisp
