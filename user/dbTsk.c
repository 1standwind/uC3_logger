#include "uc3include.h"
#include "global.h"
#include "typedef.h"
#include "stdlib.h"
#define CONTENTS_SIZE  256



/*******************************
        DBPutTask
 *******************************/
/* {{UC3_TASK(DBPutTask) */

static void itoa_std(UW num, char* str)
{
    char c, *p, *q;

    p = q = str;

    /* Convert to ascii */
    do {
        c = num%10;
        *p++ = '0'+ c;
        num = num/10;
    } while(num);
    *p-- = '\0';
    
    /* Reverse the string */
    do {
       c = *p;
      *p = *q;
      *q = c;
      p--; q++;
    } while (q < p);
}
void DBPutTask(VP_INT exinf)
{
    int numy;//年
    int nummo;//月
    int numd;//日
    int numh;//時
    int numm;//分
    int nums;//秒
    int temp;//温度
    int atemp;//絶対値変換前温度
     
    char numyy[5] = "";//文字列に変換したもの(年)
    char nummomo[3] = "";//文字列に変換したもの(月)
    char numdd[3] = "";//文字列に変換したもの(日)
    char numhh[3] = "";//文字列に変換したもの(時)
    char nummm[3] = "";//文字列に変換したもの(分)
    char numss[3] = "";//文字列に変換したもの(秒)
    
    char t;//保存用変数
    
    char dbDate[11] = "";//日付のデータを入れる配列
    char dbTime[10] = "";//時刻のデータを入れる配列
    char dbTemp[6] = "";//温度のデータを入れる配列
    
    char contents[256] = "";//連結先文字列のアドレス
    
    
    wai_sem(ID_DATA_SEM);
    numy = nowDay->yea;//メンバ変数を代入(年)
    nummo = nowDay->mon;//メンバ変数を代入(月)
    numd = nowDay->day;//メンバ変数yeaを代入(日)
    numh = nowTime->hou;//メンバ変数を代入(時)
    numm = nowTime->min;//メンバ変数を代入(分)
    nums = nowTime->sec;//メンバ変数を代入(秒)
    temp = nowTemp;//メンバ変数を代入(温度)
    sig_sem(ID_DATA_SEM);
    
    atemp = temp;//おんどを代入
    
    getYEA(numy,numyy);//値から文字列に変換(年)
    getRTC(nummo,nummomo);//値から文字列に変換(月)
    getRTC(numd,numdd);//値から文字列に変換(日)
    getRTC(numh,numhh);//値から文字列に変換(時)
    getRTC(numm,nummm);//値から文字列に変換(分)
    getRTC(nums,numss);//値から文字列に変換(秒)
   
    strcat(dbDate,numyy);//dbDate配列の中に文字連結していく
    strcat(dbDate,"-");
    strcat(dbDate,nummomo);
    strcat(dbDate,"-");
    strcat(dbDate,numdd);
    dbDate[11] = '\0';
    
    strcat(dbTime,numhh);;//dbTime配列の中に文字連結していく
    strcat(dbTime,":");
    strcat(dbTime,nummm);
    strcat(dbTime,":");
    strcat(dbTime,numss);
    dbTime[8] = '\0';
    
    temp = abs(temp);//絶対値に変換
    itoa_std(temp,dbTemp);//値から文字列に変換(温度)
    
    if(temp >= 100){
    	t = dbTemp[2];//配列の2番目をコピー
    	dbTemp[2] = '.';//小数点を格納
    	dbTemp[3] = t;//コピーしたものを3番目に入れる
    	dbTemp[5] = "\0";//null文字
    }else if(temp < 100){//10度未満の場合
    	t = dbTemp[1];//配列の1番目をコピー
    	dbTemp[1] = '.';//小数点を格納
    	dbTemp[2] = t;//コピーしたものを2番目に入れる
	dbTemp[4] = "\0";//null文字
    }
    
    
    strcat(contents,"GET /a20/insert.php?user=a20h1g4&name=Fan&datetime=");//送信データを文字連結する
    strcat(contents,dbDate);
    strcat(contents,"%20");
    strcat(contents,dbTime);
    strcat(contents,"&temperature=");
    if(atemp < 0 ){//絶対値変換前の温度がマイナスだったとき
    	strcat(contents,"-");
    }
    strcat(contents,dbTemp);
    strcat(contents," HTTP/1.0\r\n\r\n");

    DB_STATUS = dbPut(contents);//送信できたかどうか
    ter_tsk(ID_DBPUT_TSK);
}



/* }}UC3_TASK */