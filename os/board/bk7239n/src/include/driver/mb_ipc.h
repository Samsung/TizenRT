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

#ifndef _mb_ipc_h_
#define _mb_ipc_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>

#if (CONFIG_CPU_CNT > 1)

enum
{
	MB_IPC_CONNECT_CMD = 0,
	MB_IPC_DISCONNECT_CMD,

	MB_IPC_SEND_CMD,

	MB_IPC_CMD_MAX  = 0x7F,  /* cmd id can NOT great than 0x7F. */
};

enum
{
	/* 4 bits for route status, must be in range 0 ~ 15. */
	IPC_ROUTE_STATUS_OK = 0x00,
	IPC_ROUTE_QUEUE_FULL,
	IPC_ROUTE_UNREACHABLE,
	IPC_ROUTE_UNSOLICITED_RSP,
	IPC_ROUTE_UNMATCHED_RSP,
	IPC_ROUTE_RX_BUSY,
};

enum
{
	/* 4 bits for api-implementation status, must be in range 0 ~ 15. */
	IPC_API_IMPL_STATUS_OK = 0x00,
	IPC_API_IMPL_NOT_INITED,
	IPC_API_IMPL_RX_BUSY,
	IPC_API_IMPL_RX_NOT_CONNECT,
	IPC_API_IMPL_RX_DATA_FAILED,
//	IPC_API_IMPL_RX_SEQ_ERROR,
};

enum
{
	MB_IPC_TX_OK = 0,

	/* error codes for occuring in tx side. */
	MB_IPC_TX_FAILED,
	MB_IPC_DISCONNECTED,
	MB_IPC_INVALID_PORT,
	MB_IPC_INVALID_HANDLE,
	MB_IPC_INVALID_STATE,
	
	MB_IPC_INVALID_PARAM,
	MB_IPC_NOT_INITED,
	MB_IPC_TX_TIMEOUT,
	MB_IPC_TX_BUSY,
	MB_IPC_NO_DATA,
	
	MB_IPC_RX_DATA_FAILED,

	/* base error code for router. */
	/* errors occuring in router layer of tx side or router cpu. */
	MB_IPC_ROUTE_BASE_FAILED = 0x20,

	/* base error code for API-implementation. */
	/* errors occuring in destination cpu/port API layer. */
	MB_IPC_API_BASE_FAILED = 0x30,
};

#define INVALID_USER_CMD_ID 0xFF

u32 mb_ipc_socket(u8 port, void * rx_callback);
int mb_ipc_connect(u32 handle, u8 dst_cpu, u8 dst_port, u32 time_out);
int mb_ipc_disconnect(u32 handle, u8 dst_cpu, u8 dst_port, u32 time_out);
int mb_ipc_close(u32 handle, u32 time_out);
int mb_ipc_send(u32 handle, u8 user_cmd, u8 * data_buff, u32 data_len, u32 time_out);
int mb_ipc_get_recv_data_len(u32 handle);
int mb_ipc_get_recv_event(u32 handle, u32 * event_flag);
int mb_ipc_recv(u32 handle, u8 * user_cmd, u8 * data_buff, u32 buff_len, u32 time_out);

u32 mb_ipc_server_get_connect_handle(u32 handle, u32 connect_id);
int mb_ipc_server_close(u32 handle, u32 time_out);

bk_err_t mb_ipc_init(void);

#else  /* (CONFIG_CPU_CNT > 1) */

#define mb_ipc_init(void)	

#endif  /* (CONFIG_CPU_CNT > 1) */


#ifdef __cplusplus
}
#endif

#endif /* _mb_ipc_h_ */

