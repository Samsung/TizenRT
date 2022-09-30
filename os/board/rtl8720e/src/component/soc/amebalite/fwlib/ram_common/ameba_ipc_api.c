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

/**
  * @brief  init ipc interrupt handler table.
  * @param  where IPCx can be IPCKR4_DEV for KR4, IPCKM4_DEV for CM4, IPCDSP_DEV for DSP.
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
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_DSP_TO_KR4)) {
				IPCx_DEV = IPCKR4_DEV;
			} else if ((IPC_Dir == IPC_DSP_TO_KM4) || (IPC_Dir == IPC_KR4_TO_KM4)) {
				IPCx_DEV = IPCKM4_DEV;
			} else {
				IPCx_DEV = IPCDSP_DEV;
			}
		} else {
			/* for rx intr, this ipc entry should be registered in rx cpu*/
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_KM4_TO_DSP)) {
				IPCx_DEV = IPCKM4_DEV;
			} else if ((IPC_Dir == IPC_KR4_TO_KM4) || (IPC_Dir == IPC_KR4_TO_DSP)) {
				IPCx_DEV = IPCKR4_DEV;
			} else {
				IPCx_DEV = IPCDSP_DEV;
			}
		}

		if (IPCx_DEV != IPCx) {
			continue;
		}

		/* set ipc group shift according to intr mode */
		if (ipc_init_table[i].IPC_Intr_Mode == IPC_RX_FULL) {
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_KR4_TO_KM4) || (IPC_Dir == IPC_KR4_TO_DSP)) {
				IPC_GroupShift = 24;
			} else {
				IPC_GroupShift = 16;
			}
		} else {
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_KR4_TO_KM4) || (IPC_Dir == IPC_DSP_TO_KR4)) {
				IPC_GroupShift = 8;
			} else {
				IPC_GroupShift = 0;
			}
		}

		IPC_ChShift = IPC_ChNum + IPC_GroupShift;

		/* check if channel conflict */
		IPC_IMR = IPC_IERGet(IPCx);
		if (IPC_IMR & BIT(IPC_ChShift)) {
			/* overwrite the old call back func for cpu reset happens */
			DBG_PRINTF(MODULE_IPC, LEVEL_INFO, "Channel Conflict for CPU %d Channel %d ! Ignore If CPU Has Reset\r\n", SYS_CPUID(), IPC_ChShift);
		}

		/* enable intr and register call back func*/
		if ((ipc_init_table[i].func != NULL) || (ipc_init_table[i].IrqData != NULL)) {
			IPC_INTUserHandler(IPCx, IPC_ChShift, (void *)ipc_init_table[i].func, ipc_init_table[i].IrqData);
		}
	}
}

/**
  * @brief  share ipc memory between internal core by global register.
  * @param None
  * @retval   None
  */
void ipc_share_memory_init(void)
{
	RRAM_DEV->IPC_Share_Mem = (u32) __kr4_ipc_memory_start__;
}

/**
  * @brief  exchange messages between KM0 and KM4.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_KR4_TO_KM4: KR4 send request to KM4
  *		 		@arg IPC_KR4_TO_DSP: KR4 send request to DSP
  *		 		@arg IPC_KM4_TO_KR4: KM4 send request to KR4
  *		 		@arg IPC_KM4_TO_DSP: KM4 send request to DSP
  *		 		@arg IPC_DSP_TO_KR4: DSP send request to KR4
  *		 		@arg IPC_DSP_TO_KM4: DSP send request to KM4
  * @param  IPC_ChNum: the IPC channel number.
  * @param  Message: pointer to the message to be exchanged,and should not stored in stack.
  * @retval   None
  */
void ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg)
{
	/* Check the parameters */
	assert_param(IS_IPC_DIR_MODE(IPC_Dir));

	PIPC_MSG_STRUCT IPC_MSG = (PIPC_MSG_STRUCT)__kr4_ipc_memory_start__;
	u32 msg_idx = 16 * ((IPC_Dir >> 4) & 0xF) + 8 * (IPC_Dir & 0xF) + IPC_ChNum;
	IPC_TypeDef *IPCx = NULL;

	switch (IPC_Dir) {
	case IPC_KR4_TO_KM4:
	case IPC_KR4_TO_DSP:
		IPCx = IPCKR4_DEV;
		break;

	case IPC_KM4_TO_KR4:
	case IPC_KM4_TO_DSP:
		IPCx = IPCKM4_DEV;
		break;

	case IPC_DSP_TO_KR4:
	case IPC_DSP_TO_KM4:
		IPCx = IPCDSP_DEV;
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
  *		 		@arg IPC_KR4_TO_KM4: KR4 send request to KM4
  *		 		@arg IPC_KR4_TO_DSP: KR4 send request to DSP
  *		 		@arg IPC_KM4_TO_KR4: KM4 send request to KR4
  *		 		@arg IPC_KM4_TO_DSP: KM4 send request to DSP
  *		 		@arg IPC_DSP_TO_KR4: DSP send request to KR4
  *		 		@arg IPC_DSP_TO_KM4: DSP send request to KM4
  * @param  IPC_ChNum: the IPC channel number.
  * @retval  : pointer to the message to be exchanged.
  * @note for data massage, corresponding data cache should be invalidate before access.
  */
PIPC_MSG_STRUCT ipc_get_message(u32 IPC_Dir, u8 IPC_ChNum)
{
	PIPC_MSG_STRUCT IPC_MSG = NULL;
	u32 msg_idx = 16 * ((IPC_Dir >> 4) & 0xF) + 8 * (IPC_Dir & 0xF) + IPC_ChNum;

#ifndef ARM_CORE_CA7
	DCache_Invalidate((u32)__kr4_ipc_memory_start__, 3 * 16 * sizeof(IPC_MSG_STRUCT));
#endif
	IPC_MSG = (PIPC_MSG_STRUCT)__kr4_ipc_memory_start__;

	return &IPC_MSG[msg_idx];
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
