#include "uc3include.h"
#include "global.h"
#include "typedef.h"
#include"net_hdr.h"
#include"net_cfg.h"


 char rcvData[128] ;
bool dbPut(char *logData)
{
   bool result = DB_ERROR;//結果用変数
   UH size = net_strlen(logData);//送信データのデータサイズ
  
   ER ercd = E_OK;//エラーコード
   
   T_NODE remote;//外部サーバに送るための情報
   remote.port = 80;//ソケットのポート番号
   remote.ver = IP_VER4;//IP_VERを指定
   remote.num = 1;//デバイス番号
   remote.ipa = ip_aton("192.168.4.9");//リモート先のIPアドレス
   
   ercd = con_soc(ID_TCP_HTTP,&remote,SOC_CLI);//外部サーバーに能動接続する
   ercd = snd_soc(ID_TCP_HTTP,(VP)logData,size);//送信データを送信する
   ercd = rcv_soc(ID_TCP_HTTP,(VP)rcvData,sizeof(rcvData));
   ercd = cls_soc(ID_TCP_HTTP,SOC_TCP_CLS);//ソケットを切断する
  
   
   if(ercd == -1){//正常終了しなかったら
   	result = DB_ERROR;//false
	return result;//falseを返す
   }else{
   	result = DB_NORMALLY;//true
	return result;//tureを返す
   }
}
