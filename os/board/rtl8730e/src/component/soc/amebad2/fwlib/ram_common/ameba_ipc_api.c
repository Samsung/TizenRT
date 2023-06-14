/**
  ******************************************************************************
  * @file    rtl8721d_ipc_api.c
  * @author
  * @version V1.0.0
  * @date    2018-06-11
  * @brief   This file contains all the API for the IPC function
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

#include "ameba_soc.h"

#if defined ( __ICCARM__ )
#pragma section=".ipc.table.data"

SECTION(".data") u8 *__ipc_table_start__ = 0;
SECTION(".data") u8 *__ipc_table_end__ = 0;
#endif

/**
  * @brief  init ipc interrupt handler table.
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @retval   None
  */
void ipc_table_init(IPC_TypeDef *IPCx)
{
	u32 i;
	u32 IPC_Dir;
	u32 IPC_ChNum;
	u32 IPC_GroupShift;
	u32 IPC_ChShift;
	u32 IPC_IMR;
	IPC_TypeDef *IPCx_DEV;

#if defined ( __ICCARM__ )
	__ipc_table_start__ = (u8 *)__section_begin(".ipc.table.data");
	__ipc_table_end__ = (u8 *)__section_end(".ipc.table.data");
#endif
	IPC_INIT_TABLE *ipc_init_table = (IPC_INIT_TABLE *)__ipc_table_start__;
	u32 ipc_num = ((__ipc_table_end__ - __ipc_table_start__) / sizeof(IPC_INIT_TABLE));

	for (i = 0; i < ipc_num; i++) {
		IPC_Dir = ipc_init_table[i].IPC_Direction;
		IPC_ChNum = ipc_init_table[i].IPC_Channel;

		/* check ipc entry has register on a correct cpu */
		if (ipc_init_table[i].IPC_Intr_Mode == IPC_RX_FULL) {
			/* for rx intr, this ipc entry should be registered in rx cpu*/
			if ((IPC_Dir == IPC_NP_TO_LP) || (IPC_Dir == IPC_AP_TO_LP)) {
				IPCx_DEV = IPCLP_DEV;
			} else if ((IPC_Dir == IPC_LP_TO_NP) || (IPC_Dir == IPC_AP_TO_NP)) {
				IPCx_DEV = IPCNP_DEV;
			} else {
				IPCx_DEV = IPCAP_DEV;
			}
		} else {
			/* for rx intr, this ipc entry should be registered in rx cpu*/
			if ((IPC_Dir == IPC_NP_TO_LP) || (IPC_Dir == IPC_NP_TO_AP)) {
				IPCx_DEV = IPCNP_DEV;
			} else if ((IPC_Dir == IPC_LP_TO_NP) || (IPC_Dir == IPC_LP_TO_AP)) {
				IPCx_DEV = IPCLP_DEV;
			} else {
				IPCx_DEV = IPCAP_DEV;
			}
		}

		if (IPCx_DEV != IPCx) {
			continue;
		}

		/* set ipc group shift according to intr mode */
		if (ipc_init_table[i].IPC_Intr_Mode == IPC_RX_FULL) {
			if ((IPC_Dir == IPC_NP_TO_LP) || (IPC_Dir == IPC_LP_TO_NP) || (IPC_Dir == IPC_LP_TO_AP)) {
				IPC_GroupShift = 24;
			} else {
				IPC_GroupShift = 16;
			}
		} else {
			if ((IPC_Dir == IPC_NP_TO_LP) || (IPC_Dir == IPC_LP_TO_NP) || (IPC_Dir == IPC_AP_TO_LP)) {
				IPC_GroupShift = 8;
			} else {
				IPC_GroupShift = 0;
			}
		}

		IPC_ChShift = IPC_ChNum + IPC_GroupShift;

		/* check if channel conflict */
		IPC_IMR = IPC_IERGet(IPCx);
		if (IPC_IMR & BIT(IPC_ChShift)) {
			DBG_PRINTF(MODULE_IPC, LEVEL_ERROR, "Channel Conflict for Channel %d !\r\n", IPC_ChShift);
			continue;
		}

		/* enable intr and register call back func*/
		if ((ipc_init_table[i].func != NULL) || (ipc_init_table[i].IrqData != NULL)) {
			IPC_INTUserHandler(IPCx, IPC_ChShift, (void *)ipc_init_table[i].func, ipc_init_table[i].IrqData);
		}
	}
}

/**
  * @brief  exchange messages between KM0 and KM4.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_LP_TO_NP: LP send request to NP
  *		 		@arg IPC_LP_TO_AP: LP send request to NP
  *		 		@arg IPC_NP_TO_LP: NP send request to LP
  *		 		@arg IPC_NP_TO_AP: NP send request to AP
  *		 		@arg IPC_AP_TO_LP: AP send request to LP
  *		 		@arg IPC_AP_TO_NP: AP send request to NP
  * @param  IPC_ChNum: the IPC channel number.
  * @param  Message: pointer to the message to be exchanged,and should not stored in stack.
  * @retval   None
  */
void ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg)
{
	/* Check the parameters */
	assert_param(IS_IPC_DIR_MODE(IPC_Dir));

	PIPC_MSG_STRUCT IPC_MSG = (PIPC_MSG_STRUCT)__km0_ipc_memory_start__;
	u32 msg_idx = IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_Dir) + IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_Dir) + IPC_ChNum;
	IPC_TypeDef *IPCx = NULL;

	switch (IPC_Dir) {
	case IPC_LP_TO_NP:
	case IPC_LP_TO_AP:
		IPCx = IPCLP_DEV;
		break;

	case IPC_NP_TO_LP:
	case IPC_NP_TO_AP:
		IPCx = IPCNP_DEV;
		break;

	case IPC_AP_TO_LP:
	case IPC_AP_TO_NP:
		IPCx = IPCAP_DEV;
		break;

	default:
		break;
	}

	if (IPC_Msg) {
		memcpy(&IPC_MSG[msg_idx], IPC_Msg, sizeof(IPC_MSG_STRUCT));
		DCache_Clean((u32)&IPC_MSG[msg_idx], sizeof(IPC_MSG_STRUCT));
	}
	IPC_INTRequest(IPCx, IPC_Dir, IPC_ChNum);
}

/**
  * @brief  get ipc message.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_LP_TO_NP: LP send request to NP
  *		 		@arg IPC_LP_TO_AP: LP send request to NP
  *		 		@arg IPC_NP_TO_LP: NP send request to LP
  *		 		@arg IPC_NP_TO_AP: NP send request to AP
  *		 		@arg IPC_AP_TO_LP: AP send request to LP
  *		 		@arg IPC_AP_TO_NP: AP send request to NP
  * @param  IPC_ChNum: the IPC channel number.
  * @retval  : pointer to the message to be exchanged.
  * @note for data massage, corresponding data cache should be invalidate before access.
  */
PIPC_MSG_STRUCT ipc_get_message(u32 IPC_Dir, u8 IPC_ChNum)
{
	PIPC_MSG_STRUCT IPC_MSG = NULL;
	u32 msg_idx = IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_Dir) + IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_Dir) + IPC_ChNum;

	IPC_MSG = (PIPC_MSG_STRUCT)__km0_ipc_memory_start__;
	DCache_Invalidate((u32)&IPC_MSG[msg_idx], sizeof(IPC_MSG_STRUCT));

	return &IPC_MSG[msg_idx];
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
