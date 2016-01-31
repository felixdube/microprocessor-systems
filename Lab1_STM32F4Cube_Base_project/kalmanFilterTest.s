	AREA kalmanfiltertest_asm, CODE, READONLY
	EXPORT kalmanfiltertest
	;IMPORT __kalmanFilterState
	IMPORT kalmanfilter
	
kalmanfiltertest
	
	LDR R0, =datai
	LDR R1, =datao
	MOV R2, #4
	;R3, =__kalmanFilterState
	
	LDR R4, =kalmanfilter
	BLX R4
	
	ALIGN
datai DCB 1, 2
datao DCB 2, 3
	END