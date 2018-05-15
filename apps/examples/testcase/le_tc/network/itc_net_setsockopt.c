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

/// @file itc_net_setsockopt.c
/// @brief ITC Test Case Example for setsockopt() API

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "tc_internal.h"

/**
* @fn                   :itc_net_setsockopt_n_invalid_opt_name
* @brief                :setsockopt accept opt name
* @Scenario             :sending invalid opt name
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/
static void itc_net_setsockopt_n_invalid_opt_name(int s)
{
	int ret = -1;
	int optname = 16; //invalid opt name
	void *optval = NULL;
	socklen_t optlen = 0;

	ret = setsockopt(s, SOL_SOCKET, optname, optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_rcvbuf
* @brief                :setsockopt setting flag SO_RCVBUF
* @Scenario             :setsockopt setting flag SO_RCVBUF
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/
static void itc_net_setsockopt_p_rcvbuf(int s)
{
	int ret = -1;
	int size = 1000;

	ret = setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, size);
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

#if 0
/**
* @testcase             :itc_net_setsockopt_p_reuseport
* @brief                :setsockopt setting flag SO_REUSEPORT
* @scenario             :setsockopt setting flag SO_REUSEPORT
* @apicovered           :setsockopt()
* @precondition         :socket file descriptor
* @postcondition        :None
*/
static void itc_net_setsockopt_p_reuseport(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (char *)&option, sizeof option);
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase             :itc_net_setsockopt_p_reuseaddr
* @brief                :setsockopt setting flag SO_REUSEADDR
* @scenario             :setsockopt setting flag SO_REUSEADDR
* @apicovered           :setsockopt()
* @precondition         :socket file descriptor
* @postcondition        :None
*/
static void itc_net_setsockopt_p_reuseaddr(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             :itc_net_setsockopt_p_rcvtimo
* @brief                :setsockopt setting flag SO_RCVTIMEO
* @scenario             :setsockopt setting flag SO_RCVTIMEO
* @apicovered           :setsockopt()
* @precondition         :socket file descriptor
* @postcondition        :None
*/
static void itc_net_setsockopt_p_rcvtimo(int s)
{
	int ret = -1;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_sndtimo
* @brief                :setsockopt setting flag SO_SNDTIMEO
* @Scenario             :setsockopt setting flag SO_SNDTIMEO
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/
static void itc_net_setsockopt_p_sndtimo(int s)
{
	int ret = -1;
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	ret = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_no_check
* @brief                :setsockopt setting flag SO_NO_CHECK
* @Scenario             :setsockopt setting flag SO_NO_CHECK
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/
static void itc_net_setsockopt_p_no_check(int s)
{
	int ret = -1;
	int optval = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_NO_CHECK, &optval, sizeof(optval));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_ip_ttl
* @brief                :setsockopt setting flag IP_TTL
* @Scenario             :setsockopt setting flag IP_TTL
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/
static void itc_net_setsockopt_p_ip_ttl(int s)
{
	int ret = -1;
	int optval = 1;

	ret = setsockopt(s, IPPROTO_IP, IP_TTL, &optval, sizeof(optval));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_multicast_ttl_loop
* @brief                :setsockopt setting flag IP_MULTICAST_LOOP
* @Scenario             :setsockopt setting flag IP_MULTICAST_LOOP
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/

static void itc_net_setsockopt_p_multicast_ttl_loop(int s)
{
	int ret = -1;
	int loop = 250;
	ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_net_setsockopt_p_multicast_ttl_loop_own
* @brief                :setsockopt setting flag IP_MULTICAST_LOOP
* @Scenario             :setsockopt setting flag IP_MULTICAST_LOOP
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :None
*/

static void itc_net_setsockopt_p_multicast_ttl_loop_own(int s)
{
	int ret = -1;
	int loop = 1;
	ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	TC_ASSERT_EQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
* Name: setsockopt()
****************************************************************************/

int itc_net_setsockopt_main(void)
{
	int fd = -1;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket creation failed\n");
		return 0;
	}

	itc_net_setsockopt_n_invalid_opt_name(fd);
	itc_net_setsockopt_p_rcvbuf(fd);
	//itc_net_setsockopt_p_reuseport(fd);
	itc_net_setsockopt_p_reuseaddr(fd);
	itc_net_setsockopt_p_rcvtimo(fd);
	itc_net_setsockopt_p_sndtimo(fd);
	itc_net_setsockopt_p_ip_ttl(fd);

	close(fd);
	fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("socket creation failed\n");
		return 0;
	}

	itc_net_setsockopt_p_no_check(fd);
	itc_net_setsockopt_p_multicast_ttl_loop(fd);
	itc_net_setsockopt_p_multicast_ttl_loop_own(fd);

	close(fd);
	return 0;
}
