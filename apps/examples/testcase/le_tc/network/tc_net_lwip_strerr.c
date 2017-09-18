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

/// @file tc_net_lwip_strerr.c
/// @brief Test Case Example for lwip_strerr() API
#include <net/lwip/err.h>

#include "tc_internal.h"

/**
* @testcase				: tc_net_lwip_strerr_p
* @brief				:
* @scenario				:
* @apicovered			: lwip_strerr
* @precondition			:
* @postcondition		:
*/
static void tc_net_lwip_strerr_p(void)
{
	s8_t err = 0;
	int ret;
	char comp[] = "Ok.";

	ret = strcmp (comp, lwip_strerr(err));
	TC_ASSERT_EQ("strerr", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_lwip_strerr_n
* @brief				:
* @scenario				:
* @apicovered			: lwip_strerr
* @precondition			:
* @postcondition		:
*/
static void tc_net_lwip_strerr_n(void)
{
	s8_t err = -1;
	int ret;
	char comp[] = "Ok.";

	ret = strcmp (comp, lwip_strerr(err));
	TC_ASSERT_NEQ("strerr", ret, 0);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: lwip_strerr()
 ****************************************************************************/
int net_lwip_strerr_main(void)
{
	tc_net_lwip_strerr_p();
	tc_net_lwip_strerr_n();
	return 0;
}
