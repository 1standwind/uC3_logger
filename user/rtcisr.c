#include "uc3include.h"
#include "global.h"
/*******************************
        rtc1s_isr
 *******************************/
/* {{UC3_ISR(rtc1s_isr) */
void rtc1s_isr(VP_INT exinf)
{
    act_tsk(ID_LCDPUT_TSK);
    act_tsk(ID_SERIAL_TSK);
    
    if(nowTime->sec == 0){  //1分ごとにdbPutTask()を起動するため
        act_tsk(ID_DBPUT_TSK);
    }
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