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

/// @file tc_net_decisec_time.c
/// @brief Test Case Example for decisec() API
#include <tinyara/clock.h>

#include "tc_internal.h"

#define DESISEC 1000

/**
* @testcase					: tc_net_decisec_dsec2tick
* @brief					:
* @scenario					:
* @apicovered				: net_dsec2tick
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_dsec2tick(void)
{
	int ret;
	int dsec = DESISEC;

	ret = net_dsec2tick(dsec);
	TC_ASSERT_GEQ("time", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_net_decisec_dsec2timeval
* @brief					:
* @scenario					:
* @apicovered				: net_dsec2timeval
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_dsec2timeval(void)
{
	int val, tv_sec;
	int dsec = DESISEC;
	struct timeval tv;

	val = dsec / DSEC_PER_SEC;
	net_dsec2timeval(dsec, &tv);
	tv_sec = tv.tv_sec;

	TC_ASSERT_EQ("time", val, tv_sec);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_net_decisec_timeval2dsec
* @brief					:
* @scenario					:
* @apicovered				: net_timeval2dsec
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_timeval2dsec(void)
{
	int dsec = DESISEC;
	unsigned int ret;
	uint16_t remainder;
	struct timeval tv;

	tv.tv_sec = dsec / DSEC_PER_SEC;
	remainder = dsec - tv.tv_sec * DSEC_PER_SEC;
	tv.tv_usec = remainder * USEC_PER_DSEC;

	ret = net_timeval2dsec(&tv);

	TC_ASSERT_GEQ("time", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: decisec()
 ****************************************************************************/
int net_decisec_time_main(void)
{
	tc_net_decisec_dsec2tick();
	tc_net_decisec_dsec2timeval();
	tc_net_decisec_timeval2dsec();
	return 0;
}
