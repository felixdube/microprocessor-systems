;/*************************************************************************************/
;// File: kalmanfilter_asm.s
;// Description: Implement a 1D kalman filter
;// Date: February, 2016
;// Author: August Lalande, Juan Morency Trudel, Felix Dube
;/*************************************************************************************/
	
	AREA text, CODE, READONLY
	EXPORT kalmanFilter_asm
kalmanFilter_asm
		;R0 pointer to input array
		;R1 pointer to filtered data
		;R2 array's length
		;R3 pointer to kalmen filter state
		
		;load filter state data
		VLDM R3, {S1-S5}		
		
loop
		VLDR.f32 S0, [R0] 				;measurement/ initial value
		
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
		;p = (1 - k) * p = p-pk
		VMUL.f32 S6, S4, S5		
		VSUB.f32 S4, S4, S6	
		;store the output
		VSTR.f32 S3, [R1]
		ADD R1, R1, #4
		ADD R0, R0, #4			
		;update counter
		SUBS R2, #1
		;Branch if R2 != 0	
		BNE loop
		;verify overflow
		VMRS R0, FPSCR ; Copy flags from FPSCR to flags in APSR
		;This is the return value of the function : 
		; 0 or 10 is a good result
		; 15 or 11 means there has been an error, don't trust the output array
		
		;;check for most common  0x00000010
		;SUB R1, #0x00000010
		;CBZ R1, exit   ;if no error this will be zero
		;;check for second most common 0x00000000
		;ADD R1, #0x00000010
		;CBZ R1, exit   ;if no error this will be zero
		;;if not 0x00000010 or 0x00000000 error code
		;MOV R0, #1
exit	BX LR
		END