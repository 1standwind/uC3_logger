#include "uc3include.h"
#include "global.h"
// #include "iodefine.h"

// 扇風機が止まっている
#define FAN_STOP false
// 扇風機が動いている
#define FAN_MOVE true

/*******************************
    tempGet
*******************************/
/* {{UC3_CYC(tempGet) */
void tempGet(VP_INT exinf)
{
    Temperature fanBorder_local;          //閾値のローカル変数
    FanMode fanMode_local;                //扇風機のモード 1～3 ローカル変数
    FanPower fanPower_local;              //扇風機の風量 1～9 ローカル変数
    Temperature nowTemp_local;            //現在の温度を記録する ローカル変数
    static Temperature tempMin[60] = {0}; //1秒ごとの温度の配列
    static unsigned char cnt = 0;         //記録した回数をカウント
    static bool fanStatus = FAN_STOP;     // 扇風機状態　＝　0（止まっている）
    int i;                                //for文を回すため
    Temperature tempMinAve = -310;        //1分間の温度の平均
    short int elapsedMin;                 //00時00分からの経過秒
    unsigned char hor;
    unsigned char min;
    unsigned char sec;

    // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓時刻の取得↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
    // ICUのIER07.IEN7ビットに“0”を書く
    ICU.IER[07].BIT.IEN7 = 0;
    // RCR1レジスタのCIEビットに“1”を書く
    RTC.RCR1.BIT.CIE = 1;
    // ICUのIR63.IRフラグに“0”を書く
    ICU.IR[63].BIT.IR = 0;
    do
    {
        // 必要なカウンタレジスタをすべて読み出す
        sec = RTC.RSECCNT.BIT.SEC1 + RTC.RSECCNT.BIT.SEC10 * 10;
        min = RTC.RMINCNT.BIT.MIN1 + RTC.RMINCNT.BIT.MIN10 * 10;
        hor = RTC.RHRCNT.BIT.HOUR1 + RTC.RHRCNT.BIT.HOUR10 * 10;
    } while (ICU.IR[63].BIT.IR == 1); // ICUのIR63.IRフラグを読み出してチェック
    // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑時刻の取得↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

    // 温度を取得
    nowTemp_local = tempRead();
    tempMin[cnt++] = nowTemp_local;
    if (sec == 0)
    {
        tempMinAve = 0;
        for (i = 0; i < cnt; i++)
        {
            tempMinAve += tempMin[i];
        }
        tempMinAve /= cnt;

        cnt = 0;
        elapsedMin = hor * 60 + min;
    }

    // セマフォ（SEM＿DATA）獲得
    wai_sem(ID_DATA_SEM);

    // 温度のデータストアに保存する
    nowTemp = nowTemp_local;

    if (tempMinAve != -310)
    {
        logTemp[elapsedMin] = tempMinAve;
        tempMinAve = -310;
    }

    // 閾値・扇風機モード・風量をデータストアから値を取得
    fanBorder_local = fanBorder;
    fanMode_local = fanMode;
    fanPower_local = fanPower;

    // セマフォ（SEM＿DATA）返却
    sig_sem(ID_DATA_SEM);

    if (fanMode_local == TEMP_ACTION) // 扇風機モード＝＝オートモード
    {
        // if ((fanStatus == FAN_STOP) && (nowTemp_local > fanBorder_local)) //（現在の扇風機状態＝＝停止）＆＆（温度データ＞閾値)
        if ((MTU4.TGRD == 0) && (nowTemp_local > fanBorder_local)) //（モータが止まっているか）＆＆（温度データ＞閾値)
        {
            // moter関数呼び出し、取得した風量でモータを動かす
            motorPut(fanPower_local);
            // 扇風機状態＝１（動いている）
            fanStatus = FAN_MOVE;
        }
        // else if (fanStatus == FAN_MOVE && nowTemp_local < fanBorder_local) //（現在の扇風機状態＝＝起動）＆＆（温度データ＜閾値）
        else if (MTU4.TGRD > 0 && nowTemp_local < fanBorder_local) //（モータが動いているか）＆＆（温度データ＜閾値）
        {
            // moter関数呼び出し、モータを止める
            motorPut(0);
            // 扇風機状態＝０（止まっている）
            fanStatus = FAN_STOP;
        }
    }
}
/* }}UC3_CYC */
