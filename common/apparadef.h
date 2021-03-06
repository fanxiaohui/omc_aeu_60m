/********************  COPYRIGHT(C) ***************************************
**--------------文件信息--------------------------------------------------------
**文   件   名: apparadef.h
**创   建   人: 于宏图
**创 建  日 期: 
**程序开发环境:
**描        述: APx协议使用参数宏定义头文件
**--------------历史版本信息----------------------------------------------------
** 创建人: 于宏图
** 版  本: v1.0
** 日　期: 
** 描　述: 原始版本
**--------------当前版本修订----------------------------------------------------
** 修改人:
** 版  本:
** 日　期:
** 描　述:
**------------------------------------------------------------------------------
**
*******************************************************************************/
#ifndef _APPARADEF_H
#define _APPARADEF_H

#include "../common/druheader.h"

// 接入层(AP:Access Protocol)协议类型 
#define AP_A    0x01
#define AP_B    0x02
#define AP_C    0x03

//访问层(VP:Visiting Protocol)承载协议类型 
#define VP_A    0x01

// VP层交互标志 (VP interact flag)
#define VP_INTERACT_NORMAL    0x00  //通信请求的执行结果,表示执行正常
#define VP_INTERACT_BUSY      0x01  //通信请求的执行结果,表示设备忙,无法处理命令请求
#define VP_INTERACT_REQUEST   0x80  //正常的命令请求

//监控控制层(MCP:Monitoring Control Protocol)
#define MCP_A    0x01//协议包由命令单元和数据单元组成
#define MCP_B    0x02
#define MCP_C    0x03

// 命令标识(command identifier)
#define COMMAND_REPORT          0x01  //设备主动上报(告警、开站、登录、心跳等)
#define COMMAND_QUERY           0x02  //查询命令
#define COMMAND_SET             0x03  //设置命令
#define COMMAND_SW_UPDATE_MOD   0x10  //转换到软件升级模式
#define COMMAND_SWVERISONSWITCH 0x11  //切换监控软件版本
#define COMMAND_NULL            0xFF  //空命令(自定义)

//应答标识(response flag)
#define RESPONSE_SUCCESS            0x00  //成功
#define RESPONSE_CONDITIOND_EXCUTE  0x01  //命令被有条件执行
#define RESPONSE_COMMANDNO_ERR      0x02  //命令编号错
#define RESPONSE_LENGTH_ERR         0x03  //长度错
#define RESPONSE_CRC_ERR            0x04  //CRC校验错
#define RESPONSE_MCPAC_ERR          0x05  //MCPC对MCP协议应答
#define RESPONSE_OTHERS_ERR         0xFE  //其它错误
#define RESPONSE_COMMAND            0xFF  //命令,表示此包为发出的命令,而非命令的应答包
#define RESPONSE_SWVERISONSWITCH_ERR   0xFE  //软件版本切换执行失败

//设备上报信息定义(Notification)
#define REPORT_NULL             0  //无上报
#define REPORT_ALARM            1  //告警上报
#define REPORT_CONFIG_CREATE    2  //开站上报
#define REPORT_CHECK            3  //巡检上报
#define REPORT_RESTORE          4  //故障修复上报
#define REPORT_CONFIG_CHANGE    5  //配置变更上报
#define REPORT_LOGIN_OMC        6  //登录到监控中心上报
#define REPORT_HEART_BEAT       7  //心跳上报
#define REPORT_SW_UPDATE        8  //设备监控软件更新上报
#define REPORT_PSLOGIN_FAIL     9  //PS域登录失败上报
#define REPORT_SAMPLEJOB_END    10 //批采结束上报

//接入层协议A,APA标识定义
#define APA_STARTFLAG         '~'   // APA协议起始标志0x7E
#define APA_ENDFLAG           '~'   // APA协议结束标志0x7E
//接入层协议B,APB标识定义
#define APB_STARTFLAG         '!'   // APB协议起始标志 
#define APB_ENDFLAG           '!'   // APB协议结束标志 
//接入层协议C,APC标识定义
#define APC_STARTFLAG         '~'   //APC协议起始标志0x7E
#define APC_ENDFLAG           '~'   //APC协议结束标志0x7E

