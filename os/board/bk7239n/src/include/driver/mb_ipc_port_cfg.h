// Copyright 2020-2025 Beken
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

#ifndef _mb_ipc_port_cfg_h_
#define _mb_ipc_port_cfg_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>


#if (CONFIG_CPU_CNT > 1)

#define IPC_SVR_ID_START(cpu_id)		(((cpu_id) & 0x03) << 6)            /* svr port number 1~15. */
#define IPC_CLIENT_ID_START(cpu_id)		((((cpu_id) & 0x03) << 6) + 15)    /* client port number 16~63. */

#define IPC_GET_ID_CPU(id)		(((id) >> 6) & 0x03)
#define IPC_GET_ID_PORT(id)		((id)  & 0x3F)

typedef enum
{
	//   servers resided in cpu0.
	CPU0_SERVER_ID_START = IPC_SVR_ID_START(0),
	FLASH_SERVER,
	
	//   servers resided in cpu1.
	CPU1_SERVER_ID_START = IPC_SVR_ID_START(1),
	
	//   servers resided in cpu2.
	CPU2_SERVER_ID_START = IPC_SVR_ID_START(2),

} mb_ipc_svr_id_t;

typedef enum
{
#ifdef CONFIG_SYS_CPU0
	//   clients resided in cpu0
	CPU0_CLIENT_ID_START = IPC_CLIENT_ID_START(0),
#endif
	
#ifdef CONFIG_SYS_CPU1
	//   clients resided in cpu1
	CPU1_CLIENT_ID_START = IPC_CLIENT_ID_START(1),
	FLASH_CLIENT,
#endif
	
#ifdef CONFIG_SYS_CPU2
	//   clients resided in cpu2
	CPU2_CLIENT_ID_START = IPC_CLIENT_ID_START(2),
	FLASH_CLIENT,
#endif

} mb_ipc_client_id_t;

#else  /* (CONFIG_CPU_CNT > 1) */

#endif  /* (CONFIG_CPU_CNT > 1) */


#ifdef __cplusplus
}
#endif

#endif /* _mb_ipc_port_cfg_h_ */

