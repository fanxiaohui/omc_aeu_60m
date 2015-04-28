/********************  COPYRIGHT(C)  ***************************************
**--------------�ļ���Ϣ--------------------------------------------------------
**��   ��   ��: modem.c
**��   ��   ��: �ں�ͼ
**�� ��  �� ��: 
**���򿪷�������
**��        ��: Modem Gprs����ͨѶ����
**--------------��ʷ�汾��Ϣ----------------------------------------------------
** ������: �ں�ͼ
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**--------------��ǰ�汾�޶�----------------------------------------------------
** �޸���:
** ��  ��:
** �ա���:
** �衡��:
**------------------------------------------------------------------------------
**
*******************************************************************************/
#include "../../common/commonfun.h"
#include "../../common/druheader.h"
#include "../../protocol/approtocol.h"
#include "../../protocol/apbprotocol.h"
#include "../../protocol/apcprotocol.h"
#include "modem.h"

extern int OpenCommPort(int fd, int comport);
extern int SetCommState(int fd, int nSpeed, int nBits, char nEvent, int nStop);
extern int DbGetThisStrPara(unsigned int objectid, char *buf);
extern INT32U GetSelfIp(char *ethname);
extern void OMCDevParaDeal(void);
extern void ApPackTransimtUnPack(APPack_t *p_packbuf, ComBuf_t *p_combuf);
extern void ApPackTransimtPack(APPack_t *p_packbuf, ComBuf_t *p_combuf);

extern int g_OMCPackNo, g_DevType;
extern DevicePara_t g_DevicePara;
extern ComBuf_t g_OMCCom, g_Ir_OMCTranCom, g_OMC_IrTranCom, rs485_omc_combuf, omc_rs485_combuf;

ComBuf_t g_SmsCom, g_Sms_Net_Buf, g_Net_Sms_Buf;
SelfThread_t g_GprsComThread;

//pppd file gprs 
//�ж�PPP0�Ѿ�����:��ioctl����ȡppp0�ĵ�ַ;
//����/var/lock/***����һ����pppd�Ľ���ID�����������,���Ҳ��ųɹ��ε��绰�ߺ�,���е���ЩҪһ/�����Ӻ����ʧ

/*******************************************************************************
*�������� : int ModemUartReceiveData(ComBuf_t *p_combuf, int waittime)
*��    �� : ���յ�����ת�浽p_combuf.Buf���ݻ�������,������,��ʱʱ��waittime
*������� : ComBuf_t *p_combuf:��Ӧ�豸����;int waittime:��ʱʱ��ms
*������� : ���յ����ݳ��Ȼ�����ʶ
*******************************************************************************/
int ModemUartReceiveData(ComBuf_t *p_combuf, int waittime)
{
int res, rcsum;
fd_set readfs;
struct timeval tv;

	rcsum = 0;
_SMSRERECVFLAG:
  tv.tv_sec = waittime/1000;//SECOND
  tv.tv_usec = (waittime%1000)*1000;//USECOND
  FD_ZERO(&readfs);
  FD_SET(p_combuf->Fd, &readfs);

  res = select(p_combuf->Fd + 1, &readfs, NULL, NULL, &tv);
  if (res > 0)
  {
    rcsum = read(p_combuf->Fd, &p_combuf->Buf[p_combuf->RecvLen], (COMBUF_SIZE - p_combuf->RecvLen));
    if (rcsum > 0)
    {
		p_combuf->RecvLen = p_combuf->RecvLen + rcsum;
		waittime = 10;
		goto _SMSRERECVFLAG;//���½��յȴ�����
    }
    else if (rcsum < 0)
    {
      perror("UartReceiveData:read() error!");
      return -1;
    }
  }
  else if (res < 0)
  {
    perror("UartReceiveData:select() error!");
    return -1;
  }
  return p_combuf->RecvLen;
}

/*******************************************************************************
*�������� : int ModemUartSendData(int fd, char *sbuf, int len)
*��    �� : ����fd ��������
*������� : ����fd,sbuf,����
*������� : �������ɹ�����1,���򷵻�-1
*******************************************************************************/
int ModemUartSendData(int fd, char *sbuf, int len)
{
int sr;

  sr = write(fd, sbuf, len);
  if(sr == -1)
  {
    printf("Write sbuf error!\r\n");
    return -1;
  }
  return 1;
}

