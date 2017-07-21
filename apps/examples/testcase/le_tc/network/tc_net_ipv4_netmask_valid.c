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

/// @file tc_net_ipv4_netmask_valid.c
/// @brief Test Case Example for ip4_addr_netmask_valid() API

#include <tinyara/config.h>
#include <stdio.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <apps/netutils/netlib.h>
#include <net/lwip/ipv4/ip_addr.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
 * @testcase         : tc_net_ipv4_addr_netmask_valid_p
 * @brief            :
 * @scenario         :
 * @apicovered       : ip4_addr_netmask_valid
 * @precondition     :
 * @postcondition    :
 **/
void tc_net_ipv4_addr_netmask_valid_p(void)
{
    int ret;
    int netmask = 0x0000ffff; /* Value for checking */

    ret = ip4_addr_netmask_valid(netmask);

    if (ret)
        printf("the netmask is valid\n");
    else
        printf("the netmask is not valid\n");

}

/****************************************************************************
 * Name: ip4_addr_netmask_valid()
 ****************************************************************************/

int net_ipv4_addr_netmask_valid_main(void)
{
  tc_net_ipv4_addr_netmask_valid_p();

  return 0;
}
