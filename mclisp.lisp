;;;; This file is automatically loaded in mclisp main().

;;; Functions from section 3.c, The Elementary S-functions and Predicates.
;;; These are all implemented as special forms. We wrap them in functions here
;;; so they can be treated like lambdas.
(defun, atom, (x), (atom, x))
(defun, eq, (x, y), (eq, x, y))
(defun, car, (x), (car, x))
(defun, cdr, (x), (cdr, x))
(defun, cons, (x, y), (cons, x, y))
(defun, eval, (x), (eval, x))

;;; These are not mentioned in the paper.
(defun, load, (x), (load, x))
(defun, print, (x), (print, x))


;;; Functions from section 3.d, Recursive S-functions.
(defun, ff, (x),
   (cond, ((atom, x), x),
          (t, (ff, (car, x)))))

(defun, subst, (x, y, z),
   (cond, ((atom, z),
           (cond, ((eq, z, y), x),
                  (t, z))),
          (t, (cons, (subst, x, y, (car, z)),
                     (subst, x, y, (cdr, z))))))

(defun, equal, (x, y),
   (cond, ((and, (atom, x), (atom, y)), (eq, x, y)),
          ((and, (not, (atom, x)), (not, (atom, y))),
           (cond, ((equal, (car, x), (car, y)),
                   (equal, (cdr, x), (cdr, y))))),
          (t, nil)))

(defun, append, (x, y),
   (cond, ((null, x), y),
          (t, (cons, (car, x), (append, (cdr, x), y)))))

(defun, among, (x, y),
   (cond, ((not, (null, y)),
           (or, (eq, x, (car, y)),
                (among, x, (cdr, y)))),
          (t, nil)))

(defun, list, lst, lst)

(defun, pair, (x, y),
   (cond, ((and, (null, x), (null, y)), nil),
          ((and, (not, (atom, x)), (not, (atom, y))),
           (cons, (list, (car, x), (car, y)),
                  (pair, (cdr, x), (cdr, y)))),
          (t, nil)))

(defun, caar, (x),
   (car, (car, x)))

(defun, cadar, (x),
   (car, (cdr, (car, x))))

(defun, assoc, (x, y),
   (cond, ((null, y), 'notfound),
          ((eq, (caar, y), x), (cadar, y)),
          (t, (assoc, x, (cdr, y)))))

(defun, sub2, (x, z),
   (cond, ((null, x), z),
          ((eq, (caar, x), z), (cadar, x)),
          (t, (sub2, (cdr, x), z))))

(defun, sublis, (x, y),
   (cond, ((atom, y), (sub2, x, y)),
          (t, (cons, (sublis, x, (car, y)), (sublis, x, (cdr, y))))))


;;; Section 3.f The Universal S-function apply
(defun, apply, (f, args),
   (eval, (cons, f, (appq, args))))

(defun, appq, (m),
   (cond, ((null, m), nil),
          (t, (cons, (list, 'quote, (car, m)), (appq, (cdr, m))))))


;;; Section 3.g Functions with Functions as Arguments
(defun, maplist, (e, f),
   ;; Renamed the first argument from x to e to prevent shadowing the x in
   ;; diff, below.
   (cond, ((null, e), nil),
          (t, (cons, (f, e), (maplist, (cdr, e), f)))))

;;; Partial derivative of y w.r.t. x.
(defun, diff, (y, x),
 (cond, ((atom, y),
         (cond, ((eq, y, x), 'one),
                (t, 'zero))),
        ((eq, (car, y), 'plus),
         (cons, 'plus,
                (maplist, (cdr, y),
                          '(lambda, (z), (diff, (car, z), x))))),
        ((eq, (car, y), 'times),
         (cons, 'plus,
                (maplist, (cdr, y),
                          '(lambda, (z),
                            (cons, 'times,
                                   (maplist, (cdr, y),
                                             '(lambda, (w),
                                               ;; There was a minor bug here.
                                               ;; McCarthy wrote (eq, z, w),
                                               ;; but z and w are always lists.
                                               (cond, ((not, (equal, z, w)), (car, w)),
                                                      (t, (diff, (car, w), x))))))))))))

(defun, search, (x, p, f, u),
   (cond, ((null, x), (u)),
          ((p, x), (f, x)),
          (t, (search, (cdr, x), p, f, u))))

;;; Utilities
(defun, not, (x),
   (cond, (x, nil),
          (t, t)))

(defun, null, (x),
   (cond, ((atom, x), (eq, x, nil))))

;;; These are poor subsitutes for and/or since they don't short-circuit.  In
;;; the absence of macros, these will have to do.
(defun, and, args,
   (cond, ((null, args), t),
          ((not, (car, args)), nil),
          (t, (apply, and, (cdr, args)))))

(defun, or, args,
   (cond, ((null, args), nil),
          ((car, args), t),
          (t, (apply, or, (cdr, args)))))

