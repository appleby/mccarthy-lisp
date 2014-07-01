#ifndef Eval_H_
#define Eval_H_

#include "cons.h"

namespace mclisp
{

const ConsCell *Eval(const ConsCell *exp, ConsCell *env=nullptr);

} // namespace mclisp
#endif // Eval_H_
