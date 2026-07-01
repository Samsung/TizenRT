/**
  ******************************************************************************
  * @file    rtw_coex_ipc_def.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef RTW_COEX_IPC_DEF_H
#define RTW_COEX_IPC_DEF_H

#define COEX_API_BASE						0x0000
#define BT_API_BASE						0x0000

#define IPC_API_PROCESS_DONE				0

enum IPC_COEX_C2H_API_ID {
	/* 0x0000~0x1000 reserved for COEX API */
	IPC_API_ID_COEX_C2H_INFO						= COEX_API_BASE + 1,

};

enum IPC_COEX_H2C_API_ID {
	/* 0x0000~0x1000 reserved for COEX API */
	IPC_API_ID_COEX_H2C_INFO						= COEX_API_BASE + 1,

};


#endif

