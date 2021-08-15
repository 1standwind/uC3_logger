#include <stdio.h>

#include "utils.h"
#include "typedef.h"

void inHMS(unsigned long x,Time *time) {
    //Convert Ntp hours to seconds, minutes and hours
    int sec = x % 60;
    int min = (x % 3600) / 60;
    int hou = ((x % 86400) / 3600) + 9;

    //Convert seconds, minutes and hours to RTC format and assign to Time
    time->sec = sec%10;
    time->sec += (sec/10)*0x10;
    time->min = min%10;
    time->min += (min/10)*0x10;
    time->hou = hou%10;
    time->hou += (hou/10)*0x10;
    
    return time;
    
}

Day inYMD(unsigned long x,Day *day){
    //Convert year, month and day to seconds, minutes and hours
    short monthday[] = {0,31,59,90,120,151,181,212,243,273,304,334,365,395};
    
    unsigned char today = 1;
    unsigned char month = 1;
    unsigned short year = 1;
    unsigned long days = x / 86400;
    
    int i = 0;
    
    days -= (days/365)/4;
    year = days/365+1900;
    days = days%365+1;
    
    if(year % 4 == 0){
	    for(i = 2; i < 13; i++)monthday[i]++;
	    days++;
    }
    
    for(i = 0; days>monthday[i + 1]; i++);
    today = days - monthday[i];
    month = i + 1;
    
    //Convert year, month and day to RTC format and substitute for day
    day->day = today%10;
    day->day += (today/10)*0x10;
    day->mon = month%10;
    day->mon += (month/10)*0x10;
    day->yea = year % 10;
    day->yea += ((year % 100) / 10) * 0x10;
    day->yea += ((year % 1000) / 100) * 0x100;
    day->yea += year / 1000 * 0x1000;
}

void getRTC(unsigned char x, char *string){
    //Convert 8bit RTC format to char * format.
    int time = x;
    int digit1 = (int)(x & 0x0f);
    int digit2 = 0;

    while(time > 10){
        time -= 0x10;
        digit2++;
    }

    string[0] = digit2 + 0x30;
    string[1] = digit1 + 0x30;
    string[2] = '\0';
}

void getYEA(unsigned short x, char *string){
    //Convert 16bit RTC format to char * format.
    int time = x;
    int digit1 = (int)(x & 0x0f);
    int digit2 = 0;
    int digit3 = 0;
    int digit4 = 0;

    while(time > 1000){
        time -= 0x1000;
        digit4++;
    }
    
    while(time > 100){
        time -= 0x100;
        digit3++;
    }
    
    while(time > 10){
        time -= 0x10;
        digit2++;
    }

    string[0] = digit4 + 0x30;
    string[1] = digit3 + 0x30;
    string[2] = digit2 + 0x30;
    string[3] = digit1 + 0x30;
    string[4] = '\0';
}