	AREA text, CODE, READONLY
	EXPORT kalmanFilterTestbench
	IMPORT kalmanFilter_asm
	IMPORT datai
	IMPORT datao
	
kalmanFilterTestbench
	;input and output data array pointers	
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
	VLDR.f32 S0, =-10.0
	VSTR.f32 S0, [R0, #24]	
	VLDR.f32 S0, =10.0
	VSTR.f32 S0, [R0, #28]
	
	;number of element in the input array
	MOV 	 R2, #10

	VLDR.f32 S0, =0.0 ;k
	vpush S0
	VLDR.f32 S0, =0.1 ;p
	vpush S0
	VLDR.f32 S0, =0.0 ;x
	vpush S0
	VLDR.f32 S0, =0.1 ;r
	vpush S0	
	VLDR.f32 S0, =0.1 ;q
	vpush S0	
	MOV		 R3, SP
	
	LDR 	 R4, =kalmanFilter_asm
	BLX 	 R4
	
	BX 		 LR
	END