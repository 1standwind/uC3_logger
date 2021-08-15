/***********************************************************************
  Sample program

   Generated at 2020-12-08 19:35:59

 ***********************************************************************/

/* {{UC3_INCLUDE */
#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include "net_hdr.h"
#include "net_id.h"
#include "http_server.h"
/* }}UC3_INCLUDE */

extern void rx_init_peripheral(void);
extern void init_peripheral(void);

/* {{UC3_CODE */


/******************************************************************
    Default interrupt handler
     Reached when the interrupt is accepted even though it is not the generation of an
     interrupt service routine also define the interrupt handler.
 ******************************************************************/

void int_abort(void)
{
    for(;;);
}

/*******************************
        rtc1s_isr
 *******************************/
/* {{UC3_ISR(rtc1s_isr) */
void rtc1s_isr(VP_INT exinf)
{
}
/* }}UC3_ISR */

/*******************************
        rtc1d_isr
 *******************************/
/* {{UC3_ISR(rtc1d_isr) */
void rtc1d_isr(VP_INT exinf)
{
}
/* }}UC3_ISR */

/*******************************
        swReader
 *******************************/
/* {{UC3_CYC(swReader) */
void swReader(VP_INT exinf)
{
}
/* }}UC3_CYC */

/*******************************
        tempGet
 *******************************/
/* {{UC3_CYC(tempGet) */
void tempGet(VP_INT exinf)
{
}
/* }}UC3_CYC */

/*******************************
        ledPut
 *******************************/
/* {{UC3_CYC(ledPut) */
void ledPut(VP_INT exinf)
{
}
/* }}UC3_CYC */

/*******************************
        ntpGetTask
 *******************************/
/* {{UC3_TASK(ntpGetTask) */
void ntpGetTask(VP_INT exinf)
{
}
/* }}UC3_TASK */

/*******************************
        lcdPutTask
 *******************************/
/* {{UC3_TASK(lcdPutTask) */
void lcdPutTask(VP_INT exinf)
{
}
/* }}UC3_TASK */

/*******************************
        serialPutTask
 *******************************/
/* {{UC3_TASK(serialPutTask) */
void serialPutTask(VP_INT exinf)
{
}
/* }}UC3_TASK */

/*******************************
        DBPutTask
 *******************************/
/* {{UC3_TASK(DBPutTask) */
void DBPutTask(VP_INT exinf)
{
}
/* }}UC3_TASK */

/*******************************
        start_tsk
 *******************************/
/* {{UC3_TASK(start_tsk) */
void start_tsk(VP_INT exinf)
{
}
/* }}UC3_TASK */


/* }}UC3_CODE */


/* {{UNET3_CODE */
/*******************************
  CGI Script
 ******************************/
void advancedSettingFunc(T_HTTP_SERVER *http)
{
}
void indexFunc(T_HTTP_SERVER *http)
{
}
void settingFunc(T_HTTP_SERVER *http)
{
}
/* }}UNET3_CODE */


/*******************************
        Main entry
 *******************************/

int main(void)
{
    ER ret = E_OK;

    /* Initialize hardware */
    init_peripheral();

    /* Generate system and start */
    ret = start_uC3();

    return ret;
}

/* end */
