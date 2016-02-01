	; should verify if all the calculations are like the pseudo-code
	
	AREA text, CODE, READONLY
	EXPORT kalmanFilter_asm
kalmanFilter_asm
		;R0 pointer to input array
		;R1 pointer to filtered data
		;R2 array's length
		;R3 pointer to kalmen filter state
		;POP {R3}
		;LDR R7, =instance1
		
		;initialize R6 at 0
		MOV R6, #0		;counter of input array element's filtered
		;adjustment to the stack pointer
		;SUB SP, SP, #18
loop		
		;load filter state data
		VLDR.f32 S0, [R0] 				;measurement/ initial value
		VLDR.f32 S1, [R3]				;noise covariance q
		VLDR.f32 S2, [R3, #4]			;noise covariance r
		VLDR.f32 S3, [R3, #8]			;estimated value x
		VLDR.f32 S4, [R3, #12]			;estimation error covariance p
		VLDR.f32 S5, [R3, #16]			;adaptive kalman filter k
		; S6 scratch register
		

		;P = P + Q
		VADD.f32 S4, S4, S1
		
		; maybe can optimise
		;also check if we have to use updated p or initial p
		;k = p/ (p + r)
		VADD.f32 S6, S4, S2
		VDIV.f32 S5, S4, S6
		
		;x = x + k * (measurement - x)
		VSUB.f32 S6, S0, S3
		VMUL.f32 S6, S6, S5
		VADD.f32 S3, S3, S6 ;simplyfied these two with one command
		;VLMA.f32 S3, S5, S6 ; multiply and accumulate into x
		
		;p = (1 - k) * p = p-pk
		VLDR.f32 S7, =1.0
		VSUB.f32 S6, S7, S5
		VMUL.f32 S4, S4, S6
		;VLMS.f32 S4, S4, S5
			
		;store the output
		VSTR.f32 S3, [R1]
		ADD R1, R1, #4
		ADD R0, R0, #4
		
		;update counter
		ADD R6, R6, #1
		
		;Store the new state variables
		VSTR.f32 S1, [R3]				;noise covariance q
		VSTR.f32 S2, [R3, #4]			;noise covariance r
		VSTR.f32 S3, [R3, #8]			;estimated value x
		VSTR.f32 S4, [R3, #12]			;estimation error covariance p
		VSTR.f32 S5, [R3, #16]			;adaptive kalman filter k
		
		;check if all the data has been filtered
		CMP R2, R6
		BNE loop
		

		
		BX LR
		END