/*******************************************************************************
*�������� : int ModemAtChat(ComBuf_t *p_combuf, char *atcmd, char *rc, int timeo)
*��    �� : Modem ATָ��Ի�����
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����;char *atcmd:atָ��;char *rc:atָ���ֵ;int timeo:�ȴ���ʱʱ��ms
*������� : �������ݰ���atָ���ֵ,����1;���򷵻�-1
*******************************************************************************/
int ModemAtChat(ComBuf_t *p_combuf, char *atcmd, char *rc, int timeo)
{
int rcnum;

  DEBUGOUT("Send:%s\r\n", atcmd);
  memset(p_combuf->Buf, 0, COMBUF_SIZE);
  p_combuf->RecvLen = 0;
  usleep(1000);
  ModemUartSendData(p_combuf->Fd, atcmd, strlen(atcmd));
  ComStrWriteLog(atcmd, strlen(atcmd));//����־

  rcnum = ModemUartReceiveData(p_combuf, timeo);
  if (rcnum > 0)
  {
    DEBUGOUT("%s\r\n", p_combuf->Buf);
    p_combuf->Timer = 0;
    ComStrWriteLog(p_combuf->Buf, p_combuf->RecvLen);//����־
    if (strstr(p_combuf->Buf, rc) != 0)//���յ�rc��ָ�ַ�����Ӧ��
    {
      return  1;
    }
    else if (strstr(p_combuf->Buf, "ERROR") != 0)//���յ������Ӧ��
    {
      return  -1;
    }
  }
  usleep(1000);
  DEBUGOUT("AtChat TimeOut!!\r\n");
  return -2;
}

/*******************************************************************************
*�������� : int ModemAutoBaud(ComBuf_t *p_combuf)
*��    �� : ͨѶModem,�Զ�����Modem���ڲ�����Ϊ115200
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����
*������� : �ɹ�����bps;ʧ�ܷ���-1
*******************************************************************************/
int ModemAutoBaud(ComBuf_t *p_combuf)
{
int   i;
int   bps[2] = {115200,9600};
char  sdbuf[100];

  strcpy(sdbuf, "AT+CFUN=1\r");//Modem����
  ModemAtChat(p_combuf, sdbuf, "OK\r\n", 6000);
  sleep(10);
  for (i = 0 ; i < 2; i++)
  {
    SetCommState(p_combuf->Fd, bps[i], 8, 'N', 1);//���ô����豸
    sleep(1);
    //AT
    memset(sdbuf, 0, sizeof(sdbuf));
    strcpy(sdbuf, "AT\r");//Modem����
    if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) == 1)
    {
      DEBUGOUT("Modem bps:%d\r\n", bps[i]);
      if (bps[i] != 115200)
      {
        memset(sdbuf, 0, sizeof(sdbuf));
        strcpy(sdbuf, "AT+IPR=115200\r");
        if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) == 1)
        {
          //AT&W����
          memset(sdbuf, 0, sizeof(sdbuf));
          strcpy(sdbuf, "AT&W\r");
          if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) == 1)
          {
            DEBUGOUT("Set Modem bps:115200!\r\n");
            return  115200;
          }
          else
          {
            DEBUGOUT("AT&W Error!\r\n");
            return  -3;
          }
        }
        else
        {
          DEBUGOUT("AT+IPR=115200 Error!\r\n");
          return  -2;
        }
      }
      else
        return  bps[i];
    }
  }
  DEBUGOUT("Modem bps set Failed!\r\n");
  return  -1;
}

/*******************************************************************************
*�������� : int ModemSmsModeInit(ComBuf_t *p_combuf, char *smsctel)
*��    �� : Modem����ģʽ���ó�ʼ������
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����;char *smsctel:���ŷ������ĺ���
*������� : None
*******************************************************************************/
int ModemSmsModeInit(ComBuf_t *p_combuf, char *smsctel)
{
char  sdbuf[100];

  //AT
  memset(sdbuf, 0, sizeof(sdbuf));
  strcpy(sdbuf, "AT\r");
  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
    return -1;
    
	ModemReadCI(p_combuf);
	
  //Characters are not echoed���Թ��ܹر�
  memset(sdbuf, 0, sizeof(sdbuf));
  strcpy(sdbuf, "ATE0\r");
  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
    return -1;

  //������������
  /*
  memset(sdbuf, 0, sizeof(sdbuf));
  sprintf(sdbuf, "AT+CSCA=+86%s\r", smsctel);
  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
    return -1;
  */
  if(get_net_group() == 0x331){
	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT+CPMS=\"SM\",\"SM\",\"SM\"\r");
	  if (ModemAtChat(p_combuf, sdbuf, "+CPMS:", 3000) < 0)
		  return -1;
	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT+CNMI=1,2,0,1,0\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT^RSSIREP=0\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT^PPPCFG=\"CARD\",\"CARD\"\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

  }else{
	  //Set TEXT mode
	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT+CMGF=1\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT+CNMI=2,2,0,0,0\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT+ICF=3,4\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;

	  //AT&W����
	  memset(sdbuf, 0, sizeof(sdbuf));
	  strcpy(sdbuf, "AT&W\r");
	  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 3000) < 0)
		  return -1;
  }	

  //All messages are deleted
  memset(sdbuf, 0, sizeof(sdbuf));
  strcpy(sdbuf, "AT+CMGD=1,4\r");
  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 10000) < 0)//�ȴ�ʱ��ϳ�
    return -1;

  return 1;
}

