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
#include <fcntl.h>
#include <tinyara/timer.h>

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#include "amebasmart_config.h"
#include "arch_timer.h"
#include "ameba_soc.h"
#include "osdep_service.h"
#include "timer_api.h"

gtimer_t g_timer1;
extern struct pm_timer_s g_pm_timer;

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

/* Follow the callback structure in os/include/tinyara/timer.h */
/* Update next_interval_us will only benefit timer with periodical mode */
void timer_int_handler(uint32_t *next_interval_us, pm_timer_type_t timer_type)
{
	pmvdbg("PM Timer interrupt handler!!\n");
	switch (timer_type) {
		case PM_LOCK_TIMER:
			// Relax the lock on the PM state transitions after the timer interrupt
			pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
			break;
		case PM_WAKEUP_TIMER:
			// Switch status back to normal mode after wake up from interrupt
			pm_activity(PM_IDLE_DOMAIN, 9);
			break;
		case PM_PERIODICAL_TIMER:
			/* Anything else to do ? */
			break;
		default:
			pmdbg("Timer callback triggered without setting timer, Unexpected Error!!!\n");
			break;
	}
	// Reset the global struct
	g_pm_timer.timer_type &= ~(timer_type);
	g_pm_timer.timer_interval = 0;
}

void up_set_pm_timer(int pm_timer_type) {
	// Check whether timer interrupt need to be set
	struct timer_notify_s notify;
	notify.arg   = NULL;
	notify.pid   = (pid_t)getpid();
	notify.timer_type = pm_timer_type;
	int fd = 0;
	switch (pm_timer_type) {
		case PM_WAKEUP_TIMER:
			fd = open("/dev/timer1", O_RDONLY);
			ioctl(fd, TCIOC_SETMODE, MODE_ONESHOT);
			break;
		case PM_LOCK_TIMER:
			fd = open("/dev/timer2", O_RDONLY);
			ioctl(fd, TCIOC_SETMODE, MODE_ONESHOT);
			break;
		case PM_PERIODICAL_TIMER:
			fd = open("/dev/timer3", O_RDONLY);
			ioctl(fd, TCIOC_SETMODE, MODE_PERIODICAL);
			break;
		default:
			pmdbg("Invalid input for timer type!\n");
			return;
	}
	/* Set timeout value */
	ioctl(fd, TCIOC_SETTIMEOUT, g_pm_timer.timer_interval);
	/* Set callback structure */
	if (ioctl(fd, TCIOC_NOTIFICATION, (unsigned long)((uintptr_t)&notify)) < 0) {
		fprintf(stderr, "ERROR: Failed to set the timer handler: %d\n", errno);
		return;
	}
	/* Fire the timer */
	if (ioctl(fd, TCIOC_START, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to start the timer: %d\n", errno);
		return;
	}
	return;
}

/* Interrupt callback from wifi-keepalive, which LP received designated packet*/
void SOCPS_LPWAP_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	pmvdbg("IPC wakeup interrupt handler!!\n");
	pm_activity(PM_IDLE_DOMAIN, 9);
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
