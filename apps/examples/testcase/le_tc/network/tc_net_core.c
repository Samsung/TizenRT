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

/// @file tc_net_core.c
/// @brief Test Case Example for lwip_htonl() API
#include <arpa/inet.h>

#include "tc_internal.h"

#define  TC_VAL1 0x1012
#define  TC_VAL2 0x112A380

/**
* @testcase             : tc_net_core_htonl_p
* @brief                : function converts the unsigned short integer from host byte order to network byte order.
* @scenario             : none
* @apicovered           : ntohs()
* @precondition         : Valid unsigned integer
* @postcondition        : none
*/
void tc_net_core_ntohs_p(void)
{
	short int x;				/* Value for checking */

	x = ntohs(TC_VAL1);			/* Observe value in network byte order */
	TC_ASSERT_NEQ("tc_net_core_ntohs_p", x, TC_VAL1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_core_ntohl_p
* @brief                : function converts the unsigned integer netlong from network byte order to host byte order.
* @scenario             : none
* @apicovered           : ntohl()
* @precondition         : valid unsigned integer
* @postcondition        : none
*/
void tc_net_core_ntohl_p(void)
{
	long int x;					/* Value for checking */

	x = ntohl(TC_VAL2);			/* Observe value in network byte order */
	TC_ASSERT_NEQ("tc_net_core_ntohl_p", x, TC_VAL2);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: ntohs() and ntohl()
 ****************************************************************************/
int net_core_ntohs_main(void)
{
	tc_net_core_ntohs_p();
	tc_net_core_ntohl_p();
	return 0;
}
