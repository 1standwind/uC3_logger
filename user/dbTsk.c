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
    int numy;//�N
    int nummo;//��
    int numd;//��
    int numh;//��
    int numm;//��
    int nums;//�b
    int temp;//���x
    int atemp;//��Βl�ϊ��O���x
     
    char numyy[5] = "";//������ɕϊ���������(�N)
    char nummomo[3] = "";//������ɕϊ���������(��)
    char numdd[3] = "";//������ɕϊ���������(��)
    char numhh[3] = "";//������ɕϊ���������(��)
    char nummm[3] = "";//������ɕϊ���������(��)
    char numss[3] = "";//������ɕϊ���������(�b)
    
    char t;//�ۑ��p�ϐ�
    
    char dbDate[11] = "";//���t�̃f�[�^������z��
    char dbTime[10] = "";//�����̃f�[�^������z��
    char dbTemp[6] = "";//���x�̃f�[�^������z��
    
    char contents[256] = "";//�A���敶����̃A�h���X
    
    
    wai_sem(ID_DATA_SEM);
    numy = nowDay->yea;//�����o�ϐ�����(�N)
    nummo = nowDay->mon;//�����o�ϐ�����(��)
    numd = nowDay->day;//�����o�ϐ�yea����(��)
    numh = nowTime->hou;//�����o�ϐ�����(��)
    numm = nowTime->min;//�����o�ϐ�����(��)
    nums = nowTime->sec;//�����o�ϐ�����(�b)
    temp = nowTemp;//�����o�ϐ�����(���x)
    sig_sem(ID_DATA_SEM);
    
    atemp = temp;//����ǂ���
    
    getYEA(numy,numyy);//�l���當����ɕϊ�(�N)
    getRTC(nummo,nummomo);//�l���當����ɕϊ�(��)
    getRTC(numd,numdd);//�l���當����ɕϊ�(��)
    getRTC(numh,numhh);//�l���當����ɕϊ�(��)
    getRTC(numm,nummm);//�l���當����ɕϊ�(��)
    getRTC(nums,numss);//�l���當����ɕϊ�(�b)
   
    strcat(dbDate,numyy);//dbDate�z��̒��ɕ����A�����Ă���
    strcat(dbDate,"-");
    strcat(dbDate,nummomo);
    strcat(dbDate,"-");
    strcat(dbDate,numdd);
    dbDate[11] = '\0';
    
    strcat(dbTime,numhh);;//dbTime�z��̒��ɕ����A�����Ă���
    strcat(dbTime,":");
    strcat(dbTime,nummm);
    strcat(dbTime,":");
    strcat(dbTime,numss);
    dbTime[8] = '\0';
    
    temp = abs(temp);//��Βl�ɕϊ�
    itoa_std(temp,dbTemp);//�l���當����ɕϊ�(���x)
    
    if(temp >= 100){
    	t = dbTemp[2];//�z���2�Ԗڂ��R�s�[
    	dbTemp[2] = '.';//�����_���i�[
    	dbTemp[3] = t;//�R�s�[�������̂�3�Ԗڂɓ����
    	dbTemp[5] = "\0";//null����
    }else if(temp < 100){//10�x�����̏ꍇ
    	t = dbTemp[1];//�z���1�Ԗڂ��R�s�[
    	dbTemp[1] = '.';//�����_���i�[
    	dbTemp[2] = t;//�R�s�[�������̂�2�Ԗڂɓ����
	dbTemp[4] = "\0";//null����
    }
    
    
    strcat(contents,"GET /a20/insert.php?user=a20h1g4&name=Fan&datetime=");//���M�f�[�^�𕶎��A������
    strcat(contents,dbDate);
    strcat(contents,"%20");
    strcat(contents,dbTime);
    strcat(contents,"&temperature=");
    if(atemp < 0 ){//��Βl�ϊ��O�̉��x���}�C�i�X�������Ƃ�
    	strcat(contents,"-");
    }
    strcat(contents,dbTemp);
    strcat(contents," HTTP/1.0\r\n\r\n");

    DB_STATUS = dbPut(contents);//���M�ł������ǂ���
    ter_tsk(ID_DBPUT_TSK);
}



/* }}UC3_TASK */