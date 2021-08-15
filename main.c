/***********************************************************************
Sample program

Generated at 2020-11-19 14:10:14

***********************************************************************/

/* {{UC3_INCLUDE */
#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include "net_hdr.h"
#include "net_id.h"
#include "http_server.h"
/* }}UC3_INCLUDE */
#include "typedef.h"
#include "global.h"
#include "swFunc.h"
#include "motorFunc.h"
#include "tempFunc.h"
#include "lcdFunc.h"
#include "serialFunc.h"
#include "ledFunc.h"
#include "led7segFunc.h"

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
    for (;;);
}

/* }}UC3_CODE */

/*******************************
        Main entry
*******************************/

int main(void)
{
    //volatile Time *nowTime = (volatile Time *)(0x0008C402);

    ER ret = E_OK;

    /* Initialize hardware */
    init_peripheral();    

    /* Generate system and start */
    ret = start_uC3();

    return ret;
}

/*******************************
        start_tsk
 *******************************/
/* {{UC3_TASK(start_tsk) */
void start_tsk(VP_INT exinf)
{
    ER ercd;

    int i; //for文を回す変数

    //時刻の変数にレジスタのアドレスを入れる。変数を見に行けばRTCの時分秒を参照できるようにする
    nowTime = (volatile Time *)(0x0008C402);
    //日付の変数にレジスタのアドレスを入れる。変数を見に行けばRTCの年月日を参照できるようにする
    nowDay = (volatile Day *)(0x0008C40A);

    //エラー値で現在温度を初期化
    nowTemp = -310;

    // 扇風機のモードの初期値を1(常にOFF)にする
    fanMode = ALWAYS_OFF;
    //fanMode = TEMP_ACTION;
    // 扇風機の風量の初期値を5に設定にする
    fanPower = 5;
    // 扇風機の温度の閾値の初期値を摂氏25度で設定する(25度なら250と設定する)
    fanBorder = 300;

    // LCDの温度表示を摂氏にする
    tempMode = TEMP_CELSIUS;

    // NTPの接続状態をエラーで初期設定
    NTP_STATUS = NTP_ERROR;

    // DBの接続状態をエラーで初期設定
    DB_STATUS = DB_ERROR;
    
    lcdInit();
    serialInit();
    
    

    //RTC コントロールレジスタ1（RCR1）
    // *((char *)0x8c422) = 0x3;
    RTC.RCR1.BIT.AIE = 1; //AIE ビット（アラーム割り込み許可ビット）
    RTC.RCR1.BIT.CIE = 1; //CIE ビット（桁上げ割り込み許可ビット）

    //RTC コントロールレジスタ2（RCR2）
    // *((char *)0x8c424) = 0x1;
    RTC.RCR2.BIT.START = 1; //1：年、月、曜日、日、時、分、秒、64Hzカウンタおよびプリスケーラは通常動作
    
    RTC.RCR1.BIT.PIE = 0;
    RTC.RCR1.BIT.PES = 6;
    RTC.RCR1.BIT.PIE = 1;
    IEN(RTC,PRD) = 1;
    IPR(RTC,PRD) = 10;

    //logTemp[]の要素を全て-310で初期化
    for (i = 0; i < (sizeof(logTemp) / sizeof(logTemp[0])); i++)
    {
        logTemp[i] = -310;
    }

    // ボタンの初期化
    swInit();

    // モータの初期化
    motorInit();

    // 温度センサの初期化
    tempInit();


    // シリアル端末の初期化
    //serialInit();

    // 外部ウェブサーバの初期化
    //なし

    // ブラウザの初期化
    /* Initialize uNet3 */
    PORTA.ICR.BIT.B3 = 1;           /* Ether-In: MDIO */
    PORTB.ICR.BYTE = 0x8f;          /* Ether-In: RMII_RXD1, _RXD0, _REF50CK, _RX_ER, _CRS_DV */
    IOPORT.PFENET.BYTE = 0x80;      /* PHY=RMII */
    

    // LEDの初期化
    ledInit();

    //7セグLEDの初期化
    segInit();
    //7セグLEDに現在の風量を表示
    segPut(fanPower);

    // ID_TEMPGET_CYCの起動
    sta_cyc(ID_TEMPGET_CYC);
    // ID_SWR_CYCの起動
    sta_cyc(ID_SWR_CYC);
    // ID_LEDP_CYCの起動
    sta_cyc(ID_LEDP_CYC);
    
    act_tsk(ID_NTPGET_TSK);

    ercd = net_setup();
    // 自分で休止状態に移行
    ter_tsk(ID_START_TSK);
}
/* }}UC3_TASK */

/* end */
