/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ameba_soc.h"
#include "semphr.h"

#include "ipc.h"

#define MSG_DATA_HEAP_SIZE	0xA000
#define MSG_INFO_HEAP_SIZE	0x2000
#define DATA_BUF_SIZE		2048
#define BYTE_ALIGNMENT		128


typedef struct {
	uint32_t addr;
	uint32_t len;
	uint32_t dir;
	uint32_t rsvd0[29];
	uint32_t wptr;
	uint32_t rsvd1[31];
	uint32_t rptr;
	uint32_t rsvd2[31];
} MessageInfo;


static void *g_wake_sema[IPC_ID_NUM];
static uint32_t g_ipc_buffer = 0;
static const uint32_t g_ipc_irq_data0 = 0;
static const uint32_t g_ipc_irq_data1 = 1;

#if defined(CONFIG_IPC_MSG_MASTER)

typedef enum {
	IPC_ID_RPC_AP_TO_NP 	= IPC_ID_NUM - 1,
	IPC_ID_RPC_LP_TO_AP 	= IPC_ID_NUM - 2,
	IPC_ID_RPC_NP_TO_LP 	= IPC_ID_NUM - 3,
	IPC_ID_RPC_NP_TO_AP 	= IPC_ID_NUM - 4,
	IPC_ID_RPC_AP_TO_LP 	= IPC_ID_NUM - 5,
	IPC_ID_RPC_LP_TO_NP 	= IPC_ID_NUM - 6,
} RPC_ID;

static const IMQ2_INIT_TABLE ipc_msg_config[] = {
	//ID						DIR
	{IPC_ID_RPC_NP_TO_AP, 	IPC_NP_TO_AP}, // Internal used for RPC NP -> AP
	{IPC_ID_RPC_AP_TO_NP, 	IPC_AP_TO_NP}, // Internal used for RPC AP -> NP

	{IPC_ID_NP_TO_AP, 		IPC_NP_TO_AP}, // User for NP -> AP
	{IPC_ID_AP_TO_NP, 		IPC_AP_TO_NP}, // User for AP -> NP
};

static uint8_t msg_data_heap[MSG_DATA_HEAP_SIZE]__attribute__((aligned(BYTE_ALIGNMENT)));
static uint8_t msg_info_heap[MSG_INFO_HEAP_SIZE]__attribute__((aligned(BYTE_ALIGNMENT)));


static void IPC_Message_Buffer_Init(void)
{
	int8_t i = 0;
	int num = NELEM(ipc_msg_config);
	for (i = 0; i < num; i++) {
		uint8_t id = ipc_msg_config[i].id;
		MessageInfo *info = (MessageInfo *)(g_ipc_buffer + id * sizeof(MessageInfo));
		info->dir = ipc_msg_config[i].dir;
		info->addr = (uint32_t)(msg_data_heap + id * DATA_BUF_SIZE);
		info->len = DATA_BUF_SIZE;
		info->wptr = info->addr;
		info->rptr = info->addr;
		DCache_Clean((uint32_t)info, sizeof(MessageInfo));
		IPC_LOGI("id: %d init done, info=%p\n", (int)id, info);
	}
}

int32_t IPC2_Message_Queue_Init(void)
{
	RRAM_TypeDef *rram = IPC_RRAM;
	DCache_Invalidate((uint32_t)rram, sizeof(RRAM_TypeDef));
	if (rram->IMQ_INIT_DONE == 1) {
		IPC_LOGE("IMQ_INIT_DONE aleady\n");
		return IPC_ERROR;
	}
	memset((void *)msg_info_heap, 0, MSG_INFO_HEAP_SIZE);
	rram->IMQ_HEAP_ADDR = (uint32_t)msg_info_heap;
	g_ipc_buffer = rram->IMQ_HEAP_ADDR;
	IPC_Message_Buffer_Init();
	rram->IMQ_INIT_DONE = 1;
	DCache_Clean((uint32_t)rram, sizeof(RRAM_TypeDef));
	for (uint8_t i = 0; i < IPC_ID_NUM; i++) {
		g_wake_sema[i] = xSemaphoreCreateBinary();
	}
	IPC_LOGI("Ipc_Message_Init master End\n");
	return 0;
}
#else
int32_t IPC2_Message_Queue_Init(void)
{
	RRAM_TypeDef *rram = IPC_RRAM;
	if (g_ipc_buffer != 0) {
		IPC_LOGE("IPC_MSG_INIT aleady\n");
		return IPC_ERROR;
	}
	while (rram->IMQ_INIT_DONE == 0) {
		vTaskDelay(2);
		DCache_Invalidate((uint32_t)rram, sizeof(RRAM_TypeDef));
	}
	g_ipc_buffer = rram->IMQ_HEAP_ADDR;
	for (uint8_t i = 0; i < IPC_ID_NUM; i++) {
		g_wake_sema[i] = xSemaphoreCreateBinary();
	}
	IPC_LOGI("Ipc_Message_Init End\n");
	return 0;
}
#endif

