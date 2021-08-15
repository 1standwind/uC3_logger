#include "uc3include.h"
#include "global.h"
#include "motorFunc.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "net_hdr.h"
#include "net_strlib.h"

#include "dns_client.h"
#include "sntp_client.h"
#include "ping_client.h"

#define CONTENTS_SIZE 8192 /* response contents size */

#ifdef NET_C
#include "kernel_id.h"
#include "net_id.h"
#else
ID ID_CONTENTS_MPF = 0;
const T_CMPF c_contents_mpf = {TA_TFIFO, 2, CONTENTS_SIZE, "contents_mpf"};
#endif

TMO LedTmo = 1000;
static char tmp[16];

/* {{UNET3_CODE */
/*******************************
  CGI Script
 ******************************/

static void itoa_std(UW num, char *str)
{
	char c, *p, *q;

	p = q = str;

	/* Convert to ascii */
	do
	{
		c = num % 10;
		*p++ = '0' + c;
		num = num / 10;
	} while (num);
	*p-- = '\0';

	/* Reverse the string */
	do
	{
		c = *p;
		*p = *q;
		*q = c;
		p--;
		q++;
	} while (q < p);
}

static ER get_contents_buf(char **buf)
{
	ER ercd;
	*buf = (char *)0;
	ercd = tget_mpf(ID_CONTENTS_MPF, (VP *)buf, TMO_POL);
	if (ercd == E_OK && *buf)
	{
		net_memset(*buf, 0, CONTENTS_SIZE);
	}
	return ercd;
}

static void ret_contents_buf(char *buf)
{
	if (!buf)
	{
		return;
	}
	rel_mpf(ID_CONTENTS_MPF, buf);
	return;
}

ER led_blink(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	char *contents;
	ER ercd;
	int i;
	for (i = 0; i < cnt; i++)
	{
		if (net_strcmp(name[i], "led") == 0)
		{
			LedTmo = atoi(value[i]) * 100;
			if (LedTmo < 0)
			{
				LedTmo = 0;
			}
			break;
		}
	}
	if (i == cnt)
	{
		return E_OBJ;
	}
	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{
		return ercd;
	}

	net_strcat(contents, "<html><body><center>Set LED interval ");
	net_strcat(contents, value[i]);
	net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);
	return ercd;
}

ER ping_send(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	T_PING_CLIENT ping = {0};
	char *contents;
	ER ercd;
	int i, j;

	for (i = 0, j = 0; i < cnt; i++)
	{
		if (net_strcmp(name[i], "remote") == 0)
		{
			ping.ipa = ip_aton(value[i]);
			j = i;
		}
		if (net_strcmp(name[i], "timeout") == 0)
		{
			ping.tmo = atoi(value[i]);
		}
	}
	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{
		return ercd;
	}

#ifdef NET_C
	ping.sid = ID_ICMP;
#endif
	ping.devnum = http->NetChannel;
	ping.tmo = PING_TIMEOUT;
	ercd = ping_client(&ping);

	net_strcat(contents, "<html><body><center>");
	if (ercd == E_OK)
	{
		net_strcat(contents, "Success reply from ");
	}
	else
	{
		net_strcat(contents, "No response from ");
	}
	net_strcat(contents, value[j]);
	net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);
	return E_OK;
}

ER sntp_send(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	T_SNTP_CLIENT sntp;
	char *contents;
	UW time[2];
	ER ercd;
	int i;

	net_memset(&sntp, 0, sizeof(sntp));
	net_memset(time, 0, sizeof(time));

	for (i = 0; i < cnt; i++)
	{
		if (net_strcmp(name[i], "sntp") == 0)
		{
			sntp.ipa = ip_aton(value[i]);
			break;
		}
	}
	if (i == cnt)
	{
		return E_OBJ;
	}
	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{
		return ercd;
	}

#ifdef NET_C
	sntp.sid = ID_UDP_SMPL;
#endif
	sntp.devnum = http->NetChannel;
	ercd = sntp_client(&sntp, &time[0], &time[1]);

	net_strcat(contents, "<html><body><center>");
	if (ercd == E_OK)
	{
		itoa_std((time[0] - NTP_BASE_TIME), tmp);
		net_strcat(contents, "<script type=\"text/javascript\">");
		net_strcat(contents, "var d=new Date(");
		net_strcat(contents, tmp);
		net_strcat(contents, "*1000);");
		net_strcat(contents, "document.write(d.toString());</script>");
	}
	else
	{
		net_strcat(contents, "No response from ");
		net_strcat(contents, value[i]);
	}
	net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);
	return E_OK;
}

