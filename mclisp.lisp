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

(defun, or, (x, y),
   (cond, (x, t),
          (y, t),
          (t, nil)))

(defun, and, (x, y),
   (cond, (x, (cond, (y, t),
                     (t, nil))),
          (t, nil)))

(defun, and3, (x, y, z),
   (and, x, (and, y, z)))

(defun, and4, (w, x, y, z),
   (and, w, (and3, x, y, z)))

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

(defun, list2, (x, y),
   (cons, x, (cons, y, nil)))

(defun, pair, (x, y),
   (cond, ((and, (null, x), (null, y)), nil),
          ((and, (not, (atom, x)), (not, (atom, y))),
           (cons, (list2, (car, x), (car, y)),
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

(ff, '((a . b) . c))
(subst, 'x, 'y, '(x, y, z))
(equal, 'x, 'x)
(equal, 'x, 'y)
(equal, '((x, y), x), '((x, y), x))
(equal, '((x, y), x), '((x, y), y))
(null, nil)
(null, 'x)
(append, '(x, y, z), '(x))
(among, 'x, '(y, z, x))
(among, 'x, '(y, z))
(pair, '(x, y, z), '(x, y, z))
(assoc, 'x, '((a, b), (c, d), (x, y)))
(assoc, 'z, '((a, b), (c, d), (x, y)))
(sublis, '((x, (a, b)), (y, (b, c))), '(a, x . y))