int32_t IPC2_Message_Queue_Send(uint32_t id, uint8_t *buf, uint32_t size, int32_t timeout)
{
	MessageInfo *info;
	uint32_t segment_len = 0;
	uint32_t remain_bytes = 0;
	uint32_t end_addr;

	if (id > IPC_ID_NUM) {
		IPC_LOGE("id: %d IPC_INVALID_CH\n", (int)id);
		return IPC_INVALID_CH;
	}

	if (!size || !buf) {
		IPC_LOGE("id: %d, invalid size or buf\n", (int)id);
		return IPC_INVALID_SIZE;
	}

	info = (MessageInfo *)(g_ipc_buffer + id * sizeof(MessageInfo));
	DCache_Invalidate((uint32_t)info, sizeof(MessageInfo));
	do {
		uint32_t rptr;
		rptr = info->rptr;
		if (rptr > info->wptr) {
			remain_bytes = rptr - info->wptr - 1;
		} else {
			remain_bytes = rptr + info->len - info->wptr - 1;
		}
		if (remain_bytes > size) {
			break;
		}

		if (timeout == 0) {
			return IPC_TX_TIMEOUT;
		}
		if (timeout != WAIT_FOREVER) {
			timeout--;
		}
		vTaskDelay(1);
		DCache_Invalidate((uint32_t)&info->rptr, BYTE_ALIGNMENT);
	} while (timeout > 0 || timeout == WAIT_FOREVER);

	end_addr = info->addr + info->len;
	if (info->wptr + size > end_addr) {
		segment_len = end_addr - info->wptr;
		memcpy((void *)info->wptr, (void *)buf, segment_len);
		memcpy((void *)info->addr, (void *)(buf + segment_len), size - segment_len);
		DCache_Clean(info->wptr, segment_len);
		DCache_Clean(info->addr, size - segment_len);
		info->wptr = info->addr + (size - segment_len);
	} else {
		memcpy((void *)info->wptr, (void *)buf, size);
		DCache_Clean(info->wptr, size);
		info->wptr += size;
	}
	DCache_Clean((uint32_t) & (info->wptr), BYTE_ALIGNMENT);
	uint8_t channel = IPC_RX_TRAN_0;
	IPC_MSG_STRUCT ipc_message = { 0 };
	ipc_message.msg = id;
	ipc_message.msg_type = IPC_USER_POINT;
	ipc_send_message(info->dir, channel, &ipc_message);
	IPC_LOGV("ipc_send_message: %x, channel=%d\n", (int)info->dir, channel);
	return IPC_SUCCESS;
}

int32_t IPC2_Message_Queue_Recv(uint32_t id, uint8_t *buf, uint32_t *size)
{
	MessageInfo *info;
	uint32_t end_addr;
	uint32_t segment_len = 0;
	uint32_t readable_size = 0;
	uint32_t wptr;
	if (!buf || *size == 0) {
		IPC_LOGE("id: %d, invalid buf or size : %d\n", (int)id, (int)*size);
		return IPC_INVALID_SIZE;
	}

	if (id > IPC_ID_NUM) {
		IPC_LOGE("id: %dis available\n", (int)id);
		return IPC_INVALID_CH;
	}

	info = (MessageInfo *)(g_ipc_buffer + id * sizeof(MessageInfo));
	DCache_Invalidate((uint32_t)info, sizeof(MessageInfo));

	do {
		wptr = info->wptr;
		if (info->rptr > wptr) {
			readable_size = wptr + info->len - info->rptr;
		} else {
			readable_size = wptr - info->rptr;
		}

		if (readable_size < *size) {
			xSemaphoreTake(g_wake_sema[id], 0xFFFFFFFF);
			DCache_Invalidate((uint32_t)info, sizeof(MessageInfo));
		}
	} while (readable_size < *size);

	end_addr = info->addr + info->len;
	if (info->rptr + *size > end_addr) {
		segment_len = end_addr - info->rptr;
		DCache_Invalidate(info->rptr, segment_len);
		DCache_Invalidate(info->addr, *size - segment_len);
		memcpy((void *)buf, (void *)info->rptr, segment_len);
		memcpy((void *)(buf + segment_len), (void *)info->addr, *size - segment_len);
		info->rptr = info->addr + (*size - segment_len);
	} else {
		DCache_Invalidate(info->rptr, *size);
		memcpy((void *)buf, (void *)info->rptr, *size);
		info->rptr += *size;
	}

	DCache_Clean((uint32_t)&info->rptr, BYTE_ALIGNMENT);
	return IPC_SUCCESS;
}

static void IPC_Message_Handler(VOID *data, uint32_t irq_status, uint32_t chan_num)
{
	(void)irq_status;
	(void)chan_num;

	uint32_t irq_data = *(uint32_t *)data;

	PIPC_MSG_STRUCT p_recv_msg;
	switch (irq_data) {
	case g_ipc_irq_data0:
		p_recv_msg = ipc_get_message(IPC_DIR_MSG_RX_0, IPC_RX_TRAN_0);
		break;
	case g_ipc_irq_data1:
		p_recv_msg = ipc_get_message(IPC_DIR_MSG_RX_1, IPC_RX_TRAN_1);
		break;
	default:
		break;
	}

	DCache_Invalidate((uint32_t)p_recv_msg, sizeof(IPC_MSG_STRUCT));
	IPC_LOGV("%s, id: %d\n", __func__, p_recv_msg->msg);
	if (g_wake_sema[p_recv_msg->msg]) {
		portBASE_TYPE task_woken = pdFALSE;
		xSemaphoreGiveFromISR(g_wake_sema[p_recv_msg->msg], &task_woken);
		portYIELD_FROM_ISR(task_woken);
	}
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   rpc_message_handlers[] = {
	{IPC_USER_POINT,	IPC_Message_Handler,	(VOID *) &g_ipc_irq_data0, IPC_DIR_MSG_RX_0, IPC_RX_TRAN_0, IPC_RX_FULL},
	{IPC_USER_POINT,	IPC_Message_Handler,	(VOID *) &g_ipc_irq_data1, IPC_DIR_MSG_RX_1, IPC_RX_TRAN_1, IPC_RX_FULL},
};
