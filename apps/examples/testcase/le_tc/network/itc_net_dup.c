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

///@file itc_net_dup.c
///@brief Test Case Example for dup() API

#include <sys/socket.h>
#include "tc_internal.h"

#define LOOP_COUNT 10

/**
* @testcase         :itc_net_dup_p_multiple_time
* @brief            :clone a file descriptor to arbitrary descriptor
* @scenario         :open socket and clone a file descriptor to arbitrary descriptor multiple times
* @apicovered       :dup()
* @precondition     :open socket()
* @postcondition    :close socket descriptor
*/
static void itc_net_dup_p_multiple_time(void)
{
	int index;
	int sock;
	int ret;
	int fd;

	for (index = 0; index < LOOP_COUNT; index++) {
		sock = socket(AF_INET, SOCK_STREAM, 0);
		TC_ASSERT_GEQ("socket", sock, CONFIG_NFILE_DESCRIPTORS);//If ASSERT FAILS, no need to close(sock) as socket is not created

		fd = dup(sock);
		TC_ASSERT_GT_CLEANUP("dup", fd, CONFIG_NFILE_DESCRIPTORS, close(sock));

		ret = close(sock);
		TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(fd));

		ret = close(fd);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :itc_net_dup2_p_multiple_time
* @brief            :clone a file descriptor to specific descriptor
* @scenario         :open socket twice to get 2 socket descriptor and clone a file descriptor to specific descriptor
* @apicovered       :dup2()
* @precondition     :open socket()
* @postcondition    :close socket descriptors
*/
static void itc_net_dup2_p_multiple_time(void)
{
	int index;
	int sock_old;
	int sock_new;
	int ret;

	for (index = 0; index < LOOP_COUNT; index++) {
		sock_old = -1;
		sock_old = socket(AF_INET, SOCK_STREAM, 0);
		TC_ASSERT_GEQ("socket", sock_old, CONFIG_NFILE_DESCRIPTORS);

		sock_new = -1;
		sock_new = socket(AF_INET, SOCK_DGRAM, 0);
		TC_ASSERT_GEQ_CLEANUP("socket", sock_new, CONFIG_NFILE_DESCRIPTORS, close(sock_old));

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

int itc_net_dup_main(void)
{
	itc_net_dup_p_multiple_time();
	itc_net_dup2_p_multiple_time();

	return 0;
}
