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
	VLDR.f32 S0, =-3.4028235E38
	VLDR.f32 S1, =3.4028235E38
	VLDR.f32 S2, =-3.4028235E38
	VLDR.f32 S3, =3.4028235E38
	VLDR.f32 S4, =-3.4028235E38
	VSTM.f32 R0, {S0-S4}

	
	;number of element in the input array
	MOV 	 R2, #5

	;VLDR.f32 S0, =0.0 ;k
	;vpush S0
	;VLDR.f32 S0, =0.1 ;p
	;vpush S0
	;VLDR.f32 S0, =0.0 ;x
	;vpush S0
	;VLDR.f32 S0, =0.1 ;r
	;vpush S0	
	;VLDR.f32 S0, =0.1 ;q
	;vpush S0	
	ALIGN
	VLDR.f32 S0, =0.1 ;q
	VLDR.f32 S1, =0.1 ;r
	VLDR.f32 S2, =0 ;x
	VLDR.f32 S3, =0.1 ;p
	VLDR.f32 S4, =0 ;k
	vpush {S0,S1,S2,S3,S4,S5}

	MOV		 R3, SP
	MOV		 R5, LR
	LDR 	 R4, =kalmanFilter_asm
	BLX 	 R4
	MOV		 LR, R5	
	BX 		 LR
	END