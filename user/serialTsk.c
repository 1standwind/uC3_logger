#include "uc3include.h"
#include "typedef.h"
#include "global.h"
#include "serialFunc.h"
#include <stdio.h>
# include "iodefine.h"

static void itoa_std(UW num, char* str)
{
    char c, *p, *q;

    p = q = str;

    /* Convert to ascii */
    do {
        c = num%10;
        *p++ = '0'+ c;
        num = num/10;
    } while(num);
    *p-- = '\0';
    
    /* Reverse the string */
    do {
       c = *p;
      *p = *q;
      *q = c;
      p--; q++;
    } while (q < p);
}
/*******************************
        serialPutTask
 *******************************/
/* {{UC3_TASK(serialPutTask) */
void serialPutTask(VP_INT exinf)
{
	char tempC[] = " 00.0\0";
	char dispData[28] = "";
	char yea[] = "0000\0";
	char mon[] = "00\0";
	char day[] = "00\0";
	char sec[] = "00\0";
	char hou[] = "00\0";
	char min[] = "00\0";
	char fmo[2];
	unsigned short nyea;
	unsigned char nmon;
	unsigned char nday;
	unsigned char numh;
	unsigned char numm;
	unsigned char nums;
	int numy;
	int tempI;
	int fm;
	float tempf;
	
	wai_sem(ID_DATA_SEM);
	numh = nowTime->hou;
	numm = nowTime->min;
	nums = nowTime->sec;
	nday = nowDay->day;
	nmon = nowDay->mon;
	nyea = nowDay->yea;
	numy = 0;
	tempI = nowTemp;
	fm = (int)fanMode;
	sig_sem(ID_DATA_SEM);
	
	getYEA(nyea,yea);
	getRTC(nmon,mon);
	getRTC(nday,day);
	getRTC(numh,hou);
	getRTC(numm,min);
	getRTC(nums,sec);
	
	tempC[0] = ' ';
	tempC[1] = (tempI / 100) + 0x30;
	tempC[2] = (tempI % 100)/10 + 0x30;
	tempC[3] = '.';
	tempC[4] = (tempI % 10) + 0x30;
	tempC[5] = '\0';
	
	if(tempI > 999){
		tempC[0] = '1';
		tempC[1] = (tempI%1000)/100 + 30;
	}else if(tempI < 0){
		tempC[0] = '-';
	}
	itoa_std(fm,fmo);
	
	if(NTP_STATUS){
		strcat(dispData,yea);
		strcat(dispData,"/");
		strcat(dispData,mon);
		strcat(dispData,"/");
		strcat(dispData,day);
		strcat(dispData,"/");
		strcat(dispData,hou);
		strcat(dispData,":");
		strcat(dispData,min);
		strcat(dispData,":");
		strcat(dispData,sec);
		strcat(dispData,tempC);
		strcat(dispData,"C");
		sig_sem(ID_DATA_SEM);
	}else if(nyea< 2000){
		strcat(dispData,"NTPERROR");
		strcat(dispData,tempC);
		strcat(dispData,"C");
		sig_sem(ID_DATA_SEM);
	}else{
		strcat(dispData,yea);
		strcat(dispData,"/");
		strcat(dispData,mon);
		strcat(dispData,"/");
		strcat(dispData,day);
		strcat(dispData,"/");
		strcat(dispData,hou);
		strcat(dispData,":");
		strcat(dispData,min);
		strcat(dispData,":");
		strcat(dispData,sec);
		strcat(dispData,tempC);
		strcat(dispData,"C");
		sig_sem(ID_DATA_SEM);
		
	}
	dispData[27] = '\0';
	
	serialPut(dispData);
        
	ter_tsk(ID_SERIAL_TSK);
}
