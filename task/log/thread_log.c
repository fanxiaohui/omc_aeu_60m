/*******************************************************************************
********************************************************************************
* �ļ�����:  dru_connect.c
* ��������:  �������Ӵ���
* ʹ��˵��:  
* �ļ�����:	H4
* ��д����: ��2012/10/23��
* �޸���ʷ:
* �޸�����    �޸���       �޸�����
*-------------------------------------------------------------------------------
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "thread_log.h"
#define MAX_LOG_SIZE 1024*1024*2 //2M
#define LOG_NAME "/flashDev/data/log.txt"
#define LOG_BAK "/flashDev/data/log_bak.txt"
//#define MAX_LOG_SIZE 1024*2
//#define LOG_NAME "log.txt"
//#define LOG_BAK "log_bak.txt"
#define QUEUE_LENGTH 200
#define MAX_LOG_LENGTH 256

typedef struct thread_log_queue{
	char log_buffer[QUEUE_LENGTH][MAX_LOG_LENGTH];
	int front;
	int rear;
	pthread_rwlock_t q_lock;
}log_queue_t;

typedef struct thread_log{
	
	log_queue_t lq;
	FILE *log_file;
	
}log_struct;
static log_struct log_t;
/*******************************************************************************
* ��������: queue_init
* ��    ��:��ʼ��log���� 
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/22  V1.0     H4     ��       ��
* *****************************************************************************/
int queue_init(log_queue_t *lq)
{
	int err;
	lq->front=lq->rear=0;
	err =pthread_rwlock_init(&lq->q_lock,0);
	if(err!=0){
		return err;
	}else{
		return 0;
	}
}
/*******************************************************************************
* ��������: queue_in
* ��    ��: ��log���в���һ������ 
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/22  V1.0     H4     ��       ��
* *****************************************************************************/
int queue_in(log_queue_t *lq,char *args)
{
	pthread_rwlock_wrlock(&lq->q_lock);
	if(lq->front==(lq->rear+1)%QUEUE_LENGTH){
		printf("quene is full!\r\n");
		pthread_rwlock_unlock(&lq->q_lock);
		return -1;
	}
	memcpy(lq->log_buffer[lq->rear],args,MAX_LOG_LENGTH);
	lq->rear=(lq->rear+1)%QUEUE_LENGTH;
//	printf("quene in a args!\r\n");
	pthread_rwlock_unlock(&lq->q_lock);
	return 0;
}
/*******************************************************************************
* ��������: queue_in
* ��    ��: ��log���в���һ������ 
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/22  V1.0     H4     ��       ��
* *****************************************************************************/
int queue_out(log_queue_t *lq,char *args)
{
	int index;
	pthread_rwlock_rdlock(&lq->q_lock);
	if(lq->front==lq->rear){
	//	printf("log queue is empty!\r\n");
		pthread_rwlock_unlock(&lq->q_lock);
		return -1;
	}
	index=lq->front;
	memcpy(args,lq->log_buffer[index],MAX_LOG_LENGTH);
	lq->front=(lq->front+1)%QUEUE_LENGTH;
	//printf("log buffer has args\r\n");
	pthread_rwlock_unlock(&lq->q_lock);
	return index;
}
/*******************************************************************************
* ��������: log_write
* ��    ��: ��log����д������
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/22  V1.0     H4     ��       ��
* *****************************************************************************/
int log_write(char *args)
{
	int ret;
	char bffer[MAX_LOG_LENGTH+20];
	time_t now;
	struct tm *timenow;
	time(&now);
	timenow =localtime(&now);
	memset(bffer,0,MAX_LOG_LENGTH);
	sprintf(bffer,"%d-%d-%d-%d:%d:%d@%s\r\n",timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec,args);
	ret=queue_in(&log_t.lq,bffer);
	return ret;
}
/*******************************************************************************
* ��������: log_file_thread
* ��    ��: д��͹���log�ļ�
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/23  V1.0     H4     ��       ��
* *****************************************************************************/
void * log_file_thread(void *args)
{
	int index;
	long size;
	char bffer[MAX_LOG_LENGTH];
	log_t.log_file=	fopen(LOG_NAME,"a+");
	if(log_t.log_file==0)
	{
		printf("log_thread exit!\r\n");
		return ;
	}
	printf("log thread boot\r\n");
	while(1)
	{
		usleep(100000);
		index=queue_out(&log_t.lq,bffer);
		//printf("%s\r\n",bffer);
		if(index!=-1)
		{
			fputs(bffer,log_t.log_file);
			fflush(log_t.log_file);
			fseek(log_t.log_file,0,SEEK_END);
			size=ftell(log_t.log_file);
			if(size>MAX_LOG_SIZE)
			{
				fclose(log_t.log_file);
				rename(LOG_NAME,LOG_BAK);
				log_t.log_file=	fopen(LOG_NAME,"a+");
				if(log_t.log_file==0)
				{
					printf("log_thread exit!\r\n");
					return ;
				}
			}
		}
	}	
}
/*******************************************************************************
* ��������: log_thread_init
* ��    ��: ����log�߳�
* ��    ��:
* ��������         ����                ����
* ����ֵ:
* 
* ˵   ��:
* ��   ��     �汾    ����   �޸���      DEBUG
* -----------------------------------------------------------------------------
* 2013/05/23  V1.0     H4     ��       ��
* *****************************************************************************/
void log_thread_init(void)
{
	int err;
	pthread_t pid;
	err=pthread_create(&pid,0,log_file_thread,0);
	if(err!=0)
	{
		printf("\n log thread error  \r\n");
		return;
	}	
}
/*void main(int args)
{
	int err;
	pthread_t pid;
	err=pthread_create(&pid,0,log_file_thread,0);
	if(err!=0){
		printf("\n send to client thread error  \n");
		return;
	}	
	printf("write log\r\n");
	while(1){
		sleep(1);
		log_write("test the log of");
	}
}*/