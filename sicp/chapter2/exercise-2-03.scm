; exercise 2.3
;  a, b and c are ocnsecutive vertex of a rectangle
(define (make-rectangle a b c)
  (cons (sqrt (+ (double (- (x-point a) (x-point b)))
                 (double (- (y-point a) (y-point b)))))
        (sqrt (+ (double (- (x-point b) (x-point c)))
                 (double (- (y-point b) (y-point c)))))))
(define (perimeter r)
  (+ (* 2 (car r))
     (* 2 (cdr r))))
(define (area r) (* (car r) (cdr r)))

; (define (make-rectangle width height) (cons width height))
