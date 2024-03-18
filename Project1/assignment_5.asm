        .ORIG x3000
        LD      R6, US_S    ; initialize pointer
start   JSR     readS
        JSR     isPrime
        JSR     resultS
        BRnzp   start
;
; --------------------------- Assignment 2 -------------------------------- ;
;
; reads two seperate digits from the input, and makes it into a decimal number
;
; input: two digits read from input I/O device
; output: R0 - the decimal number
readS   ADD     R6, R6, #-1 ; store used registers onto the stack
        STR     R1, R6, #0
        ADD     R6, R6, #-1
        STR     R2, R6, #0
        ADD     R6, R6, #-1
        STR     R3, R6, #0
        ADD     R6, R6, #-1
        STR     R4, R6, #0
        ADD     R6, R6, #-1
        STR     R5, R6, #0
;
        AND     R1, R1, #0
        AND     R2, R2, #0
        AND     R3, R3, #0
        AND     R4, R4, #0
        AND     R5, R5, #0
;
; prints console initial output
        LEA     R0, PROMPT
        TRAP    x22
        LD      R0, NEWLINE
        TRAP    x21
        AND     R4, R4, #0
        ADD     R4, R4, #2      ; COUNTER
;
READ    AND     R0, R0, #0
        TRAP    x23             ; read
        ADD     R4, R4, #-1
        BRnz    LoadR2
        ADD     R1, R0, #0
        BRnzp   READ
LoadR2  ADD     R2, R0, #0
; print the numbers
        AND     R0, R0, #0
        ADD     R0, R1, #0
        TRAP    x21             ; print
        AND     R0, R0, #0
        ADD     R0, R2, #0
        TRAP    x21             ; print
        LD      R0, NEWLINE     ; newline after both digits are printed
        TRAP    x21
; change the numbers to decimals
        LD      R5, ASCII
        NOT     R5, R5
        ADD     R5, R5, #1
        ADD     R1, R1, R5
        ADD     R2, R2, R5
;
        AND     R0, R0, #0
        ADD     R0, R0, #10
        ADD     R6, R6, #-1 ; store R7 (ret pointer)
        STR     R7, R6, #0  ; onto the stack
        JSR     MUL
        LDR     R7, R6, #0  ; restore R7 (ret pointer)
        ADD     R6, R6, #1  ; from the stack
;
END     ADD     R0, R0, R2
        LDR     R5, R6, #0  ; restore registers from the stack
        ADD     R6, R6, #1
        LDR     R4, R6, #0
        ADD     R6, R6, #1
        LDR     R3, R6, #0
        ADD     R6, R6, #1
        LDR     R2, R6, #0
        ADD     R6, R6, #1
        LDR     R1, R6, #0
        ADD     R6, R6, #1
        RET
;
; ------------------------------ Assignment 4 ------------------------------- ;
;
; displays on the console, whether the given argument is prime or not
;
; input: R0 - the number to check
resultS ADD     R0, R0, #0
        BRz     resSN
        LEA     R0, resP
        BRnzp   resSP
resSN   LEA     R0, resNP
resSP   TRAP    x22
        LD      R0, NEWLINE
        TRAP    x21
        RET
;
; --------------------------- Assignment 3 -------------------------------- ;
;
; function to check if a given number is a prime number
;
; input: R0 - the number to check
; output: R0 - 0 if not prime, 1 if prime
;
isPrime ;save registers
        ADD     R6, R6, #-1 ; store R1 (modulo argument)
        STR     R1, R6, #0  ; onto the stack
        ADD     R6, R6, #-1 ; store R2 (storage)
        STR     R2, R6, #0  ; onto the stack
        ADD     R6, R6, #-1 ; store R3 (storage)
        STR     R3, R6, #0  ; onto the stack
;
; check if R0 is 2 (prime) - or is less than 2 (not prime)
        ADD     R0, R0, #-2 ; check if R0
        BRz     YES         ; is equal to 2
        ADD     R0, R0, #0
        BRn     NO
        ADD     R0, R0, #2  ; restore R0
