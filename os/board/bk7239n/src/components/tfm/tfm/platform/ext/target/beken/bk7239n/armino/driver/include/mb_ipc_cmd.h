// Copyright 2020-2022 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _mb_ipc_cmd_h_
#define _mb_ipc_cmd_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>
#include "amp_res_lock.h"

enum
{
	IPC_TEST_CMD = 0,               /* CPU0 <-> CPU1 */

	IPC_CPU1_POWER_UP_INDICATION,           /* CPU1 -> CPU0  */
	IPC_CPU1_HEART_BEAT_INDICATION,         /* CPU1 -> CPU0  */
	IPC_GET_POWER_SAVE_FLAG,            /* CPU0 -> CPU1  */
	IPC_GET_CPU1_HEART_RATE,            /* CPU0 -> CPU1  */
	IPC_SET_CPU1_HEART_RATE,            /* CPU0 -> CPU1  */

	IPC_RES_ACQUIRE_CNT,                    /* CPU1 -> CPU0  */
	IPC_RES_RELEASE_CNT,                    /* CPU1 -> CPU0  */

	IPC_RES_AVAILABLE_INDICATION,   /* CPU0 <-> CPU1 */
	IPC_ALLOC_DMA_CHNL,                     /* CPU1 -> CPU0  */
	IPC_FREE_DMA_CHNL,                      /* CPU1 -> CPU0  */
	IPC_DMA_CHNL_USER,                      /* CPU1 -> CPU0  */

	IPC_CALL_CMD = 0x60,                    /* CPU1 -> CPU0  */

	IPC_CMD_MAX  = 0x7F,  /* cmd id can NOT great than 0x7F. */
};

/**    =============================     RPC    =============================   **/

#define FIELD_OFFSET(type, member)		((u32)(&(((type *)0)->member)))
#define FIELD_SIZE(type, member)		(sizeof(((type *)0)->member))

/* FIELD_IDX works only when every member size of type is the SAME ! */
#define FIELD_IDX(type, member)			(FIELD_OFFSET(type, member) / FIELD_SIZE(type, member))


#define RPC_CTRL_NO_RETURN		0x00

typedef union
{
	struct
	{
		u8	mod_id;
		u8	api_id;
		u8	ctrl;
		u8	data_len;
	};
	u32		call_id;
} rpc_call_hdr_t;

typedef struct
{
	rpc_call_hdr_t	call_hdr;
	u8	call_param[0];
} rpc_call_def_t;

typedef struct
{
	rpc_call_hdr_t	call_hdr;
	u8	ret_data[0];		/* api_ret_data_t if has ret data. */
} rpc_ret_def_t;

enum
{
	RPC_MOD_GPIO = 0,
	RPC_MOD_DMA,
	RPC_MOD_MAX,
} ;

#if (CONFIG_CPU_CNT > 1)

u32 ipc_send_test_cmd(u32 param);
bk_err_t ipc_send_available_ind(u16 resource_id);

#if CONFIG_SYS_CPU1

bk_err_t ipc_client_init(void);
bk_err_t ipc_send_power_up(void);
bk_err_t ipc_send_heart_beat(u32 param);
bk_err_t ipc_send_res_acquire_cnt(u16 resource_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list);
bk_err_t ipc_send_res_release_cnt(u16 resource_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list);
u8 ipc_send_alloc_dma_chnl(u32 user_id);
bk_err_t ipc_send_free_dma_chnl(u32 user_id, u8 chnl_id);
u32 ipc_send_dma_chnl_user(u8 chnl_id);

bk_err_t rpc_client_init(void);
bk_err_t rpc_client_call(rpc_call_def_t *rpc_param, u16 param_len, rpc_ret_def_t *ret_buf, u16 buf_len);

#define ipc_init	ipc_client_init
#define rpc_init	rpc_client_init

#endif  /* CONFIG_SYS_CPU1 */

#if CONFIG_SYS_CPU0

bk_err_t ipc_server_init(void);
u32 ipc_send_get_ps_flag(void);
u32 ipc_send_get_heart_rate(void);
bk_err_t ipc_send_set_heart_rate(u32 param);

bk_err_t rpc_server_init(void);
bk_err_t rpc_server_rsp(rpc_ret_def_t *rsp_param, u16 param_len);
int rpc_server_listen_cmd(u32 timeout_ms);
void rpc_server_handle_cmd(void);

#define ipc_init	ipc_server_init
#define rpc_init	rpc_server_init

#endif  /* CONFIG_SYS_CPU0 */

#else  /* (CONFIG_CPU_CNT > 1) */

#define ipc_init(void)	
#define rpc_init(void)	

#endif  /* (CONFIG_CPU_CNT > 1) */

/**    ============================    RPC end   ============================   **/

#ifdef __cplusplus
}
#endif

#endif /* _mb_ipc_cmd_h_ */

