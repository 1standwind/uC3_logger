#include "uc3include.h"
#include "global.h"
#include "kernel.h"
#include "net_hdr.h"
#include "sntp_client.h"
#include "utils.h"
/*******************************
        ntpGetTask
 *******************************/
/* {{UC3_TASK(ntpGetTask) */
void ntpGetTask(VP_INT exinf)
{
	T_SNTP_CLIENT sntp = {0};
	ER ercd;
	UW sec;
	UW msec;
	sntp.sid=ID_UDP_SMPL;
	sntp.ipa=ip_aton("192.168.0.14");
	sntp.tmo = 10000;
	sntp.devnum=1;
	sntp.port=123;
	sntp.ipv=IP_VER4;
	ercd = sntp_client(&sntp,&sec,&msec);
	if(ercd == E_OK){
		wai_sem(ID_DATA_SEM);
		inYMD(sec,nowDay);
		inHMS(sec,nowTime);
		NTP_STATUS = NTP_NORMALLY;
		sig_sem(ID_DATA_SEM);
	}else{
		NTP_STATUS = NTP_ERROR;

	}
	ter_tsk(ID_NTPGET_TSK);
}
