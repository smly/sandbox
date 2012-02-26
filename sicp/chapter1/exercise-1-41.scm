; ex 1.41
; double
(define (double p)
  (lambda (x) (p (p x))))

(define (inc x) (+ x 1))

;; gosh> (((double (double double)) inc) 5)
;; 21

;; MEMO: exponential -> expt
;; gosh> (expt 2 3)
;; 8
