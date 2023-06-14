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

#ifndef IPC_MESSAGE_H
#define IPC_MESSAGE_H
#include <stdint.h>
#include "ameba_ipc.h"

//#define DEBUG_ENABLE

#define TAG "[IPC_MSG]"
#define TAG_ERR "[IPC_MSG][ERR]"

#ifdef DEBUG_ENABLE
#define IPC_LOGV(fmt, ...)  printf(TAG fmt, ##__VA_ARGS__)
#else
#define IPC_LOGV(fmt, ...)  do { } while(0)
#endif
#define IPC_LOGI(fmt, ...)  printf(TAG fmt, ##__VA_ARGS__)
#define IPC_LOGE(fmt, ...)  printf(TAG_ERR fmt, ##__VA_ARGS__)

#define NELEM(tab) (sizeof(tab) / sizeof((tab)[0]))

#define WAIT_FOREVER	-1
#define IPC_RRAM RRAM_DEV
#if defined(ARM_CORE_CM4)
#define IPC_RX_TRAN_0	IPC_R2M_IMQ_TRX_TRAN
#define IPC_RX_TRAN_1	IPC_D2M_IMQ_TRX_TRAN
#define IPC_TX_TRAN_0	IPC_M2R_IMQ_TRX_TRAN
#define IPC_TX_TRAN_1	IPC_M2D_IMQ_TRX_TRAN
#define IPC_DIR_MSG_RX_0 IPC_KR4_TO_KM4
#define IPC_DIR_MSG_RX_1 IPC_DSP_TO_KM4
#elif defined(RSICV_CORE_KR4)
#define IPC_RX_TRAN_0	IPC_M2R_IMQ_TRX_TRAN
#define IPC_RX_TRAN_1	IPC_D2R_IMQ_TRX_TRAN
#define IPC_TX_TRAN_0	IPC_R2M_IMQ_TRX_TRAN
#define IPC_TX_TRAN_1	IPC_R2D_IMQ_TRX_TRAN
#define IPC_DIR_MSG_RX_0 IPC_KM4_TO_KR4
#define IPC_DIR_MSG_RX_1 IPC_DSP_TO_KR4
#else
//DSP
#endif

typedef enum {
	IPC_ID_KR4_TO_KM4	= 0,
	IPC_ID_KM4_TO_KR4	= 1,
	IPC_ID_KR4_TO_DSP	= 2,
	IPC_ID_DSP_TO_KR4	= 3,
	IPC_ID_DSP_TO_KM4	= 4,
	IPC_ID_KM4_TO_DSP	= 5,
	/*14-19 used for RPC*/
	IPC_ID_NUM			= 20
} IPC_ID;

typedef enum {
	IPC_SUCCESS         =  0,
	IPC_ERROR           = -1,
	IPC_INVALID_CH      = -2,
	IPC_INVALID_SIZE    = -3,
	IPC_NO_MEMORY       = -4,
	IPC_NOT_INIT        = -5,
	IPC_TX_TIMEOUT      = -6,
	IPC_RX_TIMEOUT      = -7,
	IPC_TX_ERROR        = -8,
	IPC_RX_ERROR        = -9,
} ipc_error_t;

typedef struct {
	uint32_t	id;
	uint8_t		dir;
} IMQ2_INIT_TABLE;

int32_t IPC2_Message_Queue_Init(void);
int32_t IPC2_Message_Queue_Send(uint32_t id, uint8_t *buf, uint32_t size, int32_t timeout);
int32_t IPC2_Message_Queue_Recv(uint32_t id, uint8_t *buf, uint32_t *size);

#endif //IPC_MESSAGE_H
