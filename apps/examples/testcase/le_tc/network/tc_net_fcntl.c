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

/// @file tc_net_fcntl.c
/// @brief Test Case Example for fcntl() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
   * @testcase		   :tc_net_fcntl_nonblock_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :fcntl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_fcntl_nonblock_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	close(fd);

	TC_ASSERT_NEQ("fcntl", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_fcntl_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :fcntl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_fcntl_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = fcntl(fd, F_SETFL, 0);
	close(fd);

	TC_ASSERT_NEQ("fcntl", ret, -1);
	TC_SUCCESS_RESULT();

}


/**
   * @testcase		   :tc_net_fcntl_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :fcntl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_fcntl_n(void)
{

	int fd = -1;
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);

	TC_ASSERT_NEQ("fcntl", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_fcntl_ndelay_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :fcntl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_fcntl_ndelay_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = fcntl(fd, F_SETFL, O_NDELAY);
	close(fd);

	TC_ASSERT_NEQ("fcntl", ret, -1);
	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: fcntl()
 ****************************************************************************/

int net_fcntl_main(void)
{

	tc_net_fcntl_nonblock_p();
	tc_net_fcntl_p();
	tc_net_fcntl_n();
	tc_net_fcntl_ndelay_p();
	return 0;
}
