/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/**
 * @file
 * SNMP netconn frontend.
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 */

#include <net/lwip/apps/snmp_opts.h>

#if LWIP_SNMP && SNMP_USE_NETCONN

#include <string.h>
#include <net/lwip/api.h>
#include <net/lwip/ip.h>
#include <net/lwip/udp.h>
#include <net/lwip/sys.h>
#include "snmp_msg.h"

/** SNMP netconn API worker thread */
static void snmp_netconn_thread(void *arg)
{
	struct netconn *conn;
	struct netbuf *buf;
	err_t err;
	LWIP_UNUSED_ARG(arg);

	/* Bind to SNMP port with default IP address */
#if LWIP_IPV6
	conn = netconn_new(NETCONN_UDP_IPV6);
	netconn_bind(conn, IP6_ADDR_ANY, SNMP_IN_PORT);
#else							/* LWIP_IPV6 */
	conn = netconn_new(NETCONN_UDP);
	netconn_bind(conn, IP4_ADDR_ANY, SNMP_IN_PORT);
#endif							/* LWIP_IPV6 */
	LWIP_ERROR("snmp_netconn: invalid conn", (conn != NULL), return;);

	snmp_traps_handle = conn;

	do {
		err = netconn_recv(conn, &buf);

		if (err == ERR_OK) {
			snmp_receive(conn, buf->p, &buf->addr, buf->port);
		}

		if (buf != NULL) {
			netbuf_delete(buf);
		}
	} while (1);
}

err_t snmp_sendto(void *handle, struct pbuf *p, const ip_addr_t *dst, u16_t port)
{
	err_t result;
	struct netbuf buf;

	memset(&buf, 0, sizeof(buf));
	buf.p = p;
	result = netconn_sendto((struct netconn *)handle, &buf, dst, port);

	return result;
}

u8_t snmp_get_local_ip_for_dst(void *handle, const ip_addr_t *dst, ip_addr_t *result)
{
	struct netconn *conn = (struct netconn *)handle;
	struct netif *dst_if;
	const ip_addr_t *dst_ip;

	LWIP_UNUSED_ARG(conn);		/* unused in case of IPV4 only configuration */

	ip_route_get_local_ip(&conn->pcb.udp->local_ip, dst, dst_if, dst_ip);

	if ((dst_if != NULL) && (dst_ip != NULL)) {
		ip_addr_copy(*result, *dst_ip);
		return 1;
	} else {
		return 0;
	}
}

/**
 * Starts SNMP Agent.
 */
void snmp_init(void)
{
	sys_thread_new("snmp_netconn", snmp_netconn_thread, NULL, SNMP_STACK_SIZE, SNMP_THREAD_PRIO);
}

#endif							/* LWIP_SNMP && SNMP_USE_NETCONN */
