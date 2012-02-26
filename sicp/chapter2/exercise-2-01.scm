; exercise 2.1
(define (Make-rat n d)
  (define (norm num)
    (if (> num 0) num (- 0 num)))
  (let ((numer (norm n))
        (denom (norm d)))
    (let ((g (gcd numer denom)))
      (if (> (* n d) 0)
          (cons (/ numer g)
                (/ denom g))
          (cons (- 0 (/ numer g))
                (/ denom g))))))

;; gosh> (print-rat (Make-rat -1 -2))
;; 1/2
;; #<undef>
;; gosh> (print-rat (Make-rat -1 -2))
;; 1/2
;; #<undef>
;; gosh> (print-rat (Make-rat -1 2))
;; -1/2
;; #<undef>
;; gosh> (print-rat (Make-rat 1 -2))
;; -1/2
;; #<undef>
;; gosh> (print-rat (Make-rat 1 2))
;; 1/2
