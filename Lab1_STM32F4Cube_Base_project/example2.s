	AREA test, CODE, READONLY
	export example2
	import myString1
	import myString2
example2
	LDR R0, =myString3	;load the addr of myString3 in R0
	LDR R1, [R0]		;load the content of the addr in R0 to R1
	LDR R2, =myString1	;load the addr of myString1 to R2
	STR R1, [R2]		;Store the content in R1 to the addr in R2
	BX LR
	
	ALIGN
myString3 DCB 1, 2, 3, 4

	END