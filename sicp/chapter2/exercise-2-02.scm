; exercise 2.2
(define (average x y)
  (/ (+ x y) 2))

(define (print-point p)
  (newline)
  (display "(")
  (display (x-point p))
  (display ",")
  (display (y-point p))
  (display ")"))

(define (x-point p) (car p))
(define (y-point p) (cdr p))
(define (make-point x y) (cons x y))
(define (make-segment st ed) (cons st ed))
(define (start-segment s) (car s))
(define (end-segment s) (cdr s))

(define (midpoint-segment s)
  (let ((st (start-segment s))
        (ed (end-segment s)))
    (make-point (average (x-point st) (x-point ed))
                (average (y-point st) (y-point ed)))))
