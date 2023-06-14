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

#define LOG_TAG "RPCCommon"
#include "rpc_utils.h"

#include "xdr/xdr.h"
#include "xdr/xdr_prefix.h"

#include "rpc_struct.h"

#include "rpc_api.h"
#include "rpc_common.h"
#include "rpc_client.h"
#include "rpc_server.h"

#include "ameba_soc.h"

//#define VOICE_RPC_TEST
//#define CONFIG_RPC_TEST
#ifdef ARM_CORE_CM4
#define RPC_CHANNEL_AP2NP (RPC_AP_NP<<4) | (BLOCK_MODE | (RPC_NP_AP << 1))
#define RPC_CHANNEL_2DSP (RPC_DSP_NP<<4) | (BLOCK_MODE | (RPC_NP_DSP << 1))
#else
#define RPC_CHANNEL_AP2NP (RPC_NP_AP<<4) | (BLOCK_MODE | (RPC_AP_NP << 1))
#define RPC_CHANNEL_2DSP (RPC_DSP_AP<<4) | (BLOCK_MODE | (RPC_AP_DSP << 1))
#endif
#ifdef CONFIG_RPC_TEST
#if defined(ARM_CORE_CM4)
extern struct REG_STRUCT *TEST1_AGENT_0_register(struct REG_STRUCT *rnode);
extern struct REG_STRUCT *TEST2_AGENT_0_register(struct REG_STRUCT *rnode);
#else
extern struct REG_STRUCT *TEST1_SYSTEM_0_register(struct REG_STRUCT *rnode);
#endif
#endif

#ifdef VOICE_RPC_TEST
extern struct REG_STRUCT *VOICE_AGENT_0_register(struct REG_STRUCT *rnode);
#endif

static RPCHwManager *g_rpc_manager = NULL;
static RPC_Mutex g_lock;

/*
 * XDR type of struct RPC_STRUCT
 */
bool_t xdr_RPC_STRUCT(XDR *xdrs, RPC_STRUCT *objp)
{
	if (!xdr_u_long(xdrs, &objp->program_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->version_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->procedure_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->task_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->parameter_size)) {
		return FALSE;
	}
	if (!xdr_u_int(xdrs, &objp->context)) {
		return FALSE;
	}

	return TRUE;
};

/*
 * assign user's setting value into the CLNT_STRUCT
 */
CLNT_STRUCT RPC_PrepareCLNT(int32_t opt, long program_id, long version_id)
{
	CLNT_STRUCT clnt;
	clnt.send_mode = opt;
	clnt.program_id = program_id;
	clnt.version_id = version_id;
	return clnt;
}

static void RPC_InitThread(void *param)
{
	(void)param;
	if (g_rpc_manager) {
		RPC_LOGE("RPC_Init already DONE.\n");
		vTaskDelete(NULL);
		return;
	}
	RPC_MutexInit(&g_lock);
	if (!g_rpc_manager) {
		g_rpc_manager = GetRPCManager();
	}
	if (g_rpc_manager) {
		RPC_LOGD("GetRPCManager Success.\n");
	} else {
		RPC_LOGE("GetRPCManager failed.\n");
	}
#ifdef VOICE_RPC_TEST
#ifdef ARM_CORE_CM4
//#ifdef RSICV_CORE_KR4
	struct REG_STRUCT *pReg = NULL;
	pReg = VOICE_AGENT_0_register(pReg);
	pReg = ReplyHandler_register(pReg);
	RPC_InitProxy(pReg, RPC_CHANNEL_2DSP);
#endif
#endif
#ifdef CONFIG_RPC_TEST
#if defined(ARM_CORE_CM4)
	struct REG_STRUCT *pReg1 = NULL;
	struct REG_STRUCT *pReg2 = NULL;
	pReg1 = TEST1_AGENT_0_register(pReg1);
	pReg1 = ReplyHandler_register(pReg1);
	RPC_InitProxy(pReg1, RPC_CHANNEL_AP2NP);

	pReg2 = TEST2_AGENT_0_register(pReg2);
	pReg2 = ReplyHandler_register(pReg2);
	RPC_InitProxy(pReg2, RPC_CHANNEL_2DSP);
#else
	struct REG_STRUCT *pReg1 = NULL;
	pReg1 = TEST1_SYSTEM_0_register(pReg1);
	pReg1 = ReplyHandler_register(pReg1);
	RPC_InitProxy(pReg1, RPC_CHANNEL_AP2NP);
#endif
#endif
	vTaskDelete(NULL);
}

void ipc2_app_init(void)
{
	RPC_Init();
}

/*
 * init RPC
 */
int32_t RPC_Init(void)
{
	if (xTaskCreate(RPC_InitThread, ((const char *)"RPC_InitThread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		RPC_LOGE("\n\r%s xTaskCreate(RPC_InitThread) failed", __FUNCTION__);
	}
	return 0;
}

void RPC_DeInit(void)
{
	RPC_DeInitClient();
	RPC_DeInitServer();
	RPC_MutexLock(&g_lock);
	if (g_rpc_manager) {
		DestoryRPCHwManager(g_rpc_manager);
		g_rpc_manager = NULL;
	}
	RPC_MutexUnlock(&g_lock);
	RPC_MutexDestroy(&g_lock);
}

RPCHwManager *GetRPCManager(void)
{
	RPC_MutexLock(&g_lock);
	if (!g_rpc_manager) {
		g_rpc_manager = GetRPCHwManager();
	}
	RPC_MutexUnlock(&g_lock);
	return g_rpc_manager;
}
