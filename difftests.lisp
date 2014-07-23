(print, 'difftests)
(print, (equal, (diff, 'y, 'x), 'zero))
(print, (equal, (diff, 'x, 'x), 'one))
(print, (equal, (diff, '(plus, x, x), 'x), '(plus, one, one)))
(print, (equal, (diff, '(plus, x, y), 'x), '(plus, one, zero)))
(print, (equal, (diff, '(times, x, x), 'x),
                '(plus, (times, one, x), (times, x, one))))
(print, (equal, (diff, '(times, x, y), 'x),
                '(plus, (times, one, y), (times, x, zero))))
(print, (equal, '(plus, (times, one, (plus, x, a), y),
                        (times, x, (plus, one, zero), y),
                        (times, x, (plus, x, a), zero)),
        (diff, '(times, x, (plus, x, a), y), 'x)))
