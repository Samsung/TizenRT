/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

static const char *const TAG = "PMC";
extern SLEEP_ParamDef sleep_param;

/**
  * @brief  set AP wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: ENABLE/DISABLE.
  * @retval None
  */
void SOCPS_SetAPWakeEvent(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;
	u32 RegIndex = (Option >> 30) & 0x3;
	u32 Reg = 0;
	switch (RegIndex) {
	case 0x0:
	case 0x1:
	case 0x2:
		Reg = WAK_MASK0_AP;
		break;
	case 0x3:
		Reg = WAK_MASK1_AP;
		break;
	default:
		return;
	}
	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, Reg);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, Reg, WakeEvent);
}

/**
  * @brief  get aon wake reason.
  * @param  None
  * @retval aon wake up reason
  *		This parameter can be one or combination of the following values:
  *		 @arg AON_BIT_RTC_ISR_EVT
  *		 @arg AON_BIT_GPIO_PIN3_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN2_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN1_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN0_WAKDET_EVT
  *		 @arg AON_BIT_TIM_ISR_EVT
  *		 @arg AON_BIT_BOR_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_LP_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_SP_ISR_EVT
  */
int SOCPS_AONWakeReason(void)
{
	int reason = 0;

	reason = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_WAK_EVT);

	return reason;
}

/**
  * @brief  ap request to enter sleep.
  * @param  none
  * @retval none
  */
void SOCPS_Sleep(void)
{
	DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);
	RTK_LOGI(TAG, "CA32 sleep\n");
	ipc_send_message(IPC_AP_TO_LP, IPC_A2L_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);

	asm volatile("wfe");
	asm volatile("wfe");
}

void SOCPS_LPWAP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	ipc_get_message(IPC_LP_TO_AP, IPC_L2A_Channel1);

}

IPC_TABLE_DATA_SECTION

const IPC_INIT_TABLE ipc_LPWHP_table[] = {
	{
		.USER_MSG_TYPE = IPC_USER_DATA,
		.Rxfunc = SOCPS_LPWAP_ipc_int,
		.RxIrqData = (void *) NULL,
		.Txfunc = IPC_TXHandler,
		.TxIrqData = (void *) NULL,
		.IPC_Direction = IPC_LP_TO_AP,
		.IPC_Channel = IPC_L2A_Channel1
	},

};
