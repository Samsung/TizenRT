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

///@file tc_net_dup.c
///@brief Test Case Example for dup() API

#include <sys/socket.h>
#include "tc_internal.h"


/**
* @testcase         :tc_net_dup_n
* @brief            :clone a file descriptor to arbitrary descriptor
* @scenario         :dup file descriptor with invalid socket descriptor range
* @apicovered       :dup()
* @precondition     :none
* @postcondition    :none
*/
static void tc_net_dup_n(void)
{
	/* the sock value is over valid socket descriptors range*/
	/* so dup() check that it is the wrong descriptor and will generate an error*/
	int sock = CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS;
	int ret = dup(sock);
	TC_ASSERT_EQ("dup", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         :tc_net_dup_p
* @brief            :clone a file descriptor to arbitrary descriptor
* @scenario         :open socket and clone a file descriptor to arbitrary descriptor
* @apicovered       :dup()
* @precondition     :open socket()
* @postcondition    :close socket descriptor
*/
static void tc_net_dup_p(void)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", sock, 0);

	int fd = dup(sock);
	TC_ASSERT_GT_CLEANUP("dup", fd, 0, close(sock));

	int ret = close(sock);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :tc_net_dup_multiple_time_p
* @brief            :clone a file descriptor to arbitrary descriptor
* @scenario         :open socket and clone a file descriptor to arbitrary descriptor multiple times
* @apicovered       :dup()
* @precondition     :open socket()
* @postcondition    :close socket descriptor
*/
static void tc_net_dup_multiple_time_p(void)
{
	int index;
	int loop_count = 10;
	int sock;
	int ret;
	int fd;

	for (index = 0; index < loop_count; index++) {
		sock = socket(AF_INET, SOCK_STREAM, 0);
		TC_ASSERT_GEQ("socket", sock, 0);

		fd = dup(sock);
		TC_ASSERT_GT_CLEANUP("dup", fd, 0, close(sock));

		ret = close(sock);
		TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(fd));

		ret = close(fd);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :tc_net_dup2_n
* @brief            :clone a file descriptor to specific descriptor
* @scenario         :dup file descriptor to specific descriptor with invalid socket descriptor range
* @apicovered       :dup2()
* @precondition     :none
* @postcondition    :none
*/
static void tc_net_dup2_n(void)
{
	/* the sock value is over valid socket descriptors range*/
	/* so dup() check that it is the wrong descriptor and will generate an error*/
	int sock_old = CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS;
	int sock_new = 0;
	int ret = dup2(sock_old, sock_new);
	TC_ASSERT_EQ("dup2", ret, -1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :tc_net_dup2_p
* @brief            :clone a file descriptor to specific descriptor
* @scenario         :open socket twice to get 2 socket descriptor and clone a file descriptor to specific descriptor
* @apicovered       :dup2()
* @precondition     :open socket()
* @postcondition    :close socket descriptors
*/
static void tc_net_dup2_p(void)
{
	int sock_old = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", sock_old, 0);

	int sock_new = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", sock_new, 0);

	int ret = dup2(sock_old, sock_new);
	TC_ASSERT_EQ_CLEANUP("dup2", ret, 0, close(sock_old); close(sock_new));

	ret = close(sock_old);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(sock_new));

	ret = close(sock_new);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :tc_net_dup2_multiple_time_p
* @brief            :clone a file descriptor to specific descriptor
* @scenario         :open socket twice to get 2 socket descriptor and clone a file descriptor to specific descriptor
* @apicovered       :dup2()
* @precondition     :open socket()
* @postcondition    :close socket descriptors
*/
static void tc_net_dup2_multiple_time_p(void)
{
	int index;
	int loop_count = 10;
	int sock_old;
	int sock_new;
	int ret;

	for (index = 0; index < loop_count; index++) {
		sock_old = -1;
		sock_old = socket(AF_INET, SOCK_STREAM, 0);
		TC_ASSERT_GEQ("socket", sock_old, 0);

		sock_new = -1;
		sock_new = socket(AF_INET, SOCK_DGRAM, 0);
		TC_ASSERT_GEQ("socket", sock_new, 0);

		ret = dup2(sock_old, sock_new);
		TC_ASSERT_EQ_CLEANUP("dup2", ret, 0, close(sock_old); close(sock_new));

		ret = close(sock_old);
		TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(sock_new));

		ret = close(sock_new);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
* Name: dup()
****************************************************************************/

int net_dup_main(void)
{
	tc_net_dup_n();
	tc_net_dup_p();
	tc_net_dup_multiple_time_p();
	tc_net_dup2_n();
	tc_net_dup2_p();
	tc_net_dup2_multiple_time_p();

	return 0;
}
