/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/arch/arm/src/amebasmart/amebasmart_pmhelpers.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#include "amebasmart_config.h"
#include "arch_timer.h"
#include "ameba_soc.h"
#include "timer_api.h"

static gtimer_t g_timer1;
static int g_bsp_domain_id[BSP_DOMAIN_MAX];

void SOCPS_SetAPWakeEvent_MSK0(u32 Option, u32 NewStatus)
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
void SOCPS_SetAPWakeEvent_MSK1(u32 Option, u32 NewStatus)
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

void up_set_pm_timer(unsigned int interval_us) 
{
	// Check whether timer interrupt need to be set
	if (interval_us > 0) {
		gtimer_init(&g_timer1, TIMER1);
		/* Pass in timer obj to avoid compile warning, the last argument will not be used in the callback handler */
		gtimer_start_one_shout(&g_timer1, interval_us, NULL, (uint32_t)&g_timer1);
	}
	return;
}

void bsp_pm_domain_register(char *domain_name, int bsp_drv_id)
{
	int domain_id = pm_domain_register(domain_name);
	if (domain_id < 0) {
		pmdbg("Unable to register %s DOMAIN\n", domain_name);
	} else {
		g_bsp_domain_id[bsp_drv_id] = domain_id;
	}
}

void bsp_pm_domain_control(int bsp_drv_id, bool is_suspend)
{
	/* Retrive the domain id from the bsp driver id */
	int domain_id = g_bsp_domain_id[bsp_drv_id];
	if (is_suspend) {
		/* Check BSP_DRV_DOMAIN enum for more info */
		if (pm_suspend(domain_id) != OK) {
			pmdbg("Unable to suspend bsp_drv_id: %d!\n", bsp_drv_id);
		}
	} else {
		if (pm_resume(domain_id) != OK) {
			pmdbg("Unable to resume bsp_drv_id: %d!\n", bsp_drv_id);
		}
	}
}

/* Interrupt callback from wifi-keepalive, which LP received designated packet*/
void SOCPS_LPWAP_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
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
		.RxIrqData = (VOID *) NULL,
		.Txfunc = IPC_TXHandler,
		.TxIrqData = (VOID *) NULL,
		.IPC_Direction = IPC_LP_TO_AP,
		.IPC_Channel = IPC_L2A_Channel1
	},

};

#endif
