; ex 1.39
; tangent
(define (tan-cf x k)
  (define (t-cont-frac n d k)
    (define (t-cont-frac-rec i)
      (if (= k i)
	  (/ (n i) (d i))
	  (/ (n i) (+ (d i) (t-cont-frac-rec (+ i 1))))))
    (t-cont-frac-rec 1))
  (t-cont-frac (lambda (i) (if (= i 1) x (- (* x x))))
		  (lambda (i) (- (* 2 i) 1))
		  k))

;; gosh> (tan-cf 3.0 10)
;; -0.1425465438397583
;; gosh> (tan 3.0)
;; -0.1425465430742778
