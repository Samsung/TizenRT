/****************************************************************************

*
* Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file itc_net_netdb.c
/// @brief ITC Test Case Example for getaddrinfo() API

#include <sys/types.h>
#include <sys/socket.h>
#include <tinyara/config.h>
#ifdef CONFIG_NET_LWIP_NETDB
#include <netdb.h>
#endif

#include "tc_internal.h"

#ifdef CONFIG_NET_LWIP_NETDB

/**
* @testcase          :itc_net_netdb_getaddrinfo_n
* @brief             :getaddrinfo() returns one or more addrinfo structures
* @scenario          :passing invalid node and service
* @apicovered        :getaddrinfo()
* @precondition      :None
* @postcondition     :None
*/
static void itc_net_netdb_getaddrinfo_n(void)
{

	struct addrinfo hints;
	struct addrinfo *res;
	int ret;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(NULL, NULL, &hints, &res);
	TC_ASSERT_NEQ("getaddrinfo", ret, 0);
	TC_SUCCESS_RESULT();
}

#endif

/****************************************************************************
* Name: getaddrinfo()
****************************************************************************/

int itc_net_netdb_main(void)
{
#ifdef CONFIG_NET_LWIP_NETDB
	itc_net_netdb_getaddrinfo_n();
#endif
	return 0;
}
