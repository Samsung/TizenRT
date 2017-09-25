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

// @file tc_net_send.c
// @brief Test Case Example for send() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netutils/netlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "tc_internal.h"

#define PORTNUM    5020

/**
* @fn                   :tc_net_raw_client
* @brief                :creates raw socket
* @scenario             :creating raw socket and bind it 
* API's covered         :socket, bind
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
void tc_net_raw_client(void)
{
	int ret;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_RAW, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof(sa));
        sa.sin_family = PF_INET;
        sa.sin_port = htons(PORTNUM);
        sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(sock));
	close(sock);
        TC_SUCCESS_RESULT();
}


/****************************************************************************
 * Name: net_raw_main()
 ****************************************************************************/
int net_raw_main(void)
{
	tc_net_raw_client();
	return 0;
}
