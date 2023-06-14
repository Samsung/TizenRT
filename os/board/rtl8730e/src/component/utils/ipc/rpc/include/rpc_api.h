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


#ifndef AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H
#define AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H

#include "rpc_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t RPC_Init(void);

void RPC_DeInit(void);

CLNT_STRUCT RPC_PrepareCLNT(int32_t opt, long program_id, long version_id);

REG_STRUCT *ReplyHandler_register(struct REG_STRUCT *reg);

int32_t RPC_InitProxy(struct REG_STRUCT *reg, int32_t channel_id);

int32_t RPC_Unregister(struct REG_STRUCT *reg);

#ifdef __cplusplus
}
#endif
#endif //AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H