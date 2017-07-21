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

/// @file tc_internal.h

/// @brief Header file for Network TestCase Example
#ifndef __EXAMPLES_TESTCASE_NETWORK_TC_INTERNAL_H
#define __EXAMPLES_TESTCASE_NETWORK_TC_INTERNAL_H

#define RETURN_ERR return

#include "tc_common.h"

extern int total_pass;
extern int total_fail;

/**********************************************************
* TC Function Declarations
**********************************************************/

#ifdef CONFIG_TC_NET_SOCKET
int net_socket_main(void);
#endif
#ifdef CONFIG_TC_NET_GETSOCKOPT
void net_getsockopt_main(void);
#endif
#ifdef CONFIG_TC_NET_SETSOCKOPT
int net_setsockopt_main(void);
#endif
#ifdef CONFIG_TC_NET_CONNECT
int net_connect_main(void);
#endif
#ifdef CONFIG_TC_NET_CLOSE
int net_close_main(void);
#endif
#ifdef CONFIG_TC_NET_BIND
int net_bind_main(void);
#endif
#ifdef CONFIG_TC_NET_LISTEN
int net_listen_main(void);
#endif
#ifdef CONFIG_TC_NET_GETSOCKNAME
int net_getsockname_main(void);
#endif
#ifdef CONFIG_TC_NET_FCNTL
int net_fcntl_main(void);
#endif
#ifdef CONFIG_TC_NET_IOCTL
int net_ioctl_main(void);
#endif
#ifdef CONFIG_TC_NET_ACCEPT
int net_accept_main(void);
#endif
#ifdef CONFIG_TC_NET_SEND
int net_send_main(void);
#endif
#ifdef CONFIG_TC_NET_RECV
int net_recv_main(void);
#endif
#ifdef CONFIG_TC_NET_GETPEERNAME
int net_getpeername_main(void);
#endif
#ifdef CONFIG_TC_NET_SENDTO
int net_sendto_main(void);
#endif
#ifdef CONFIG_TC_NET_RECVFROM
int net_recvfrom_main(void);
#endif
#ifdef CONFIG_TC_NET_SHUTDOWN
int net_shutdown_main(void);
#endif
#ifdef CONFIG_TC_NET_SELECT
int net_select_main(void);
#endif
#ifdef CONFIG_TC_NET_CORE_NTOHS
int net_lwip_ntohs_main(void);
#endif
#ifdef CONFIG_TC_NET_IPV4_NETMASK_VALID
int net_ipv4_addr_netmask_valid_main(void);
#endif
#ifdef CONFIG_TC_NETDEV_CARRIER
int netdev_carrier_main(void);
#endif
#ifdef CONFIG_TC_NET_DECISEC_TIME
int net_decisec_time_main(void);
#endif
#ifdef CONFIG_TC_NET_API
int net_api_main(void);
#endif
#ifdef CONFIG_TC_NET_CORE
int net_core_main(void);
#endif

#endif /* __EXAMPLES_TESTCASE_NETWORK_TC_INTERNAL_H */