/*******************************************************************************
*�������� : void ModemReadCI(ComBuf_t *p_combuf)
*��    �� : Modem��ȡ��ԴС��CI
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����
*������� : None
*******************************************************************************/
void ModemReadCI(ComBuf_t *p_combuf)
{
char  atcmd[100], *ptr, *ptr1;
int i, rcnum, ci;

  memset(atcmd, 0, sizeof(atcmd));
  sprintf(atcmd, "AT+CCED=0,1\r");
  ModemUartSendData(p_combuf->Fd, atcmd, strlen(atcmd));
  ComStrWriteLog(atcmd, strlen(atcmd));//����־
  sleep(3);

  rcnum = ModemUartReceiveData(p_combuf, 6000);
  if (rcnum > 0)
  {
    DEBUGOUT("%s\r\n", p_combuf->Buf);
    ComStrWriteLog(p_combuf->Buf, p_combuf->RecvLen);//����־
    ptr = strstr(p_combuf->Buf, "+CCED:");
    if (ptr != NULL)
    {
    	ptr = strchr(ptr, ',');
    	ptr = strchr((ptr+1), ',');
    	ptr = strchr((ptr+1), ',');
    	ptr1 = strchr((ptr+1), ',');
    	memset(atcmd, 0, sizeof(atcmd));
    	memcpy(atcmd, (ptr+1), (ptr1-ptr-1));
	    ci = atoi(atcmd);
	    DEBUGOUT("��ԴС��ʶ����:%d\r\n", ci);
	    DbSaveThisIntPara(SOURCECI_ID, ci);//�洢���������е�����������,�����Ĳ���
    }
  }
}

/*******************************************************************************
*�������� : int ModemInit(void)
*��    �� : ͨѶModem��ʼ�����ú���
*������� : none
*������� : �ɹ�p_combuf->Fd;ʧ�ܷ���-1
*******************************************************************************/
int ModemInit(void)
{
ComBuf_t *p_combuf;

	p_combuf = &g_SmsCom;
  p_combuf->Fd = -1;
  p_combuf->Timer = 0;
  p_combuf->Status = 0;
  p_combuf->RecvLen = 0;
  memset(p_combuf->Buf, 0, COMBUF_SIZE);
  
	//��������
  p_combuf->Fd = OpenCommPort(p_combuf->Fd, MODEMUART);
  if (p_combuf->Fd < 0)//���豸ʧ��
  	return -1;
  //�Զ����Modem���ڲ�����
  system(GPRS_DIALOFF);
  ModemAutoBaud(p_combuf);
  sleep(1);
  SetCommState(p_combuf->Fd, 115200, 8, 'N', 1);//���ô����豸

  if (ModemSmsModeInit(p_combuf, g_DevicePara.SmscTel) < 0)
	{
    printf("SMS Init Error!\r\n");
    p_combuf->Status = MODEM_ERROR;
  }
  else
  {
    printf("SMS Init OK!\r\n");
    p_combuf->Timer = 0;
    p_combuf->RecvLen = 0;
    p_combuf->Status = MODEM_SMS;
    memset(p_combuf->Buf, 0, COMBUF_SIZE);
  }

	GprsComThreadStart();
  sleep(1);
  return p_combuf->Fd;
}

/*******************************************************************************
*�������� : int GprsModeSendSms(ComBuf_t *p_combuf, char *tel, APPack_t *p_packbuf)
*��    �� : gprsģʽ�·��Ͷ���
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����;char *tel:�绰����,APPack_t *p_packbuf:����������
*������� : �ɹ�����1,���󷵻�-1
*******************************************************************************/
int GprsModeSendSms(ComBuf_t *p_combuf, char *tel, APPack_t *p_packbuf)
{
char  sdbuf[100];

  //����+++,Switch from online to offline mode
  memset(sdbuf, 0, sizeof(sdbuf));
  strcpy(sdbuf, "+++\r");
  if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 1000) == 1)
  {
    ModemSendPackSms(p_combuf, tel, p_packbuf);
    sleep(10);
    //����ATO,Switch from offline to online mode
    strcpy(sdbuf, "ATO\r");
    if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 1000) == 1)
      return  1;//�л���online modeģʽ
    else
      return  2;//δ�л��ص�online modeģʽ,offline modeģʽ
  }
  else
    return  -1;
}