//APA协议转义字符标志
#define APA_ESCFLAG           '^'   //APA协议转义字符标志0x5E
#define APA_ESC0X5E           0x5E  //APA协议转义字符0x5E
#define APA_ESC0X7E           0x7E  //APA协议转义字符0x7E
//APC协议转义字符标志
#define APC_ESCFLAG           '^'   //APC协议转义字符标志0x5E
#define APC_ESC0X5E           0x5E  //APC协议转义字符0x5E
#define APC_ESC0X7E           0x7E  //APC协议转义字符0x7E

#define MSG_RECEIVE_ERR       0x00  //包数据接收错误,数据超界或没有接收到结束标志 
#define MSG_RECEIVE_NOEND     0x01  //正在接收包数据,一包数据还没有传完 
#define MSG_RECEIVE_END       0x02  //完成一次格式正确的包数据接收 

#define MSG_REPORT_OK         0x01  //上报命令包正确处理
#define MSG_QUERY_OK          0x02  //查询命令包正确处理
#define MSG_SET_OK            0x03  //设置命令包正确处理
#define MSG_SW_UPDATE_MOD_OK  0x10  //转换到软件升级模式OK
#define MSG_SW_UPDATE_VER_OK  0x11  //切换监控软件版本OK

#define MSG_REPORTBACK_OK       0x81  //上报返回命令包正确处理
#define MSG_QUERYBACK_OK        0x82  //查询返回命令包正确处理
#define MSG_SETBACK_OK          0x83  //设置返回命令包正确处理
#define MSG_SW_UPDATE_QUERY_OK  0x90  //查询软件升级包OK
#define MSG_SW_UPDATE_OK        0x91  //软件升级包OK

#define MCPVAR_NOID_ERR   			0x10  //1:监控数据标识无法识别
#define MCPVAR_OVER_ERR    			0x20  //2:监控数据的设置值超出范围
#define MCPVAR_CODE_ERR      		0x30  //3:监控数据标识与监控数据的值不符合要求,非要求的ASCII码范围
#define MCPVAR_LEN_ERR  				0x40  //4:监控数据标识与监控数据长度不匹配
#define MCPVAR_UNDER_ERR            0x50  // 5: 低于下限 显示--
#define MCPVAR_UP_ERR             0x60  // 6: 高于上限 显示++
#define MCPVAR_DATA_ERR             0x70  // 7: 监控数据无法检测

//按照中国移动AP协议:通信错误检查优先级
#define MSG_AP_ERR            -1   //AP协议类型错误
#define MSG_CRC_ERR           -2   //CRC校验错误
#define MSG_VP_ERR            -3   //VP承载层错误
#define MSG_STATION_ERR       -4   //VP站点编号错误
#define MSG_DEV_ERR           -4   //VP设备编号错误
#define MSG_VP_INTERACT_ERR   -5   //VP交互标志错误
#define MSG_MCP_ERR           -6   //VP层的监控控制层协议标识
#define MSG_AUTHORIZATION_FAILURE -7   //通信安全处理(电话号码鉴权)
#define MSG_MCP_RESPONSE_ERR      -8   //MCP层命令单元中的应答标志,当不为0xFF时应丢弃该包
#define MSG_MCP_PDU_LEN_ERR       -8   //MCP层数据单元中的监控数据总长度
#define MSG_MCP_COMMAND_ERR       -9   //MCP层命令单元中的命令标识
#define MSG_MCP_DATA RANGE_ERR    -10  //MCP层数据单元中的监控数据是否在合适范围内
#define MSG_ESC_ERR    						-11  //转义有错误

