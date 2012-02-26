; 2.28
(define b (list (list 1 2) (list 3 4)))

(define (fringe items)
  (define (fringe_ items res)
    (cond ((null? items) res)
	  ((not (pair? items)) (car items))
	  (else (fringe_ (cdr items)
			 (append res
				 (car items))))))
    (fringe_ items '()))

; gosh> (fringe b)
; (1 2 3 4)
