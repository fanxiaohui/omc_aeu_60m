#ifndef _IR_DEF_H
#define _IR_DEF_H
#include "../common/type_def.h"
//������Ϣ���
#define CREATE_CHANNEL_REQ 1
#define CREATE_CHANNEL_CFG 2
#define CREATE_CHANNEL_CFG_RSP 3
#define UPDATE_RESULT_DIS 4
#define UPDATE_RESULT_RSP 5
#define RRU_VERSION_REQ 11
#define RRU_VERSION_RSP 12
#define VERSION_DOWNLOARD_REQ 21
#define VERSION_DOWNLOARD_RSP 22
#define VERSION_DOWNLOARD_RESULT_DIS 23
#define RRU_VERSION_ALIVE_DIS 31
#define RRU_VERSION_ALIVE_RSP 32
#define RRU_STATUS_REQ 41
#define RRU_STATUS_RSP 42
#define RRU_PARAMETER_REQ 51
#define RRU_PARAMETER_RSP 52
#define RRU_PARAMETER_CFG 61
#define RRU_PARAMETER_CFG_RSP 62
#define INIT_ADJUST_RESULT_REP 71
#define ADJUST_PARAMETER_CFG_REQ 81
#define ADJUST_PARAMETER_CFG_RSP 82
#define TIME_DELAY_MEASURE_REQ 101
#define TIME_DELAY_MEASURE_RSP 102
#define TIME_DELAY_CFG 103
#define TIME_DELAY_CFG_RSP 104
#define WARNING_ALARM_REP 111
#define WARNING_ALARM_REQ 121
#define WARNING_ALARM_RSP 122
#define UPDATE_LOG_REQ 131
#define UPDATE_LOG_RSP 132
#define UPDATE_LOG_RESULT_DIS 133
#define RESTART_DIS 141
#define REMOTE_RESTART_DIS 151
#define RRU_ALIVE_MSG 171
#define BBU_ALIVE_MSG 181
#define RRU_TO_BBU 230
#define BBU_TO_RRU 240
//ie���ͬʱ��ѯ����
#define MAX_IE_BUFFER 20
#pragma pack(1)
//ʱ�Ӳ��������ö���
typedef struct delay_control_t{
U8 enable_flag;
U8 delay_flag;
U8 up_link_port;
U16 max_delay;
U16 path_delay;
U16 next_delay[8];
}delay_control;
//ie��ַ�洢
typedef struct ie_addr_t{
	U8 *paddr;
	U16 length;
}ie_addr;
//Э���뺯�����պ���
typedef struct function_t{
U32 number;
void (* deal)(U8 *inbuf);
}function;
//��Ϣͷ�ṹ����
typedef struct msg_head_t{
U32 msg_number;
U32 msg_length;
U8 RRU_ID;
U8 BBU_ID;
U8 light_port_number;
U32 serial_number;
}msg_head;
/**************************************************/
//ͨ����������ie�ṹ����

//rru��Ʒ��ʶie
typedef struct rru_mark_t{
CU16 ie_symbol;
CU16 ie_length;
U8 made_name[16];
U8 publisher_name[16];
U8 serial_number[16];
U8 production_date[16];
U8 modify_date[16];
U8 other_info[16];
}rru_mark;
//ͨ������ԭ��ie
typedef struct create_channel_reason_t{
CU16 ie_symbol;
CU16 ie_length;
U8 reason;
U32 alarm;
}create_channel_reason;
//RRU����ie
typedef struct rru_series_t{
CU16 ie_symbol;
CU16 ie_length;
U8 series;
U64 port_number[8];
}rru_series;
//RRUӲ����Ϣie
typedef struct rru_hardware_info_t{
CU16 ie_symbol;
CU16 ie_length;
U8 device_type[32];
U8 device_version[16];
}rru_hardware_info;
//RRU������Ϣie
typedef struct rru_software_info_t{
CU16 ie_symbol;
CU16 ie_length;
U8 software_version[40];
U8 firmware_version[40];
}rru_software_info;
//RRU������Ϣ��ͨ������ʱ��bbu�洢
typedef struct rru_info_t{
rru_mark r_mark;
create_channel_reason c_reason;
rru_series r_series;
rru_hardware_info r_hardware;
rru_software_info r_software;
}rru_info;
/*******************************************************
			ͨ���������˶԰汾
*******************************************************/
/******************************************************/
//ͨ���������ð�����ie

