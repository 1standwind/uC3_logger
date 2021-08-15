#include "uc3include.h"
#include "typedef.h"
#include "global.h"
#include "iodefine.h"
#include "ledFunc.h"

static volatile char led = 0;
static volatile char seg = 0;

/*******************************
        ledPut
 *******************************/
/* {{UC3_CYC(ledPut) */
void ledPut(VP_INT exinf)
{
	//LED0を反転するため
	ledSet(ledGet() ^ 0x1);

	wai_sem(ID_DATA_SEM); //セマフォを取得する（SEM_DATA）
	
	// if (データストアのNTP_STATU == エラーフラグ)
	if (NTP_STATUS == NTP_ERROR)
	{
		// NTPのエラーフラグが建っていたらLED1を点灯する
		PORTE.DR.BIT.B1 = 0;
	}
	else
	{
		// エラーじゃなかったらLED1を消灯する
		PORTE.DR.BIT.B1 = 1;
	}

	// if (データストアのDB_STATU == エラーフラグ)
	if (DB_STATUS == DB_ERROR)
	{
		// DBのエラーフラグが建っていたらLED2を点灯する
		PORTE.DR.BIT.B2 = 0;
	}
	else
	{
		// エラーじゃなかったらLED2を消灯する
		PORTE.DR.BIT.B2 = 1;
	}

	// セマフォID：SEM_NETSTATUSを返却する
	sig_sem(ID_DATA_SEM); //セマフォの解放(SEM_DATA)
}
/* }}UC3_CYC */