	AREA kalmanfilter_asm, CODE, READONLY
	EXPORT kalmanfilter
kalmanfilter
		;R0 pointer to input array
		;R1 pointer to filtered data
		;R2 array's length
		;R3 pointer to kalmen filter state
		;POP {R3}
		;LDR R7, =instance1
		
		;find a way to do these 2 lines only once
		MOV R4, R0		;input data addr with offset
		MOV R5, R1		;filtered data addr with offset
		MOV R6, #0		;counter of input array element's filtered
		SUB SP, SP, #20
loop		
		;load filter state data
		VLDR.f32 S0, [R4] 				;measurement/ initial value
		VLDR.f32 S1, [sp]				;noise covariance q
		VLDR.f32 S2, [sp, #4]			;noise covariance r
		VLDR.f32 S3, [sp, #8]			;estimated value x
		VLDR.f32 S4, [sp, #12]			;estimation error covariance p
		VLDR.f32 S5, [sp, #16]			;adaptive kalman filter k
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
		VLDR.f32 S7, =1.0 ; don't know if you can declare a floating point this way
		VSUB.f32 S6, S7, S5
		VMUL.f32 S4, S4, S6
		;VLMS.f32 S4, S4, S5
		
		

		
		;store the output
		VSTR.f32 S3, [R5]
		ADD R5, R5, #4
		ADD R4, R4, #4
		
		;update counter
		ADD R6, R6, #1
		
		;Output on S0, maybe not necessary
		;VMOV.f32 S0, S3
		
		;check if all the data has been filtered
		CMP R2, R6
		BNE loop
		
		;Store the new state variables
		;TODO
		
		BX LR
		END