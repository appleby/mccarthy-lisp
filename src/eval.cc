#include "eval.h"
#include "env.h"

namespace
{
using namespace mclisp;

// TODO Move these to separate module.
ConsCell* Append(const ConsCell* L1, ConsCell* L2)
{
  TYPECHECK(L1, Listp);
  TYPECHECK(L2, Listp);

  if (Null(L1))
    return L2;

  return Cons(Car(L1), Append(Cdr(L1), L2));
}

ConsCell* Pair(const ConsCell *L1, const ConsCell *L2)
{
  TYPECHECK(L1, Listp);
  TYPECHECK(L2, Listp);

  if (Null(L1) && Null(L2))
    return kNil;

  return Cons(List(Car(L1), Car(L2)), Pair(Cdr(L1), Cdr(L2)));
}

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
#define EQ(exp, sym) Eq(exp, g_builtin_symbols[#sym])

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

    return Eval(Cons(Assoc(Car(exp), env), Cdr(exp)), env);
  }

  if (EQ(Caar(exp), LABEL))
    return Eval(Cons(Caddar(exp), Cdr(exp)), Cons(List(Cadar(exp), Car(exp)), env));

  if (EQ(Caar(exp), LAMBDA))
    return Eval(Caddar(exp), Append(Pair(Cadar(exp), Evlis(Cdr(exp), env)), env));

#undef EQ
  return MakeSymbol("42");
}

} // namespace mclisp
