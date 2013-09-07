(letrec
;; what:
(
(unzip (lambda (ps)
    (letrec
      ((unzipt
         (lambda (pairs z1 z2)
           (if (null? pairs)
             (list z1 z2)
             (let ((pair (car pairs))
                   (rest (cdr pairs)))
               (let ((p1 (car pair))
                     (p2 (cadr pair)))
                 (unzipt rest (append z1 (list p1)) (append z2 (list p2)))))))))
      (unzipt ps '() '()))))

(compile-bindings
  (lambda (bs)
    (if (null? bs) '(LDC ())
        (append (compile-bindings (cdr bs))
                (compile (car bs))
                '(CONS)))))

(compile-begin-acc
  (lambda (stmts acc)   ; acc must be '(LDC ()) at the beginning
    (if (null? stmts)
        (append acc '(CAR))
        (compile-begin-acc (cdr stmts)
                           (append acc (compile (car stmts)) '(CONS))))))

(compile-cond
  (lambda (conds)
    (if (null? conds)
        '(LDC ())
        (let ((this-cond (car (car conds)))
              (this-expr (cadr (car conds))))
          (if (eq? this-cond 'else)
              (compile this-expr)
              (append (compile this-cond) '(SEL)
                      (list (append (compile this-expr) '(JOIN)))
                      (list (append (compile-cond (cdr conds)) '(JOIN)))))))))

(compile-form (lambda (f)
  (let ((hd (car f))
        (tl (cdr f)))
    (cond
      ((eq? hd 'quote)
        (list 'LDC (car tl)))
      ((eq? hd '+)
        (append (compile (cadr tl)) (compile (car tl)) '(ADD)))
      ((eq? hd '-)
        (append (compile (cadr tl)) (compile (car tl)) '(SUB)))
      ((eq? hd '*)
        (append (compile (cadr tl)) (compile (car tl)) '(MUL)))
      ((eq? hd '/)
        (append (compile (cadr tl)) (compile (car tl)) '(DIV)))
      ((eq? hd 'remainder)
        (append (compile (cadr tl)) (compile (car tl)) '(REM)))
      ((eq? hd '<=)
        (append (compile (cadr tl)) (compile (car tl)) '(LEQ)))
      ((eq? hd 'eq? )
        (append (compile (cadr tl)) (compile (car tl)) '(EQ)))
      ((eq? hd 'cons)
        (append (compile (cadr tl)) (compile (car tl)) '(CONS)))
      ((eq? hd 'atom?)
        (append (compile (car tl)) '(ATOM)))
      ((eq? hd 'car)
        (append (compile (car tl)) '(CAR)))
      ((eq? hd 'cdr)
        (append (compile (car tl)) '(CDR)))
      ((eq? hd 'cadr)
        (append (compile (car tl)) '(CDR CAR)))
      ((eq? hd 'caddr)
        (append (compile (car tl)) '(CDR CDR CAR)))
      ((eq? hd 'if )
        (let ((condc (compile (car tl)))
              (thenb (append (compile (cadr tl)) '(JOIN)))
              (elseb (append (compile (caddr tl)) '(JOIN))))
          (append condc '(SEL) (list thenb) (list elseb))))
      ((eq? hd 'lambda)
        (let ((args (car tl))
              (body (append (compile (cadr tl)) '(RTN))))
          (list 'LDF (list args body))))
      ((eq? hd 'let)
        (let ((bindings (unzip (car tl)))
              (body (cadr tl)))
          (let ((args (car bindings))
                (exprs (cadr bindings)))
            (append (compile-bindings exprs)
                    (list 'LDF (list args (append (compile body) '(RTN))))
                    '(AP)))))
      ((eq? hd 'letrec)
        (let ((bindings (unzip (car tl)))
              (body (cadr tl)))
          (let ((args (car bindings))
                (exprs (cadr bindings)))
              (append '(DUM)
                      (compile-bindings exprs)
                      (list 'LDF (list args (append (compile body) '(RTN))))
                      '(RAP)))))

      ;; (begin (e1) (e2) ... (eN)) => LDC () <e1> CONS <e2> CONS ... <eN> CONS CAR
      ((eq? hd 'begin)
        (compile-begin-acc tl '(LDC ())))
      ((eq? hd 'cond)
        (compile-cond tl))
      ((eq? hd 'display)
        (append (compile (car tl)) '(PRINT)))
      ((eq? hd 'read)
        '(READ))
      ((eq? hd 'eval)
        (append '(LDC () LDC () ) (compile (car tl)) '(CONS LD make-closure AP AP)))
      ((eq? hd 'quit)
        '(STOP))
      (else
        (let ((compiled-head 
                (if (symbol? hd) (list 'LD hd) (compile hd))))
         (append (compile-bindings tl) compiled-head '(AP))))
    ))))

(compile (lambda (s)
  (cond
    ((symbol? s) (list 'LD s))
    ((number? s) (list 'LDC s))
    (else (compile-form s)))))

(repl (lambda () 
    (let ((inp (read)))
      (if (eof-object? inp) (quit)
        (begin
          ;(display inp)
          (display (append (compile inp) '(STOP)))
          (repl))))))
)

;; <let> in
(repl))
