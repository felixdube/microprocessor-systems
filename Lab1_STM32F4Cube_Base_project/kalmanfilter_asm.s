	AREA kalmanfilter_asm, CODE, READONLY
	EXPORT kalmanfilter
kalmanfilter
		;R0 pointer to input array
		;R1 pointer to filtered data
		;R2 array's length
		;R3 pointer to kalmen filter state
		
		MOV R4, #0		;data addr offset
		MOV R5, #1		;array offset
		
		;load data
		ADD R0, R0, R4
		VLDR.f32 S1, [R0] 			;measurement
		VLDR.f32 S2, [R3, #32]			;noise covariance q
		VLDR.f32 S3, [R3, #64]			;estimated value x
		VLDR.f32 S4, [R3, #96]			;estimation error covariance p
		VLDR.f32 S5, [R3, #128]			;adaptive kalman filter k
		

		;P = P + Q
		VADD.f32 S4, S4, S2
		
		;k = p/ (p + q)
		VADD.f32 S6, S4, S2
		VDIV.f32 S5, S4, S6
		
		;x = x + k * (measurement - x)
		VSUB.f32 S6, S1, S3
		VMUL.f32 S6, S6, S5
		VADD.f32 S3, S3, S6
		
		;p = (1 - k) * p
		VLDR.f32 S7, =1.0
		VSUB.f32 S6, S7, S5
		VMUL.f32 S4, S4, S6
		
		;store the output
		ADD R1, R1, R4
		VSTR.f32 S3, [R1]
		ADD R4, R4, #32
		
		;update counter
		ADD R4, R4, #32
		ADD R5, R5, #1
		
		;check if all the data have been filtered
		CMP R2, R4
		BNE kalmanfilter

		END