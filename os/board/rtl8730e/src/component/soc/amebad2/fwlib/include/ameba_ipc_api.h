/**
  ******************************************************************************
  * @file    ameba_ipc_api.h
  * @author
  * @version V1.0.0
  * @date    2018-06-11
  * @brief   This file contains all the functions prototypes for the IPC API function
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBAD2_IPC_API_H
#define _AMEBAD2_IPC_API_H

typedef struct _IPC_MSG_STRUCT_ {
	u32 msg_type;
	u32 msg;
	u32 msg_len;
	u32 rsvd;
} IPC_MSG_STRUCT, *PIPC_MSG_STRUCT;


void ipc_table_init(IPC_TypeDef *IPCx);
void ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg);
PIPC_MSG_STRUCT ipc_get_message(u32 IPC_Dir, u8 IPC_ChNum);

#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
