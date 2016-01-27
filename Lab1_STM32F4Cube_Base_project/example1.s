	AREA text, CODE, READONLY
	EXPORT example1
example1
	VLDR.f32 S1, =2.5
	VLDR.f32 S2, =7.5
	VADD.f32 S1, S2, S1
	BX LR;
	END
		