/*******************************************************************************
*�������� : int ModemSendPackSms(ComBuf_t *p_combuf, char *tel, APPack_t *p_packbuf)
*��    �� : ���ŷ���
*������� : ComBuf_t *p_combuf:Modem��Ӧ�豸����;char *tel:�绰����;APPack_t *p_packbuf:����������
*������� : �ɹ�����1,���󷵻�-1
*******************************************************************************/
int ModemSendPackSms(ComBuf_t *p_combuf, char *tel, APPack_t *p_packbuf)
{
char  cmdbuf[100], sdbuf[COMBUF_SIZE];
int   sdsum, rcsum, j;
  
  if (GetSelfIp("ppp0") != 0)//gprsģʽ��
 	{
 		DEBUGOUT("�˳� ppp0 ״̬\r\n");
 		GprsDisconnet(&g_OMCCom);
//  	//����+++,Switch from online to offline mode
//  	memset(sdbuf, 0, sizeof(sdbuf));
//  	strcpy(sdbuf, "+++\r");
//  	if (ModemAtChat(p_combuf, sdbuf, "OK\r\n", 1000) < 0)
//  	{
//  		DEBUGOUT("GprsMode Send +++ Error!\r\n");
//			return -1;
//		}
 	}
  sdsum = 0;
  memset(sdbuf, 0, sizeof(sdbuf));
  if (p_packbuf->APType == AP_B)
  {
  	sdsum = APBPack(p_packbuf, sdbuf);
  }
  else
  {
    DEBUGOUT("APType Error!\r\n");
    ClearAPPackBuf(p_packbuf);//�����
    return  -1;
  }
  if (sdsum > 0)
  {
	  if(get_net_group() != 0x331){
		  for (j = 0; j < 50; j++)
		  {
			  sprintf(cmdbuf, "AT+CPAS\r", tel);
			  if (ModemAtChat(p_combuf, cmdbuf, "0", 1000) == 1)//���յ���ȷ��Ӧ��,�ȴ�
				  break;
			  sleep(1);
		  }
	  }
	  if(get_net_group() == 0x331){ // ����
		  sprintf(cmdbuf, "AT^HCMGS=\"%s\"\r", tel);
	  }else{
		  sprintf(cmdbuf, "AT+CMGS=\"%s\"\r", tel);
	  }
    if (ModemAtChat(p_combuf, cmdbuf, ">", 1000) == 1)//���յ���ȷ��Ӧ��,�ȴ�
    {
      sdbuf[sdsum] = 0x1A; //���Ͷ��ű�ʶ
      sdsum++;
      ComDataWriteLog(sdbuf, sdsum);//����־
      DEBUGOUT("Send:%s", sdbuf);
      //ͨ��������modem��������
      ClearComBuf(p_combuf);
      ModemUartSendData(p_combuf->Fd, sdbuf, sdsum);
      sleep(4);
	  	for (j = 0; j < 5; j++)
	  	{
	  		rcsum = ModemUartReceiveData(p_combuf, 5000);
  			if (rcsum > 0)//�ж��Ƿ��з��Ͷ��ź����ȷӦ��
  			{
				DEBUGOUT("Modem Send Result:%s\r\n", p_combuf->Buf);//���յ���ȷ��Ӧ��,�ȴ�
				if(get_net_group() == 0x331){
					if (strstr(p_combuf->Buf, "^HCMGSS:") != NULL)
						break;
					if (strstr(p_combuf->Buf, "^HCMGSF:") != NULL)
						return -1;
				}else{
					if (strstr(p_combuf->Buf, "+CMGS:") != NULL)
					{
						if (strstr(p_combuf->Buf, "OK") != NULL)
							break;
					}
				}
  			}
  			sdbuf[0] = 0x1A; //���Ͷ��Ž�����ʶ
  			ModemUartSendData(p_combuf->Fd, sdbuf, 1);
	  		sleep(1);
	  	}
      return 1;
    }
  }
  DEBUGOUT("Send Data Error!\r\n");
  return -1;
}

void send_cnma(ComBuf_t * p_combuf)
{
	ComBuf_t st_tmp;
	memcpy(&st_tmp, p_combuf, sizeof(ComBuf_t));
	ModemAtChat(&st_tmp, "AT+CNMA\r", "OK\r\n", 3000);
}
/*******************************************************************************
*�������� : int ModemSmsReceiveData(char *tel)
*��    �� : ����Ԫͨ������è��ʽ��OMCͨѶ
*������� : ���ݽ��ܶ��ŷ��ض��ŵ绰
*������� : �ж��ŷ��ض�������,���򷵻�0
*******************************************************************************/ 
int ModemSmsReceiveData(ComBuf_t *p_combuf, char *tel)
{
char *ptr, *ptr1;
int i, rcsum;
DevicePara_t *p_devpara;



  p_devpara = &g_DevicePara;

  //���ؼ�ؽ���
  //p_combuf->Status = MODEM_SMS;
  rcsum = ModemUartReceiveData(p_combuf, 5000);
  if (rcsum > 0)//�ж��Ƿ��ж��Ž���
	{
	  //+CMT: "+8615110003614",,"14/04/15,14:36:01+32"
    //����,AT+CMGS="13391601848"
    //��ȡ�绰����,���е绰�����Ȩ
    DEBUGOUT("Modem Sms:%s\r\n", p_combuf->Buf);//������Ȩ���ź������
    if(strstr(p_combuf->Buf, "+CMTI:") != NULL)
    {
		if(get_net_group() == 0x331){
			send_cnma(p_combuf);
			ModemAtChat(p_combuf, "AT+CNMI=1,2,0,1,0\r", "OK\r\n", 3000);
			DEBUGOUT("Modem Set Error!\r\n");//���ô���,��������
			ClearComBuf(p_combuf);
		}else{
			ModemAtChat(p_combuf, "AT+CNMI=2,2,0,0,0\r", "OK\r\n", 3000);
			DEBUGOUT("Modem Set Error!\r\n");//���ô���,��������
			ClearComBuf(p_combuf);
		}
		  return 0;
  	}

	if(get_net_group() == 0x331){
		ptr = strstr(p_combuf->Buf, "^HCMT:");
		send_cnma(p_combuf);
	}else{
		ptr = strstr(p_combuf->Buf, "+CMT:");
	}
    if (ptr != NULL)
    {
		if(get_net_group() == 0x331){
			ptr = strchr(ptr, ':');
			ptr1 = strchr((ptr+1), ',');
		}else{
			ptr = strchr(ptr, '\"');
			ptr1 = strchr((ptr+1), '\"');
		}
		memcpy(tel, (ptr+1), (ptr1-ptr-1));
		DEBUGOUT("TEL_NUM: %s\n", tel);	
		//�绰�����Ȩ
	    //if (ptr == NULL)
	    {
			ptr = NULL;
		    for (i = 0; i < 5; i++)
			{
				if(p_devpara->QueryTel[i][0] != 0){
					ptr = strstr(tel, p_devpara->QueryTel[i]);
					if (ptr != NULL)
						return rcsum;
				}
		    }
		  }
		  //��Ч�绰����
		  if (ptr == NULL)
		  {
				DEBUGOUT("Modem Sms Tel Error!\r\n");//������Ȩ���ź������
				ClearComBuf(p_combuf);
		  	return 0;
		  }
    }
    else
    {
    	ClearComBuf(p_combuf);
    	return 0;
    }
	}
	return rcsum;
}

