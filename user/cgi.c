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

//3?????????????????????????????????10??????1????????????????????????????????????????????????
//temp???285??????str??????"28.5"???????????????
void tempString(Temperature temp, char *str)
{
	char buf[5];
	itoa_std(temp / 10, str);
	itoa_std(temp % 10, buf);
	net_strcat(str, ".");
	net_strcat(str, buf);
}

// ?????????????????????????????????true?????????
bool isdigitStr(char *str)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		// ????????????????????????????????????
		if (!(isdigit(str[i])))
		{
			// false?????????
			return false;
		}
	}
	// ?????????????????????true?????????
	return true;
}

void advancedSettingFunc(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{

	char *contents;	 //??????????????????????????????(??????char *)
	Temperature thr; //??????????????????(255????????????)
	char thrString[5];
	char *IP_id; //IP????????????????????????()
	ER ercd;	 //??????????????????(??????ER)

	wai_sem(ID_DATA_SEM); //?????????????????????(SEM_DATA)
	thr = fanBorder;	  //??????????????????????????????????????????????????????
	//R_FlashDataAreaAccess(0xffff,0xffff);//IP???????????????????????????*************************
	sig_sem(ID_DATA_SEM); //?????????????????????(SEM_DATA)

	ercd = get_contents_buf(&contents); //?????????????????????(get_comtents_buf())

	if (ercd != E_OK)
	{				 //??????????????????????????????
		return ercd; //???????????????????????????
	}

	//snprintf(thrString, sizeof(thrString), "%2.1f", thr);

	net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8' /><title>advansedSetting</title></head><body>"); //(????????????)net_strcat()??????????????????<html><body><href=???index.cgi???><input(type = button)>???(??????????????????)
	net_strcat(contents, "<button onclick=\"location.href='/index.cgi'\">???????????????</button>");
	//net_strcat(contents, "<from action=\"/Setting.cgi\" method=\"get\">");
	net_strcat(contents, "<div><noscript><h2>?????????????????????JavaScript???????????????????????????JavaScript?????????????????????????????????</h2></noscript></div>");
	net_strcat(contents, "<div><h2>???????????????????????????:<span id='areaDisp'></span></h2>");

	net_strcat(contents, "<select name=\"areaSelect\" id=\"areaSelect\">");
	net_strcat(contents, "<option value=\"HOKKAIDO\">?????????</option><option value=\"AOMORI\">??????</option>");
	net_strcat(contents, "<option value=\"IWATE\">??????</option><option value=\"MIYAGI\">??????</option>");
	net_strcat(contents, "<option value=\"AKITA\">??????</option><option value=\"YAMAGATA\">??????</option>");
	net_strcat(contents, "<option value=\"FUKUSHIMA\">??????</option><option value=\"IBARAKI\">??????</option>");
	net_strcat(contents, "<option value=\"TOCHIGI\">??????</option><option value=\"GUNMA\">??????</option>");
	net_strcat(contents, "<option value=\"SAITAMA\">??????</option><option value=\"CHIBA\">??????</option>");
	net_strcat(contents, "<option value=\"TOKYO\">??????</option><option value=\"KANAGAWA\">?????????</option>");
	net_strcat(contents, "<option value=\"NIIGATA\">??????</option><option value=\"TOYAMA\">??????</option>");
	net_strcat(contents, "<option value=\"ISHIKAWA\">??????</option><option value=\"FUKUI\">??????</option>");
	net_strcat(contents, "<option value=\"YAMANASHI\">??????</option><option value=\"AGANO\">??????</option>");
	net_strcat(contents, "<option value=\"GIFU\">??????</option><option value=\"SHIZUOKA\">??????</option>");
	net_strcat(contents, "<option value=\"AICHI\">??????</option><option value=\"MIE\">??????</option>");
	net_strcat(contents, "<option value=\"SHIGA\">??????</option><option value=\"KYOTO\">??????</option>");
	net_strcat(contents, "<option value=\"OSAKA\">??????</option><option value=\"HYOGO\">??????</option>");
	net_strcat(contents, "<option value=\"NARA\">??????</option><option value=\"WAKAYAMA\">?????????</option>");
	net_strcat(contents, "<option value=\"TOTTORI\">??????</option><option value=\"SHIMANE\">??????</option>");
	net_strcat(contents, "<option value=\"OKAYAMA\">??????</option><option value=\"HIROSHIMA\">??????</option>");
	net_strcat(contents, "<option value=\"YAMAGUCHI\">??????</option><option value=\"TOKUSHIMA\">??????</option>");
	net_strcat(contents, "<option value=\"KAGAWA\">??????</option><option value=\"EHIME\">??????</option>");
	net_strcat(contents, "<option value=\"KOUCHI\">??????</option><option value=\"FUKUOKA\">??????</option>");
	net_strcat(contents, "<option value=\"SAGA\">??????</option><option value=\"NAGASAKI\">??????</option>");
	net_strcat(contents, "<option value=\"KUMAMOTO\">??????</option><option value=\"OITA\">??????</option>");
	net_strcat(contents, "<option value=\"MIYAGI\">??????</option><option value=\"KAGOSHIMA\">?????????</option>");
	net_strcat(contents, "<option value=\"OKINAWA\">??????</option></select>");

	net_strcat(contents, "<script src=\"/advancedSetting.js\"></script>");
	net_strcat(contents, "<button onclick='areaBT()' name='setting'>????????????</button></div>");

	tempString(thr, thrString);

	net_strcat(contents, "<div><form action=\"/setting.cgi\" method=\"get\"><h2>???????????????:");
	net_strcat(contents, thrString);
	net_strcat(contents, "???</h2><p>??????:<input type=\"number\" name=\"thresholdTemp\" id=\"\" value=\"");
	net_strcat(contents, thrString);
	net_strcat(contents, "\" max=\"99\" min=\"-30\"><input type=\"submit\" value=\"????????????\"></p></form></div>");

	net_strcat(contents, "<div><form action=\"/setting.cgi\" method=\"get\"><h2>????????????????????????IP???????????????");
	net_strcat(contents, "<div><input type='number' name='IP_1' id='' value='192'>");
	net_strcat(contents, "<input type='number' name='IP_2' id='' value='168'>");
	net_strcat(contents, "<input type='number' name='IP_3' id='' value='4'>");
	net_strcat(contents, "<input type='number' name='IP_4' id='' value='222'>");
	net_strcat(contents, "<input type=\"submit\" value=\"IP??????????????????\"></form></div>");

	// net_strcat(contents, "<action=???setting.cgi??? (method = get)><input(type = submit)></form></body></html>"); //???<action=???setting.cgi??? (method = get)><input(type = submit)></form></body></html>??? (????????????)

	HttpSendText(http, contents, net_strlen(contents)); //???????????????(HttpSendText())
	ret_contents_buf(contents);							//?????????????????????(ret_contents_buf())
	return E_OK;
	//???????????????????????????
}

void indexFunc(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
	char *contents; //??????????????????????????????(??????char *)
	int t, i;
	Temperature LTemp; //????????????
	char nowTempStr[5];
	char hor[] = "00\0";
	char min[] = "00\0";
	char sec[] = "00\0";
	Time *nTime;
	char ChnTime[10];
	FanPower fan_volume; //??????
	char Fan_v[10];
	FanMode mode;			  //?????????????????????(??????int)
	char modeString[2] = {0}; //???????????????????????????
	Temperature oneTemp[1440];

	int sizeofTest = 0;

	ER ercd; //??????????????????(??????ER)

	wai_sem(ID_DATA_SEM); //?????????????????????(SEM_DATA)
	getRTC(nowTime->hou, hor);
	getRTC(nowTime->min, min);
	getRTC(nowTime->sec, sec);
	LTemp = nowTemp;						   //????????????????????????????????????????????????????????????
	fan_volume = fanPower;					   //??????????????????????????????????????????????????????
	mode = fanMode;							   //????????????????????????????????????????????????????????????
	memcpy(oneTemp, logTemp, sizeof(logTemp)); //??????????????????????????????
	sig_sem(ID_DATA_SEM);					   //?????????????????????(SEM_DATA)

	ercd = get_contents_buf(&contents); //?????????????????????(get_comtents_buf())
	if (ercd != E_OK)
	{				 //??????????????????????????????
		return ercd; //???????????????????????????
	}

	net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8' /><title>???????????????</title><script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js\"></script></head><body>"); //(????????????)net_strcat()??????????????????<html><body><href=???index.cgi???><input(type = button)>???(??????????????????)
	net_strcat(contents, "<button onclick=\"location.href='/advancedSetting.cgi'\">????????????</button>");
	net_strcat(contents, "<div><noscript><h2>?????????????????????JavaScript???????????????????????????JavaScript?????????????????????????????????</h2></noscript></div>");

	tempString(LTemp, nowTempStr); //?????????????????????????????????
	net_strcat(contents, "<div><h3>??????:");
	net_strcat(contents, nowTempStr);			  //??????
	net_strcat(contents, "???</h3><h3>???????????????"); //??????<br>??????????????????
	itoa_std(nTime, ChnTime);
	net_strcat(contents, hor); //?????????????????????????????????
	net_strcat(contents, ":");
	net_strcat(contents, min);
	net_strcat(contents, ":");
	net_strcat(contents, sec);

	net_strcat(contents, "</h3></div><div><h3>????????????</h3>");

	net_strcat(contents, "<span id='areaPhoto'></span>");
	net_strcat(contents, "<img id = \"image01\" src=\"\" width=\"500\" height=\"500\"/></div>");

	//??????????????????
	net_strcat(contents, "<div style=\"width: 100%;height: 700px;\"><div class=\"chart-container\" style=\"position: relative; width: 100%; height: 100%;\"><canvas id = \"myLineChart\"></canvas></div></div>");
	net_strcat(contents, "<script>let temp_array=[");
	sizeofTest = sizeof(oneTemp) / sizeof(oneTemp)[0]; //?????????????????????????????????
	for (i = 0; i < sizeofTest; i++)

	{ //??????????????????????????????
		if (oneTemp[i] <= -310)
		{								   //????????????null????????????
			net_strcat(contents, "null,"); //???????????????(??????Temperature[])
		}
		else
		{
			tempString(oneTemp[i], nowTempStr); //?????????????????????????????????
			net_strcat(contents, nowTempStr);
			net_strcat(contents, ",");
		}
	}
	if (oneTemp[1439] <= -310)
	{
		net_strcat(contents, "null"); //????????????null????????????
	}
	else
	{
		tempString(oneTemp[i], nowTempStr); //?????????????????????????????????
		net_strcat(contents, nowTempStr);	//??????
	}
	net_strcat(contents, "];</script>");

	itoa_std(fan_volume, Fan_v);
	itoa_std(mode, modeString);
	net_strcat(contents, "<div><form action='/setting.cgi' method='get'><div><h2>??????????????????</h2>");
	net_strcat(contents, "<h3>??????:"); //???<script ????????????URL(index.js)></>???
	net_strcat(contents, Fan_v);	   //???<br>??????????????????<br>????????????
	net_strcat(contents, "</h3><p>??????:<input type=\"number\" name=\"fanPower\" id=\"\" value=\"");
	net_strcat(contents, Fan_v);
	net_strcat(contents, "\" max=\"9\" min=\"1\"></p></div>"); //???<input(type = number)><br>?????????????????????
	net_strcat(contents, "<div><h3>???????????????:");
	net_strcat(contents, modeString);																																																																		 //???????????????
	net_strcat(contents, "</h3>");																																																																			 //???????????????																																															   //???????????????
	net_strcat(contents, "<label><input type=\"radio\" name=\"fanmode\" value=\"1\" checked=\"checked\">1:??????off</label><br><label><input type=\"radio\" name=\"fanmode\" value=\"2\">2:??????????????????????????????on</label><br><label><input type=\"radio\" name=\"fanmode\" value=\"3\">3:??????on</label></div>"); //???<input(type = number)><br>1:??????OFF<br>2:???????????????????????????\ON?????????<br>3:??????ON<br>???
	net_strcat(contents, "<div><input type='submit' value='????????????'></div></form></div>");
	net_strcat(contents, "<script src=\"/index.js\"></script></body></html>");

	HttpSendText(http, contents, net_strlen(contents)); //???????????????(HttpSendText())
	ret_contents_buf(contents);
	return E_OK; //???????????????????????????
}

void settingFunc(T_HTTP_SERVER *http)
{
	ER ercd;						//??????????????????(??????ER)
	int cnt;						//for??????????????????????????????????????????int???
	int quan;						//???????????????????????????????????????????????????(??????int)
	char *name[16] = {0};			//????????????????????????????????????????????????:char*[2])
	char *value[16] = {0};			//?????????????????????????????????????????????????????????????????????:char*[2])
	int error = 0;					//??????????????????????????????(??????int??????????????????)0???????????????????????????????????????
	char *contents;					//??????????????????????????????(??????char *)
	char settingMessage[256] = {0}; //??????????????????????????????

	Temperature thr;		 //???????????????????????????????????????(??????int)
	char thrString[5] = {0}; //???????????????????????????????????????

	FanMode fanMode_local;	 //?????????????????????????????????????????????
	FanPower fanPower_local; //??????????????????(??????????????????)

	// IP???????????????8???????????????
	char ip_1[4] = {0}; // ??? : 192
	char ip_2[4] = {0}; // ??? : 168
	char ip_3[4] = {0}; // ??? : 4
	char ip_4[4] = {0}; // ??? : 217
	char ipAddress[16] = {0};
	bool ip1_flag = false;
	bool ip2_flag = false;
	bool ip3_flag = false;
	bool ip4_flag = false;

	T_NET_ADR adr;
	UH devid;
	devid = 1; /* ????????????ID */

	if (net_strcmp(http->hdr.method, "GET") == 0)
	{ /* GET request */
		if (http->hdr.url_q)
		{
			CgiGetParam(http->hdr.url_q, net_strlen(http->hdr.url_q), name, value, &quan);
		}
	}
	else
	{
		error = 1; //???????????????????????????????????????
	}

	wai_sem(ID_DATA_SEM); //??????????????????????????????SEM_DATA???

	for (cnt = 0; cnt < quan; cnt++)
	{													 //??????????????????????????????????????????????????????
		if (net_strcmp(name[cnt], "thresholdTemp") == 0) //?????????????????????????????? thresholdTemp???????????????
		{
			thr = atof(value[cnt]) * 10;
			if ((thr < -30) || (thr > 1000))
			{			   //??????-30?????????100?????????????????????????????????
				error = 1; //???????????????????????????????????????
			}
			else
			{
				fanBorder = thr; //?????????????????????????????????????????????

				tempString(thr, thrString); //???????????????????????????
				net_strcat(settingMessage, "<p>?????????");
				net_strcat(settingMessage, thrString);
				net_strcat(settingMessage, "????????????????????????</p>");
			}
		}

		if (net_strcmp(name[cnt], "fanPower") == 0) //???????????????????????????"fanPower"????????????
		{
			fanPower_local = atoi(value[cnt]);				  //????????????????????????
			if ((fanPower_local < 1) || (fanPower_local > 9)) //??????1??????9??????????????????????????????
			{
				error = 1; //???????????????????????????????????????
			}
			else
			{
				fanPower = fanPower_local; //?????????????????????????????????????????????

				net_strcat(settingMessage, "<p>?????????");
				net_strcat(settingMessage, value[cnt]);
				net_strcat(settingMessage, "?????????????????????</p>");
				segPut(fanPower);
			}
		}

		if (net_strcmp(name[cnt], "fanmode") == 0) //???????????????????????????"fanmode"????????????
		{
			fanMode_local = atoi(value[cnt]); //???????????????????????????????????????????????????????????????????????????
			switch (fanMode_local)
			{
			case 1:
				fanMode = fanMode_local;
				motorPut(0); //??????????????????
				net_strcat(settingMessage, "<p>????????????????????????1(??????OFF)?????????????????????</p>");
				break;
			case 2:
				fanMode = fanMode_local;
				net_strcat(settingMessage, "<p>????????????????????????2(??????????????????????????????ON)?????????????????????</p>");
				break;
			case 3:
				fanMode = fanMode_local;
				net_strcat(settingMessage, "<p>????????????????????????3(??????ON)?????????????????????</p>");
				motorPut(fanPower);
				break;
			default:
				error = 1; //????????????????????????????????????????????????
				break;
			}
		}

		if (net_strcmp(name[cnt], "IPadress") == 0)
		{
			adr.ipaddr = ip_aton(value[cnt]);		/* IP????????????         */
			adr.mask = ip_aton("255.255.255.0");	/* IP???????????????????????? */
			adr.gateway = ip_aton("192.168.4.254"); /* IP??????????????????     */

			net_cfg(devid, NET_IP4_CFG, (VP)&adr);
		}

		if (net_strcmp(name[cnt], "IP_1") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// ??????????????????????????????????????????
				strcpy(ip_1, value[cnt]);
				ip1_flag = true;
			}
			else
			{
				error = 1; //?????????????????????????????????????????????
			}
		}
		if (net_strcmp(name[cnt], "IP_2") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// ??????????????????????????????????????????
				strcpy(ip_2, value[cnt]);
				ip2_flag = true;
			}
			else
			{
				error = 1; //?????????????????????????????????????????????
			}
		}
		if (net_strcmp(name[cnt], "IP_3") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// ??????????????????????????????????????????
				strcpy(ip_3, value[cnt]);
				ip3_flag = true;
			}
			else
			{
				error = 1; //?????????????????????????????????????????????
			}
		}
		if (net_strcmp(name[cnt], "IP_4") == 0)
		{
			if ((isdigitStr(value[cnt])) && (atoi(value[cnt]) >= 0) && (atoi(value[cnt]) <= 255))
			{
				// ??????????????????????????????????????????
				strcpy(ip_4, value[cnt]);
				ip4_flag = true;
			}
			else
			{
				error = 1; //?????????????????????????????????????????????
			}
		}
	}
	sig_sem(ID_DATA_SEM); //?????????????????????(SEM_DATA)

	if (ip1_flag == true && ip2_flag == true && ip3_flag == true && ip4_flag == true) //IP????????????????????????????????????????????????
	{
		net_strcat(ipAddress, ip_1);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_2);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_3);
		net_strcat(ipAddress, ".");
		net_strcat(ipAddress, ip_4);

		adr.ipaddr = ip_aton(ipAddress);		/* IP????????????         */
		adr.mask = ip_aton("255.255.255.0");	/* IP???????????????????????? */
		adr.gateway = ip_aton("192.168.4.254"); /* IP??????????????????     */

		net_strcat(settingMessage, "<p>IP???????????????");
		net_strcat(settingMessage, ipAddress);
		net_strcat(settingMessage, "????????????????????????</p>");

		net_cfg(devid, NET_IP4_CFG, (VP)&adr);
	}

	//??????????????????????????????????????????
	if (settingMessage[0] == '\0')
	{
		error = 1;
	}

	ercd = get_contents_buf(&contents);
	if (ercd != E_OK)
	{				 //??????????????????????????????
		return ercd; //???????????????????????????
	}

	if (error == 1)
	{																															   //????????????????????????????????????
		net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'><title>???????????????</title></head><body>"); //net_strcat()??????????????????<html><body>????????????????????????????????????
		net_strcat(contents, "<div><h2>?????????????????????????????????</h2></div></body></html>");
	}
	else
	{
		net_strcat(contents, "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'><title>????????????</title></head><body>");
		net_strcat(contents, "<div><h3>");
		net_strcat(contents, settingMessage);
		net_strcat(contents, "</h3></div>");
	}

	net_strcat(contents, "<button onclick=\"location.href='/index.cgi'\">??????????????????</button>");

	HttpSendText(http, contents, net_strlen(contents));
	ret_contents_buf(contents);

	if (ercd != E_OK)
	{
		HttpSendErrorResponse(http, "500 Internal Server Error\r\n");
	}
}
/*}}UNET3_CODE */