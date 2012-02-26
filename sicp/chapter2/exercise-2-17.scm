; Exercise 2.17
(define (last-pair items)
  (if (null? (cdr items))
      items
      (last-pair (cdr items))))

; gosh> (last-pair (list 23 72 149 34))
; (34)
