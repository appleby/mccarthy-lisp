#ifndef Eval_H_
#define Eval_H_

#include "cons.h"

namespace mclisp
{

ConsCell *Eval(const ConsCell *exp, ConsCell *env=kNil);

} // namespace mclisp
#endif // Eval_H_
