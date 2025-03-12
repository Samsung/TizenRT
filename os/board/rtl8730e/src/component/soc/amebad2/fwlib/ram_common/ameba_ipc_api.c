/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include "os_wrapper.h"
/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup IPC
* @brief IPC driver modules
* @{
*/

/** @defgroup IPC_Exported_Constants IPC Exported Constants
  * @{
  */
static const char *TAG = "IPC";
#ifndef CONFIG_PLATFORM_TIZENRT_OS
rtos_sema_t ipc_Semaphore[IPC_TX_CHANNEL_NUM];
#else
_sema ipc_Semaphore[IPC_TX_CHANNEL_NUM];
#endif

void (*ipc_delay)(uint32_t);

#if defined ( __ICCARM__ )
#pragma section=".ipc.table.data"

SECTION(".data") u8 *__ipc_table_start__ = 0;
SECTION(".data") u8 *__ipc_table_end__ = 0;
#endif
/**@}*/

/** @defgroup IPC_Exported_Functions IPC Exported Functions
  * @{
  */
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
	u32 IPC_IntMode;

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
		if (IS_SEND_TO_NP(IPC_Dir)) {

			IPC_IntMode = (IPCx == IPCNP_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;

		} else if (IS_SEND_TO_AP(IPC_Dir)) {
			IPC_IntMode = (IPCx == IPCAP_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;

		} else if (IS_SEND_TO_LP(IPC_Dir)) {

			IPC_IntMode = (IPCx == IPCLP_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;
		} else {
			continue;
		}

		/* set ipc group shift according to intr mode */
		if (IPC_IntMode == IPC_RX_FULL) {
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
			RTK_LOGE(TAG, "Channel Conflict for Channel %lu !\r\n", IPC_ChShift);
			continue;
		}

		if (IPC_IntMode == IPC_RX_FULL) {
			/* enable rx intr and register call back func*/
			IPC_INTUserHandler(IPCx, IPC_ChShift, (void *)ipc_init_table[i].Rxfunc, ipc_init_table[i].RxIrqData);
		} else {
			/* enable tx intr and register call back func*/
			IPC_INTUserHandler(IPCx, IPC_ChShift, (void *)ipc_init_table[i].Txfunc, ipc_init_table[i].TxIrqData);
		}
	}
}

/**
  * @brief  The common IPC Tx interrupt handler
  * @param  Data: the data pointer to IPCx
  * @param  IrqStatus: Value of IPC_ISR
  * @param  ChanNum: ChanNum
  */
void IPC_TXHandler(void *Data, u32 IrqStatus, u32 ChanNum)
{
	UNUSED(Data);
	UNUSED(IrqStatus);

	u32 CPUID = SYS_CPUID();
	IPC_TypeDef *IPCx = IPC_GetDevById(CPUID);

	IPC_INTConfig(IPCx, ChanNum, DISABLE);

	if (ipc_Semaphore[ChanNum] != NULL) {
		rtos_sema_give(ipc_Semaphore[ChanNum]);
	}
}

/**
  * @brief  Processing functions when the IPC channel is occupied
  * @param  IPCx: where IPCx can be IPCKM0_DEV for KM0, IPCKM4_DEV for CM4.
  * @param  IPC_ChNum: IPC_ChNum
  * @retval IPC_REQ_TIMEOUT or IPC_SEMA_TIMEOUT or SUCCESS
  */
u32 IPC_wait_idle(IPC_TypeDef *IPCx, u32 IPC_ChNum)
{
	u32 timeout;

	timeout = 10000000;

	if (CPU_InInterrupt() || (IPC_IrqHandler[IPC_ChNum] == NULL)) {
		while (IPCx->IPC_TX_DATA & (BIT(IPC_ChNum))) {
			timeout--;
			if (timeout == 0) {
				RTK_LOGS(TAG, " IPC Request Timeout\r\n");
				return IPC_REQ_TIMEOUT;
			}
		}
	} else {
		if (ipc_Semaphore[IPC_ChNum] == NULL) {
			rtos_sema_create_binary(&ipc_Semaphore[IPC_ChNum]);
		}

		IPC_INTConfig(IPCx, IPC_ChNum, ENABLE);

		if (rtos_sema_take(ipc_Semaphore[IPC_ChNum], IPC_SEMA_MAX_DELAY) != SUCCESS) {
			RTK_LOGS(TAG, " IPC Get Semaphore Timeout\r\n");
			IPC_INTConfig(IPCx, IPC_ChNum, DISABLE);
			return IPC_SEMA_TIMEOUT;
		}
	}
	return 0;
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
u32 ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg)
{
	u32 ipc_shift;

	ipc_shift = 0;
	/* Check the parameters */
	assert_param(IS_IPC_DIR_MODE(IPC_Dir));
	assert_param(IS_IPC_VALID_CHNUM(IPC_ChNum));

	PIPC_MSG_STRUCT IPC_MSG = (PIPC_MSG_STRUCT)__km0_ipc_memory_start__;
	u32 msg_idx = IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_Dir) + IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_Dir) + IPC_ChNum;
	IPC_TypeDef *IPCx = NULL;

	switch (IPC_Dir) {
	case IPC_LP_TO_NP:
		ipc_shift = 8;
		IPCx = IPCLP_DEV;
		break;
	case IPC_LP_TO_AP:
		IPCx = IPCLP_DEV;
		break;

	case IPC_NP_TO_LP:
		ipc_shift = 8;
		IPCx = IPCNP_DEV;
		break;
	case IPC_NP_TO_AP:
		IPCx = IPCNP_DEV;
		break;

	case IPC_AP_TO_LP:
		ipc_shift = 8;
		IPCx = IPCAP_DEV;
		break;
	case IPC_AP_TO_NP:
		IPCx = IPCAP_DEV;
		break;

	default:
		break;
	}

	if (IPCx->IPC_TX_DATA & (BIT(IPC_ChNum + ipc_shift))) {
		if (IPC_wait_idle(IPCx, IPC_ChNum + ipc_shift)) {
			return IPC_SEND_TIMEOUT;
		}
	}

	if (IPC_Msg) {
		memcpy(&IPC_MSG[msg_idx], IPC_Msg, sizeof(IPC_MSG_STRUCT));
		DCache_Clean((u32)&IPC_MSG[msg_idx], sizeof(IPC_MSG_STRUCT));
	}
	IPCx->IPC_TX_DATA = (BIT(IPC_ChNum + ipc_shift));

	return IPC_SEND_SUCCESS;
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

/**
  * @brief  Get core-to-core hardware semaphone.
  * @param  SEM_Idx: 0~15.
  * @param  timeout: timeout to wait. 0 means never wait, 0xffffffff means waiting permanently.
  * @retval   TRUE/FALSE
  */
u32 IPC_SEMTake(u32 SEM_Idx, u32 timeout)
{
	u32 Sema_Stat;
	/* Check the parameters */
	assert_param(IS_IPC_VALID_SEMID(SEM_Idx));

	if ((SYSCFG_RLVersion()) >= SYSCFG_CUT_VERSION_D) {

		do {
			Sema_Stat = HAL_READ32(IPC_SEMA_BASE, SEM_Idx * 4);

			if (Sema_Stat == 0) {
				return _TRUE;
			} else {
				if (timeout == 0) {
					return _FALSE;
				}

				/* yield os for high priority thread*/
				if (ipc_delay) {
					ipc_delay(1);
				}
			}

			if (timeout != 0xffffffff) {
				timeout--;
			}

		} while (timeout);
	} else {

		do {
			Sema_Stat = HAL_READ16(IPC_IPC_SEMA_BASE, 0x0);
			if (Sema_Stat & BIT(SEM_Idx)) {
				if (timeout == 0) {
					return _FALSE;
				}

				/* yield os for high priority thread*/
				if (ipc_delay) {
					ipc_delay(1);
				}
				timeout--;

			} else {
				HAL_WRITE16(IPC_IPC_SEMA_BASE, 0x0, Sema_Stat | BIT(SEM_Idx));
				return _TRUE;
			}

		} while (timeout);

	}

	return _FALSE;
}


/**
  * @brief  Free core-to-core hardware semaphone.
  * @param  SEM_Idx: 0~15.
  * @retval   TRUE/FALSE
  */

u32 IPC_SEMFree(u32 SEM_Idx)
{
	/* Check the parameters */
	assert_param(IS_IPC_VALID_SEMID(SEM_Idx));

	if ((SYSCFG_RLVersion()) >= SYSCFG_CUT_VERSION_D) {
		HAL_WRITE32(IPC_SEMA_BASE, (SEM_Idx * 4), 1);
	} else {
		HAL_WRITE16(IPC_IPC_SEMA_BASE, 0x0, HAL_READ16(IPC_IPC_SEMA_BASE, 0x0) & (~ BIT(SEM_Idx)));
	}

	return _TRUE;
}


/**
  * @brief  Set delay function for ipc sema.
  * @param  pfunc: delay function.
  * @retval   None
  */
void IPC_SEMDelayStub(void (*pfunc)(uint32_t))
{
	ipc_delay = pfunc;
}
/**@}*/
/**@}*/
/**@}*/