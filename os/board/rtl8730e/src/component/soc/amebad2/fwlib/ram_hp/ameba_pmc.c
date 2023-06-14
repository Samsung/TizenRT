/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"
_OPTIMIZE_NONE_
VOID SOCPS_SleepPG(VOID)
{
	u32 nDeviceIdOffset = 0;

	//InterruptDis(UART_LOG_IRQ);
	Img2EntryFun0.RamWakeupFun = SOCPS_WakeFromPG_HP;

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);

		if (tickless_debug) {
			DBG_8195A("DBG: KM4 Sleep PG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}

	SOCPS_SleepPG_LIB();

	pmu_exec_wakeup_hook_funs(PMU_MAX);


}

void SOCPS_SleepCG(void)
{

	u32 nDeviceIdOffset = 0;
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	irq_disable(WL_PROTOCOL_IRQ);
#endif
	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);

		if (tickless_debug) {
			DBG_8195A("DBG: KM4 Sleep CG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}
	SOCPS_SleepCG_LIB();

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	irq_enable(WL_PROTOCOL_IRQ);
#endif
}
/**
  *  @brief Get dslp wake status.
	* @param none:

	* @retval DslpWake status.
	* 		   @arg TRUE: boot from deep sleep
	* 		   @arg FALSE: boot from power on
  */
u32 SOCPS_DsleepWakeStatusGet(void)
{
	u32 Temp = BOOT_Reason();

	if (Temp & AON_BIT_RSTF_DSLP) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  get wakepin index.
  * @param  None
  * @retval BIT(0): wakepin0, BIT(1): wakepin1, BIT(2): wakepin2, BIT(3): wakepin3
  */
int SOCPS_WakePinCheck(void)
{
	int WakePin = 0;

	WakePin = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_WAK_EVT);
	WakePin = WakePin >> 4;

	return WakePin;
}

/**
  * @brief  get current AON timer.
  * @param  None
  * @note wakeup state:sleep PG & CG & deep sleep
  * @retval The current Aon Timer counter, the unit is ms.
  */
u32 SOCPS_AONTimerGet(VOID)
{
	u32 Rtemp = 0;

	/* use 32K */
	Rtemp = AON_GET_TIM_CNT_CUR(HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_CNT));
	Rtemp = Rtemp / 100;
	return Rtemp;
}

/**
  * @brief  LP wake NP by IPC
  * @param  None
  * @note  ipc_msg_temp.msg represents who wakes up KM4
  		* ipc_msg_temp.msg = 0: FW wakeup KM4
  * @retval None
  */
void SOCPS_LPWHP_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_LP_TO_NP, IPC_L2N_WAKE_NP);

	u32 type = ipc_msg_temp->msg;

	if (type == FW_LPWNP_IPC) {
		//DBG_8195A("%s: FW wakeup KM4 via IPC \n", __func__);
	}

}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_LPWHP_table[] = {
	{IPC_USER_DATA, 	SOCPS_LPWHP_ipc_int,	(VOID *) NULL, IPC_LP_TO_NP, IPC_L2N_WAKE_NP, IPC_RX_FULL},
};

