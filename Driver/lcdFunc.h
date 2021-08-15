#ifndef _LCDFUNC_H_
#define _LCDFUNC_H_ 
int  lcdDisppos( char row, char col, char valc );
void  lcdDispstr( char *str );
void lcdInit(void);
static void  lcdPut( char valc, char sel );
#endif