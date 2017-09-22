/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file network_tc_main.c

/// @brief Main Function for Network TestCase Example

#include <tinyara/config.h>
#include <stdio.h>
#include <semaphore.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "tc_internal.h"

extern sem_t tc_sem;
extern int working_tc;

/****************************************************************************
 * Name: network_tc_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int network_tc_main(int argc, char *argv[])
#endif
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	int sock_tcp1 = socket(AF_INET, SOCK_STREAM, 0);
	int sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	printf("=== TINYARA Network TC START! ===\n");

#ifdef CONFIG_TC_NET_SOCKET
	net_socket_main();
#endif
#ifdef CONFIG_TC_NET_SETSOCKOPT
	net_setsockopt_main();
#endif
#ifdef CONFIG_TC_NET_CONNECT
	net_connect_main();
#endif
#ifdef CONFIG_TC_NET_CLOSE
	net_close_main();
#endif
#ifdef CONFIG_TC_NET_BIND
	net_bind_main();
#endif
#ifdef CONFIG_TC_NET_LISTEN
	net_listen_main();
#endif
#ifdef CONFIG_TC_NET_GETSOCKNAME
	net_getsockname_main();
#endif
#ifdef CONFIG_TC_NET_GETSOCKOPT
	net_getsockopt_main();
#endif
#ifdef CONFIG_TC_NET_FCNTL
	net_fcntl_main(sock_tcp);
#endif
#ifdef CONFIG_TC_NET_IOCTL
	net_ioctl_main(sock_tcp);
#endif
#ifdef CONFIG_TC_NET_ACCEPT
	net_accept_main();
#endif
#ifdef CONFIG_TC_NET_SEND
	net_send_main();
#endif
#ifdef CONFIG_TC_NET_RECV
	net_recv_main();
#endif
#ifdef CONFIG_TC_NET_SENDTO
	net_sendto_main();
#endif
#ifdef CONFIG_TC_NET_RECVFROM
	net_recvfrom_main();
#endif
#ifdef CONFIG_TC_NET_SHUTDOWN
	net_shutdown_main();
#endif
#ifdef CONFIG_TC_NET_INET
	net_inet_main();
#endif
#ifdef CONFIG_TC_NET_ETHER
	net_ether_main(sock_udp);
#endif
#ifdef CONFIG_TC_NET_NETDB
	net_netdb_main();
#endif
#ifdef CONFIG_TC_NET_IPV4_NETMASK_VALID
	net_ipv4_addr_netmask_valid_main();
#endif
#ifdef CONFIG_TC_NETDEV_CARRIER
	netdev_carrier_main();
#endif
#ifdef CONFIG_TC_NET_DECISEC_TIME
	net_decisec_time_main();
#endif
#ifdef CONFIG_TC_NET_API
	net_api_main();
#endif
#ifdef CONFIG_TC_NET_TCPIP
	net_tcpip_main();
#endif
#ifdef CONFIG_TC_NET_MAC
	net_mac_main(sock_tcp, sock_tcp1);
#endif
#ifdef CONFIG_TC_NETBUF_ALLOC
	netbuf_alloc_main();
#endif
#ifdef CONFIG_TC_MEM_ALLOC
	net_mem_allocate_main();
#endif
#ifdef CONFIG_TC_NET_NETBUF
	tc_net_netbuf_main();
#endif
#ifdef CONFIG_NET_NETIFAPI
	net_netifapi_main();
#endif
#ifdef CONFIG_TC_NET_GETPEERNAME
	net_getpeername_main();
#endif
#ifdef CONFIG_TC_NET_SELECT
	net_select_main();
#endif
#ifdef CONFIG_TC_NET_CORE
	net_core_main(sock_tcp);
#endif
	close(sock_tcp);
	close(sock_tcp1);
	close(sock_udp);
	printf("\n=== TINYARA Network TC COMPLETE ===\n");
	printf("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);
	return 0;
}
