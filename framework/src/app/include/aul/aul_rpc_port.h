/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __AUL_RPC_PORT_H__
#define __AUL_RPC_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define AUL_K_RPC_PORT "__K_RPC_PORT"

int aul_rpc_port_prepare_stub(const char *appid, const char *port_name);

int aul_rpc_port_create_socket_pair(const char *appid, const char *port_name,
		int (*fds)[2]);

int aul_rpc_port_notify_rpc_finished(void);

#ifdef __cplusplus
}
#endif

#endif /* __AUL_RPC_PORT_H__ */
