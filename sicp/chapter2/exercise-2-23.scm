(define (map proc items)
  (if (null? items)
      '()
      (cons (proc (car items))
            (map proc (cdr items)))))

(define (for-each func items)
  (map func items))

(for-each (lambda (x) (newline) (display x))
          (list 57 321 88))
