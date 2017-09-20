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

#define TC_VAL1 20
#define TC_VAL2 10
#define TC_VAL1_HEX 0x1400
#define TC_VAL2_HEX 0xa000000

/**
* @testcase              : tc_net_def_htons
* @brief                 : Convert host bytes to network bytes order
* @scenario              : Converts host bytes to network bytes for short integer
* @apicovered            : htons()
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_def_htons(void)
{
	uint16_t ret;
	uint16_t ref;
	uint16_t var = TC_VAL1;

	ret = htons(var);
	ref = TC_VAL1_HEX;

	TC_ASSERT_EQ("htons", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_def_ntohs
* @brief                 : Convert network bytes to hosst bytes order
* @scenario              : Convert network bytes to hosst bytes for short integer
* @apicovered            : ntohs()
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_def_ntohs(void)
{
	uint16_t ret;
	uint16_t ref;
	uint16_t var = TC_VAL1_HEX;

	ret = ntohs(var);
	ref = TC_VAL1;

	TC_ASSERT_EQ("ntohs", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_def_htonl
* @brief                 : Convert host bytes to network bytes order
* @scenario              : Convert host bytes to network bytes order for long integer
* @apicovered            : htonl()
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_def_htonl(void)
{
	uint32_t ret;
	uint32_t ref;
	uint32_t var = TC_VAL2;

	ret = htonl(var);
	ref = TC_VAL2_HEX;

	TC_ASSERT_EQ("htonl", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_def_ntohl
* @brief                 : Convert network bytes to hosst bytes order
* @scenario              : Convert network bytes to hosst bytes order for long integer
* @apicovered            : ntohl()
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_def_ntohl(void)
{
	uint32_t ret;
	uint32_t ref;
	uint32_t var = TC_VAL2_HEX;

	ret = ntohl(var);
	ref = TC_VAL2;

	TC_ASSERT_EQ("ntohl", ret, ref);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: inet()
 ****************************************************************************/
int net_def_main(void)
{
	tc_net_def_htons();
	tc_net_def_ntohs();
	tc_net_def_htonl();
	tc_net_def_ntohl();
	return 0;
}