ER dns_resolve(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	char *contents;
	ER ercd;
	UW ip, dns;
	int i, j;

	for (i = 0, j = 0, dns = 0; i < cnt; i++)
	{
		if (net_strcmp(name[i], "dns") == 0)
		{
			dns = ip_aton(value[i]);
		}
		if (net_strcmp(name[i], "fqdn") == 0)
		{
			j = i;
		}
	}
	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{
		return ercd;
	}

	ercd = dns_get_ipaddr(ID_UDP_SMPL, dns, value[j], &ip);

	net_strcat(contents, "<html><body><center>");
	if (ercd == E_OK)
	{
		ip_ntoa(tmp, ip);
		net_strcat(contents, value[j]);
		net_strcat(contents, "<p>");
		net_strcat(contents, tmp);
	}
	else
	{
		net_strcat(contents, "Can not resolve ");
		net_strcat(contents, value[j]);
	}
	net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);
	return E_OK;
}

//3桁の温度を受け取ったら10分の1して実数にして文字列の配列に格納
//tempが285ならstrには"28.5"と格納する
void tempString(Temperature temp, char *str)
{
	char buf[5];
	itoa_std(temp / 10, str);
	itoa_std(temp % 10, buf);
	net_strcat(str, ".");
	net_strcat(str, buf);
}

// 文字列が数字文字列ならtrueを返す
bool isdigitStr(char *str)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		// 文字が数字ではなかったら
		if (!(isdigit(str[i])))
		{
			// falseを返す
			return false;
		}
	}
	// 数字文字列ならtrueを返す
	return true;
}

