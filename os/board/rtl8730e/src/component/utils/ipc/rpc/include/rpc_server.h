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

#ifndef AMEBA_BASE_RPC_SRC_RPC_SERVER_H
#define AMEBA_BASE_RPC_SRC_RPC_SERVER_H

#include "rpc_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REPLYID 99   	// for registering the ReplyHandler
#define RPC_DEBUG (0)
#define DEFAULT_STACK_SIZE 4096

typedef struct SEND_STRUCT SEND_STRUCT;

struct SEND_STRUCT {
	RPC_STRUCT  *rpc_ptr;
	u_long      procedure_id;
	int         send_mode;
	xdrproc_t   xdr_args;
	caddr_t     args_ptr;
	long        args_size;
};

typedef struct THREAD_STRUCT THREAD_STRUCT;
struct THREAD_STRUCT {
	REG_STRUCT  *reg_ptr;
	RPC_Mutex   *mutex;
	int         flag;
};

int32_t RPC_GetArgs(RPC_STRUCT *rpc, xdrproc_t xdr_argument, caddr_t argument, int32_t opt, char *param_buf);

struct REG_STRUCT *RPC_Register(struct REG_STRUCT *reg, u_long program_id, u_long version_id,  void (*func)(RPC_STRUCT *, int32_t, RPC_Mutex *, char *));

void RPC_SendReply(u_long req_taskID, int32_t req_context, char *reply_param, u_long param_size, xdrproc_t xdr_result, int32_t opt);

void RPC_DeInitServer(void);

void DispatchRPCRequest(struct REG_STRUCT *reg, char *buf, int32_t opt, RPC_Mutex *mutex, int32_t size);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_RPC_SRC_RPC_SERVER_H
