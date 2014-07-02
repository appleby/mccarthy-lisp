#ifndef Eval_H_
#define Eval_H_

#include "cons.h"
#include "env.h"

namespace mclisp
{

ConsCell *Eval(const ConsCell *exp, ConsCell *env=env::g_user_env);

} // namespace mclisp
#endif // Eval_H_
