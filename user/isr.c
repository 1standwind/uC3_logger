#include "uc3include.h"
#include "global.h"

void ddr_rx_rtc1s_isr(void){
    //lcdPutTask(),serialPutTask()を起動
    act_tsk(ID_LCDPUT_TSK);
    //act_tsk(ID_SERIAL_TSK);
    if(nowTime->sec==0){  //1分ごとにdbPutTask()を起動するため
        act_tsk(ID_DBPUT_TSK);
    }
}


void ddr_rx_rtc1d_isr(void)
{
    /*act_tsk(ID_NTPGET_TSK);
    ICU.IR92.IR=0;  //割り込みフラグをクリア
    //Renesas-RX62N_ユーザーズマニュアル.pdfのP.1159を参照*/
}
