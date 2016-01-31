	AREA kalmanfiltertest_asm, CODE, READONLY
	EXPORT kalmanfiltertest
	;IMPORT __kalmanFilterState
	IMPORT kalmanfilter
	IMPORT datai
	IMPORT datao
	
kalmanfiltertest
	
	LDR R0, =datai
	LDR R1, =datao
	
	VLDR.f32 S0, =1.0
	VSTR.f32 S0, [R0]
	
	VLDR.f32 S0, =2.0
	VSTR.f32 S0, [R0, #4]
	
	VLDR.f32 S0, =3.0
	VSTR.f32 S0, [R0, #8]
	
	VLDR.f32 S0, =4.0
	VSTR.f32 S0, [R0, #12]
	
	MOV R2, #4
	;R3, =__kalmanFilterState
	
	LDR R4, =kalmanfilter
	BLX R4

	END