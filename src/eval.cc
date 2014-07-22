#include <iostream>

#include "eval.h"
#include "env.h"
#include "load.h"
#include "utils.h"

namespace
{
using namespace mclisp;

ConsCell *Evcon(const ConsCell *clauses, ConsCell *env)
{
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
      ConsCell *fname = Cadr(exp);
      ConsCell *formals = Caddr(exp);
      ConsCell *body = Cdddr(exp);
      ConsCell *lambda = Append(List(BUILTIN(LAMBDA), formals), body);
      env::g_user_env = env::Extend(env::g_user_env, fname, lambda);
      return fname;
    }

    if (EQ(Car(exp), EVAL))
      return Eval(Eval(Cadr(exp), env), env);

    if (EQ(Car(exp), LOAD))
    {
      // We append ".lisp" because '.' can't be a valid symbol character (since
      // symbols can have whitespace and '.' is also used for dotted lists).
      LoadFile(ToLower(SymbolName(Eval(Cadr(exp), env))) + ".lisp");
      return kNil;
    }

    if (EQ(Car(exp), PRINT))
    {
      ConsCell *v = Eval(Cadr(exp), env);
      std::cerr << *v << std::endl;
      return v;
    }

    return Eval(Cons(env::Lookup(env, Car(exp)), Cdr(exp)), env);
  }

  if (EQ(Caar(exp), LABEL))
    return Eval(Cons(Caddar(exp), Cdr(exp)),
                env::Extend(env, Cadar(exp), Car(exp)));

  if (EQ(Caar(exp), LAMBDA))
  {
    ConsCell *body = Caddar(exp);
    ConsCell *formals = Cadar(exp);
    ConsCell *parameters = Cdr(exp);

    if (Listp(formals))
      PARITY_CHECK(formals, parameters);

    // The double Listp check is wasteful, but I think it makes the code easier
    // to read.
    auto extendfn = Listp(formals) ? env::ExtendAll : env::Extend;
    ConsCell *values = Evlis(parameters, env);
    ConsCell *newenv = extendfn(env, formals, values);

    return Eval(body, newenv);
  }

  ERROR("Invalid expression: " + ToString(exp));
}

} // namespace mclisp
