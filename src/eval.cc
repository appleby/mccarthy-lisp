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
      // TODO fix defun (maybe).
      //
      // This won't work as expected if the defun isn't a toplevel form. If a
      // non-toplevel defun is encountered, the binding it creates won't be
      // available until the next toplevel form executes, since we extend the
      // global user env, rather the current dynamic env. For example, the
      // following will cause an UnboundSymbol exception due to foo not being
      // bound in the body of the lambda:
      //
      // ((lambda (ignore) (foo))
      //  (defun foo () 'foo))
      //
      // Not that any sane person would write code like that... The defun
      // brokeness would be a bigger deal if mclisp supported some kind of
      // progn, implicit or otherwise, in which case one might reasonably expect
      // to be able to write internal defuns, like so:
      //
      // (defun foo (x)
      //    (defun helper (a) ...)
      //    (helper x))
      //
      // Even this is not idiomatic lisp, and it's unlikely you would want an
      // internal defun to both create a global binding *and* make the binding
      // immediately available in current env.  More likely, you want only the
      // latter, which can be accomplished in mclisp with a label expression.
      //
      // All the above whining aside, it wouldn't be hard to fix this by either
      // 1) restricting defuns to toplevel forms only, or 2) sticking the
      // binding on the symbol's plist, and falling back to plist lookup if
      // lookup in the dynamic env fails.
      ConsCell *fname = Cadr(exp);
      ConsCell *formals = Caddr(exp);
      ConsCell *body = Cdddr(exp);
      ConsCell *lambda = Append(List(BUILTIN(LAMBDA), formals), body);
      env::g_user_env = env::Extend(env::g_user_env, fname, lambda);
      return fname;
    }

    if (EQ(Car(exp), LOAD))
    {
      // For the same reasons as mentioned in the above comment about DEFUN,
      // LOAD only makes sense as a toplevel form.
      //
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
    return Eval(Caddar(exp),
                env::ExtendAll(env, Cadar(exp), Evlis(Cdr(exp), env)));

  // TODO throw an error.
  return MakeSymbol("42");
}

} // namespace mclisp
