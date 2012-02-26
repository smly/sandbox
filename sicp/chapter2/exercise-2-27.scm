; 2.27
(define a (list (list 1 2) (list 3 4)))

(define (deep-reverse items)
  (define (deep-reverse_ xs res)
    (if (null? xs)
	res
	(deep-reverse_ (cdr xs)
		       (cons (if (pair? (car xs))
				 (deep-reverse (car xs))
				 (car xs))
			     res))))
  (deep-reverse_ items '()))

; gosh> (deep-reverse a)
; ((4 3) (2 1))
