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

/// @file tc_net_ipaddr_addr.c
/// @brief Test Case Example for ipaddr_addr() API

#include <tinyara/config.h>
#include <stdio.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <apps/netutils/netlib.h>
#include <net/lwip/ipv4/ip_addr.h>
#include <net/lwip/ipv4/inet.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
 * @testcase         : tc_net_ipaddr_ntoa_p
 * @brief            :
 * @scenario         :
 * @apicovered       : ipaddr_ntoa
 * @precondition     :
 * @postcondition    :
 **/
void tc_net_ipaddr_ntoa_p(void)
{
   char *ret;
   struct ip_addr local;
   IP4_ADDR(&local,127,0,0,1); // Set local = "127.0.0.1"

   ret = ipaddr_ntoa(&local);

}
/**
 * @testcase         : tc_net_ipaddr_addr_p
 * @brief            :
 * @scenario         :
 * @apicovered       : ipaddr_addr
 * @precondition     :
 * @postcondition    :
 **/
void tc_net_ipaddr_addr_p(void)
{
  unsigned int ret;
  const char *cp = "192.168.1.23";

  ret = ipaddr_addr(cp);

}

/****************************************************************************
 * Name: ipaddr_addr()
 ****************************************************************************/

int net_ipaddr_addr_main(void)
{

  tc_net_ipaddr_addr_p();
  tc_net_ipaddr_ntoa_p();
  return 0;
}
