; 2.29a
(define (make-mobile left right)
  (list left right))
(define (make-branch length structure)
  (list length structure))
(define (left-branch mobile)
  (car mobile))
(define (right-branch mobile)
  (car (cdr cmobile)))
(define (branch-length branch)
  (car branch))
(define (branch-structure branch)
  (car (cdr branch)))

; 2.29b
(define (total-weight mobile)
  (+ (branch-length (left-branch mobile))
     (branch-length (right-branch mobile))))

