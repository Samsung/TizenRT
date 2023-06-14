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

#ifndef AMEBA_BASE_RPC_SRC_RPC_CLIENT_H
#define AMEBA_BASE_RPC_SRC_RPC_CLIENT_H

#include "rpc_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t WaitReply(void);

RPC_STRUCT RPC_PrepareCall(CLNT_STRUCT *clnt, int32_t result);

int32_t RPC_ClientCall(RPC_STRUCT *rpc, u_long procedure_id, int32_t opt, xdrproc_t xdr_args, caddr_t args_ptr, long args_size);

void RPC_DeInitClient(void);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_RPC_SRC_RPC_CLIENT_H
