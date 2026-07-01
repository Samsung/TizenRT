/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"


/**
  * @brief  Configure CHIPEN work mode,it works in HW reset mode
  * by default.
  * @param  mode: new work mode of CHIPEN.
  *   This parameter can be:
  *    CHIPEN_HW_RESET_MODE ,
  *    CHIPEN_INT_RESET_MODE,
  *    CHIPEN_PULSE_RESET_MODE.
  * @retval None
  */
void CHIPEN_WorkMode(enum CHIPEN_WORK_MODE mode)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);

	Temp &= ~ AON_MASK_CHIPEN_INTR_MODE;
	Temp |= AON_CHIPEN_INTR_MODE(mode);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL, Temp);
}

/**
 * @brief  Get CHIPEN work mode
 * @param  None
 * @retval mode: Work mode of CHIPEN. This value can be:
 *    CHIPEN_HW_RESET_MODE ,
 *    CHIPEN_INT_RESET_MODE,
 *    CHIPEN_PULSE_RESET_MODE.
 */
u8 CHIPEN_WorkModeGet(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);
	u8 Mode = AON_GET_CHIPEN_INTR_MODE(Temp);

	if (Mode >= CHIPEN_PULSE_RESET_MODE) {
		Mode = CHIPEN_PULSE_RESET_MODE;
	}

	return Mode;
}

/**
  * @brief  Set the CHIPEN Debounce Time.
  * Debounce timer works in all work mode
  * @param  Debounce: new debug counter.
  * CHIPEN_DBC_0US
  * CHIPEN_DBC_100US
  * CHIPEN_DBC_500US
  * CHIPEN_DBC_1MS
  * CHIPEN_DBC_2MS
  * CHIPEN_DBC_4MS
  * CHIPEN_DBC_8MS
  * CHIPEN_DBC_16MS
  * @retval None
  */
void CHIPEN_DebounceSet(u32 Debounce)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);

	assert_param(Debounce <= 7);

	Temp &= ~AON_MASK_CHIPEN_TDBC;
	Temp |= AON_CHIPEN_TDBC(Debounce);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL, Temp);
}

/**
  * @brief  Read CHIPEN pin status,only works in pulse reset mode.
  * @param None
  * @retval CHIPEN still pressed or not.
  * TRUE: Still pressed
  * FALSE: Released
  */

bool CHIPEN_IsPress(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);

	if (Temp & AON_BIT_CHIPEN_ST) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
  * @brief  Set long press and short press threshold.
  * Only works in interrupt reset mode
  * @param  Thres_LP:
  * CHIPEN_LP_1S
  * CHIPEN_LP_1P5S
  * CHIPEN_LP_2S
  * CHIPEN_LP_2P5S
  * CHIPEN_LP_3S
  * CHIPEN_LP_3P5S
  * CHIPEN_LP_4S
  * CHIPEN_LP_4P5S
  * @param  Thres_SP:
  * CHIPEN_SP_0MS
  * CHIPEN_SP_50MS
  * CHIPEN_SP_100MS
  * CHIPEN_SP_150MS
  * CHIPEN_SP_200MS
  * CHIPEN_SP_250MS
  * CHIPEN_SP_300MS
  * CHIPEN_SP_350MS
  * @retval None
  */
void CHIPEN_ThresHoldSet(u32 Thres_LP, u32 Thres_SP)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);

	assert_param(Thres_LP <= 7);
	assert_param(Thres_SP <= 7);

	Temp &= ~(AON_MASK_CHIPEN_TSP | AON_MASK_CHIPEN_TLP);
	Temp |= AON_CHIPEN_TSP(Thres_SP);
	Temp |= AON_CHIPEN_TLP(Thres_LP);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL, Temp);
}


/**
  * @brief  Set the ack threshlod for long press interrupt.
  * if long press interrupt can't be cleared within Tack, the system will reboot
  * @param  Tack
  * CHIPEN_ACK_50MS
  * CHIPEN_ACK_100MS
  * CHIPEN_ACK_200MS
  * CHIPEN_ACK_400MS
  * @retval None
  */
void CHIPEN_AckTimeSet(u32 Tack)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL);

	assert_param(Tack <= 3);

	Temp &= ~AON_MASK_CHIPEN_TACK ;
	Temp |= AON_CHIPEN_TACK(Tack);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_CTRL, Temp);
}

/**
  * @brief  Get the Debug timer atom value + 1.
  *   The read action will trigger Atom increase
  * @param  None
  * @retval atom counter
  */
void CHIPEN_ClearINT(u32 INTrBit)
{
	u32 Temp = INTrBit & (AON_BIT_CHIPEN_LP_ISR | AON_BIT_CHIPEN_SP_ISR);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_ISR, Temp);
}

/**
  * @brief  Get CHIPEN interrupt status .
  * @param  None
  * @retval interrupt status
  */
u32 CHIPEN_GetINT(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CHIPEN_ISR);

	Temp &= (AON_BIT_CHIPEN_LP_ISR | AON_BIT_CHIPEN_SP_ISR);

	return Temp;
}

/**
  * @brief  Get boot reason
  * @param  NA
  * @retval Boot Reason.
  *   This parameter can be any of the following values or combinations:
  *		 @arg BIT_AON_RSTF_WDG4
  *		 @arg BIT_AON_RSTF_WDG3
  *		 @arg BIT_AON_RSTF_WDG2
  *		 @arg BIT_AON_RSTF_WDG1
  *		 @arg BIT_AON_RSTF_IWDG
  *		 @arg BIT_AON_RSTF_APSYS
  *		 @arg BIT_AON_RSTF_NPSYS
  *		 @arg BIT_AON_RSTF_LPSYS
  *		 @arg BIT_AON_SHIFT_RSTF_DSLP
  *		 @arg BIT_AON_SHIFT_RSTF_BOR
    *		 @arg BIT_AON_SHIFT_RSTF_THM
  */
u32 BOOT_Reason(void)
{
	return (u32)(HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_REASON_SW) & 0xFFFF);
}

/**
  * @brief  warm reset the system
  * @param  None
  * @retval None
  */
void System_Reset(void)
{
	u32 CPUID = SYS_CPUID();
	u32 Trig = LSYS_BIT_LPSYS_RST;

	/*valid CPU ID or not*/
	if (CPUID <= AP_CPU_ID) {
		Trig <<= CPUID;
	} else {
		/*invalid CPU ID*/
		return;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SW_RST_TRIG, SYS_RESET_KEY);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SW_RST_CTRL, Trig);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SW_RST_TRIG, SYS_RESET_TRIG);

	/* wait for reset */
	while (1);
}
