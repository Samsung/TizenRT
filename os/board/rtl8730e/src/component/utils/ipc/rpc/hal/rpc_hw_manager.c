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

#include "rpc_hw_manager.h"

#include <stdlib.h>

#include "ameba_soc.h"
#include "ipc.h"


#define WAIT_TIME_OUT       10
#define NONBLOCK_MODE       0x00
#define BLOCK_MODE          0x01
#define RPC_TAG "[RPC_HAL]"
#define RPC_TAG_ERR "[RPC_HAL][error]"
#ifdef DEBUG_ENABLE
#define RPC_LOGV(fmt, ...)  printf(RPC_TAG fmt, ##__VA_ARGS__)
#else
#define RPC_LOGV(fmt, ...)  do { } while(0)
#endif
#define RPC_LOGI(fmt, ...)  printf(RPC_TAG fmt, ##__VA_ARGS__)
#define RPC_LOGE(fmt, ...)  printf(RPC_TAG_ERR fmt, ##__VA_ARGS__)


static int g_rpc_inited = 0;


static int32_t ReadBuffer(int32_t id, int32_t opt, uint8_t *buf, int size)
{
	(void)opt;
	int32_t res = IPC2_Message_Queue_Recv(IPC_ID_NUM - id - 1, buf, (uint32_t *)&size);
	return (res == RPC_SUCCESS) ? size : res;
}

static int32_t WriteBuffer(int32_t id, int32_t opt, uint8_t *buf, int size)
{
	int32_t res = IPC2_Message_Queue_Send(IPC_ID_NUM - id - 1, buf, (uint32_t)size, ((opt & BLOCK_MODE) ? WAIT_FOREVER : WAIT_TIME_OUT));
	return (res == RPC_SUCCESS) ? size : res;
}

struct RPCHwManager *GetRPCHwManager(void)
{
	RPC_LOGI("GetRPCHwManager\n");
	if (g_rpc_inited == 0) {
		IPC2_Message_Queue_Init();
		g_rpc_inited = 1;
	}
	RPCHwManager *rpc_manager = (RPCHwManager *)malloc(sizeof(RPCHwManager));
	if (!rpc_manager) {
		RPC_LOGE("fail to alloc RPCHwManager.\n");
		return NULL;
	}

	rpc_manager->WriteBuffer = WriteBuffer;
	rpc_manager->ReadBuffer = ReadBuffer;
	return rpc_manager;
}

void DestoryRPCHwManager(struct RPCHwManager *manager)
{
	RPC_LOGI("DestoryRPCHwManager\n");
	if (manager != NULL) {
		free(manager);
	}
}