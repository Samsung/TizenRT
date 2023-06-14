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


#ifndef AMEBA_BASE_RPC_INTERFACES_RPC_STRUCT_H
#define AMEBA_BASE_RPC_INTERFACES_RPC_STRUCT_H

#include "xdr/xdr.h"
#include "rpc_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define rpc_malloc(size)		malloc(size)
#define rpc_free(ptr)			free(ptr)

#define NONBLOCK_MODE   0x00
#define BLOCK_MODE      0x01

enum {
	RPC_AP_NP	=	0,
	RPC_AP_DSP	=	1,
	RPC_NP_DSP	=	2,
	RPC_NP_AP	=	3,
	RPC_DSP_AP	=	4,
	RPC_DSP_NP	=	5,
};

typedef struct CLNT_STRUCT CLNT_STRUCT;
struct CLNT_STRUCT {
	int32_t send_mode;      // (NONBLOCK_MODE, BLOCK_MODE) | (USE_POLL_BUF, USE_INTR_BUF)
	int32_t program_id;     // The program ID defined in IDL file
	int32_t version_id;  	// The version ID defined in IDL file
};

typedef struct RPC_STRUCT RPC_STRUCT;
struct RPC_STRUCT {
	u_long program_id;          // program ID defined in IDL file
	u_long version_id;          // version ID defined in IDL file
	u_long procedure_id;        // function ID defined in IDL file
	u_long task_id;             // the caller's task ID, assign 0 if NONBLOCK_MODE
	u_long parameter_size;      // packet's body size
	u_int  context;             // return address of reply value
};

/*
 * The linking-list for register function
 */
typedef struct REG_STRUCT REG_STRUCT;
struct REG_STRUCT {
	u_long program_id;    // program ID record in IDL file
	u_long version_id;    // version ID record in IDL file
	void (*func)(RPC_STRUCT *, int, RPC_Mutex *, char *);

	REG_STRUCT *next;     // next node
};

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_RPC_INTERFACES_RPC_STRUCT_H
