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
void tc_net_core_ntohs_p(void);
void tc_net_core_ntohl_p(void);

/**
 * @testcase		       : tc_net_core_htonl_p
 * @brief		          :
 * @scenario		       :
 * @apicovered	      : ntohs()
 * @precondition	    :
 * @postcondition	   :
 */
void tc_net_core_ntohs_p(void)
{
    short int x = 0x1012; /* Value for checking */

    x = ntohs(x); /* Observe value in network byte order */
}

/**
 * @testcase         : tc_net_core_ntohl_p
 * @brief            :
 * @scenario         :
 * @apicovered       : ntohl()
 * @precondition     :
 * @postcondition    :
 **/
void tc_net_core_ntohl_p(void)
{
    long int x = 0x112A380; /* Value for checking */

    x = ntohl(x);  /* Observe value in network byte order */
}

/****************************************************************************
 * Name: ntohs() and ntohl()
 ****************************************************************************/

int net_lwip_ntohs_main(void)
{
    tc_net_core_ntohs_p();
    tc_net_core_ntohl_p();

    return 0;
}
