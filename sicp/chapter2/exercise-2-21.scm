; Exercise 2.21
(define (square x y) (* x y))
 
(define (square-list-a items)
  (if (null? items)
      '()
      (cons (* (car items) (car items))
            (square-list (cdr items)))))
 
(define (square-list-b items)
  (map (lambda (x) (* x x)) items))
