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

; Initialize registers
        AND     R1, R1, #0      ;   Clear R1 through R5
        AND     R2, R2, #0      ;
        AND     R3, R3, #0      ;
        AND     R4, R4, #0      ;
        AND     R5, R5, #0      ;
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
        RET                     ;
;
;
; multiply function
; R0 = first number
; R1 = second number
MUL     ; if R1 is 0, return R1
        ADD     R1, R1, #0      ;
        BRz     endMZ           ;
; save registers
        ADD     R6, R6, #-1     ;   store R2
        STR     R2, R6, #0      ;   onto the stack
        AND     R2, R2, #0      ;   load R0 into R2
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
        LDR     R2, R6, #0      ;   restore R2
        ADD     R6, R6, #1      ;   from the stack
        RET                     ;
;
;
endMZ   AND     R0, R0, #0      ;
        ADD     R0, R1, #0      ;
        RET                     ;
;
;
; -------------------------- precompile variables -------------------------- ;
;
;
NEWLINE .FILL x000A
ASCII   .FILL x0030
PROMPT  .STRINGZ "Hello. Please input a 1-5 digit decimal number"