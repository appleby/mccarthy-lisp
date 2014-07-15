#include "eval.h"
#include "env.h"

namespace
{
using namespace mclisp;

ConsCell *Evcon(const ConsCell *clauses, ConsCell *env)
{
  // TODO Might want throw something more descriptive than TypeError.
  TYPECHECK(clauses, Listp);

  if (Null(clauses))
    return kNil;

  TYPECHECK(Car(clauses), Consp);

  if (*Eval(Caar(clauses), env))
    return Eval(Cadar(clauses), env);

  return Evcon(Cdr(clauses), env);
}

ConsCell* Evlis(ConsCell* lst, ConsCell* env)
{
  TYPECHECK(lst, Listp);

  if (Null(lst))
    return kNil;

  return Cons(Eval(Car(lst), env), Evlis(Cdr(lst), env));
}
} // namespace

namespace mclisp
{

ConsCell *Eval(const ConsCell *exp, ConsCell *env /* env::g_user_env */)
{
  if (Atom(exp))
    return env::Lookup(env, exp);

  if (Atom(Car(exp)))
  {
    if (EQ(Car(exp), QUOTE))
      return Cadr(exp);

    if (EQ(Car(exp), ATOM))
      return FromBool(Atom(Eval(Cadr(exp), env)));

    if (EQ(Car(exp), EQ))
      return FromBool(Eq(Eval(Cadr(exp), env), Eval(Caddr(exp), env)));

    if (EQ(Car(exp), COND))
      return Evcon(Cdr(exp), env);

    if (EQ(Car(exp), CAR))
      return Car(Eval(Cadr(exp), env));

    if (EQ(Car(exp), CDR))
      return Cdr(Eval(Cadr(exp), env));

    if (EQ(Car(exp), CONS))
      return Cons(Eval(Cadr(exp), env), Eval(Caddr(exp), env));

    if (EQ(Car(exp), DEFUN))
    {
      // TODO fix defun.
      //
      // This won't work if the defun isn't a toplevel form.
      ConsCell *fname = Cadr(exp);
      ConsCell *formals = Caddr(exp);
      ConsCell *body = Cdddr(exp);
      ConsCell *lambda = Append(List(BUILTIN(LAMBDA), formals), body);
      env::g_user_env = env::Extend(env::g_user_env, fname, lambda);
      return kNil;
    }

    return Eval(Cons(env::Lookup(env, Car(exp)), Cdr(exp)), env);
  }

  if (EQ(Caar(exp), LABEL))
    return Eval(Cons(Caddar(exp), Cdr(exp)),
                env::Extend(env, Cadar(exp), Car(exp)));

  if (EQ(Caar(exp), LAMBDA))
    return Eval(Caddar(exp),
                env::ExtendAll(env, Cadar(exp), Evlis(Cdr(exp), env)));

  return MakeSymbol("42");
}

} // namespace mclisp
