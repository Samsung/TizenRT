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

///@file tc_net_fcntl.c
///@brief Test Case Example for fcntl() API

#include <sys/socket.h>
#include "tc_internal.h"

/**
* @testcase          :tc_net_fcntl_nonblock_p
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag F_SETFL and O_NONBLOCK
* @apicovered        :fcntl()
* @precondition      :socket open
* @postcondition     :socket close
*/
static void tc_net_fcntl_nonblock_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0);

	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	TC_ASSERT_NEQ_CLEANUP("fcntl", ret, -1, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :tc_net_fcntl_p
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag F_SETFL
* @apicovered        :fcntl()
* @precondition      :open socket
* @postcondition     :close socket
*/
static void tc_net_fcntl_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0);

	int ret = fcntl(fd, F_SETFL, 0);
	TC_ASSERT_NEQ_CLEANUP("fcntl", ret, -1, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :tc_net_fcntl_dupfd_p
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag F_DUPFD which duplicates the file descriptor
* @apicovered        :fcntl()
* @precondition      :open socket
* @postcondition     :close socket
*/
static void tc_net_fcntl_dupfd_p(void)
{
	int ret;
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0);

	int nfd = fcntl(fd, F_DUPFD, 0);
	TC_ASSERT_GEQ_CLEANUP("fcntl", nfd, fd, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(nfd));

	ret = close(nfd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :tc_net_fcntl_dupfd_nonblock_p
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag O_NONBLOCK and F_DUPFD which duplicates the file descriptor
* @apicovered        :fcntl()
* @precondition      :socket open
* @postcondition     :socket close
*/
static void tc_net_fcntl_dupfd_nonblock_p(void)
{
	int ret;
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0);

	int nfd = fcntl(fd, F_DUPFD, O_NONBLOCK);
	TC_ASSERT_GEQ_CLEANUP("fcntl", nfd, fd, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(nfd));

	ret = close(nfd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :tc_net_fcntl_n
* @brief             :manipulate file descriptor
* @scenario          :call fcntl with invalid descriptor
* @apicovered        :fcntl()
* @precondition      :none
* @postcondition     :none
*/
static void tc_net_fcntl_n(void)
{
	int fd = -1;
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	TC_ASSERT_NEQ("fcntl", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :tc_net_fcntl_ndelay_p
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag F_SETFL and O_NDELAY
* @apicovered        :fcntl()
* @precondition      :open socket
* @postcondition     :close socket descriptor
*/
static void tc_net_fcntl_ndelay_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0);

	int ret = fcntl(fd, F_SETFL, O_NDELAY);
	TC_ASSERT_NEQ_CLEANUP("fcntl", ret, -1, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: fcntl()
 ****************************************************************************/

int net_fcntl_main(void)
{
	tc_net_fcntl_nonblock_p();
	tc_net_fcntl_p();
	tc_net_fcntl_dupfd_p();
	tc_net_fcntl_dupfd_nonblock_p();
	tc_net_fcntl_n();
	tc_net_fcntl_ndelay_p();

	return 0;
}
