;
;       function to check if a given number is a prime number
;
;       input: R0 - the number to check
;       output: R0 - 0 if not prime, 1 if prime
;
isPrime ; Stack preparation for saving registers
        ADD     R6, R6, #-1     ;   Create R1 (modulo argument)
        STR     R1, R6, #0      ;   Store in the stack
        ADD     R6, R6, #-1     ;   Create R2 (storage)
        STR     R2, R6, #0      ;   Store in the stack
        ADD     R6, R6, #-1     ;   Create R3 (storage)
        STR     R3, R6, #0      ;   Store in the stack
;
;
        ; Special checks for numbers less than 2 and equal to 2
        ADD     R0, R0, #-2     ;   Adjust R0 to check for value 2
        BRz     YES             ;   If R0 was 2, it's prime
        BRn     NO              ;   If less than 2, it's not prime
        ADD     R0, R0, #2      ;   Restore original R0 value
;
;
        ; Divisibility by 2 check excluding the number 2 itself
        AND     R2, R2, #0      ;   Reset R2 for use
        ADD     R2, R0, #0      ;
        AND     R1, R1, #0      ;   Reset R1 for divisor (2)
        ADD     R1, R1, #2      ;   Set divisor to 2
        ADD     R6, R6, #-1     ;   Save return address for MOD subroutine
        STR     R7, R6, #0      ;   Save R7 on the stack
        JSR     MOD             ;   Call MOD subroutine
        LDR     R7, R6, #0      ;   Restore return address
        ADD     R6, R6, #1      ;   Adjust stack pointer back
        ADD     R0, R0, #0      ;   Adjust R0 position in the stack
        BRz     NO              ;   If divisible by 2, not prime
        AND     R0, R0, #0      ;   Restore R0 from R2
        ADD     R0, R2, #0      ;
;
;
        ; Setup for checking divisibility by numbers > 2
        AND     R1, R1, #0      ;   Reset R1
        ADD     R1, R1, #3      ;   Start with divisor 3
;
;
        ; sqrt check
checkP  AND     R2, R2, #0      ;   Temp storage for R0
        ADD     R2, R0, #0      ;
        AND     R3, R3, #0      ;   Temp storage for R1
        ADD     R3, R1, #0      ;
        AND     R0, R0, #0      ;
        ADD     R0, R1, #0      ;
        ADD     R6, R6, #-1     ;   Store R7 (ret pointer)
        STR     R7, R6, #0      ;   Onto the stack
        JSR     MUL             ;   Check if sqrt(R1) < R0 by multiplying it by itself, and adding it to R0 (temp in R2), and checking if negative
        LDR     R7, R6, #0      ;   Restore R7 (ret pointer)
        ADD     R6, R6, #1      ;   From the stack
        NOT     R0, R0          ;
        ADD     R0, R0, #1      ;
        ADD     R0, R2, R0      ;
        BRn     YES             ;
        AND     R0, R0, #0      ;   Restore R0 from R2
        ADD     R0, R2, #0      ;
        AND     R1, R1, #0      ;   Restore R1 from R3
        ADD     R1, R3, #0      ;
;
;
        ; mod check
        ADD     R6, R6, #-1     ;   Store R7 (ret pointer)
        STR     R7, R6, #0      ;   Onto the stack
        JSR     MOD             ;
        LDR     R7, R6, #0      ;   Restore R7 (ret pointer)
        ADD     R6, R6, #1      ;   Rrom the stack
        ADD     R0, R0, #0      ;
        BRz     NO              ;
        AND     R0, R0, #0      ;   Restore R0 from R2 (mod changed it)
        ADD     R0, R2, #0      ;
        AND     R1, R1, #0      ;   Restore R1 from R3 (mod could change it)
        ADD     R1, R3, #0      ;
        ADD     R1, R1, #2      ;   Add 2
        BRnzp   checkP          ;
;
;
NO      AND     R0, R0, #0      ;   Set R0 to 0, number is not prime
        BRnzp   RESTORE         ;   Go to register restoration
;
;
YES     AND     R0, R0, #0      ;   Clear R0 for setting result
        ADD     R0, R0, #1      ;   Set R0 to 1, number is prime
;
;
        ; Restore registers from the stack before returning
RESTORE LDR     R3, R6, #0      ;   Restore R3
        ADD     R6, R6, #1      ;   From the stack
        LDR     R2, R6, #0      ;   Restore R2
        ADD     R6, R6, #1      ;   From the stack
        LDR     R1, R6, #0      ;   Restore R1
        ADD     R6, R6, #1      ;   From the stack
        RET                     ;   Return from subroutine
                                ;
                                ;
                                ;   Multiply function
                                ;   R0 = first number
                                ;   R1 = second number
MUL                             ;   Check if R1 is 0. If it is, return R1
        ADD     R1, R1, #0      ;
        BRz     endMZ           ;
;
;        
        ; save registers
        ADD     R6, R6, #-1     ;   Store R2
        STR     R2, R6, #0      ;
        AND     R2, R2, #0      ;   Load R0 into R2
        ADD     R2, R0, #0      ;
        AND     R0, R0, #0      ;
;
;
repMUL  ADD     R0, R0, R2      ;
        ADD     R1, R1, #-1     ;
        BRp     repMUL          ;
;
;
        ; restore registers
        LDR     R2, R6, #0      ;   Restore R2
        ADD     R6, R6, #1      ;
        RET                     ;
;
;
endMZ   AND     R0, R0, #0      ;
        ADD     R0, R1, #0      ;
        RET                     ;
;
;
                                ; takes modulo of two arguments, R0 and R1
                                ; returns 0 if mod-R1 = 0, else returns
                                ; a negative number
                                ; you COULD add R1 back to R0 to get the rest
                                ; aka. a correct modulo
;
;
MOD     NOT     R1, R1          ;
        ADD     R1, R1, #1      ;
repM    ADD     R0, R0, R1      ;
        BRp     repM            ;
        RET                     ;
;
;
US_S    .FILL   xFE00           ;   The stack pointer