#define MSG_REPORT_ERR          -31  //上报命令包正确处理错误
#define MSG_QUERY_ERR           -32  //查询命令包正确处理错误
#define MSG_SET_ERR             -33  //设置命令包正确处理错误

#define MSG_PACK_ERR            -100  //数据包错误
//通讯协议中,定义的参数
//0x000B:远程数据通信模块
#define GSM_MODEM               0
#define TD_MODEM                1  
#define TD_GSM_MODEM            2
#define WCDMA_MODEM             3
#define WCDMA_GSM_MODEM         4
#define NET8023                 5
//MCP_A:0x0010,MCP_B:0x0201:设备监控软件运行模式
#define SW_MONITOR_MODE         0   //0:监控模式
#define SW_UPDATE_MODE          1   //1:软件升级模式
#define SW_VERISONSWITCH_MODE   251 //251:软件版本切换模式

//0x0011:APC协议最大长度
#define APC_MSG_MAX_LEN         0x500  //APC协议最大长度1280Bytes

//0x0020:设备使用的远程升级方式
#define SW_UPDATE_NO          0   //0:表示设备无法进行远程升
#define SW_UPDATE_MCPB        1   //1:表示采用MCP:B方式
#define SW_UPDATE_FTP         2   //2:表示采用FTP方式
//0x0138:PS域传输协议定义
#define IP_UDP_MODE           1   //1:使用IP+UDP协议
#define IP_TCP_MODE           2   //2:使用IP+TCP协议
//0x0142:通信方式定义
#define SMS_MODE              1   //1:短信方式,
#define PS_MODE               2   //2:PS域方式

#define AP_MSG_HEAD_TAIL_LEN  17  //数据包控制协议字节数(除数据单元外所有数据,包头+CRC+包尾)
#define APA_MSG_HEAD_LEN      13  //数据包包头字节数(不包括起始标志)
#define APB_MSG_HEAD_LEN      13  //数据包包头字节数(不包括起始标志)
#define APC_MSG_HEAD_LEN      13  //数据包包头字节数(不包括起始标志)
#define AP_MSG_UNCRC_LEN      4   //数据包中不用CRC数据长度
#define AP_PDU_HEAD_LEN       5   //数据单元头长度,数据单元长度:1字节,ID:2字节

#define BROADCAST_STATION     0x00000000  //广播站点编号
#define BROADCAST_DEV         0xFF        //广播设备编号

#define APA_IDPACK_MAX_LEN    230 //APA监控列表查询数据包最大程度
#define APB_IDPACK_MAX_LEN    40 //APB监控列表查询数据包最大程度
#define APC_IDPACK_MAX_LEN    230 //APC监控列表查询数据包最大程度

#define PACK_VALUE_SIZE  APC_MSG_MAX_LEN

#define ALARMREPORTINTERVAL   180   //告警上报计时间隔180s
#define ALARMDECTINTERVAL     5     //告警查询计时间隔3s
#define ALARMSUCCESSFLAG      0x80  //告警上报成功标志

//MCP_B协议定义参数
//0203,数据块长度
#define UPDATEBLOCK_LEN       0x300  //0203,数据块长度1024Bytes
//1：表示文件传输开始，
//2：表示文件传输结束，
//3：表示监控中心取消软件升级，
//4：表示软件升级正常结束

//AP协议定义参数ID
#define IDLIST_ID             0x00000009  //设备监控列表ID
#define SWRUNVER_ID           0x0000000A  //当前监控软件版本 str型,最大长度20个字节
#define COMMODEMTYPE_ID       0x0000000B  //远程数据通信模块
#define SWRUNMODE_ID          0x00000010  //设备监控软件运行模式 uint1型 0:监控模式,1:软件升级模式,其它值为系统保留 
#define APCMAXLEN_ID          0x00000011  //AP:C协议最大长度 uint2型
#define SWUPDATERESULT_ID     0x00000018  //设备执行软件升级的结果 uint1型
#define FPGAVER_ID            0x00000021  //FPGA版本信息 str型,最大长度20个字节
#define QUERYLOG_ID           0x000000A0  //日志数据查询

