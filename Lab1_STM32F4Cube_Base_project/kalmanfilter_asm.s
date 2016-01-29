	AREA kalmanfilter, CODE, READONLY
	EXPORT kalmanfilter
kalmanfilter
l:
		;R0 pointer to input array
		;R1 pointer to filtered data
		;R2 array's length
		;R3 pointer to kalmen filter state
		
		LDR R10, #0		;data addr offset
		LDR R11, #1		;array offset
		
		;load data
		LDR R4, [R0, R10] 	;measurement
		LDR R5,	[R3, #32]			;noise covariance q
		LDR R6, [R3, #64]			;estimated value x
		LDR R7, [R3, #96]			;estimation error covariance p
		LDR R8, [R3, #128]			;adaptive kalman filter k
		

		;P = P + Q
		VADD.f32 R7, R7, R5
		
		;k = p/ (p + q)
		VADD.f32 R9, R7, R5
		VDIV.f32 R8, R7, R9
		
		;x = x + k * (measurement - x)
		VSUB.f32 R9, R4, R6
		VMUL.f32 R9, R9, R8
		VADD.f32 R6, R6, R9
		
		;p = (1 - k) * p
		VSUB.f32 R9, =1.0, R8
		VMUL.f32 R7, R7, R9
		
		;store the output
		STR RX, [R1, R10]
		ADD R10, R10, #32
		
		;check of the loop should be executed again
		IF R11 < R2
			ADD R10, R10, #1
			B loop
		
		
		BX LR;
		END