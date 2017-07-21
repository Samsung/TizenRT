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

/// @file tc_net_decisec_time.c
/// @brief Test Case Example for decisec() API
#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <stdio.h>

#include "tc_internal.h"

/**
   * @testcase		   :tc_net_decisec_time_one
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_decisec_time_one(void)
{
    int ret, dsec = 1000;
    ret = net_dsec2tick(dsec);

}

/**
   * @testcase		   :tc_net_decisec_time_two
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_decisec_time_two(void)
{
    int dsec = 1000;
	struct timeval *tv;
    tv = (struct timeval *) malloc (sizeof(struct timeval));

    net_dsec2timeval (dsec, tv);

}

/**
   * @testcase		   :tc_net_decisec_time_three
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_decisec_time_three(void)
{
    int dsec = 1000;
    struct timeval *tv;
    tv = (struct timeval *) malloc (sizeof(struct timeval));

    net_dsec2timeval (dsec, tv);

    net_timeval2dsec(tv);

}

/****************************************************************************
 * Name: decisec()
 ****************************************************************************/
int net_decisec_time_main(void)
{
	tc_net_decisec_time_one();
	tc_net_decisec_time_two();
	tc_net_decisec_time_three();

	return 0;
}
