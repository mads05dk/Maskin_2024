;
;   Function to take in 0 or 1 and output whether it is prime
;
;   input: R0 - the number to check
;
;
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
;
NEWLINE .FILL x000A
resP    .STRINGZ "The number is prime"
resNP   .STRINGZ "The number is not prime"