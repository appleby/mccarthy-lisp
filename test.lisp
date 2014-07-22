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

;;; Not mentioned in McCarthy's paper, but useful nonetheless. Also defined in
;;; mclisp.lisp.
(equal, (list), nil)
(equal, (list, 'a), '(a))
(equal, (list, 'a, 'b), '(a, b))
(equal, (list, 'a, '(b, c)), '(a, (b, c)))