/*******************************************************************************
*�������� : void GprsDialOff(void)
*��    �� : Modem GprsͨѶ���ź���
*������� : ����������
*������� : �ɹ�����1
*******************************************************************************/
void GprsDialOff(void)
{
  printf("Gprs Dial Off ...\n");
	system(GPRS_DIALOFF);
	sleep(1);
}

/*******************************************************************************
*�������� : int GprsDial(void)
*��    �� : Modem GprsͨѶ���ź���
*������� : ����������
*������� : �ɹ�����1
*******************************************************************************/
int GprsDial(void)
{
int   setflag;
char  *pbuf, buf[100], var[100], syscmd[150];
FILE  *fp = NULL, *tmfp = NULL;

	GprsDialOff();
	printf("Start Gprs Dial ...\n");
  //�ж���ز����ļ��Ƿ����
  sprintf(syscmd, "%s", GPRS_PPPSCRIPT);
  if (access(syscmd, F_OK) != 0)
  {
    perror(GPRS_PPPSCRIPT);
    return -1;
  }
  usleep(1000);
  sprintf(syscmd, "%s", GPRS_CONNECT);
  if (access(syscmd, F_OK) != 0)
  {
    perror(GPRS_CONNECT);
    return -1;
  }
  usleep(1000);
  sprintf(syscmd, "%s", GPRS_DISCONNECT);
  if (access(syscmd, F_OK) != 0)
  {
    perror("gprs-disconnect");
    return -1;
  }
  usleep(1000);
	//��������,�޸�gprs-connect�ļ��е�APN����
  fp = fopen(GPRS_CONNECT, "r");
  if (fp == NULL) return -1;
  sprintf(syscmd, "%s", GPRS_TMPFILE);
  tmfp = fopen(syscmd, "w");
  if (tmfp == NULL)
  {
    fclose(fp);
    return -1;
  }

  setflag = 0;
  while(!feof(fp))
  {
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), fp);//���ж�ȡ�ļ�
    pbuf = strstr(buf, "AT+CGDCONT=1");//�Ƿ����ַ���"AT+CGDCONT=1"
    if (pbuf != NULL)
    {
		  //�޸�gprs-connect�ļ���ӦAPN����
		  memset(var, 0, sizeof(var));
		  DbGetThisStrPara(GPRSAPN_ID, var);
      if (strstr(buf, var) == NULL)//�ļ���δ����
      {
      	setflag = 1;
      }
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "OK   'AT+CGDCONT=1,\"IP\",\"%s\"\' \\\n", var);
    }
    fwrite(buf, 1, strlen(buf), tmfp);//д����ʱ�ļ���
  }
  fclose(fp);
  fclose(tmfp);
  if (setflag == 1)
  {
  	sprintf(syscmd, "cp %s %s", GPRS_TMPFILE, GPRS_CONNECT);//�����ļ��滻���ļ�
  	system(syscmd);
  }
  sprintf(syscmd, "rm %s", GPRS_TMPFILE);
  system(syscmd);//ɾ����ʱ�ļ�
  
  //��������,�޸�gprs-gsm�ļ��е��û������������
	fp = fopen(GPRS_PPPSCRIPT, "r");
  if (fp == NULL) return -1;
  sprintf(syscmd, "%s", GPRS_TMPFILE);
  tmfp = fopen(syscmd, "w");
  if (tmfp == NULL)
  {
    fclose(fp);
    return -1;
  }
  setflag = 0;
  while(!feof(fp))
  {
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), fp);//���ж�ȡ�ļ�
    //user����
    if (str1nstr2(buf, "user", sizeof("user")) == 0)
    {
		  memset(var, 0, sizeof(var));
		  DbGetThisStrPara(GPRSUSER_ID, var);
      if (strstr(buf, var) == NULL)//�ļ���δ����
      {
      	setflag = 1;
      }
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "user %s \n", var);
    }
    //password����
    if (str1nstr2(buf, "password", sizeof("password")) == 0)
    {
			memset(var, 0, sizeof(var));
			DbGetThisStrPara(GPRSPASSWORD_ID, var);
	    if (strstr(buf, var) == NULL)//�ļ���δ����
	    {	
	      setflag = 1;
	    }
	    memset(buf, 0, sizeof(buf));
	    sprintf(buf, "password %s \n", var);
    }
    fwrite(buf, 1, strlen(buf), tmfp);//д����ʱ�ļ���
  }
  fclose(fp);
  fclose(tmfp);
  if (setflag == 1)
  {
  	sprintf(syscmd, "cp %s %s", GPRS_TMPFILE, GPRS_PPPSCRIPT);//�����ļ��滻���ļ�
  	system(syscmd);
  }
  sprintf(syscmd, "rm %s", GPRS_TMPFILE);
  system(syscmd);//ɾ����ʱ�ļ�
  
  sprintf(syscmd, "chmod +x -R %s", GPRS_PPPDIR);
  system(syscmd);
  sleep(3);

	system(ROUTE_OFF);
  system(GPRS_DIALON);
  return 1;
}

