(define (add-complex-to-schemenum z x)
  (max-from-real-imag (+ (real-part z) x)
                      (image-part z)))

(put 'add (complex scheme-number)
     (lambda (z x) (tag (add-complex-to-schemenum z x))))

; 2.81