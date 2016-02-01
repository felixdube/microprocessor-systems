	AREA kalmanfiltertest_asm, CODE, READONLY
	EXPORT kalmanfiltertest
	IMPORT kalmanfilter
	IMPORT datai
	IMPORT datao
	
kalmanfiltertest
; input and output data array pointers	
	LDR R0, =datai
	LDR R1, =datao
	
;input data
	VLDR.f32 S0, =-10.0
	VSTR.f32 S0, [R0]
	VLDR.f32 S0, =10.0
	VSTR.f32 S0, [R0, #4]
	VLDR.f32 S0, =-10.0
	VSTR.f32 S0, [R0, #8]	
	VLDR.f32 S0, =10.0
	VSTR.f32 S0, [R0, #12]
	VLDR.f32 S0, =-10.0
	VSTR.f32 S0, [R0, #16]
	VLDR.f32 S0, =10.0
	VSTR.f32 S0, [R0, #20]
	VLDR.f32 S0, =-10.0
	VSTR.f32 S0, [R0, #24]	
	VLDR.f32 S0, =10.0
	VSTR.f32 S0, [R0, #28]

;number of element in the input array
	MOV 	 R2, #8
	;for the C code to work, we might need to put the right address in R3 before calling this
	SUB		 SP, #18
	MOV		 R3, SP
	LDR 	 R4, =kalmanfilter
	BLX 	 R4
	
	BX 		 LR
	END