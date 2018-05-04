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

/// @file tc_net_dup.c
/// @brief Test Case Example for dup() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>

#include "tc_internal.h"

#define UTC_NET_LOGE printf

/**
 * @testcase         :tc_net_dup_n
 * @brief            :
 * @scenario         :
 * @apicovered       :dup()
 * @precondition     :socket()
 * @postcondition    :
 */
static void tc_net_dup_n(void)
{
	/* the sock value is over valid socket descriptors range */
	/* so dup() check that it is the wrong descriptor and will generate an error*/
	int sock = CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS;
	int fd = dup(sock);
	TC_ASSERT_EQ("dup", fd, -1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         :tc_net_dup_p
 * @brief            :
 * @scenario         :
 * @apicovered       :dup()
 * @precondition     :socket()
 * @postcondition    :
 */
static void tc_net_dup_p(void)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		UTC_NET_LOGE("get socket error (%d)\n", errno);
		return;
	}
	int fd = dup(sock);
	if (fd < 0) {
		printf("socket dup error line:%d\n", __LINE__);
		close(sock);
		return;
	}
	close(sock);
	close(fd);
	TC_ASSERT_GT("dup", fd, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         :tc_net_dup2_n
 * @brief            :
 * @scenario         :
 * @apicovered       :dup2()
 * @precondition     :socket()
 * @postcondition    :
 */
static void tc_net_dup2_n(void)
{
	int sock_old = CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS;
	int sock_new = 0;
	int res = dup2(sock_old, sock_new);
	TC_ASSERT_EQ("dup2", res, -1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         :tc_net_dup2_p
 * @brief            :
 * @scenario         :
 * @apicovered       :dup2()
 * @precondition     :socket()
 * @postcondition    :
 */
static void tc_net_dup2_p(void)
{
	int sock_old = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_old == -1) {
		UTC_NET_LOGE("get old socket error (%d)\n", errno);
		return;
	}
	int sock_new = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_new == -1) {
		UTC_NET_LOGE("get new socket error (%d)\n", errno);
		close(sock_old);
		return;
	}
	int res = dup2(sock_old, sock_new);
	close(sock_old);
	close(sock_new);
	TC_ASSERT_EQ("dup2", res, 0);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dup()
 ****************************************************************************/

int net_dup_main(void)
{
	tc_net_dup_n();
	tc_net_dup_p();
	tc_net_dup2_n();
	tc_net_dup2_p();

	return 0;
}
