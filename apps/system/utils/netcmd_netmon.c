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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <queue.h>

#include <netinet/in.h>
#include <net/if.h>

#include <netutils/netlib.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#ifdef CONFIG_NET_IPv6
#define PRINT_IPV(sock)										\
	do {													\
		if (sock->type & NETMON_TYPE_IPV6) {				\
			printf("IPv6\t");								\
			printf("%-15s:", inet6_ntoa(sock->local_ip));	\
			if (sock->local_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->local_port);			\
			}												\
			printf("%-15s:", inet6_ntoa(sock->remote_ip));	\
			if (sock->remote_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->remote_port);			\
			}												\
		} else {											\
			printf("IPv4\t");								\
			printf("%-15s:", inet_ntoa(sock->local_ip));	\
			if (sock->local_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->local_port);			\
			}												\
			printf("%-15s:", inet_ntoa(sock->remote_ip));	\
			if (sock->remote_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->remote_port);			\
			}												\
		}													\
	} while (0)
#else
#define PRINT_IPV(sock)									\
	do {												\
		printf("IPv4\t");								\
		printf("%-15s:", inet_ntoa(sock->local_ip));	\
		if (sock->local_port == 0) {					\
			printf("%-8s", "*");						\
		} else {										\
			printf("%-8d", sock->local_port);			\
		}												\
		printf("%-15s:", inet_ntoa(sock->remote_ip));	\
		if (sock->remote_port == 0) {					\
			printf("%-8s", "*");						\
		} else {										\
			printf("%-8d", sock->remote_port);			\
		}												\
	} while (0)
#endif

/****************************************************************************
* Global Data
****************************************************************************/


/****************************************************************************
* Private Functions
****************************************************************************/
/**
 * Print a externally used socket information.
 */
static void print_socket(sq_queue_t *q_sock)
{
	int i = 1;
	struct netmon_sock *sock_info = (struct netmon_sock *) sq_remfirst(q_sock);
	printf("\nSockfd\tProto\tType\tLocal\t\t\tRemote\t\t\tstate\tPID\n");
	while (sock_info) {
		printf("%-8d", i);
		/* Protocol */
		if (sock_info->type & NETMON_TCP) {
			printf("TCP\t");
			PRINT_IPV(sock_info);
		} else if (sock_info->type & NETMON_UDP) {
			printf("UDP\t");
			PRINT_IPV(sock_info);
		} else {
			printf("RAW\t");
			PRINT_IPV(sock_info);
		}
		switch (sock_info->state) {
		case NETMON_WRITE:
			printf("WRITE\t");
			break;
		case NETMON_LISTEN:
			printf("LISTEN\t");
			break;
		case NETMON_CONNECT:
			printf("CONNECT\t");
			break;
		case NETMON_CLOSE:
			printf("CLOSE\t");
			break;
		default:
			printf("NONE\t");
			break;
		}
		printf("%d:%s\n", sock_info->pid, sock_info->pid_name);
		i++;
		free(sock_info);
		sock_info = (struct netmon_sock *) sq_remfirst(q_sock);
	}

	return ;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int cmd_netmon(int argc, char **argv)
{
	sq_queue_t q_data;
	sq_init(&q_data);
	int ret;
	if (argc > 2) {
		printf("Bad input\n");
	} else if (argc == 1) {
		/* Get default information: SIOCGETSOCK (default) */
		ret = netlib_netmon_sock(&q_data);
		if (!ret) {
			/* Free sq_queue entry of netmon_sock in print_socket() */
			print_socket(&q_data);
		} else {
			printf("Failed to fetch socket info.\n");
		}
	} else if (!(strncmp(argv[1], "sock", strlen("sock")))) {
		/* Get socket information: SIOCGETSOCK */
		ret = netlib_netmon_sock(&q_data);
		if (!ret) {
			/* Free sq_queue entry of netmon_sock in print_socket() */
			print_socket(&q_data);
		} else {
			printf("Failed to fetch socket info.\n");
		}
	} else if (!(strncmp(argv[1], "wifi", strlen("wifi")))) {
		/* Get socket information: SIOCGETWIFI */
		printf("wifi option will be supported\n");
	} else {
		printf("No such an option\n");
	}

	return OK;
}

