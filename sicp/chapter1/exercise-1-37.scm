; ex 1.37a
; recursive process
(define (cont-frac n d k)
  (define (cont-frac-rec i)
    (if (= k i)
        (/ (n i) (d i))
        (/ (n i) (+ (d i) (cont-frac-rec (+ i 1))))))
  (cont-frac-rec 1))

; ex 1.37b
; iterative process
(define (cont-frac-it n d k)
  (define (cont-frac-iter i result)
    (if (= 0 i)
	result
	(cont-frac-iter (- i 1)
			(/ (n i) (+ (d i) result)))))
  (cont-frac-iter k 0))
