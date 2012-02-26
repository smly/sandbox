; ex 1.44
; smoothing function
(define (smooth f)
  (define (dx 0.0001))
  (lambda (x) (/ (+ (f x)
		    (f (- x dx))
		    (f (+ x dx)))
		 3)))

; & n-fold smoothed function
(define (n-fold-smooth f n)
  ((repeated smooth n) f))
