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

/**
* @testcase					: tc_net_decisec_dsec2tick_p
* @brief					:
* @scenario					:
* @apicovered				: net_dsec2tick
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_dsec2tick_p(void)
{
	int ret = -1;
	int dsec = 1000;

	ret = net_dsec2tick(dsec);
	TC_ASSERT_NEQ("time", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_net_decisec_dsec2tick_n
* @brief					:
* @scenario					:
* @apicovered				: net_dsec2tick
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_dsec2tick_n(void)
{
	int ret = -1;
	int dsec = 0;

	ret = net_dsec2tick(dsec);
	TC_ASSERT_EQ("time", ret, -1);
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
	struct timeval *tv;
	int dsec = 1000;
	tv = (struct timeval *)malloc(sizeof(struct timeval));

	net_dsec2timeval(dsec, tv);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_net_decisec_timeval2dsec_p
* @brief					:
* @scenario					:
* @apicovered				: net_timeval2dsec
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_timeval2dsec_p(void)
{
	unsigned int ret;
	uint16_t remainder;
	struct timeval *tv;
	int dsec = 1000;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	tv->tv_sec = dsec / DSEC_PER_SEC;
	remainder = dsec - tv->tv_sec * DSEC_PER_SEC;
	tv->tv_usec = remainder * USEC_PER_DSEC;

	ret = net_timeval2dsec(tv);
	TC_ASSERT_NEQ("time", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_net_decisec_timeval2dsec_n
* @brief					:
* @scenario					:
* @apicovered				: net_timeval2dsec
* @precondition				:
* @postcondition			:
*/
static void tc_net_decisec_timeval2dsec_n(void)
{
	unsigned int ret;
	struct timeval *tv;
	uint16_t remainder = 0;
	int dsec = 1000;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	tv->tv_sec = dsec / DSEC_PER_SEC;
	tv->tv_usec = remainder * USEC_PER_DSEC;

	ret = net_timeval2dsec(tv);
	TC_ASSERT_EQ("time", ret, 0);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: decisec()
 ****************************************************************************/
int net_decisec_time_main(void)
{
	tc_net_decisec_dsec2tick_p();
	tc_net_decisec_dsec2tick_n();
	tc_net_decisec_dsec2timeval();
	tc_net_decisec_timeval2dsec_p();
	tc_net_decisec_timeval2dsec_n();
	return 0;
}
