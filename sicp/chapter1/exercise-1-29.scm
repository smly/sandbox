(define (cube a) (* a a a))

(define (sum term a next b)
  (if (> a b)
      0
      (+ (term a)
	 (sum term (next a) next b))))

(define (integral f a b dx)
  (define (add-dx x) (+ x dx))
  (* (sum f (+ a (/ dx 2.0)) add-dx b)
     dx))

(define (simpsons f a b n)
  (define (inc2 n) (+ n 2))
  (define h (/ (- b a) n))
  (define (y k) (f (+ a (* k h))))
  (define (S1 f n) (sum y 1 inc2 (- n 1)))
  (define (S2 f n) (sum y 2 inc2 (- n 1)))
  (* (/ (- b a) (* n 3.0))
     (+ (y 0)
	(* 4 (S1 f n))
	(* 2 (S2 f n))
	(y n))))
