#ifndef _RS485_H_
#define _RS485_H_

/*
** �������ܣ�����RS485�߳�
** ���������arg=�������
** �����������
** ����ֵ����
** ��ע��
*/
extern int creat_rs485_task(void);
/*
** �������ܣ��ư��ʼ��
** �����������
** �����������
** ����ֵ��0=�ɹ� ����=ʧ��
** ��ע��
*/
extern int init_led(unsigned char sta);
extern int init_power(unsigned char sta);
/*
** ��������: �ư��������
** �����������
** �����������
** ����ֵ��0=�ɹ� ����=ʧ��
** ��ע��
*/
extern int led_para_pack_power(DevInfo_t * p_devinfo, int packno, APPack_t * p_packbuf);
extern int led_para_pack_led(DevInfo_t * p_devinfo, int packno, APPack_t * p_packbuf);
#endif