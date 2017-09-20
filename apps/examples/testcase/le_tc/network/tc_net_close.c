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

/// @file tc_net_socket.c
/// @brief Test Case Example for close() API
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

/**
* @testcase            : tc_net_close_p
* @brief               : This close API close a file descriptor.
* @scenario            : Closes a file descriptor, so that it no longer refers to any file and may be reused.
* @apicovered          : close()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_close_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = close(fd);

	TC_ASSERT_NEQ("close", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_close_n
* @brief               : This close API close a file descriptor.
* @scenario            : Closes a file descriptor, so that it no longer refers to any file and may be reused.
* @apicovered          : close()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_close_n(void)
{
	int ret = close(NEG_VAL);

	TC_ASSERT_NEQ("close", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: close()
 ****************************************************************************/
int net_close_main(void)
{
	tc_net_close_p();
	tc_net_close_n();

	return 0;
}
