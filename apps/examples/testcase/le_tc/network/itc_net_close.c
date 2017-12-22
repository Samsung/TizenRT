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

///file itc_net_close.c
///brief Test Case Example for close() API

#include <sys/socket.h>
#include "tc_internal.h"

#define LOOP_COUNT 10

/**
* @testcase          :itc_net_close_p_multi_times
* @brief             :close file descriptor
* @scenario          :open socket and close file descriptor multiple times
* @apicovered        :close()
* @precondition      :open socket
* @postcondition     :none
*/
static void itc_net_close_p_multi_times(void)
{
	int fd = -1;
	int index = 0;
	int ret;

	for (index = 0; index < LOOP_COUNT; index++) {
		fd = -1;
		fd = socket(AF_INET, SOCK_STREAM, 0);
		TC_ASSERT_GEQ("socket", fd, CONFIG_NFILE_DESCRIPTORS);//If ASSERT FAILS, no need to close(fd) as socket is not created

		ret = close(fd);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :itc_net_close_n_reclose
* @brief             :close file descriptor
* @scenario          :open socket and close file descriptor and then reclose file descriptor
* @apicovered        :close()
* @precondition      :open socket
* @postcondition     :none
*/
static void itc_net_close_n_reclose(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, CONFIG_NFILE_DESCRIPTORS);

	int ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	ret = close(fd);
	TC_ASSERT_LT("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: close()
 ****************************************************************************/

int itc_net_close_main(void)
{
	itc_net_close_p_multi_times();
	itc_net_close_n_reclose();

	return 0;
}
