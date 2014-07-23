;;;; A very ghetto test suite. All should evaluate to T.

;;; Syntax
(eq, (), nil)
(eq, ( ), nil)
(eq, '(), nil)

;;; Comments are OK.
(equal, t, ; so are in-line comments
        t)

;;; Elementary S-functions (builtins)
(eq, (atom, t), t)
(eq, (atom, nil), t)
(eq, (atom, 'foo), t)
(eq, (atom, '(foo)), nil)
(eq, (car, (cons, 'foo, 'bar)), 'foo)
(eq, (cdr, (cons, 'foo, 'bar)), 'bar)
(eq, (cond), nil)
(eq, (cond, (nil, 'bar), (t, 'foo)), 'foo)
(eq, ((label, lastcar,
        (lambda, (x),
           (cond, ((null, (cdr, x)), (car, x)),
                  (t, (lastcar, (cdr, x)))))),
      '(foo, bar, baz)),
     'baz)
(eq, ((lambda, (), 'foo)), 'foo)
(eq, ((lambda, (x), x), 'bar), 'bar)
(eq, (eval, ''foo), 'foo)
(eq, (eval, '((lambda, (), 'bar))), 'bar)
(eq, ((lambda, (x), (eval, 'x)), 'foo), 'foo)
(eq, (eval, (cons, 'car, '('(a, b)))), 'a)

;;; Recursive S-functions (defined in mclisp.lisp).
(eq, (ff, '((a . b) . c)), 'a)
(eq, (equal, 'x, 'x), t)
(eq, (equal, 'x, 'y), nil)
(eq, (equal, '((x, y), x), '((x, y), x)), t)
(eq, (equal, '((x, y), x), '((x, y), y)), nil)
(eq, (null, nil), t)
(eq, (null, 'x), nil)
(eq, (among, 'x, '(y, z, x)), t)
(eq, (among, 'x, '(y, z)), nil)
(eq, (assoc, 'x, '((a, b), (c, d), (x, y))), 'y)
(eq, (assoc, 'z, '((a, b), (c, d), (x, y))), 'notfound)
(equal, (append, '(x, y, z), '(x)), '(x, y, z, x))
(equal, (pair, '(x, y, z), '(x, y, z)), '((x, x), (y, y), (z, z)))
(equal, (subst, 'x, 'y, '(x, y, z)), '(x, x, z))
(equal, (sublis, '((x, (a, b)), (y, (b, c))), '(a, x . y)), '(a, (a, b), b, c))

;;; The Universal S-function apply
(eq, (apply, ff, '(((a, b), c))), 'a)
(eq, (apply, 'car, '((foo, bar))), 'foo)
(eq, (apply, '(lambda, (), 'bar), '()), 'bar)

;;; Misc utilities. Also defined in mclisp.lisp.
(eq, (not, t), nil)
(eq, (not, nil), t)
(eq, (not, 'foo), nil)
(eq, (null, t), nil)
(eq, (null, nil), t)
(eq, (null, 'foo), nil)
(eq, (null, '(foo)), nil)
(eq, (and), t)
(eq, (and, t), t)
(eq, (and, nil), nil)
(eq, (and, 'foo), t)
(eq, (and, t, t), t)
(eq, (and, nil, t), nil)
(eq, (and, t, nil), nil)
(eq, (and, t, t, t), t)
(eq, (and, t, t, nil), nil)
(eq, (or), nil)
(eq, (or, t), t)
(eq, (or, nil), nil)
(eq, (or, t, nil), t)
(eq, (or, nil, t), t)
(eq, (or, nil, nil), nil)
(eq, (or, nil, nil, t), t)
(eq, (or, nil, nil, nil), nil)
(equal, (list), nil)
(equal, (list, 'a), '(a))
(equal, (list, 'a, 'b), '(a, b))
(equal, (list, 'a, '(b, c)), '(a, (b, c)))
