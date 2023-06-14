/**
  ******************************************************************************
  * @file    ameba_pmc_ram.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the soc power management circut:
  *           - wakeup timer
  *           - wakeup pin
  *           - sleep option
  *           - sleep mode
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"
extern SLEEP_ParamDef sleep_param;

/**
  * @brief  set REGU wakeup_pin debounce cycle.
  * @param  Dbnc_cycle: wakeup_pin debounce cycle, unit is 100k/128k.
  *		This parameter must be set to a value in the 0-0x7FFF range.
  * @param  Status: enable or disable wakepin debounce function
  * @note Dbnc_cycle is valid when Status is set ENABLE.
  * @retval None
  */
void SOCPS_SetWakepinDebounce_AP(u32 Dbnc_cycle, u32 Status)
{
	u32 Rtemp = 0;
	u32 FEN_backup = 0;
	assert_param(Dbnc_cycle <= 0x7FFF);

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	FEN_backup = AON_GET_GPIO_WAKE_FEN(Rtemp);
	Rtemp &= (~AON_MASK_GPIO_WAKE_FEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set gpio debounce cycle */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	if (ENABLE == Status) {
		Rtemp &= (~AON_MASK_GPIO_DBNC_CYC);
		Rtemp |= AON_GPIO_DBNC_CYC(Dbnc_cycle);
		Rtemp |= AON_BIT_GPIO_DBNC_FEN;
	} else {
		Rtemp &= (~AON_BIT_GPIO_DBNC_FEN);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= AON_GPIO_WAKE_FEN(FEN_backup);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  set REGU wakeup_pin (just on wakepin mux to 4 pads).
  * @param  PinMask: aon wakepin index
  *		This parameter can be one of the following values:
  *		 @arg WAKUP_0 :
  *		 @arg WAKUP_1 :
  *		 @arg WAKUP_2 :
  *		 @arg WAKUP_3 :
  * @param  Polarity: aon wakepin Polarity
  *		This parameter can be one of the following values:
  *		 @arg 1 : high wakeup
  *		 @arg 0 : low wakeup
  * @note wakeup state: sleep PG & CG & deep sleep
  * @retval None
  */
void SOCPS_SetWakepin_AP(u32 PinIdx, u32 Polarity)
{
	u32 Rtemp = 0;

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= (~(AON_GPIO_WAKE_FEN(BIT(PinIdx))));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set polarity & internal PU/PD resistence */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= ~(AON_GPIO_WAKDET_POLY(BIT(PinIdx))); /* clear polarity, set to low active */
	if (Polarity == 1) {
		Rtemp |= AON_GPIO_WAKDET_POLY(BIT(PinIdx)); /* if high active */
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= (AON_GPIO_WAKE_FEN(BIT(PinIdx)));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  set AON timer for wakeup.
  * @param  SDuration: sleep time, unit is ms
  * @note wakeup state:sleep PG & CG & deep sleep
  * @note This is 100KHz timer, max counter = 0xFFFFFFFF/100000/60 = 715min
  * @retval None
  */
void SOCPS_AONTimer_AP(u32 SDuration)
{
	if (SDuration == 0) {
		return;
	}

	SDuration = SDuration * 100;
	DBG_8195A("SOCPS_AONTimer_AP\n");
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_SET, AON_TIM_CNT_CUR(SDuration));
}

/**
  * @brief  set ap wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetAPWakeEvent_MSK0_AP(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_AP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_AP, WakeEvent);
}

/**
  * @brief  set ap wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetAPWakeEvent_MSK1_AP(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_AP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_AP, WakeEvent);
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
	DBG_8195A("CA32 sleep\n");
	ipc_send_message(IPC_AP_TO_LP, IPC_A2L_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);

	asm volatile("wfe");
	asm volatile("wfe");
}

void SOCPS_LPWAP_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	ipc_get_message(IPC_LP_TO_AP, IPC_L2A_Channel1);

}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_LPWHP_table[] = {
	{IPC_USER_DATA, 	SOCPS_LPWAP_ipc_int,	(VOID *) NULL, IPC_LP_TO_AP, IPC_L2A_Channel1, IPC_RX_FULL},
};
