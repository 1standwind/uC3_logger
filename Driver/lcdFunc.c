#include  "iodefine.h"

// 動作概要
//  キャラクタLCD：２行８桁  デバイス：ACM0802C
//  表示仕様     ：data=8bit,2行8桁 5*10フォント カーソル/ブリンク表示無し
//	P32	RS	[L]=インストラクション [H]=データレジスタ選択
//	P33	E	制御を有効にする場合、[H]にして「L]にする

#define	CTRL_FLG		0
#define	DATA_FLG		1
#define	LCD_MAX_ROW	2	// 2行
#define	LCD_MAX_COL	8	// 8桁

// 現在の、表示行位置ｗ
#define	LCD_ROW_CLR	0	// クリア直後
#define	LCD_ROW_1		1	// 1行目表示
#define	LCD_ROW_2		2	// 2行目表示

// DDRAMアドレス指示コード
#define	CODE_ROW_1	0x80	// 1行目指示
#define	CODE_ROW_2	0xC0	// 2行目指示

// (LCD制御コード)
#define	CLEAR_DISPLAY_CMD	0x01	// 全表示Clear,カーソルHome
#define	CUR_AT_HOME_CMD		0x02	// カーソルをホーム位置に戻す

#define	ENTRY_MODE_BASECMD	0x04	// データ書き込み(ベース)
	#define ENTRY_MODE_INC_ON	0x02	// カーソル右へ
	#define ENTRY_MODE_SFT_ON	0x01	// 表示をシフトさせる
#define	DISP_BASE_CMD		0x08	// 表示ON/OFF(ベース)
	#define DISP_DISP_ON	0x04	// 表示ON
	#define DISP_CUR_ON		0x02	// カーソル表示ON
	#define DISP_BLK_ON		0x01	// カーソル点滅ON
#define	CUR_SFT_BASE_CMD	0x10	// カーソル/表示シフト(ベース)
	#define CUR_SFT_DISP_SFT_ON	0x08	// 表示シフト
	#define CUR_SFT_CUR_SFT_ON	0x00	// カーソル移動
	#define CUR_SFT_RIGHT_ON	0x04	// 右シフト
	#define CUR_SFT_LEFT_ON	0x00	// 左シフト
//
#define	FUNC_BASE_CMD		0x20	// ファンクションセット(ベース)
	#define FUNC_DL_8		0x10	// バス幅 8bit
	#define FUNC_DL_4		0x00	// バス幅 4bit
	#define FUNC_LINE_2		0x08	// 2ライン表示
	#define FUNC_LINE_1		0x00	// 1ライン表示
	#define FUNC_FONT10		0x04	// フォント 5*10
	#define FUNC_FONT8		0x00	// フォント 5*8
//#define CGRAM_ADR_BASE_CMD	0x40	// CGRAMアドレスセット(ベース)
//#define DDRAM_ADR_BASE_CMD	0x80	// DDRAMアドレスセット(ベース)

// 処理概要 : データ出力
// 引数     : valc  出力データ
//          : sel   レジスタ選択 [0]=インストラクション  [1]=データレジスタ
static void  lcdPut( char valc, char sel )
{
	PORTC.DR.BYTE = valc;	// データ出力
	if( sel == 0 ){			// レジスタ選択出力[RS]
		PORT3.DR.BIT.B2 = 0;
	} else {
		PORT3.DR.BIT.B2 = 1;
	}
	PORT3.DR.BIT.B3 = 1;	// [E]信号 ON(時間管理必要)
	dly_tsk( 1 );				// 1μs待つ
	PORT3.DR.BIT.B3 = 0;	// [E]信号 Off

	dly_tsk( 1);			// ここで実行完了待ち
}
	

// キャラクタLCDドライバ初期化
void  lcdInit( void )
{
	char  valc;

	// CPUポート初期化
	PORTC.DDR.BYTE = 0xff;	// PORTC 全bitを出力ポート設定する
	PORTC.ODR.BYTE = 0x00;	// PORTCは 全bit CMOS出力
	PORT3.DDR.BIT.B2 = 1;	// P32を出力ポートにする
	PORT3.ODR.BIT.B2 = 0;	// P32 CMOS出力
	PORT3.DDR.BIT.B3 = 1;	// P33を出力ポートにする
	PORT3.ODR.BIT.B3 = 0;	// P33 CMOS出力

	
	// ファンクションセット <データbit=8,2行表示,5*10フォント>
	valc = FUNC_BASE_CMD | FUNC_DL_8 | FUNC_LINE_2 | FUNC_FONT10;
	lcdPut( valc, CTRL_FLG );

	// 表示ON/OFFコントロール <表示ON>
	valc = DISP_BASE_CMD | DISP_DISP_ON;
	lcdPut( valc, CTRL_FLG );

	// エントリーモード <アドレス増減ON>
	valc = ENTRY_MODE_BASECMD | ENTRY_MODE_INC_ON;
	lcdPut( valc, CTRL_FLG );

	lcdPut( CLEAR_DISPLAY_CMD, CTRL_FLG );
	dly_tsk(1);			// Clearは時間がかかる
}

// 処理概要 : データ表示
// 引数     : *str   表示データ格納ポインタ(NULL終端)
// 備考     : 表示データは、LCDデバイスの関係からASCIIである必要があるが、
//          : 本関数ではASCII/BINにかかわらず表示する
//          : なお表示終端データは[\0]として、呼び出し側で注意する事
void  lcdDispstr( char *str )
{
	int  cnt;
	int  chg;

	chg = 0;
	//lcdPut( CLEAR_DISPLAY_CMD, CTRL_FLG );
	//dly_tsk( 1 );			// Clearは時間がかかる
	lcdPut( CODE_ROW_1, CTRL_FLG );	// 表示位置先頭指定

	for( cnt=1;*str != '\0'; cnt++ ){		// データ終端まで表示
		if( (chg == 0) && (cnt > LCD_MAX_COL) ){// 行が変わる時、行列変更
			chg = 1;
			lcdPut( CODE_ROW_2, CTRL_FLG );
		}
		lcdPut( *str++, DATA_FLG );	// 表示データ書き込み
	}
}

// 処理概要 : 表示位置指定  データ１文字表示
// 引数	   : row   行  [1]=1行目  [2]=2行目   (範囲: 1 or 2) 
//	   : col   桁  [1]=1桁目  [2]=2桁目.. (範囲: 1 - 8)
//	   : valc  表示データ
// 戻り値   : 表示行/桁を間違えてセットされた場合、なにもせずに-1を返す
int  lcdDisppos( char row, char col, char valc )
{
	if( (row <= 0) || (row > LCD_MAX_ROW) ){	// 表示範囲であるか確認
														return -1;
	}		
	else if( (col <= 0 ) || (col > LCD_MAX_COL) ){
														return -1;
	}
	if( row == 1 ){	// 1行目表示
		lcdPut(CODE_ROW_1 + col-1, CTRL_FLG );
	}
	else {		// 2行目以降表示
		lcdPut(CODE_ROW_2 + col-1, CTRL_FLG );
	}
	lcdPut( valc, DATA_FLG );	// 表示データ書き込み

	return 0;
}
