#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "typedef.h"

extern volatile Time *nowTime;             //現在の時刻を保存する
extern volatile Day *nowDay;               //現在の日付を保存する
extern volatile Temperature nowTemp;       //現在の温度を記録する。
extern volatile Temperature logTemp[1440]; //1分ごとに温度を保存する。
extern volatile FanMode fanMode;           //扇風機のモード 1～3
extern volatile FanPower fanPower;         //扇風機の風量 1～9
extern volatile Temperature fanBorder;     //扇風機の閾値 -30～99
extern volatile bool tempMode;             //LCDの摂氏/華氏切り替え
extern volatile bool NTP_STATUS;           //NTPの状態 "NTP_NORMALLY","NTP_ERROR"で代入したり判定したりする
extern volatile bool DB_STATUS;            //DBの状態 "DB_NORMALLY","DB_ERROR"で代入したり判定したりする

// fanModeの状態を示す
// 常にOFF
#define ALWAYS_OFF 1
// 閾値によって動作
#define TEMP_ACTION 2
// 常にON
#define ALWAYS_ON 3

//tempModeの状態を示す
//摂氏
#define TEMP_CELSIUS true
//華氏
#define TEMP_FAHRENHEIT false

//NTP_STATUSの状態を示す
//正常
#define NTP_NORMALLY true
//異常
#define NTP_ERROR false

//DB_STATUSの状態を示す
//正常
#define DB_NORMALLY true
//異常
#define DB_ERROR false

#endif