; exercise 2.6
; \fx.x
(define zero (lambda (f) (lambda (x) x)))

(define (add-1 n)
  (lambda (f) (lambda (x) (f ((n f) x)))))

; \fx.fx
(define c1 (lambda (f) (lambda (x) (f x))))
; \fx.ffx
(define c2 (lambda (f) (lambda (x) (f (f x)))))
; \fx.fffx
(define c3 (lambda (f) (lambda (x) (f (f (f x))))))

(define (plus a b)
  (lambda (f) (lambda (x) ((a f) ((b f) x)))))

(define (cnum c)
  ((c (lambda (x) (+ 1 x))) 0))

;; gosh> (cnum c3)
;; 3
;; gosh> (cnum (add-1 c2))
;; 3
;; gosh> (cnum (plus c1 c2))
;; 3
