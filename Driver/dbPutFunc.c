#include "uc3include.h"
#include "global.h"
#include "typedef.h"
#include"net_hdr.h"
#include"net_cfg.h"


 char rcvData[128] ;
bool dbPut(char *logData)
{
   bool result = DB_ERROR;//���ʗp�ϐ�
   UH size = net_strlen(logData);//���M�f�[�^�̃f�[�^�T�C�Y
  
   ER ercd = E_OK;//�G���[�R�[�h
   
   T_NODE remote;//�O���T�[�o�ɑ��邽�߂̏��
   remote.port = 80;//�\�P�b�g�̃|�[�g�ԍ�
   remote.ver = IP_VER4;//IP_VER���w��
   remote.num = 1;//�f�o�C�X�ԍ�
   remote.ipa = ip_aton("192.168.4.9");//�����[�g���IP�A�h���X
   
   ercd = con_soc(ID_TCP_HTTP,&remote,SOC_CLI);//�O���T�[�o�[�ɔ\���ڑ�����
   ercd = snd_soc(ID_TCP_HTTP,(VP)logData,size);//���M�f�[�^�𑗐M����
   ercd = rcv_soc(ID_TCP_HTTP,(VP)rcvData,sizeof(rcvData));
   ercd = cls_soc(ID_TCP_HTTP,SOC_TCP_CLS);//�\�P�b�g��ؒf����
  
   
   if(ercd == -1){//����I�����Ȃ�������
   	result = DB_ERROR;//false
	return result;//false��Ԃ�
   }else{
   	result = DB_NORMALLY;//true
	return result;//ture��Ԃ�
   }
}
