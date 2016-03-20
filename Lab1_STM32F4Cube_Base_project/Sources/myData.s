	AREA myData, DATA, READWRITE
	export datai
	export datao	
	ALIGN
datai SPACE 32; specifies the number of bytes of space required to store input data 8*4
datao SPACE 32; specifies the number of bytes of space required to store output data 8*4
	END