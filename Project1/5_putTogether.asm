        .ORIG x3000
        LD      R6, US_S    ; initialize pointer
start   JSR     readS
        JSR     isPrime
        JSR     resultS
        BRnzp   start
;
;
; --------------------------- Assignment 2 -------------------------------- ;
;
;
;   function that reads two digits from input in the console into a decimal number
;
;   input: two digits read from input I/O device
;   output: R0 - the decimal number
;
readS   ; Save registers R1-R5 on the stack
        ADD     R6, R6, #-5     ;   Decrement stack pointer to make space for 5 registers
        STR     R1, R6, #4      ;   Store R1 at R6 + offset 4
        STR     R2, R6, #3      ;   Store R2 at R6 + offset 3
        STR     R3, R6, #2      ;   Store R3 at R6 + offset 2
        STR     R4, R6, #1      ;   Store R4 at R6 + offset 1
        STR     R5, R6, #0      ;   Store R5 at R6 + offset 0
;
;       
        ; Initialize registers
        AND     R1, R1, #0      ;   Clear R1 through R5
        AND     R2, R2, #0      ;
        AND     R3, R3, #0      ;
        AND     R4, R4, #0      ;
        AND     R5, R5, #0      ;
;
;
        ; prints console initial output
        LEA     R0, PROMPT      ;   Load prompt string into R0
        TRAP    x22             ;   Print to console
        LD      R0, NEWLINE     ;   Load a newline into R0
        TRAP    x21             ;
        AND     R4, R4, #0      ;
        ADD     R4, R4, #2      ;   COUNTER
;
;
READ    AND     R0, R0, #0      ;
        TRAP    x23             ;   read input
        ADD     R4, R4, #-1     ;   Add -1 to R4
        BRnz    LoadR2          ;   
        ADD     R1, R0, #0      ;
        BRnzp   READ            ;
LoadR2  ADD     R2, R0, #0      ;
; 
;
        ;print the numbers
        AND     R0, R0, #0      ;
        ADD     R0, R1, #0      ;
        TRAP    x21             ;   print
        AND     R0, R0, #0      ;
        ADD     R0, R2, #0      ;
        TRAP    x21             ;   print
        LD      R0, NEWLINE     ;   newline after both digits are printed
        TRAP    x21             ;
;
;
        ; change the numbers to decimals
        LD      R5, ASCII       ;
        NOT     R5, R5          ;
        ADD     R5, R5, #1      ;
        ADD     R1, R1, R5      ;
        ADD     R2, R2, R5      ;
;
;
        AND     R0, R0, #0      ;
        ADD     R0, R0, #10     ;
        ADD     R6, R6, #-1     ;   store R7 (ret pointer)
        STR     R7, R6, #0      ;   onto the stack
        JSR     MUL             ;
        LDR     R7, R6, #0      ;   restore R7 (ret pointer)
        ADD     R6, R6, #1      ;   from the stack
;
;
END     ADD     R0, R0, R2      ;   restore registers from the stack
        LDR     R5, R6, #0      ;   
        ADD     R6, R6, #1      ;
        LDR     R4, R6, #0      ;
        ADD     R6, R6, #1      ;
        LDR     R3, R6, #0      ;
        ADD     R6, R6, #1      ;
        LDR     R2, R6, #0      ;
        ADD     R6, R6, #1      ;
        LDR     R1, R6, #0      ;
        ADD     R6, R6, #1      ;
        RET
;
;
; ------------------------------ Assignment 4 ------------------------------- ;
;
;
;   Function to take in 0 or 1 and output whether it is prime
;
;   input: R0 to check it
;
;
resultS ADD     R0, R0, #0   ; Create a variable stored in R0
        BRz     resSN        ; If input is 0 then go to resSN
        LEA     R0, resP     ; if input was 1 then set R0 to resP
        BRnzp   resSP        ; jump to trap22
resSN   LEA     R0, resNP    ; if R0 was 0 set it to resNP
resSP   TRAP    x22          ; write out string of ASCII
        LD      R0, NEWLINE  ; Load R0
        TRAP    x21          ; Write out the R0 result to console
        RET                  ; Return from subroutine
;
;
NEWLINE .FILL x000A
resP    .STRINGZ "The number is prime"
resNP   .STRINGZ "The number is not prime"
;
;
; --------------------------- Assignment 3 -------------------------------- ;
;
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
; -------------------------- variables for precompile ---------- ;
;
;
NEWLINE .FILL x000A
ASCII   .FILL x0030
PROMPT  .STRINGZ "Input a 2 digit decimal number:"
resP    .STRINGZ "The number is prime"
resNP   .STRINGZ "The number is not prime"
US_S    .FILL   xFE00       ; the stack pointer
        .END
