	AREA kalmanfiltertest, CODE, READONLY
	EXPORT kalmanfiltertest
	IMPORT __kalmanFilterState
	IMPORT kalmanfilter
	
kalmanfiltertest
	datai db =1.2, =1.4, =1.3, =1.7
	datao db =0.0, =0.0, =0.0, =0.0
	
	LDR R0, [datai]
	LDR R1, [datao]
	LDR R2, #4
	LDR R3, =__kalmanFilterState
	
	LDR R4, =kalmanfilter
	BX R4
	END