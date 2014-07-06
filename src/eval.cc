#include "eval.h"
#include "env.h"

namespace mclisp
{
ConsCell *Eval(const ConsCell *exp, ConsCell *env /* env::g_user_env */)
{
  if (Atom(exp))
    return env::Lookup(env, exp);

  if (Atom(Car(exp)))
  {
    if (Eq(Car(exp), g_builtin_symbols["QUOTE"]))
      return Cadr(exp);

    if (Eq(Car(exp), g_builtin_symbols["ATOM"]))
      return FromBool(Atom(Eval(Cadr(exp), env)));
  }

  return MakeSymbol("42");
}

} // namespace mclisp