//ϵͳʱ��ie
typedef struct sys_time_t{
CU16 ie_symbol;
CU16 ie_length;
U8 sec;
U8 min;
U8 hour;
U8 date;
U8 mon;
U16 year;
}sys_time;
//����Ӧ���ַie
typedef struct ftp_addr_t{
CU16 ie_symbol;
CU16 ie_length;
U8 ip[4];
}ftp_addr;
//RRU����ģʽie
typedef struct rru_mode_t{
CU16 ie_symbol;
CU16 ie_length;
U32 mode;
}rru_mode;
//�����汾�˶Խ��ie
typedef struct software_version_result_t{
CU16 ie_symbol;
CU16 ie_length;
U8 type;
U32 result;
U8 path[200];
U8 name[16];
U32 length;
U8 time[20];
U8 version[40];
}software_version_result;
//ir�ڹ���ģʽ����
typedef struct ir_mode_config_t{
CU16 ie_symbol;
CU16 ie_length;
U32 mode;
}ir_mode_config;
/************************************************/
//ͨ����������Ӧ�������ie
//ͨ����������Ӧ��ie
typedef struct create_channel_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 status;
}create_channel_rsp;
/*********************************************************/
//�汾���½��ָʾ������ie
//�汾���½��ie
typedef struct update_version_result_t{
CU16 ie_symbol;
CU16 ie_length;
U8 type;
U32 status;
}update_version_result;
/*******************************************************
			�汾��ѯ
*******************************************************/
////�汾��ѯӦ��ie
//rru_hardware_info
//rru_software_info
/*******************************************************
			�汾����
*******************************************************/
////�汾�������������ie
//�����汾�˶Խ��ie
//software_version_result
////�汾����Ӧ�������ie
//�汾������Ӧie
typedef struct downloard_version_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 type;
U32 status;
}downloard_version_rsp;
////�汾���ؽ��ָʾ������ie
//�汾���ش������ָʾie
typedef struct downloard_result_t{
CU16 ie_symbol;
CU16 ie_length;
U8 type;
U32 status;
}downloard_result;
/*******************************************************
			�汾����
*******************************************************/
////RRu�汾����ָʾ������ie
//RRU�����汾��Ϣie
//rru_software_info
////RRU�汾����Ӧ�������ie
//RRU�汾����Ӧ��ie
typedef struct software_activate_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 type;
U32 status;
}software_activate_rsp;
/*******************************************************
			״̬��ѯ
*******************************************************/
////RRU״̬��ѯ������ie
//��Ƶͨ��״̬ie
typedef struct fchannel_state_req_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
}fchannel_state_req;
//�ز�״̬ie
typedef struct carrier_wave_req_t{
CU16 ie_symbol;
CU16 ie_length;
}carrier_wave_req;
//����״̬ie
typedef struct local_osc_req_t{
CU16 ie_symbol;
CU16 ie_length;
}local_osc_req;
//ʱ��״̬ie
typedef struct clock_req_t{
CU16 ie_symbol;
CU16 ie_length;
}clock_req;
//RRU����״̬ie
typedef struct run_state_req_t{
CU16 ie_symbol;
CU16 ie_length;
}run_state_req;
//ir�ڹ���ģʽ��ѯie
typedef struct ir_mode_req_t{
CU16 ie_symbol;
CU16 ie_length;
}ir_mode_req;
//��ʼ��У׼���ie
typedef struct init_result_req_t{
CU16 ie_symbol;
CU16 ie_length;
}init_result_req;
//�����Ϣ��ѯie
typedef struct light_info_req_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
}light_info_req;
////RRU״̬��ѯ��Ӧ������ie
//��Ƶͨ·״̬��Ӧie
typedef struct fchannel_state_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
U32 up;
U32 down;
}fchannel_state_rsp;
//�ز�״̬��Ӧie
typedef struct carrier_wave_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
U32 status;
}carrier_wave_rsp;
//����״̬��Ӧie
typedef struct local_osc_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 hz;
U32 status;
}local_osc_rsp;
//ʱ��״̬��Ӧie
typedef struct clock_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 status;
}clock_rsp;
//RRU����״̬��Ӧie
typedef struct run_state_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 status;
}run_state_rsp;
//ir�ڹ���ģʽ��ѯ��Ӧie
typedef struct ir_mode_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 result;
}ir_mode_rsp;
//��ʼ��У׼�����Ӧie
typedef struct init_result_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 tx_channel_number;
U32 tx_status;
U32 rx_channel_number;
U32 rx_status;
}init_result_rsp;
//�����Ϣ��ѯ��Ӧie
typedef struct light_info_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
U16 rx_capy;
U16 tx_capy;
U8 is_being;
U8 company[16];
U16 speed;
S8 temp;
U16 mv;
U16 ma;
U8 length_9ums_km;
U8 length_9ums_100m;
U8 length_50umm_10m;
U8 length_625umm_10m;
}light_info_rsp;
/*******************************************************
			������ѯ
*******************************************************/
////������ѯ������ie
//ϵͳʱ���ѯie
typedef struct sys_time_req_t{
CU16 ie_symbol;
CU16 ie_length;
}sys_time_req;
//cpuռ����ie
typedef struct cpu_occ_req_t{
CU16 ie_symbol;
CU16 ie_length;
}cpu_occ_req;
//cpuռ���ʲ�ѯ����ie
typedef struct cpu_cycle_req_t{
CU16 ie_symbol;
CU16 ie_length;
}cpu_cycle_req;
//rru�¶�ie
typedef struct rru_temp_req_t{
CU16 ie_symbol;
CU16 ie_length;
U32 type;
}rru_temp_req;
//פ����״̬��ѯie
typedef struct rru_swr_req_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
}rru_swr_req;
//פ�������޲�ѯie
typedef struct swr_limit_req_t{
CU16 ie_symbol;
CU16 ie_length;

}swr_limit_req;
//�������޲�ѯie
typedef struct temp_limit_req_t{
CU16 ie_symbol;
CU16 ie_length;
}temp_limit_req;
//������ʲ�ѯie
typedef struct out_capy_req_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
}out_capy_req;
//״̬����ѯie
typedef struct state_machine_req_t{
CU16 ie_symbol;
CU16 ie_length;
}state_machine_req;
////rru������ѯ��Ӧie
//ϵͳʱ���ѯ��Ӧie
//sys_time

