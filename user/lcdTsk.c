#include "uc3include.h"
#include <stdio.h>
#include "lcdFunc.h"
#include "global.h"
#include "utils.h"


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
void HardwareSetup(void)
{
	SYSTEM.SCKCR.BIT.ICK = 0x00; /*システムクロック　ｘ16　96MHz */
	SYSTEM.SCKCR.BIT.PCK = 0x01; /*周辺クロック　×4　48MHz*/
	SYSTEM.SCKCR.BIT.BCK = 0x01; /*外部バスクロック x4 48MHz */
}


/*******************************
        lcdPutTask
 *******************************/
/* {{UC3_TASK(lcdPutTask) */
void lcdPutTask(VP_INT exinf)
{
	char tempC[] = " 00.0C\0";
	char dispData[17] = "";
	char sec[] = "00\0";
	char hou[] = "00\0";
	char min[] = "00\0";
	char fmo[2];
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
	numy = (int)nowDay->yea;
	numy = 0;
	tempI = nowTemp;
	fm = (int)fanMode;
	sig_sem(ID_DATA_SEM);
	
	getRTC(numh,hou);
	getRTC(numm,min);
	getRTC(nums,sec);
	
	if(tempMode == TEMP_CELSIUS){
		tempC[0] = ' ';
		tempC[1] = (tempI / 100) + 0x30;
		tempC[2] = (tempI % 100)/10 + 0x30;
		tempC[3] = '.';
		tempC[4] = (tempI % 10) + 0x30;
		tempC[5] = 'C';
		tempC[6] = '\0';
	}else if(tempMode == TEMP_FAHRENHEIT){
		tempI = tempI*1.8+32;
		tempC[0] = ' ';
		tempC[1] = (tempI / 100) + 0x30;
		tempC[2] = (tempI % 100)/10 + 0x30;
		tempC[3] = '.';
		tempC[4] = (tempI % 10) + 0x30;
		tempC[5] = 'F';
		tempC[6] = '\0';
	}
	
	if(tempI > 999){
		tempC[0] = '1';
		tempC[1] = (tempI%1000)/100 + 30;
	}else if(tempI < 0){
		tempC[0] = '-';
	}
	itoa_std(fm,fmo);
	
	if(NTP_STATUS){
		strcat(dispData,hou);
		strcat(dispData,":");
		strcat(dispData,min);
		strcat(dispData,":");
		strcat(dispData,sec);
		strcat(dispData,tempC);
		strcat(dispData," ");
		strcat(dispData,fmo);
		sig_sem(ID_DATA_SEM);
		HardwareSetup();
		lcdInit();
		lcdDispstr(dispData);
	}else if(numy< 2000){
		strcat(dispData,"NTPERROR");
		strcat(dispData,tempC);
		strcat(dispData," ");
		strcat(dispData,fmo);
		sig_sem(ID_DATA_SEM);
		HardwareSetup();
		lcdInit();
		lcdDispstr(dispData);
	}else{
		strcat(dispData,hou);
		strcat(dispData,":");
		strcat(dispData,min);
		strcat(dispData,":");
		strcat(dispData,sec);
		strcat(dispData,tempC);
		strcat(dispData," ");
		strcat(dispData,fmo);
		sig_sem(ID_DATA_SEM);
		HardwareSetup();
		lcdInit();
		lcdDispstr(dispData);
		
	}
	dispData[16] = '\0';
	ter_tsk(ID_LCDPUT_TSK);
}
/* }}UC3_TASK */