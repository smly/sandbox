; ex 1.38
(define (euler-d n)
  (let ((a (remainder (+ 1 n) 3)))
    (if (= a 0)
	(* (/ (+ n 1) 3)
	   2)
	1)))

(define (e n)
  (+ (cont-frac (lambda (i) 1.0)
		euler-d
		n)
     2))
