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

///@file itc_net_fcntl.c
///@brief Test Case Example for fcntl() API

#include <sys/socket.h>
#include "tc_internal.h"

/**
* @testcase          :itc_net_fcntl_p_dupfd
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag F_DUPFD which duplicates the file descriptor
* @apicovered        :fcntl()
* @precondition      :open socket
* @postcondition     :close socket
*/
static void itc_net_fcntl_p_dupfd(void)
{
	int ret;
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);//If ASSERT FAILS, no need to close(fd) as socket is not created
	TC_ASSERT_GEQ("socket", fd, CONFIG_NFILE_DESCRIPTORS);

	int nfd = fcntl(fd, F_DUPFD, 0);
	TC_ASSERT_GEQ_CLEANUP("fcntl", nfd, CONFIG_NFILE_DESCRIPTORS, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(nfd));

	ret = close(nfd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :itc_net_fcntl_p_dupfd_nonblock
* @brief             :manipulate file descriptor
* @scenario          :open socket and call fcntl with flag O_NONBLOCK and F_DUPFD which duplicates the file descriptor
* @apicovered        :fcntl()
* @precondition      :socket open
* @postcondition     :socket close
*/
static void itc_net_fcntl_p_dupfd_nonblock(void)
{
	int ret;
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, CONFIG_NFILE_DESCRIPTORS);

	int nfd = fcntl(fd, F_DUPFD, O_NONBLOCK);
	TC_ASSERT_GEQ_CLEANUP("fcntl", nfd, CONFIG_NFILE_DESCRIPTORS, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(nfd));

	ret = close(nfd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: fcntl()
 ****************************************************************************/

int itc_net_fcntl_main(void)
{
	itc_net_fcntl_p_dupfd();
	itc_net_fcntl_p_dupfd_nonblock();

	return 0;
}
