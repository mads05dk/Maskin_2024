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