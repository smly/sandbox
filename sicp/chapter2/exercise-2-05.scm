; exercise 2.5

(define (Cons a b)
  (* (expt 2 a)
     (expt 3 b)))

(define (Car z)
  (if (= 0 (remainder z 2))
      (+ 1 (Car (/ z 2)))  
      0))                

(define (Cdr z)
  (if (= 0 (remainder z 3))
      (+ 1 (Cdr (/ z 3)))  
      0))
