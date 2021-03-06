/********************  COPYRIGHT(C)***************************************
**--------------文件信息--------------------------------------------------------
**文   件   名: localcom.h
**创   建   人: 于宏图
**创 建  日 期: 
**程序开发环境：
**描        述: 本地计算机通讯程序的头文件
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
#ifndef _LOCALCOM_H
#define _LOCALCOM_H

#include "../../common/druheader.h"

int LocalComInit(void);
void LocalComThreadInit(void);
void *LocalCom_Thread(void *pvoid);
void LocalComThreadStart(void);
void LocalComThreadStop(void);

#endif /* _LOCALCOM_H */
/******************* (C) COPYRIGHT 2007 ************************END OF FILE****/
