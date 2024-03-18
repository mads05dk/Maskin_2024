		.ORIG x3000     ;Initiation
		LD R0, A	    ;Load Variable A (Needs to be an even number and lower than Variable B)
		LD R1, B	    ;Load Variable B (Needs to be an even number and higher than Variable B)
X		NOT R2, R0	    ;(a)Create a variable stored in R2. Loaded in as the inverse of R0
		ADD R2, R2, #1	;(b) Add 1 to R2 to compensate for R2 being to low.
		ADD R2, R2, R1	;Add R1 to R2, if the result is 0, then the midpoint has been found.
		BRz DONE		;Jump to "DONE" if result above is 0
		ADD R1, R1, #-1	;Loop part, Add -1 to R1 to decrement until midpoint has been found
		ADD R0, R0, #1	;Loop part, Add 1 to R1 to increment until midpoint has been found
		BRnzp X			;Jump to X to redo the loop is done
DONE	ST R1, C		;Store the midpoint in C
		TRAP x25		;Halt the program
		A .BLKW 1
		B .BLKW 1
		C .BLKW 1
		.END
