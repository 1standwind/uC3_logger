#include "typedef.h"
#include "global.h"

volatile Time *nowTime;             //現在の時刻を保存する
volatile Day *nowDay;               //現在の日付を保存する
volatile Temperature nowTemp;       //現在の温度を記録する。
volatile Temperature logTemp[1440]; //1分ごとに温度を保存する。
volatile FanMode fanMode;           //扇風機のモード 1～3
volatile FanPower fanPower;         //扇風機の風量 1～9
volatile Temperature fanBorder;     //扇風機の閾値 -30～99
volatile bool tempMode;             //LCDの摂氏/華氏切り替え
volatile bool NTP_STATUS;           //NTPの状態 "NTP_NORMALLY","NTP_ERROR"で代入したり判定したりする
volatile bool DB_STATUS;            //DBの状態 "DB_NORMALLY","DB_ERROR"で代入したり判定したりする