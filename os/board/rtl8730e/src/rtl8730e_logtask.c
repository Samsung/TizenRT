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

// static buffer to hold log message
static u8 g_inic_ipc_logging_buf[CONFIG_KM4_MAX_LOG_QUEUE_SIZE][CONFIG_KM4_MAX_LOG_BUFFER_SIZE] = { 0 };
static u8 g_inic_ipc_logging_buf_ctr = 0;
// handle to log queue
extern void *g_km4_log_queue;
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

void inic_ipc_print_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	static km4log_msg_t message_event = { 0 };

	/* receive a log message over IPC */
	PIPC_MSG_STRUCT ipc_recv_msg = (PIPC_MSG_STRUCT)ipc_get_message(IPC_NP_TO_AP, IPC_N2A_NP_LOG_CHN);
	char *tmp_buffer = (char *)ipc_recv_msg->msg;
	DCache_Invalidate((u32)tmp_buffer, ipc_recv_msg->msg_len);

	/* fill the buffer only if the first byte is empty, otherwise SKIP and do not increment counter */
	if((char)g_inic_ipc_logging_buf[g_inic_ipc_logging_buf_ctr][0] == 0) {
		strncpy((char *)g_inic_ipc_logging_buf[g_inic_ipc_logging_buf_ctr], tmp_buffer, ipc_recv_msg->msg_len);
	} else {
		DBG_8195A("WARN: KM4 logbuf full, dropped log!\n");
		goto NOTIFY_MSG;
	}
	
	/* fill message struct */
	message_event.buffer = (void *)g_inic_ipc_logging_buf[g_inic_ipc_logging_buf_ctr];
	message_event.buffer_len = CONFIG_KM4_MAX_LOG_BUFFER_SIZE;

	/* use mq_send via osif api directly in ISR instead of semaphore-based */
	if (g_km4_log_queue == NULL || (!osif_msg_send(g_km4_log_queue, &message_event, 0))) {
		/* mixlog queue handle was invalid, or sending to queue failed, clear the memory here. */
		DBG_8195A("queue hndl is null or send failed\n");

		/* set the first byte to null to cause string to print empty in case this buffer slot is accidentally reused */
		g_inic_ipc_logging_buf[g_inic_ipc_logging_buf_ctr][0] = 0;
		goto NOTIFY_MSG;
	}

	/* increment to next buffer */
	g_inic_ipc_logging_buf_ctr = (g_inic_ipc_logging_buf_ctr + 1) % CONFIG_KM4_MAX_LOG_QUEUE_SIZE;

NOTIFY_MSG: ;
	/* Indicate logs have been printed */
	u8 *print_flag = (u8 *)ipc_recv_msg->rsvd;
	print_flag[0] = 1;
	DCache_Clean((u32)print_flag, sizeof(print_flag));
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_print_table = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = inic_ipc_print_int_hdl,
	.RxIrqData = (void *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (void *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX,
	.IPC_Channel = IPC_N2A_NP_LOG_CHN
};