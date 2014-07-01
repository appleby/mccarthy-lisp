#include "eval.h"

namespace mclisp
{

const ConsCell *Eval(const ConsCell *exp, ConsCell *env /* nullptr */)
{
  return MakeSymbol("42");
}

} // namespace mclisp
