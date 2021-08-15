#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdbool.h> //bool型を使うため

typedef struct
{
	unsigned char sec : 8;
	unsigned char : 8;
	unsigned char min : 8;
	unsigned char : 8;
	unsigned char hou : 8;
} Time;

typedef struct
{
	unsigned char day : 8;
	unsigned char : 8;
	unsigned char mon : 8;
	unsigned char : 8;
	unsigned short yea : 16;
} Day;

typedef short int Temperature;	//温度の型(25度なら250)
typedef unsigned char FanMode;	//扇風機のモード 1～3
typedef unsigned char FanPower; //扇風機の風量 1～9

#endif