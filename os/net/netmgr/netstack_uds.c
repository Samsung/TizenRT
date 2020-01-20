/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include "netstack.h"
#include "local/uds_net.h"

struct netstack_ops g_uds_stack_ops = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	NULL,
	NULL,
	NULL,
	uds_checksd,
	NULL,
	NULL,
	NULL,

	uds_socket,
	uds_bind,
	uds_connect,
	uds_accept,
	uds_listen,
	NULL,

	uds_recv,
	uds_recvfrom,
	NULL,
	uds_send,
	uds_sendto,
	NULL,

	uds_getsockname,
	uds_getpeername,
	NULL,
	NULL,
#ifdef CONFIG_NET_ROUTE
	NULL,
	NULL,
#endif
#ifdef CONFIG_NET_NETMON
	NULL,
#endif
};

struct netstack g_uds_stack = {&g_uds_stack_ops, NULL};

struct netstack *get_netstack_uds(void)
{
	return &g_uds_stack;
}
