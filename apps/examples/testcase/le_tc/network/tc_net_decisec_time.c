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

#define DESISEC    1000

/**
* @testcase            :tc_net_decisec_dsec2tick
* @brief               :convert a decisecond value to a system clock ticks.
* @scenario            :convert a decisecond value to a system clock ticks with dsec value.
* @apicovered          :net_dsec2tick
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_decisec_dsec2tick(void)
{
	int ret = net_dsec2tick(DESISEC);

	TC_ASSERT_GEQ("time", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_decisec_dsec2timeval
* @brief               :convert a decisecond timeout value to a struct timeval
* @scenario            :convert a decisecond timeout value to a struct timeval with dsec value.
* @apicovered          :net_dsec2timeval
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_decisec_dsec2timeval(void)
{
	int ret, tv_sec;
	struct timeval tv;

	ret = DESISEC / DSEC_PER_SEC;
	net_dsec2timeval(DESISEC, &tv);
	tv_sec = tv.tv_sec;

	TC_ASSERT_EQ("time", ret, tv_sec);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_decisec_timeval2dsec
* @brief               :convert a struct timeval to deciseconds.
* @scenario            :needed by setsockopt() to save new timeout values.
* @apicovered          :net_timeval2dsec
* @precondition        :set the timeval structure value.
* @postcondition       :none
* @return              :void
*/
static void tc_net_decisec_timeval2dsec(void)
{
	unsigned int ret;
	uint16_t remainder;
	struct timeval tv;

	tv.tv_sec = DESISEC / DSEC_PER_SEC;
	remainder = DESISEC - tv.tv_sec * DSEC_PER_SEC;
	tv.tv_usec = remainder * USEC_PER_DSEC;

	ret = net_timeval2dsec(&tv);

	TC_ASSERT_GEQ("time", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: net_decisec_time_main
 ****************************************************************************/
int net_decisec_time_main(void)
{
	tc_net_decisec_dsec2tick();
	tc_net_decisec_dsec2timeval();
	tc_net_decisec_timeval2dsec();
	return 0;
}