/*******************************************************************************
*�������� : void GprsDisconnet(ComBuf_t *pcombuf)
*��    �� : �Ͽ�Gprs���Ӻ���
*������� : ComBuf_t *pcombuf
*������� : none
*******************************************************************************/
void GprsDisconnet(ComBuf_t *pcombuf)
{
  //���й����жϿ�����
  if (pcombuf->Fd > 0)
  {
    printf("�Ͽ��������������!\r\n");
    close(pcombuf->Fd);
  }
  pcombuf->Fd = -1;
  pcombuf->Timer = 0;
  pcombuf->Status = GPRS_DISCONNET;
  pcombuf->RecvLen = 0;
	memset(pcombuf->Buf, 0, COMBUF_SIZE);
	
	if (GetSelfIp("ppp0") != 0)//�ж��Ƿ���gprs ppp����
  {
    printf("�Ͽ�Gprs����!\r\n");
    GprsDialOff();
    sleep(3);
    system(RM_PPP0_PID);	//move ppp0.pid file
    system(RM_LCK_TTY);		//move LCK..TTY file
    sleep(3);
  }
}

/*******************************************************************************
*�������� : void GprsComThreadInit(void)
*��    �� : Gprs����ͨѶ�̳߳�ʼ��
*������� : none
*������� : none
*******************************************************************************/ 
void GprsComThreadInit(void)
{
  g_GprsComThread.Tid = 0;
  g_GprsComThread.ThreadStatus = THREAD_STATUS_EXIT;
}

/*******************************************************************************
*�������� : void GprsComThreadStart(void)
*��    �� : ����Gprs����ͨѶ�߳�
*������� : none
*������� : none
*******************************************************************************/ 
void GprsComThreadStart(void)
{
  if (g_GprsComThread.ThreadStatus != THREAD_STATUS_RUNNING)
  {
    pthread_create(&g_GprsComThread.Tid, NULL, GprsCom_Thread, NULL);
    g_GprsComThread.ThreadStatus = THREAD_STATUS_RUNNING;
    printf("GprsCom_Thread ID: %lu.\n", g_GprsComThread.Tid);
  }
}

/*******************************************************************************
*�������� : void GprsComThreadStop(void)
*��    �� : ֹͣGprs����ͨѶ�߳�
*������� : none
*������� : none
*******************************************************************************/ 
void GprsComThreadStop(void)
{
  if (g_GprsComThread.ThreadStatus != THREAD_STATUS_EXIT)
  {
    GprsDisconnet(&g_OMCCom);
    pthread_cancel(g_GprsComThread.Tid);
    g_GprsComThread.Tid = 0;
    g_GprsComThread.ThreadStatus = THREAD_STATUS_EXIT;
    printf("GprsCom_Thread Cancel!\r\n");
  }
}

