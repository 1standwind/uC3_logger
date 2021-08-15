#include "iodefine.h"

void segInit(void){
	//Initialize PORTA.DDR.BYTE
	unsigned char valc;

	valc =PORTA.DDR.BYTE;
	valc |= 0xc7;
	PORTA.DDR.BYTE = valc;

	valc = PORTA.DR.BYTE;
	valc &= ~0x47;
	valc |= 0x47;
	PORTA.DR.BYTE = valc;
}

void segPut(int x){
	//Substitute the value of x & 0x07 for PORTA.DR.BYTE
	int outDat;
	outDat = x & 0x07;
	//Correct output of 7 or more
	if (x & 0x08) outDat |= 0x40;
	PORTA.DR.BYTE = outDat;
}