//cpuռ������Ӧie
typedef struct cpu_occ_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 occ;
}cpu_occ_rsp;
//cpuռ���ʲ�ѯ������Ӧie
typedef struct cpu_cycle_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 sec;
}cpu_cycle_rsp;
//rru�¶���Ӧie
typedef struct rru_temp_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 type;
U8 number;
U32 temp;
}rru_temp_rsp;
//פ����״̬��ѯ��Ӧie
typedef struct rru_swr_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
U32 value;
}rru_swr_rsp;
//פ�������޲�ѯ��Ӧie
typedef struct swr_limit_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 one_level;
U32 two_level;
}swr_limit_rsp;
//�������޲�ѯ��Ӧie
typedef struct temp_limit_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
S32 up_limit;
S32 down_limit;
}temp_limit_rsp;
//������ʲ�ѯ��Ӧie
typedef struct out_capy_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
S16 value;
}out_capy_rsp;
//״̬����ѯ��Ӧie
typedef struct state_machine_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 state;
}state_machine_rsp;
/*******************************************************
			��������
*******************************************************/
////�������ð�����ie
//ϵͳʱ��
//sys_time

//iq ����ͨ������ie
typedef struct iq_config_t{
CU16 ie_symbol;
CU16 ie_length;
U8 carrier_number;
U8 antenna_number;
U8 axc_number;
U8 light_number;
}iq_config;
//cpuռ����ͳ����������ie
typedef struct cpu_scycle_config_t{
CU16 ie_symbol;
CU16 ie_length;
U32 sec;
}cpu_scycle_config;
//פ������������ie
typedef struct swr_limit_config_t{
CU16 ie_symbol;
CU16 ie_length;
U32 one_level;
U32 two_level;
}swr_limit_config;
//ir����ģʽ����ie
//typedef struct ir_mode_config_t
//ir_mode_config

