;;;; Functions from section 3.d, Recursive S-functions.

(defun, ff, (x),
   (cond, ((atom, x), x),
          (t, (ff, (car, x)))))

(defun, subst, (x, y, z),
   (cond, ((atom, z),
           (cond, ((eq, z, y), x),
                  (t, z))),
          (t, (cons, (subst, x, y, (car, z)),
                     (subst, x, y, (cdr, z))))))

(defun, not, (x),
   (cond, (x, nil),
          (t, t)))

;;; These are poor subsitutes for and/or since they are limited to two
;;; arguments and don't short-circuit. In the absence of macros, these will
;;; have to do.
(defun, or, (x, y),
   (cond, (x, t),
          (y, t),
          (t, nil)))

(defun, and, (x, y),
   (cond, (x, (cond, (y, t),
                     (t, nil))),
          (t, nil)))

(defun, equal, (x, y),
   (cond, ((and, (atom, x), (atom, y)), (eq, x, y)),
          ((and, (not, (atom, x)), (not, (atom, y))),
           (cond, ((equal, (car, x), (car, y)),
                   (equal, (cdr, x), (cdr, y))))),
          (t, nil)))

(defun, null, (x),
   (and, (atom, x), (eq, x, nil)))

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
