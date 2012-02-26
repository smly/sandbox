(define (close-enough? x y)
  (< (abs (- x y)) 0.001))

(define (average x y)
  (/ (+ x y) 2))

; finding fixed point
(define tolerance 0.00001)
  (define (fixed-point f first-guess)
    (define (close-enough? v1 v2)
      (< (abs (- v1 v2)) tolerance))

    (define (try guess)
      (let ((next (f guess)))
         (if (close-enough? guess next)
             next
             (try next))))

    (try first-guess))

; gosh> (fixed-point (lambda (g) (+ 1 (/ 1 g))) 1.0)
; 1.6180327868852458