#define STATIONNO_ID          0x00000101  //站点编号 uint4型,仅供设置站点编号时使用
#define DEVICENO_ID           0x00000102  //设备编号 uint1型,仅供设置设备编号时使用。单机系统:固定为255 主站:固定为0 从站:1～254
#define SMSCTEL_ID            0x00000110  //短信服务中心号码 str型,最大长度20个字节
#define QUERYTEL_ID           0x00000111  //0x0111~0x0115 查询/设置电话号码1~5 str型,最大长度20个字节
#define NOTIFYTEL_ID          0x00000120  //上报号码 str型,最大长度20个字节
#define OMCIP_ID              0x00000130  //监控中心IP地址 数字串,由4个uint1组成
#define OMCIPPORT_ID          0x00000131  //监控中心IP地址端口号 uint2型
#define GPRSAPN_ID            0x00000133  //GPRS接入点名称(即:APN)str型,最大长度20个字节
#define HEARTBEATTIME_ID      0x00000134  //设备的心跳包间隔时间 uint2型,单位为秒钟 该值为0时,表示不发心跳
#define GPRSUSER_ID           0x00000136  //GPRS参数:用户标识 str型,最大长度20个字节
#define GPRSPASSWORD_ID       0x00000137  //GPRS参数:口令 str型,最大长度20个字节
#define PSTRANPROTOCOL_ID     0x00000138  //PS域传输协议 uint1型,1:使用IP+UDP协议,2:使用IP+TCP协议
#define DEVICEIPPORT_ID       0x00000139  //设备IP地址端口号(UDP协议) uint2型
#define DEVICECOMMTYPE_ID     0x00000142  //设备通信方式 uint1型,明确设备与OMC的通信方式1:短信方式查询、设置,短信方式上报；2:GPRS方式查询、设置,GPRS方式上报；其它值为系统保留
#define REPORTTYPE_ID         0x00000141  //0x0141 上报类型 uint1型,明确上报的类型(在上报时,放在数据单元的最前边)
#define RW1_OMCIP_ID		  0x00000143  //查询设置ip地址1
#define RW2_OMCIP_ID		  0x00000144  //查询设置ip地址2
#define DEVICETIME_ID         0x00000150  //设备当前时间 数字串,由7个字节组成,用BCD码表示(年用4位,时间用24小时格式)
#define DEVICEIP_ID           0x00000151  //设备IP地址 数字串,由4个uint1组成
#define DEVNETMASK_ID         0x00000152  //设备子网掩码 数字串,由4个uint1组成
#define DEVDEFAULTGW_ID       0x00000153  //设备默认网关 数字串,由4个uint1组成
#define WORKINGCHANNELNO_ID   0x00000410  //工作信道号1 uint2
#define SOURCECI_ID           0x00000452  //信源小区识别码 uint2
#define WORKINGCHANNEL_SW_ID  0x00000860  //工作信道开关1 bit 
#define AUTO_CHANNEL_SW_ID    0x00000185  // 自动载波跟踪开关
#define GSMULATT_ID           0x08001005  //GSM上行衰减值(通道1(GSM)) uint1
#define CDMAULATT_ID          0x08003005  //GSM上行衰减值(通道1(GSM)) uint1
#define GSMDLATT_ID           0x08001006  //GSM下行衰减值(通道1(GSM)) uint1
#define CDMADLATT_ID          0x08003006  //GSM下行衰减值(通道1(GSM)) uint1
#define G3ULATT_ID            0x08004005  //3G上行衰减值(通道2(3G)) uint1
#define G3DLATT_ID            0x08004006  //3G下行衰减值(通道2(3G)) uint1
#define LTE1ULATT_ID          0x08007005  //LTE1上行衰减值(通道3(LTE1)) uint1
#define FDD1ULATT_ID          0x08005005  //LTE1上行衰减值(通道3(LTE1)) uint1
#define LTE1DLATT_ID          0x08007006  //LTE1下行衰减值(通道3(LTE1)) uint1
#define FDD1DLATT_ID          0x08005006  //LTE1下行衰减值(通道3(LTE1)) uint1
#define LTE2ULATT_ID          0x08008005  //LTE2上行衰减值(通道4(LTE2)) uint1
#define FDD2ULATT_ID          0x08006005  //LTE2上行衰减值(通道4(LTE2)) uint1
#define LTE2DLATT_ID          0x08008006  //LTE2下行衰减值(通道4(LTE2)) uint1
#define FDD2DLATT_ID          0x08006006  //LTE2下行衰减值(通道4(LTE2)) uint1
#define GSMDLPOWER_ID         0x08001001  //下行输入功率电平(通道1(GSM))sint2
#define CDMADLPOWER_ID        0x08003001  //下行输入功率电平(通道1(GSM))sint2
#define G3DLPOWER_ID          0x08004001  //下行输入功率电平(通道2(3G))sint2
#define LTE1DLPOWER_ID        0x08007001  //下行输入功率电平(通道3(LTE1))sint2
#define FDD1DLPOWER_ID        0x08005001  //下行输入功率电平(通道3(LTE1))sint2
#define LTE2DLPOWER_ID        0x08008001  //下行输入功率电平(通道4(LTE2))sint2
#define FDD2DLPOWER_ID        0x08006001  //下行输入功率电平(通道4(LTE2))sint2
#define FTPSERVERIP_ID        0x00000160  //FTP服务器IP地址 数字串,由4个uint1组成
#define FTPSERVERIPPORT_ID    0x00000161  //FTP服务器IP地址端口号 uint2型
#define SAMPLESTARTTIME_ID    0x00000601  //批采开始时间 数字串,由7个字节组成,用BCD码表示(年用4位,时间用24小时格式)
#define QUERYSAMPLEDATA_ID    0x00000606  //批采数据查询
#define RU_ACCESS_STATE_ID    0x08000080  // 远端接入状态
#define DEVROUTE_ID           0x00000873  //设备路由登记地址 数字串,由4个字节组成
#define DEVTEMP_ID            0x0000087C  // 设备温度
#define	LASER_RXPW_ID         0x080007E0  // 光收功率1
#define	LASER_TXPW_ID         0x080007E8  // 光发功率1
//告警
#define PW_DOWN_EN_ID         0x00000201  // 电源掉电告警使能
#define PW_DOWN_ALARM_ID      0x00000301  // 电源掉电告警
#define PW_ERROR_EN_ID		  0x00000202  // 电源故障
#define PW_ERROR_ALARM_ID     0x00000302
#define BAT_ERROR_EN_ID       0x00000204  // 电池故障
#define BAT_ERROR_ALARM_ID    0x00000304
#define SHIFT_ERROR_EN_ID     0x00000205  // 位置告警
#define SHIFT_ERROR_ALARM_ID  0x00000305 
#define OTHER_ERROR_EN_ID     0x00000208  // 其他模块告警
#define OTHER_ERROR_ALARM_ID  0x00000308  
#define LOUNLOCKALEN_ID       0x00000209  //本振告警使能
#define LOUNLOCKAL_ID         0x00000309  //本振告警
#define MAIN_LINE_EN_ID       0x0000020F  // 主从监控链路告警
#define MAIN_LINE_ALARM_ID    0x0000030F 
#define OUTSIDE_EN_ID         0x00000220   // 外部告警1~6
#define OUTSIDE_ALARM_ID      0x00000320  
#define TD_ASYNC_EN_ID        0x00000272   // TD-SCDMA失步告警
#define TD_ASYNC_ALARM_ID     0x00000372
#define LTE_ASYNC_EN_ID       0x00000273   // LTE失步告警
#define LTE_ASYNC_ALARM_ID    0x00000373
#define LASER_MODULE_EN_ID    0x00000030   // 光收发故障告警1~8
#define LASER_MODULE_ALARM_ID 0x0000005C
#define DEV_TEMP_EN_ID        0x00000049   // 设备过温告警 
#define DEV_TEMP_ALARM_ID     0x00000075
#define GSM_DIN_UNDER_EN_ID   0x0800100C    // GSM下行输入欠功率告警使能
#define TD_DIN_UNDER_EN_ID    0x0800400C    // TD下行输入欠功率告警使能
#define LTE1_DIN_UNDER_EN_ID  0x0800700C    // lte1下行输入欠功率告警使能
#define LTE2_DIN_UNDER_EN_ID  0x0800800C    // lte2下行输入欠功率告警使能
#define GSM_DIN_OVER_EN_ID    0x0800100D    // GSM下行输入过功率告警使能
#define TD_DIN_OVER_EN_ID     0x0800400D    // TD下行输入过功率告警使能
#define LTE1_DIN_OVER_EN_ID   0x0800700D    // lte1下行输入过功率告警使能
#define LTE2_DIN_OVER_EN_ID   0x0800800D    // lte2下行输入过功率告警使能
#define GSM_DIN_UNDER_ID      0x0800100E    // GSM下行输入欠功率告警
#define TD_DIN_UNDER_ID       0x0800400E    // TD下行输入欠功率告警
#define LTE1_DIN_UNDER_ID     0x0800700E    // lte1下行输入欠功率告警
#define LTE2_DIN_UNDER_ID     0x0800800E    // lte2下行输入欠功率告警
#define GSM_DIN_OVER_ID       0x0800100F    // GSM下行输入过功率告警
#define TD_DIN_OVER_ID        0x0800400F    // TD下行输入过功率告警
#define LTE1_DIN_OVER_ID      0x0800700F    // lte1下行输入过功率告警
#define LTE2_DIN_OVER_ID      0x0800800F    // lte2下行输入过功率告警
#define RU_DEV_TEMP_ALARM_ID  0x08000600    // 远端设备过温告警1
#define RU_DEV_TEMP_EN_ID     0x08000580    // 远端设备过温告警使能1
#define RU_DEV_TEMP_ID		  0x08000500    // 远端设备温度
#define RU_DOUT_UNDER_ALARM_ID 0x08001480   // 远端下行输出欠功率告警
#define RU_DOUT_OVER_ALARM_ID 0x08001580   // 远端下行输出过功率告警
#define RU_DOUT_UNDER_EN_ID   0x08001400   // 远端下行输出欠功率使能 
#define RU_DOUT_OVER_EN_ID    0x08001500   // 远端下行输出过功率使能
#define RU_LASER_ALARM_ID     0x08000060   // 远端光收发模块故障告警
#define RU_LASER_EN_ID        0x08000040   // 使能
#define RU_RF_ALARM_ID        0x08000380   // 远端射频链路故障告警
#define RU_RF_EN_ID           0x08000300   // 使能
#define RU_LINK_ALARM_ID      0x08000480   // 远端链路故障告警
#define RU_LINK_EN_ID         0x08000400   // 使能
#define RU_RF_SW_ID           0x08001280   // gsm1 射频开关
#define GSM_MAX_DELAY_ID      0x08001010   // gsm最大时延
#define CDMA_MAX_DELAY_ID     0x08003010   // gsm最大时延
#define TD_MAX_DELAY_ID       0x08004010   // td最大时延
#define LTE1_MAX_DELAY_ID     0x08007010   // lte1最大时延
#define FDD1_MAX_DELAY_ID     0x08005010   // lte1最大时延
#define LTE2_MAX_DELAY_ID     0x08008010   // lte2最大时延
#define FDD2_MAX_DELAY_ID     0x08006010   // lte2最大时延
#define GSM_AUTO_DELAY_SW_ID  0x08001011   // gsm 时延调整开关
#define CDMA_AUTO_DELAY_SW_ID 0x08003011   // gsm 时延调整开关
#define TD_AUTO_DELAY_SW_ID   0x08004011   // td 时延调整开关
#define LTE1_AUTO_DELAY_SW_ID 0x08007011   // lte1 时延调整开关
// 告警门限
#define DEV_TEMP_THD_ID       0x00000172    // 设备过温门限
#define GSM_DIN_UNDER_THD     0x08001007    // gsm下行输入欠功率门限
#define TD_DIN_UNDER_THD      0x08004007    // td下行输入欠功率门限
#define LTE1_DIN_UNDER_THD    0x08007007    // lte1下行输入欠功率门限
#define LTE2_DIN_UNDER_THD    0x08008007    // lte2下行输入欠功率门限
#define GSM_DIN_OVER_THD      0x08001008    // gsm下行输入过功率门限
#define TD_DIN_OVER_THD       0x08004008    // td下行输入过功率门限
#define LTE1_DIN_OVER_THD     0x08007008    // lte1下行输入过功率门限
#define LTE2_DIN_OVER_THD     0x08008008    // lte2下行输入过功率门限
#define RU_DOUT_UNDER_THD     0x08001009    // 远端下行输出欠功率门限gsm
#define RU_DOUT_OVER_THD      0x0800100A    // 远端下行输出过功率门限gsm
// 远端参数
#define RU_DOUT_ID            0x08001100    //  远端下行输出功率 gsm 1