//������������ie
typedef struct temp_limit_config_t{
CU16 ie_symbol;
CU16 ie_length;
U32 type;
S32 up_limit;
S32 down_limit;
}temp_limit_config;
//��Ƶͨ��״̬ie
//fchannel_state_rsp

//rru������������ie
typedef struct connection_config_t{
CU16 ie_symbol;
CU16 ie_length;
U32 value;
}connection_config;
////����������Ӧie
//ϵͳʱ��������Ӧie
typedef struct sys_time_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 result;
}sys_time_rsp;
//iq����ͨ��������Ӧie
typedef struct iq_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 carrier_number;
U8 antenna_number;
U8 light_number;
U8 result;
}iq_config_rsp;
//cpuռ��������������Ӧie
typedef struct cpu_scycle_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 result;
}cpu_scycle_config_rsp;
//פ��������������Ӧie
typedef struct swr_limit_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 result;
}swr_limit_config_rsp;
//ir����ģʽ������Ӧie
typedef struct ir_mode_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 m_port;
U8 s_port;
U32 result;
}ir_mode_config_rsp;
//��������������Ӧie
typedef struct temp_limit_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 type;
U32 result;
}temp_limit_config_rsp;
//��Ƶͨ��״̬������Ӧie
typedef struct fchannel_state_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 number;
U32 result;
}fchannel_state_config_rsp;
//RRU��������������Ӧie
typedef struct connection_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U32 value;
}connection_config_rsp;

/*******************************************************
			ʱ�Ӳ���
*******************************************************/
////ʱ�Ӳ������������ie
//������ʱ�Ӳ�������ie
typedef struct time_delay_req_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
}time_delay_req;
////ʱ�Ӳ�����Ӧ������ie
//����ʱ�Ӳ�����Ӧie
typedef struct time_delay_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
U16 read_delay[8];
}time_delay_rsp;
////ʱ���������������ie
//ʱ����������ie
typedef struct time_delay_config_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
U32 delay12;
U32 delay34;
U32 offset;
}time_delay_config;
////ʱ����������Ӧ�������ie
//ʱ�����ý����Ӧie
typedef struct time_delay_config_rsp_t{
CU16 ie_symbol;
CU16 ie_length;
U8 port;
U8 result;
}time_delay_config_rsp;

/*******************************************************
			�澯�ϱ�
*******************************************************/
////�澯�ϱ����������ie
//�澯�ϱ�ie
typedef struct alarm_rep_t{
CU16 ie_symbol;
CU16 ie_length;
U16 status;
U32 word;
U32 sub_word;
U32 flag;
U8 time[20];
U8 other[100];
}alarm_rep;

/*******************************************************
			�澯��ѯ
*******************************************************/
////�澯��ѯ���������ie
//�澯��ѯ����ie
typedef struct alarm_req_t{
CU16 ie_symbol;
CU16 ie_length;
U32 word;
U32 sub_word;
}alarm_req;
////�澯��ѯӦ�������ie
//�澯��ѯӦ��ie
//alarm_rep



/*******************************************************
			͸��
*******************************************************/
//͸��Ŀ��ie
typedef struct transmit_aim_t{
CU16 ie_symbol;
CU16 ie_length;
U8 aim;
}transimt_aim;
//͸������ie
typedef struct transmit_content_t{
CU16 ie_symbol;
CU16 ie_length;
U8 buffer[1024];
}transmit_content;








#endif