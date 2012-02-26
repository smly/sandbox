; Exercise 2.18
(define (reverse items)
  (define (reverse_ xs res)
    (if (null? xs)
	res
	(reverse_ (cdr xs) (cons (car xs) res))))
  (reverse_ items '()))

; gosh> (reverse (list 1 4 9 16 25))
; (25 16 9 4 1)
