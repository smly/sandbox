; exercise 2.10
(define (ben-check-div-interval a b)
  (if (and (> 0 (lower-bound b))           (< 0 (upper-bound b)))
      (display "divide by an interval that spans zero.")
      (div-interval a b)))
