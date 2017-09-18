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
#include "tc_internal.h"

/**
* @testcase				: tc_lwip_read_n
* @brief				:
* @scenario				:
* @apicovered			: lwip_read()
* @precondition			:
* @postcondition		:
*/
static void tc_lwip_read_n(void)
{
	int fd = -1;
	int result;

	result = lwip_read(fd, NULL, 0);
	TC_ASSERT_GEQ("lwip_read", result, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_lwip_write_n
* @brief				:
* @scenario				:
* @apicovered			: lwip_write()
* @precondition			:
* @postcondition		:
*/
static void tc_lwip_write_n(void)
{
	int fd = -1;
	int result;

	result = lwip_write(fd, NULL, 0);
	TC_ASSERT_GEQ("lwip_write", result, -1);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: lwip_read_write()
 ****************************************************************************/
int net_lwip_read_write_main(void)
{
	tc_lwip_read_n();
	tc_lwip_write_n();
	return 0;
}