/*******************************************************************************
*�������� : void *GprsCom_Thread(void *pvoid)
*��    �� : Gprs����ͨѶ�߳�
*������� : none
*������� : none
*******************************************************************************/ 
void *GprsCom_Thread(void *pvoid)
{
DevicePara_t *p_devpara;
ComBuf_t *p_combuf;
APPack_t PackBuf, *p_packbuf;
DevInfo_t DevInfo, *p_devinfo;
int dialsum, resum, res;
time_t starttime, dialtime;
char smstel[20], smssettel[20];
DevInfo_t devinfo;

  pthread_detach(pthread_self());
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);//�߳�����Ϊ����ȡ��

  printf("GprsCom_Thread Run!\r\n");
  dialsum = 0;
  starttime = time(NULL)-DIALINTERVAL+3;
  memset(&devinfo, 0, sizeof(DevInfo_t));
  LoadDevicePara(&devinfo, &g_DevicePara);
  p_devpara = &g_DevicePara;
	//gprsģʽ�µĲ�������ά��
	if ((p_devpara->ComModemType == GSM_MODEM) && (p_devpara->DeviceCommType == PS_MODE))
	{
		g_SmsCom.Status = GPRS_DIAL;
		g_OMCCom.Status = NET_NULL;
	}
	memset(smstel, 0, sizeof(smstel));
	memset(smssettel, 0, sizeof(smssettel));
  
  while(1)
  {
  	memset(&devinfo, 0, sizeof(DevInfo_t));
  	LoadDevicePara(&devinfo, &g_DevicePara);
    p_devpara = &g_DevicePara;
    p_combuf = &g_SmsCom;
    p_packbuf = &PackBuf;
    p_devinfo = &DevInfo;
    dialtime = (int)(time(NULL) - starttime);

	  if ((p_devpara->ComModemType == GSM_MODEM) && (p_devpara->DeviceCommType == SMS_MODE))
    {
    	SocketClientDisconnet(&g_OMCCom);
    	g_OMCCom.Status = NET_NULL;
    	dialsum = 0;
    	starttime = time(NULL)-DIALINTERVAL+3;
    	if (GetSelfIp("ppp0") != 0)//ppp�Ѿ�����
    	{
    		GprsDialOff();
    	}
//    	if (g_Ir_OMCTranCom.RecvLen > 0)//IRЭ��ת��͸��
//      {
//        DEBUGOUT("MODEM Send SMS: Ir->OMC TransimtPack!\r\n");
//        ApPackTransimtUnPack(p_packbuf, &g_Ir_OMCTranCom);
//        //�ӻ��ϱ�����
//        if (p_packbuf->CommandFlag == COMMAND_REPORT)
//        {
//    			memcpy(smstel, p_devpara->NotifyTel, 20);
//        }
//				ModemSendPackSms(p_combuf, smstel, p_packbuf);
//				memset(smstel, 0, sizeof(smstel));
//				ClearComBuf(p_combuf);
//      }
      if (rs485_omc_combuf.RecvLen > 0)//RS485ͨѶ͸��
      {
      	DEBUGOUT("MODEM Send SMS: RS485->OMC TransimtPack!\r\n");
        ComDataHexDis(rs485_omc_combuf.Buf, rs485_omc_combuf.RecvLen);
        ApPackTransimtUnPack(p_packbuf, &rs485_omc_combuf);
        //�ӻ��ϱ�����
        if (p_packbuf->CommandFlag == COMMAND_REPORT)
        {
    			memcpy(smstel, p_devpara->NotifyTel, 20);
        }
				ModemSendPackSms(p_combuf, smstel, p_packbuf);
				memset(smstel, 0, sizeof(smstel));
				ClearComBuf(p_combuf);
      }
    }
    
    if (g_Net_Sms_Buf.RecvLen > 0)//����ͨѶģʽ�µĸ澯
    {
    	DEBUGOUT("MODEM Send SMS: Ir->OMC TransimtPack!\r\n");
        ApPackTransimtUnPack(p_packbuf, &g_Net_Sms_Buf);
     	if ((p_packbuf->APType == AP_A) || (p_packbuf->APType == AP_C))
     	{
			p_packbuf->StartFlag = '!';
     		p_packbuf->APType = AP_B;//ת��ΪAPB
			p_packbuf->EndFlag = '!';
     	}
      if (p_packbuf->CommandFlag == COMMAND_REPORT)
      {
    		memcpy(smstel, p_devpara->NotifyTel, 20);
      }
			else
			{
				memcpy(smstel, smssettel, 20);
			}
			ModemSendPackSms(p_combuf, smstel, p_packbuf);
			memset(smstel, 0, sizeof(smstel));
			ClearComBuf(p_combuf);
    }

		//����ͨѶ����
		resum = 0;
 		resum = ModemSmsReceiveData(p_combuf, smstel);
    if (resum > 0)
    {
    //if (strstr(smstel, p_devpara->NotifyTel) == NULL)
    //	{
    		memcpy(smssettel, smstel, 20);//ʹ�õ����ò�ѯ�绰����
    //	}
			res = APBUnpack(p_combuf->Buf, p_combuf->RecvLen, p_packbuf);
      DEBUGOUT("GSM MODEM Receive SMS WriteLogBook...................................\r\n");
      ComStrWriteLog(p_combuf->Buf, p_combuf->RecvLen);//����־
      if (res > 0)
      {
        GetDevInfo(p_devinfo, p_packbuf);
        if (p_devinfo->DeviceNo == p_devpara->DeviceNo)//���豸����
        {
        	if (p_devinfo->ModuleType != 0x00)//͸��,תRS485ͨѶ
        	{
          	DEBUGOUT("OMC->RS485 TransimtPack!\r\n");
          	ApPackTransimtPack(p_packbuf, &omc_rs485_combuf);
        	}
	        else
	        {
	          if (APProcess(p_packbuf, p_devpara) > 0)
	          {
	            if (p_packbuf->PackLen > 0)
	            {
	            	p_combuf->Timer = 0;
							  ModemSendPackSms(p_combuf, smstel, p_packbuf);
							  memset(smstel, 0, sizeof(smstel));
	            }
	          }
	        }
        }
        else//ͨ��IRЭ��ת��
        {
          if (g_DevType == MAIN_UNIT)
          {
            DEBUGOUT("OMC->Ir TransimtPack!\r\n");
            ApPackTransimtPack(p_packbuf, &g_OMC_IrTranCom);
          }
        }
        ClearComBuf(p_combuf);
      }
      else
      {
        DEBUGOUT("OMC_MODEM_Com APB Unpacked Error!\r\n");//��������ݰ���ʶ
        ClearComBuf(p_combuf);
      }
	  }
	  //gprsģʽ�µĲ�������ά��

    if ((p_devpara->ComModemType == GSM_MODEM) && (p_devpara->DeviceCommType == PS_MODE))
   	{
      if (GetSelfIp("ppp0") == 0)//�жϲ����Ƿ�ɹ�,���ɹ�
      {
      	g_OMCCom.Status = NET_NULL;
      	if (dialtime > DIALINTERVAL)
      	{
      		starttime = time(NULL);
         	//dialsum++;
          if (g_SmsCom.Status == GPRS_SMS)
          {
          	g_SmsCom.Status = GPRS_DIAL;
          }
          else
          {
          	dialsum++;
	          if (dialsum == CONNETFAILSUM)//������ʧ�ܡ��ϱ���һ���Ե��ϱ�,���ϱ�ʧ�ܺ�,�豸����Ҫ�Զ��ط�
	          {
	            //���ŷ���,��������ʧ��
	            memset(p_devinfo, 0, sizeof(DevInfo_t));
		          p_devinfo->StationNo = p_devpara->StationNo;
		          p_devinfo->DeviceNo = p_devpara->DeviceNo;
		          ReportParaPack(AP_B, p_devinfo, REPORT_PSLOGIN_FAIL, g_OMCPackNo++, p_packbuf);
		          ModemSendPackSms(&g_SmsCom, p_devpara->NotifyTel, p_packbuf);
		          DEBUGOUT("Gprs Dial:Fail...\r\n");
		          ClearAPPackBuf(p_packbuf);
		          g_SmsCom.Status = GPRS_SMS;
	          }
	          else if (dialsum == (CONNETFAILSUM+2))//������ʧ�ܡ��ϱ���һ���Ե��ϱ�,���ϱ�ʧ�ܺ�,�豸����Ҫ�Զ��ط�
	          {
	          	dialsum = CONNETFAILSUM;
	          	g_SmsCom.Status = GPRS_SMS;
	          }
	          else
	          {
	          	GprsDial();
	          	g_SmsCom.Status = GPRS_DIAL;
	          }
          }
        }
      }
      else if(g_SmsCom.Status == GPRS_DIAL)
      {
      	//�ж�ppp0�Ƿ�ΪĬ��·��
				system(ROUTE_ON);
				SocketClientDisconnet(&g_OMCCom);
				dialsum = CONNETFAILSUM;
				g_SmsCom.Status = GPRS_DIALOK;
			}
			else if(g_SmsCom.Status == GPRS_SMS)
			{
				g_OMCCom.Status = NET_NULL;
			  if (GetSelfIp("ppp0") != 0)//ppp�Ѿ�����
		    {
		    	GprsDialOff();
		    }
		    sleep(1);
			if(get_net_group() != 0x331){
				ModemUartSendData(p_combuf->Fd, "AT+CMGL=\"REC UNREAD\"\r", strlen("AT+CMGL=\"REC UNREAD\""));
			}
		    starttime = time(NULL);
		    g_SmsCom.Status = GPRS_DIAL;
		  }
    }
   	sleep(3);
  }
  g_GprsComThread.ThreadStatus = THREAD_STATUS_EXIT;
  g_GprsComThread.Tid = 0;
  pthread_exit(NULL);
}

