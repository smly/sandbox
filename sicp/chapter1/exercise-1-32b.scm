(define (accumulate-it combiner null-value term a next b)
  (if (> a b)
      null-value
      (combiner (term a)
		(accumulate-it combiner null-value term
			       (next a) next b))))
