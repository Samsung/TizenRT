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

/// @file tc_net_ioctl.c
/// @brief Test Case Example for ioctl() API
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
#include <sys/ioctl.h>

#include "tc_internal.h"


/**
   * @testcase		   :tc_net_ioctl_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :ioctl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_ioctl_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int dummy = 0;
	int ret = ioctl(fd, FIONBIO, (unsigned long)&dummy);
	close(fd);

	TC_ASSERT_NEQ("ioctl", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_ioctl_fionread_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :ioctl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_ioctl_fionread_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int dummy = 10;
	int ret = ioctl(fd, FIONREAD, (unsigned long)&dummy);
	close(fd);

	TC_ASSERT_NEQ("ioctl", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_ioctl_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :ioctl()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_ioctl_n(void)
{

	int fd = -1;
	int ret = ioctl(fd, FIONBIO, 0);

	TC_ASSERT_NEQ("ioctl", ret, 0);
	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: ioctl()
 ****************************************************************************/

int net_ioctl_main(void)
{

	tc_net_ioctl_p();
	tc_net_ioctl_fionread_p();

	tc_net_ioctl_n();

	return 0;
}
