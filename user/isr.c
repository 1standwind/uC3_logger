#include "uc3include.h"
#include "global.h"

void ddr_rx_rtc1s_isr(void){
    //lcdPutTask(),serialPutTask()���N��
    act_tsk(ID_LCDPUT_TSK);
    //act_tsk(ID_SERIAL_TSK);
    if(nowTime->sec==0){  //1�����Ƃ�dbPutTask()���N�����邽��
        act_tsk(ID_DBPUT_TSK);
    }
}


void ddr_rx_rtc1d_isr(void)
{
    /*act_tsk(ID_NTPGET_TSK);
    ICU.IR92.IR=0;  //���荞�݃t���O���N���A
    //Renesas-RX62N_���[�U�[�Y�}�j���A��.pdf��P.1159���Q��*/
}
