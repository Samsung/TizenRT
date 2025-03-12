/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
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
IPC_IRQ_FUN IPC_IrqHandler[IPC_CHANNEL_NUM];
void *IPC_IrqData[IPC_CHANNEL_NUM];
/**@}*/

/** @defgroup IPC_Exported_Functions IPC Exported Functions
  * @{
  */
/**
  * @brief  Enables or disables the specified IPC Channel interrupts.
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @param  IPC_Shiftbit: 0 ~ 31.
  * @param  NewState: DISABLE/ENABLE.
  * @retval   None
  */
void IPC_INTConfig(IPC_TypeDef *IPCx, u8 IPC_Shiftbit, u32 NewState)
{
	if (ENABLE == NewState) {
		IPCx->IPC_IMR |= (BIT(IPC_Shiftbit));
	} else {
		IPCx->IPC_IMR &= ~(BIT(IPC_Shiftbit));
	}
}

/**
  * @brief  set IMR of specified IPC Channel interrupts.
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @param  IPC_Chs: the Channels that want to be enable.
  * @retval   None
  */
void IPC_IERSet(IPC_TypeDef *IPCx, u32 IPC_Chs)
{
	IPCx->IPC_IMR = IPC_Chs;
}

/**
  * @brief  get IMR of specified IPC Channel interrupts.
  * @param  IPCx: where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @retval  The IMR status of specified IPC
  */
u32 IPC_IERGet(IPC_TypeDef *IPCx)
{
	return IPCx->IPC_IMR;
}

/**
  * @brief  Request a core-to-core interrupt.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_LP_TO_NP: LP send request to NP
  *		 		@arg IPC_LP_TO_AP: LP send request to NP
  *		 		@arg IPC_NP_TO_LP: NP send request to LP
  *		 		@arg IPC_NP_TO_AP: NP send request to AP
  *		 		@arg IPC_AP_TO_LP: AP send request to LP
  *		 		@arg IPC_AP_TO_NP: AP send request to NP
  * @param  IPC_ChNum: 0 ~ 7.
  * @retval   0/1
  */
u32 IPC_INTRequest(IPC_TypeDef *IPCx, u32 IPC_Dir, u8 IPC_ChNum)
{
	/* Check the parameters */
	assert_param(IS_IPC_DIR_MODE(IPC_Dir));
	u32 ipc_shift = 0;

	switch (IPC_Dir) {
	case IPC_LP_TO_NP:
	case IPC_NP_TO_LP:
	case IPC_AP_TO_LP:
		ipc_shift = 8;
		break;

	default:
		break;
	}

	if (IPCx != NULL) {
		if (IPCx->IPC_TX_DATA & (BIT(IPC_ChNum + ipc_shift))) {
			return 0;
		} else {
			IPCx->IPC_TX_DATA = (BIT(IPC_ChNum + ipc_shift));
			return 1;
		}
	} else {
		return 0;
	}
}

/**
  * @brief  Get core-to-core interrupt status.
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @retval  tx_empty or rx_full interrupt status
  */
u32 IPC_INTGet(IPC_TypeDef *IPCx)
{
	return IPCx->IPC_ISR;
}

/**
  * @brief  Clear a core-to-core interrupt.
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @param  IPC_Shiftbit: 0 ~ 31.
  * @retval   None
  */
void IPC_INTClear(IPC_TypeDef *IPCx, u8 IPC_Shiftbit)
{
	IPCx->IPC_ISR = BIT(IPC_Shiftbit);
}

/**
  * @brief  The common IPC interrupt handler
  * @param  Data: the data pointer to IPCx
  * @retval 0
  */
u32 IPC_INTHandler(void *Data)
{
	IPC_TypeDef *IPCx = (IPC_TypeDef *)Data;
	u32 IrqStatus;
	u32 i;
	IrqStatus = IPCx->IPC_ISR;

	for (i = 0; i < 32; i++) {
		if (IrqStatus & BIT(i)) {
			if (IPC_IrqHandler[i] != NULL) {
				IPC_IrqHandler[i](IPC_IrqData[i], IrqStatus, i);
				IPCx->IPC_ISR = BIT(i);
			} else {
				IPCx->IPC_ISR = BIT(i);
			}
		}
	}

	return 0;
}

/**
  * @brief  To register a user interrupt handler for a specified IPC channel
  * @param  where IPCx can be IPCLP_DEV for CM0, IPCNP_DEV for CM4, IPCAP_DEV for CA7.
  * @param  IPC_Shiftbit: 0 ~ 31.
  * @param  IrqHandler: The IRQ handler to be assigned to the specified IPC channel
  * @param  IrqData: The pointer will be pass the the IRQ handler
  * @retval None
  */
void IPC_INTUserHandler(IPC_TypeDef *IPCx, u8 IPC_Shiftbit, void *IrqHandler, void *IrqData)
{
	/* Check the parameters */
	if (IrqHandler == NULL) {
		return;
	}

	IPC_IrqHandler[IPC_Shiftbit] = (IPC_IRQ_FUN)IrqHandler;
	IPC_IrqData[IPC_Shiftbit] = IrqData;
	if (IS_IPC_RX_CHNUM(IPC_Shiftbit)) {
		IPC_INTConfig(IPCx, IPC_Shiftbit, ENABLE);
	}
}


/**
  * @brief  Get IPC device.
  * @param  cpuid:
  *			0: CPU_LP
  *		 	1: CPU_NP
  *			2: CPU_AP
  * @retval IPC device
  */
IPC_TypeDef *IPC_GetDevById(u32 cpu_id)
{
	/* Check the parameters */
	assert_param(IS_IPC_Valid_CPUID(cpu_id));

	if (cpu_id == 1) {
		return IPCNP_DEV;
	} else if (cpu_id == 2) {
		return IPCAP_DEV;
	} else {
		return IPCLP_DEV;

	}
}
/**@}*/
/**@}*/
/**@}*/