//自定义参数查询
#define CPLDVER_ID            0x008001A0  //CPLD版本信息 str型,最大长度20个字节
#define DEVMAC_ID             0x008001A1  //设备MAC地址 str型,最大长度20个字节
#define PLLAL_ID              0x008003A6  //时钟告警
//#define OPTICALEN_ID          0x02A8  //光口1告警使能,0x02A8~0x02AF
//#define OPTICAL_ID            0x03A8  //光口1告警,0x03A8~0x03AF

#define DEVICETYPE_ID         0x06D2  //查询设备下模块
#define OPTICSTATUS_ID        0x0A00  //P1口光模块状态,光模块在不在位检测,0x0A10~0x0A17 uint1 
#define LTE1DATAPOWER_ID      0x00800A08  //LTE1 数字功率 uint2
#define LTE2DATAPOWER_ID      0x00800A09  //LTE2 数字功率 uint2
#define G3DATAPOWER_ID        0x00800A0A  //3G 数字功率 uint2
#define GSMDATAPOWER_ID       0x00800A0B  //GSM 数字功率 uint2
#define LTE1VGA_ID            0x00800B00  //LTE1 VGA uint1
#define LTE2VGA_ID            0x00800B01  //LTE2 VGA uint1
#define G3VGA_ID              0x00800B02  //3G VGA uint1
#define GSMVGA_ID             0x00800B03  //GSM VGA uint1
#define LTE1DATAGAIN_ID       0x00800B04  //LTE1 数字增益 uint2
#define LTE2DATAGAIN_ID       0x00800B05  //LTE2 数字增益 uint2
#define G3DATAGAIN_ID         0x00800B06  //3G 数字增益 uint2
#define GSMDATAGAIN_ID        0x00800B07  //GSM 数字增益 uint2
#define TESTMODELSWITCH_ID    0x00800B09  //测试模式和正常模式切换 uint1
#define INTERTESTEN_ID        0x00800B0A  //内部测试源使能 uint1
#define INTEFREQRSRC_ID       0x00800B0B  //内部源频率设置 uint2
#define AGCREFERENCE_ID       0x00800B0C  //AGC 参考值设定 uint2
#define AGCSTEP_ID            0x00800B0D  //AGC 步进设置 uint1
#define GSMULGAINOFFSET_ID    0x00800B10  //GSM上行增益偏移 sint1
#define GSMDLGAINOFFSET_ID    0x00800B11  //GSM下行增益偏移 sint1
#define G3ULGAINOFFSET_ID     0x00800B12  //3G上行增益偏移 sint1
#define G3DLGAINOFFSET_ID     0x00800B13  //3G下行增益偏移 sint1
#define LTE1ULGAINOFFSET_ID   0x00800B14  //LTE1上行增益偏移 sint1
#define LTE1DLGAINOFFSET_ID   0x00800B15  //LTE1下行增益偏移 sint1
#define LTE2ULGAINOFFSET_ID   0x00800B16  //LTE2上行增益偏移 sint1
#define LTE2DLGAINOFFSET_ID   0x00800B17  //LTE2下行增益偏移 sint1
#define GSMDLPOWEROFFSET_ID   0x00800B18  //CH1 GSM下行输入功率电平校准 sint2
#define G3DLPOWEROFFSET_ID    0x00800B19  //CH2 3G下行输入功率电平校准 sint2
#define LTE1DLPOWEROFFSET_ID  0x00800B1A  //CH3 LTE1下行输入功率电平校准 sint2
#define LTE2DLPOWEROFFSET_ID  0x00800B1B  //CH4 LTE2下行输入功率电平校准 sint2
#define CH2WORKCHANNELNO_ID   0x08004004  //工作信道号通道2(3G) uint2
#define CH3WORKCHANNELNO_ID   0x08007004  //工作信道号通道3(LTE1) uint2
#define FDD1WORKCHANNELNO_ID  0x08005004  //工作信道号通道3(LTE1) uint2
#define CH4WORKCHANNELNO_ID   0x08008004  //工作信道号通道4(LTE2) uint2
#define FDD2WORKCHANNELNO_ID  0x08006004  //工作信道号通道4(LTE2) uint2
#define LTE_ROMOTE_SW_ID	  0x00800B20  // LTE远程功率开关
#define TD_ROMOTE_SW_ID		  0x00800B21  // TD远程功率开关
#define TDS_TIMESLOT_ID		  0x00000610  // TD-SCDMA时隙配置, 第二时隙切换点
#define TDLTE_TIMESLOT_ID	  0x00000609  // TD-LTE时隙配置
#define TDS_SYN_THR_ID	      0x00800B24  // TD-SCDMA同步开启门限
#define TDLTE_SYN_THR_ID	  0x00800B25  // TD-LTE同步开启门限
#define TDLTE_SUB_ID		  0x00000608  // TD-LTE特殊子帧配置
#define BINDWIDTH_MODEL_ID    0x00800B28  //宽带选频模式切换
#define LTE_AUTO_CONFIG_ID    0x00800B36  // LTE 时隙自动配置
#define TD_FIRTRP_FINE_ID	  0x008004C5  //TD第一转换点微调 sint1
#define TD_SECTRP_FINE_ID	  0x008004C6  //TD第二转换点微调 sint1
#define LTE_FIRTRP_FINE_ID	  0x00800B37  //LTE第一转换点微调 sint1
#define LTE_SECTRP_FINE_ID	  0x00800B38  //LTE第二转换点微调 sint1
#define LTE_CHNCHS_ID		  0x00800B39  //LTE接入通道选择 sint1
#define POWER_SW1_ID		  0x008006A8  // 远供电开关1
#define POWER_SW2_ID		  0x008006A9  // 远供电开关2
#define POWER_SW3_ID		  0x008006AA  // 远供电开关3
#define POWER_SW4_ID		  0x008006AB  // 远供电开关4
#define POWER_SW5_ID		  0x008006AC  // 远供电开关5
#define POWER_SW6_ID		  0x008006AD  // 远供电开关6
#define POWER_SW7_ID		  0x008006AE  // 远供电开关7
#define POWER_SW8_ID		  0x008006AF  // 远供电开关8

#define GSM_ATTENUATION		  0x008006B1  //GSM下行衰减

#define AUTHOR_ID			  0x00800FFF  // 操作权限
#endif  //_APPARADEF_H

/*********************************End Of File*************************************/
