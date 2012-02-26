; ex 1.43
(define (repeated f x)
  (if (= x 1)
      f
      (compose f
	       (repeated f (- x 1)))))

;; gosh> ((repeated square 2) 5)
;; 625