;
; check mod-2 of R0
        AND     R2, R2, #0  ; store R0 into R2 (modulo will change it)
        ADD     R2, R0, #0
        AND     R1, R1, #0  ; set second argument to 2
        ADD     R1, R1, #2
        ADD     R6, R6, #-1 ; store R7 (ret pointer)
        STR     R7, R6, #0  ; onto the stack
        JSR     MOD
        LDR     R7, R6, #0  ; restore R7 (ret pointer)
        ADD     R6, R6, #1  ; from the stack
        ADD     R0, R0, #0
        BRz     NO          ; if we can divide by 2, and we already checked that it's not 2, we return no
        AND     R0, R0, #0  ; restore R0 from R2
        ADD     R0, R2, #0
;
; check if prime number
        AND     R1, R1, #0  ; initialize R1 to 3
        ADD     R1, R1, #3
; sqrt check
checkP  AND     R2, R2, #0  ; temp storage for R0
        ADD     R2, R0, #0
        AND     R3, R3, #0  ; temp storage for R1
        ADD     R3, R1, #0
        AND     R0, R0, #0
        ADD     R0, R1, #0
        ADD     R6, R6, #-1 ; store R7 (ret pointer)
        STR     R7, R6, #0  ; onto the stack
        JSR     MUL         ; check if sqrt(R1) < R0 by multiplying it by itself, and adding it to R0 (temp in R2), and checking if negative
        LDR     R7, R6, #0  ; restore R7 (ret pointer)
        ADD     R6, R6, #1  ; from the stack
        NOT     R0, R0
        ADD     R0, R0, #1
        ADD     R0, R2, R0
        BRn     YES
        AND     R0, R0, #0  ; restore R0 from R2
        ADD     R0, R2, #0
        AND     R1, R1, #0  ; restore R1 from R3
        ADD     R1, R3, #0
; mod check
        ADD     R6, R6, #-1 ; store R7 (ret pointer)
        STR     R7, R6, #0  ; onto the stack
        JSR     MOD
        LDR     R7, R6, #0  ; restore R7 (ret pointer)
        ADD     R6, R6, #1  ; from the stack
        ADD     R0, R0, #0
        BRz     NO
        AND     R0, R0, #0  ; restore R0 from R2 (mod changed it)
        ADD     R0, R2, #0
        AND     R1, R1, #0  ; restore R1 from R3 (mod could change it)
        ADD     R1, R3, #0
        ADD     R1, R1, #2 ; add 2
        BRnzp   checkP
;
;
NO      AND     R0, R0, #0
        BRnzp   RESTORE
;
YES     AND     R0, R0, #0
        ADD     R0, R0, #1
;
RESTORE LDR     R3, R6, #0  ; restore R3
        ADD     R6, R6, #1  ; from the stack
        LDR     R2, R6, #0  ; restore R2
        ADD     R6, R6, #1  ; from the stack
        LDR     R1, R6, #0  ; restore R1
        ADD     R6, R6, #1  ; from the stack
        RET
;
;
; multiply function
; R0 = first number
; R1 = second number
MUL     ; if R1 is 0, return R1
        ADD     R1, R1, #0
        BRz     endMZ
; save registers
        ADD     R6, R6, #-1 ; store R2
        STR     R2, R6, #0  ; onto the stack
;
        AND     R2, R2, #0  ; load R0 into R2
        ADD     R2, R0, #0
        AND     R0, R0, #0
;
repMUL  ADD     R0, R0, R2
        ADD     R1, R1, #-1
        BRp     repMUL
;
; restore registers
        LDR     R2, R6, #0  ; restore R2
        ADD     R6, R6, #1  ; from the stack
        RET
;
endMZ   AND     R0, R0, #0
        ADD     R0, R1, #0
        RET
;
;
; takes modulo of two arguments, R0 and R1
; returns 0 if mod-R1 = 0, else returns
; a negative number
; you COULD add R1 back to R0 to get the rest
; aka. a correct modulo
MOD     NOT     R1, R1
        ADD     R1, R1, #1
repM    ADD     R0, R0, R1
        BRp     repM
        RET
;
; -------------------------- variables for precompile ---------- ;
;
NEWLINE .FILL x000A
ASCII   .FILL x0030
PROMPT  .STRINGZ "Input a 2 digit decimal number:"
resP    .STRINGZ "The number is prime"
resNP   .STRINGZ "The number is not prime"
US_S    .FILL   xFE00       ; the stack pointer
        .END