void advancedSettingFunc(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{

	char *contents;	 //連結先文字列用の変数(型：char *)
	Temperature thr; //閾値用の変数(255みたいな)
	char thrString[5];
	char *IP_id; //IPアドレス用の変数()
	ER ercd;	 //エラーコード(型：ER)

	wai_sem(ID_DATA_SEM); //セマフォの取得(SEM_DATA)
	thr = fanBorder;	  //閾値のデータストアから閾値を取得する
	//R_FlashDataAreaAccess(0xffff,0xffff);//IPアドレスを取得する*************************
	sig_sem(ID_DATA_SEM); //セマフォの解放(SEM_DATA)

	ercd = get_contents_buf(&contents); //バッファを取得(get_comtents_buf())

	if (ercd != E_OK)
	{				 //正常終了しなかったら
		return ercd; //エラーコードを返す
	}

	//snprintf(thrString, sizeof(thrString), "%2.1f", thr);

	net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8' /><title>advansedSetting</title></head><body>"); //(文字連結)net_strcat()を使って、“<html><body><href=’index.cgi’><input(type = button)>”(ホームに戻る)
	net_strcat(contents, "<button onclick=\"location.href='/index.cgi'\">ホーム画面</button>");
	//net_strcat(contents, "<from action=\"/Setting.cgi\" method=\"get\">");
	net_strcat(contents, "<div><noscript><h2>このサイトではJavaScriptを使用しています。JavaScriptを有効にしてください。</h2></noscript></div>");
	net_strcat(contents, "<div><h2>天気予報の地域設定:<span id='areaDisp'></span></h2>");

	net_strcat(contents, "<select name=\"areaSelect\" id=\"areaSelect\">");
	net_strcat(contents, "<option value=\"HOKKAIDO\">北海道</option><option value=\"AOMORI\">青森</option>");
	net_strcat(contents, "<option value=\"IWATE\">岩手</option><option value=\"MIYAGI\">宮城</option>");
	net_strcat(contents, "<option value=\"AKITA\">秋田</option><option value=\"YAMAGATA\">山形</option>");
	net_strcat(contents, "<option value=\"FUKUSHIMA\">福島</option><option value=\"IBARAKI\">茨城</option>");
	net_strcat(contents, "<option value=\"TOCHIGI\">栃木</option><option value=\"GUNMA\">群馬</option>");
	net_strcat(contents, "<option value=\"SAITAMA\">埼玉</option><option value=\"CHIBA\">千葉</option>");
	net_strcat(contents, "<option value=\"TOKYO\">東京</option><option value=\"KANAGAWA\">神奈川</option>");
	net_strcat(contents, "<option value=\"NIIGATA\">新潟</option><option value=\"TOYAMA\">富山</option>");
	net_strcat(contents, "<option value=\"ISHIKAWA\">石川</option><option value=\"FUKUI\">福井</option>");
	net_strcat(contents, "<option value=\"YAMANASHI\">山梨</option><option value=\"AGANO\">長野</option>");
	net_strcat(contents, "<option value=\"GIFU\">岐阜</option><option value=\"SHIZUOKA\">静岡</option>");
	net_strcat(contents, "<option value=\"AICHI\">愛知</option><option value=\"MIE\">三重</option>");
	net_strcat(contents, "<option value=\"SHIGA\">滋賀</option><option value=\"KYOTO\">京都</option>");
	net_strcat(contents, "<option value=\"OSAKA\">大阪</option><option value=\"HYOGO\">兵庫</option>");
	net_strcat(contents, "<option value=\"NARA\">奈良</option><option value=\"WAKAYAMA\">和歌山</option>");
	net_strcat(contents, "<option value=\"TOTTORI\">鳥取</option><option value=\"SHIMANE\">島根</option>");
	net_strcat(contents, "<option value=\"OKAYAMA\">岡山</option><option value=\"HIROSHIMA\">広島</option>");
	net_strcat(contents, "<option value=\"YAMAGUCHI\">山口</option><option value=\"TOKUSHIMA\">徳島</option>");
	net_strcat(contents, "<option value=\"KAGAWA\">香川</option><option value=\"EHIME\">愛媛</option>");
	net_strcat(contents, "<option value=\"KOUCHI\">高知</option><option value=\"FUKUOKA\">福岡</option>");
	net_strcat(contents, "<option value=\"SAGA\">佐賀</option><option value=\"NAGASAKI\">長崎</option>");
	net_strcat(contents, "<option value=\"KUMAMOTO\">熊本</option><option value=\"OITA\">大分</option>");
	net_strcat(contents, "<option value=\"MIYAGI\">宮城</option><option value=\"KAGOSHIMA\">鹿児島</option>");
	net_strcat(contents, "<option value=\"OKINAWA\">沖縄</option></select>");

	net_strcat(contents, "<script src=\"/advancedSetting.js\"></script>");
	net_strcat(contents, "<button onclick='areaBT()' name='setting'>地域設定</button></div>");

	tempString(thr, thrString);

	net_strcat(contents, "<div><form action=\"/setting.cgi\" method=\"get\"><h2>温度の閾値:");
	net_strcat(contents, thrString);
	net_strcat(contents, "℃</h2><p>変更:<input type=\"number\" name=\"thresholdTemp\" id=\"\" value=\"");
	net_strcat(contents, thrString);
	net_strcat(contents, "\" max=\"99\" min=\"-30\"><input type=\"submit\" value=\"閾値変更\"></p></form></div>");

	net_strcat(contents, "<div><form action=\"/setting.cgi\" method=\"get\"><h2>マイコンボードのIPアドレス：");
	net_strcat(contents, "<div><input type='number' name='IP_1' id='' value='192'>");
	net_strcat(contents, "<input type='number' name='IP_2' id='' value='168'>");
	net_strcat(contents, "<input type='number' name='IP_3' id='' value='4'>");
	net_strcat(contents, "<input type='number' name='IP_4' id='' value='222'>");
	net_strcat(contents, "<input type=\"submit\" value=\"IPアドレス変更\"></form></div>");

	// net_strcat(contents, "<action=’setting.cgi’ (method = get)><input(type = submit)></form></body></html>"); //“<action=’setting.cgi’ (method = get)><input(type = submit)></form></body></html>” (設定反映)

	HttpSendText(http, contents, net_strlen(contents)); //文字を送る(HttpSendText())
	ret_contents_buf(contents);							//バッファを解放(ret_contents_buf())
	return E_OK;
	//エラーコードを返す
}

void indexFunc(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	char *contents; //連結先文字列用の変数(型：char *)
	int t, i;
	Temperature LTemp; //現在温度
	char nowTempStr[5];
	char hor[] = "00\0";
	char min[] = "00\0";
	char sec[] = "00\0";
	Time *nTime;
	char ChnTime[10];
	FanPower fan_volume; //風量
	char Fan_v[10];
	FanMode mode;			  //モード用の変数(型：int)
	char modeString[2] = {0}; //モード番号の文字列
	Temperature oneTemp[1440];

	int sizeofTest = 0;

	ER ercd; //エラーコード(型：ER)

	wai_sem(ID_DATA_SEM); //セマフォの取得(SEM_DATA)
	getRTC(nowTime->hou, hor);
	getRTC(nowTime->min, min);
	getRTC(nowTime->sec, sec);
	LTemp = nowTemp;						   //現在温度のデータストアから温度を取得する
	fan_volume = fanPower;					   //風量のデータストアから風量を取得する
	mode = fanMode;							   //モードのデータストアからモードを取得する
	memcpy(oneTemp, logTemp, sizeof(logTemp)); //温度の配列を保存する
	sig_sem(ID_DATA_SEM);					   //セマフォの解放(SEM_DATA)

	ercd = get_contents_buf(&contents); //バッファを取得(get_comtents_buf())
	if (ercd != E_OK)
	{				 //正常終了しなかったら
		return ercd; //エラーコードを返す
	}

	net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8' /><title>ホーム画面</title><script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js\"></script></head><body>"); //(文字連結)net_strcat()を使って、“<html><body><href=’index.cgi’><input(type = button)>”(ホームに戻る)
	net_strcat(contents, "<button onclick=\"location.href='/advancedSetting.cgi'\">設定画面</button>");
	net_strcat(contents, "<div><noscript><h2>このサイトではJavaScriptを使用しています。JavaScriptを有効にしてください。</h2></noscript></div>");

	tempString(LTemp, nowTempStr); //現在温度を文字列にする
	net_strcat(contents, "<div><h3>温度:");
	net_strcat(contents, nowTempStr);			  //温度
	net_strcat(contents, "℃</h3><h3>取得時間："); //“℃<br>取得時間：”
	itoa_std(nTime, ChnTime);
	net_strcat(contents, hor); //取得時間を文字数に変換
	net_strcat(contents, ":");
	net_strcat(contents, min);
	net_strcat(contents, ":");
	net_strcat(contents, sec);

	net_strcat(contents, "</h3></div><div><h3>天気予報</h3>");

	net_strcat(contents, "<span id='areaPhoto'></span>");
	net_strcat(contents, "<img id = \"image01\" src=\"\" width=\"500\" height=\"500\"/></div>");

	//気温のグラフ
	net_strcat(contents, "<div style=\"width: 100%;height: 700px;\"><div class=\"chart-container\" style=\"position: relative; width: 100%; height: 100%;\"><canvas id = \"myLineChart\"></canvas></div></div>");
	net_strcat(contents, "<script>let temp_array=[");
	sizeofTest = sizeof(oneTemp) / sizeof(oneTemp)[0]; //温度の配列の個数を格納
	for (i = 0; i < sizeofTest; i++)

	{ //温度データある分だけ
		if (oneTemp[i] <= -310)
		{								   //エラー（null）を送る
			net_strcat(contents, "null,"); //温度の変数(型：Temperature[])
		}
		else
		{
			tempString(oneTemp[i], nowTempStr); //現在温度を文字列にする
			net_strcat(contents, nowTempStr);
			net_strcat(contents, ",");
		}
	}
	if (oneTemp[1439] <= -310)
	{
		net_strcat(contents, "null"); //エラー（null）を送る
	}
	else
	{
		tempString(oneTemp[i], nowTempStr); //現在温度を文字列にする
		net_strcat(contents, nowTempStr);	//温度
	}
	net_strcat(contents, "];</script>");

	itoa_std(fan_volume, Fan_v);
	itoa_std(mode, modeString);
	net_strcat(contents, "<div><form action='/setting.cgi' method='get'><div><h2>扇風機：設定</h2>");
	net_strcat(contents, "<h3>風量:"); //“<script グラフのURL(index.js)></>”
	net_strcat(contents, Fan_v);	   //“<br>扇風機の設定<br>風量：”
	net_strcat(contents, "</h3><p>変更:<input type=\"number\" name=\"fanPower\" id=\"\" value=\"");
	net_strcat(contents, Fan_v);
	net_strcat(contents, "\" max=\"9\" min=\"1\"></p></div>"); //“<input(type = number)><br>モード番号：”
	net_strcat(contents, "<div><h3>モード選択:");
	net_strcat(contents, modeString);																																																																		 //モード番号
	net_strcat(contents, "</h3>");																																																																			 //モード番号																																															   //モード番号
	net_strcat(contents, "<label><input type=\"radio\" name=\"fanmode\" value=\"1\" checked=\"checked\">1:常にoff</label><br><label><input type=\"radio\" name=\"fanmode\" value=\"2\">2:温度が○○度以上だとon</label><br><label><input type=\"radio\" name=\"fanmode\" value=\"3\">3:常にon</label></div>"); //“<input(type = number)><br>1:常にOFF<br>2:温度が○○以上だと\ONになる<br>3:常にON<br>”
	net_strcat(contents, "<div><input type='submit' value='設定反映'></div></form></div>");
	net_strcat(contents, "<script src=\"/index.js\"></script></body></html>");

	HttpSendText(http, contents, net_strlen(contents)); //文字を送る(HttpSendText())
	ret_contents_buf(contents);
	return E_OK; //エラーコードを返す
}

void settingFunc(T_HTTP_SERVER *http)
{
	ER ercd;						//エラーコード(型：ER)
	int cnt;						//for文用のカウンターの変数（型：int）
	int quan;						//クエリパラメータの要素の個数の変数(型：int)
	char *name[16] = {0};			//クエリパラメータの名前の変数（型:char*[2])
	char *value[16] = {0};			//クエリパラメータの要素に代入された値の変数（型:char*[2])
	int error = 0;					//エラーフラグ用の変数(型：int　初期値：０)0のとき正常、１のときエラー
	char *contents;					//連結先文字列用の変数(型：char *)
	char settingMessage[256] = {0}; //設定完了のメッセージ

	Temperature thr;		 //風量、モード、閾値用の変数(型：int)
	char thrString[5] = {0}; //閾値を文字列に変換した変数

	FanMode fanMode_local;	 //扇風機のモード（ローカル変数）
	FanPower fanPower_local; //扇風機の風量(ローカル変数)

	// IPアドレスの8ビットごと
	char ip_1[4] = {0}; // 例 : 192
	char ip_2[4] = {0}; // 例 : 168
	char ip_3[4] = {0}; // 例 : 4
	char ip_4[4] = {0}; // 例 : 217
	char ipAddress[16] = {0};
	bool ip1_flag = false;
	bool ip2_flag = false;
	bool ip3_flag = false;
	bool ip4_flag = false;

	T_NET_ADR adr;
	UH devid;
	devid = 1; /* デバイスID */

	if (net_strcmp(http->hdr.method, "GET") == 0)
	{ /* GET request */
		if (http->hdr.url_q)
		{
			CgiGetParam(http->hdr.url_q, net_strlen(http->hdr.url_q), name, value, &quan);
		}
	}
	else
	{
		error = 1; //エラーフラグを建てる（１）
	}

	wai_sem(ID_DATA_SEM); //セマフォを取得する（SEM_DATA）

	for (cnt = 0; cnt < quan; cnt++)
	{													 //クエリパラメータの要素の数だけループ
		if (net_strcmp(name[cnt], "thresholdTemp") == 0) //クエリパラメータが” thresholdTemp”だったら
		{
			thr = atof(value[cnt]) * 10;
			if ((thr < -30) || (thr > 1000))
			{			   //値が-30度から100度の範囲ではなかったら
				error = 1; //エラーフラグを建てる（１）
			}
			else
			{
				fanBorder = thr; //閾値のデータストアに書き換える

				tempString(thr, thrString); //閾値を文字列にする
				net_strcat(settingMessage, "<p>閾値を");
				net_strcat(settingMessage, thrString);
				net_strcat(settingMessage, "度に設定しました</p>");
			}
		}

		if (net_strcmp(name[cnt], "fanPower") == 0) //クエリパラメータが"fanPower"だったら
		{
			fanPower_local = atoi(value[cnt]);				  //数字を数値に変換
			if ((fanPower_local < 1) || (fanPower_local > 9)) //値が1から9の範囲ではなかったら
			{
				error = 1; //エラーフラグを建てる（１）
			}
			else
			{
				fanPower = fanPower_local; //風量のデータストアに書き換える

				net_strcat(settingMessage, "<p>風量を");
				net_strcat(settingMessage, value[cnt]);
				net_strcat(settingMessage, "に設定しました</p>");
				segPut(fanPower);
			}
		}

		if (net_strcmp(name[cnt], "fanmode") == 0) //クエリパラメータが"fanmode"だったら
		{
			fanMode_local = atoi(value[cnt]); //数字を数値に変換、モードのデータストアに書き換える
			switch (fanMode_local)
			{
			case 1:
				fanMode = fanMode_local;
				motorPut(0); //モータを停止
				net_strcat(settingMessage, "<p>扇風機のモードを1(常にOFF)に設定しました</p>");
				break;
			case 2:
				fanMode = fanMode_local;
				net_strcat(settingMessage, "<p>扇風機のモードを2(温度が○○度以上だとON)に設定しました</p>");
				break;
			case 3:
				fanMode = fanMode_local;
				net_strcat(settingMessage, "<p>扇風機のモードを3(常にON)に設定しました</p>");
				motorPut(fanPower);
				break;
			default:
				error = 1; //クエリパラメータの値が範囲外なら
				break;
			}
		}

		if (net_strcmp(name[cnt], "IPadress") == 0)
		{
			adr.ipaddr = ip_aton(value[cnt]);		/* IPアドレス         */
			adr.mask = ip_aton("255.255.255.0");	/* IPサブネットマスク */
			adr.gateway = ip_aton("192.168.4.254"); /* IPゲートウェイ     */

			net_cfg(devid, NET_IP4_CFG, (VP)&adr);
		}

		if (net_strcmp(name[cnt], "IP_1") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// クエリパラメータの値をコピー
				strcpy(ip_1, value[cnt]);
				ip1_flag = true;
			}
			else
			{
				error = 1; //クエリパラメータの値が異常なら
			}
		}
		if (net_strcmp(name[cnt], "IP_2") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// クエリパラメータの値をコピー
				strcpy(ip_2, value[cnt]);
				ip2_flag = true;
			}
			else
			{
				error = 1; //クエリパラメータの値が異常なら
			}
		}
		if (net_strcmp(name[cnt], "IP_3") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// クエリパラメータの値をコピー
				strcpy(ip_3, value[cnt]);
				ip3_flag = true;
			}
			else
			{
				error = 1; //クエリパラメータの値が異常なら
			}
		}
		if (net_strcmp(name[cnt], "IP_4") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// クエリパラメータの値をコピー
				strcpy(ip_4, value[cnt]);
				ip4_flag = true;
			}
			else
			{
				error = 1; //クエリパラメータの値が異常なら
			}
		}
	}
	sig_sem(ID_DATA_SEM); //セマフォの解放(SEM_DATA)

	if (ip1_flag == true && ip2_flag == true && ip3_flag == true && ip4_flag == true) //IPアドレスが正しく入力されていたら
	{
		net_strcat(ipAddress, ip_1);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_2);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_3);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_4);

		adr.ipaddr = ip_aton(ipAddress);		/* IPアドレス         */
		adr.mask = ip_aton("255.255.255.0");	/* IPサブネットマスク */
		adr.gateway = ip_aton("192.168.4.254"); /* IPゲートウェイ     */

		net_strcat(settingMessage, "<p>IPアドレスを");
		net_strcat(settingMessage, ipAddress);
		net_strcat(settingMessage, "に設定しました。</p>");

		net_cfg(devid, NET_IP4_CFG, (VP)&adr);
	}

	//クエリパラメータがなかったら
	if (settingMessage[0] == '\0')
	{
		error = 1;
	}

	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{				 //正常終了しなかったら
		return ercd; //エラーコードを返す
	}

	if (error == 1)
	{																															   //エラーフラグが１だったら
		net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'><title>入力エラー</title></head><body>"); //net_strcat()を使って、“<html><body>エラーが発生しました。”
		net_strcat(contents, "<div><h2>エラーが発生しました。</h2></div></body></html>");
	}
	else
	{
		net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'><title>設定内容</title></head><body>");
		net_strcat(contents, "<div><h3>");
		net_strcat(contents, settingMessage);
		net_strcat(contents, "</h3></div>");
	}

	net_strcat(contents, "<button onclick=\"location.href='/index.cgi'\">ホームに戻る</button>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);

	if (ercd != E_OK)
	{
		HttpSendErrorResponse(http, "500 Internal Server Error\r\n");
	}
}
/*}}UNET3_CODE */