/*******************************************************************************
*�������� : int str1nstr2(char *s1, char *s2, int n) 
*��    �� : ���ַ���s1��һ���ǿ��ַ���ʼ����s2��n���ַ��Ƿ����
*������� : char *s1, char *s2, int n
*������� : ��s1<s2ʱ,����ֵ<0  ��s1=s2ʱ,����ֵ=0  ��s1>s2ʱ,����ֵ>0
*******************************************************************************/ 
int str1nstr2(char *s1, char *s2, int n) 
{
  if(n==0)//nΪ�޷������α���;���nΪ0,�򷵻�0
  	return(0);
	//��һ��ѭ������:--n,����Ƚϵ�ǰn���ַ����˳�ѭ��
	//�ڶ���ѭ������:*s1,���s1ָ����ַ���ĩβ�˳�ѭ��
  //������ѭ������:*s1,���s1ָ����ַ����ǿո�
  while(--n && *s1 && *s1 == ' ') 
  {
  	s1++;//S1ָ���Լ�1,ָ����һ���ַ�
  }
  //��һ��ѭ��������--n,����Ƚϵ�ǰn���ַ����˳�ѭ��
  //�ڶ���ѭ��������*s1,���s1ָ����ַ���ĩβ�˳�ѭ��
  //�ڶ���ѭ��������*s1 == *s2,������ַ��Ƚϲ������˳�ѭ��
  while (--n && *s1 && *s1 == *s2) 
  { 
    s1++;//S1ָ���Լ�1,ָ����һ���ַ�
    s2++;//S2ָ���Լ�1,ָ����һ���ַ�
  }
  return( *s1 - *s2 );//���رȽϽ��
}