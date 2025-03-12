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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <os_wrapper.h>
#include "rtk_km4log.h"
#include "osif.h"

/* -------------------------------- Defines --------------------------------- */
#ifndef CONFIG_RTL8730E_KM4_LOGTASK_PRIO
#define CONFIG_RTL8730E_KM4_LOGTASK_PRIO 100
#endif

#ifndef CONFIG_RTL8730E_KM4_LOGTASK_STACK
#define CONFIG_RTL8730E_KM4_LOGTASK_STACK 512
#endif
/* ------------------------------- Data Types ------------------------------- */

/* -------------------------- Function declaration -------------------------- */

/* ---------------------------- Global Variables ---------------------------- */
void * g_km4_log_queue = NULL;

/* --------------------------- Private Variables ---------------------------- */
static rtos_task_t km4_log_task;

static void rtl8730e_km4_logtask(void)
{
	static km4log_msg_t event = { 0 };

	/* initialize the queue that will hold the messages */
	if (!osif_msg_queue_create(&g_km4_log_queue, CONFIG_KM4_MAX_LOG_QUEUE_SIZE, sizeof(km4log_msg_t))) {
		DBG_8195A("Fail to init km4/np log msg queue\n");
		g_km4_log_queue = NULL;	// ensure that checking against this handle will be NULL
		return;
	}

	while (true) {
		/* loop and consume an item from the queue to print */
		if (osif_msg_recv(g_km4_log_queue, &event, 0xFFFFFFFF)) {
			/* prevent accidental buffer overflow when printf */
			((uint8_t *)event.buffer)[CONFIG_KM4_MAX_LOG_BUFFER_SIZE - 1] = 0;
			dbg_noarg("%s", event.buffer);
#ifndef CONFIG_AMEBASMART_USBDEVICE
			/* only for UART-only operation due to lower baudrate
			 * ensure no mixlog due to other task buffering prints by giving some cpu time. 
			 * setting this too low will bootloop, 1us is unsafe
			 */
			usleep(10);										
#endif
			/* set the first byte to null to cause string to print empty in case this buffer slot is accidentally reused */
			((uint8_t *)event.buffer)[0] = 0;
		}
	}
}

void rtl8730e_km4_logtask_initialize(void)
{
	static bool initialized = 0;

	if (!initialized) {
		/* create the log consumption task */
		if (FAIL == rtos_task_create(&km4_log_task, (const char *)"km4_log_task", (void *)rtl8730e_km4_logtask, NULL, CONFIG_RTL8730E_KM4_LOGTASK_STACK, CONFIG_RTL8730E_KM4_LOGTASK_PRIO)) {
			printf("Fail to create init km4/np logtask\n");
			return;
		}

		printf("Logtask init ok!\n");
		initialized = true;
	}

	return;
}
