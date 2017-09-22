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

#define	NTOHS_VAL		0x1400
#define	NTOHS_CONVAL	0x0014
#define	HTONS_VAL		0x0a00
#define	HTONS_CONVAL	0x000a

#define	NTOHL_VAL		0xaa140000
#define	NTOHL_CONVAL	0x000014aa
#define	HTONL_VAL		0xaa140000
#define	HTONL_CONVAL	0x000014aa

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
	uint16_t ret = htons(HTONS_VAL);

	TC_ASSERT_EQ("htons", ret, HTONS_CONVAL);
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
	uint16_t ret = ntohs(NTOHS_VAL);

	TC_ASSERT_EQ("ntohs", ret, NTOHS_CONVAL);
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
	uint32_t ret = htonl(HTONL_VAL);

	TC_ASSERT_EQ("htonl", ret, HTONL_CONVAL);
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
	uint32_t ret = ntohl(NTOHL_VAL);

	TC_ASSERT_EQ("ntohl", ret, NTOHL_CONVAL);
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
