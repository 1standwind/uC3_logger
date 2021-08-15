#include "uc3include.h"
#include "global.h"
#include "motorFunc.h"
#include "swFunc.h"
#include "led7segFunc.h"

#define SW0 0x01
#define SW1 0x02
#define SW2 0x04
#define SW3 0x08
#define SW4 0x10
#define SW5 0x20
#define SW6 0x40
#define SW7 0x80

/*******************************
swReader
*******************************/
/* {{UC3_CYC(swReader) */
void swReader(VP_INT exinf)
{
    static unsigned char swOld = 0x00;
    unsigned char swNew;
    unsigned char chg;

    swNew = swGet();

    chg = swNew ^ swOld;
    if (chg)
    {
        wai_sem(ID_DATA_SEM);         //ID_DATA_SEMを獲得
        if (chg & SW0 && swNew & SW0) // 扇風機のモード変更
        {
            // モードを変更
            fanMode++;
            // FanModeが範囲外のとき１にする
            if (fanMode > 3 || fanMode < 1)
            {
                fanMode = 1;
            }
            // 　範囲内 1 ~3 モーターの値を書き換える。1 : motorPut(0);
            // 2 : なにもしない 3 : motorPut(FanPower);
            switch (fanMode)
            {
            case 1:
                motorPut(0);
                act_tsk(ID_LCDPUT_TSK); //  LCDに反映
                break;
            case 2:
                act_tsk(ID_LCDPUT_TSK); //  LCDに反映
                break;
            case 3:
                motorPut(fanPower);
                act_tsk(ID_LCDPUT_TSK); //  LCDに反映
                break;
            default:
                break;
            }
        }
        if (chg & SW1)
        {
            if (swNew & SW1) // 風量下げる
            {
                // 風量を下げる
                fanPower--;
                // FanModeが範囲外のとき1にする
                if (fanPower > 9 || fanPower < 1)
                {
                    fanPower = 1;
                }
                // 風量が範囲内 1 ~9で モードが３ならばモーターの値を書き換える。motorPut(FanPower)
                if (fanMode == 3 && fanPower >= 1 && fanPower <= 9)
                {
                    motorPut(fanPower);
                }
                if (fanMode == 2 && MTU4.TGRD > 0)
                {
                    motorPut(fanPower);
                }
                // ７セグLEDの値を書き換える。led7segPut(FanPower)
                segPut(fanPower);
            }
        }
        if (chg & SW2)
        {
            if (swNew & SW2) // 風量上げる
            {
                // 風量上げる
                fanPower++;
                // FanModeが範囲外のとき9にする
                if (fanPower > 9 || fanPower < 1)
                {
                    fanPower = 9;
                }
                // 風量が範囲内 1 ~9で モードが３ならばモーターの値を書き換える。motorPut(FanPower)
                if (fanMode == 3 && fanPower >= 1 && fanPower <= 9)
                {
                    motorPut(fanPower);
                }
                if (fanMode == 2 && MTU4.TGRD > 0)
                {
                    motorPut(fanPower);
                }
                // ７セグLEDの値を書き換える。led7segPut(FanPower)
                segPut(fanPower);
            }
        }

        if (chg & SW3)
        {
            if (swNew & SW3) // 摂氏華氏変更
            {
                // 摂氏華氏反転
                tempMode = !tempMode;
                //  LCDに反映
                act_tsk(ID_LCDPUT_TSK);
            }
        }
        // セマフォ（ID_DATA_SEM）返却
        sig_sem(ID_DATA_SEM);
        swOld = swNew;
    }
}
/* }}UC3_CYC */