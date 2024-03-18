        .ORIG x3000
        LD      R0,A        ;Loads A into R0
        LD      R1,B        ;Loads B into R1
X       Not     R2,R0       ;Makes RO negative into R2
        ADD     R2,R2,#1    ;Add 1 into R0
        ADD     R2,R2,R1    ;Adds R2 + R1 into R2
        BRz     DONE        ;If previous result is 0, done
        ADD     R1,R1,#-1   ;R1-1 into R1
        ADD     R0,R0,#1    ;Increment R0
        BRnzp   X           ;loops infinitely to x
DONE    ST      R1,C        ;Stores R1 in C
        TRAP    x25         ;Halts execution
A       .BLKW   1           ;Input 1 character
B       .BLKW   1           ;Input 1 character
C       .BLKW   1           ;Input 1 character
        .END