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

/// @file tc_net_def.c
/// @brief Test Case Example for def() API
#include <net/lwip/def.h>

#include "tc_internal.h"

/**
* @testcase				: tc_net_def_lwip_htons
* @brief				:
* @scenario				:
* @apicovered			: lwip_htons()
* @precondition			:
* @postcondition		:
*/
static void tc_net_def_lwip_htons(void)
{
	uint16_t ret;
	uint16_t ref;
	uint16_t var = 20;

	ret = lwip_htons(var);
	ref = 0x1400;
	TC_ASSERT_EQ("htons", ret, ref);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_def_lwip_ntohs
* @brief				:
* @scenario				:
* @apicovered			: lwip_ntohs()
* @precondition			:
* @postcondition		:
*/
static void tc_net_def_lwip_ntohs(void)
{
	uint16_t var = 0x1400;
	uint16_t ret;
	uint16_t ref;

	ret = lwip_ntohs(var);
	ref = 20;
	TC_ASSERT_EQ("ntohs", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_def_lwip_htonl
* @brief				:
* @scenario				:
* @apicovered			: lwip_htonl()
* @precondition			:
* @postcondition		:
*/
static void tc_net_def_lwip_htonl(void)
{
	uint32_t ret;
	uint32_t ref;
	uint32_t var = 10;

	ret = lwip_htonl(var);
	ref = 0xa000000;
	TC_ASSERT_EQ("htonl", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_def_lwip_ntohl
* @brief				:
* @scenario				:
* @apicovered			: lwip_ntohl()
* @precondition			:
* @postcondition		:
*/
static void tc_net_def_lwip_ntohl(void)
{
	uint32_t ret;
	uint32_t ref;
	uint32_t var = 0xa000000;

	ret = lwip_ntohl(var);
	ref = 10;
	TC_ASSERT_EQ("ntohl", ret, ref);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: inet()
 ****************************************************************************/
int net_def_main(void)
{
	tc_net_def_lwip_htons();
	tc_net_def_lwip_ntohs();
	tc_net_def_lwip_htonl();
	tc_net_def_lwip_ntohl();
	return 0;
}
