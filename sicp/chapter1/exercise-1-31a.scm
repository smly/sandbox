(define (product term a next b)
  (if (> a b)
      1
      (* (term a)
	 (product term (next a) next b))))

(define (factorial f a b)
  (define (inc n) (+ n 1))
  (product-it f a inc b))

(define (pi n)
  (define (f x)
    (if (odd? x)
	(/ (+ x 1) (+ x 2))
	(/ (+ x 2) (+ x 1))))
  (* 4.0 (factorial f 1 n)))
