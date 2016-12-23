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

/// @file tc_net_fcntl.c
/// @brief Test Case Example for fcntl() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <arch/board/board.h>
#include <apps/netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"
/**
   * @fn                   :tc_net_fcntl_p
   * @brief                :
   * @scenario             :
   * API's covered         :fcntl()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_fcntl_nonblock_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	close(fd);

	if (ret == -1) {
		printf("tc_net_fcntl_nonblock_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_fcntl_nonblock_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_fcntl_p
   * @brief                :
   * @scenario             :
   * API's covered         :fcntl()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_fcntl_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = fcntl(fd, F_SETFL, 0);
	close(fd);

	if (ret == -1) {
		printf("tc_net_fcntl_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_fcntl_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_fcntl_n
   * @brief                :
   * @scenario             :
   * API's covered         :fcntl()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_fcntl_n(void)
{

	int fd = -1;
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);

	if (ret == 0) {
		printf("tc_net_fcntl_n: FAIL\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_fcntl_n: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_fcntl_ndelay_p
   * @brief                :
   * @scenario             :
   * API's covered         :fcntl()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_fcntl_ndelay_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = fcntl(fd, F_SETFL, O_NDELAY);
	close(fd);

	if (ret == -1) {
		printf("tc_net_fcntl_ndelay_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_fcntl_ndelay_p: PASS\n");
	nw_total_pass++;
}

/****************************************************************************
 * Name: fcntl()
 ****************************************************************************/

int net_fcntl_main(void)
{

	tc_net_fcntl_nonblock_p();
	tc_net_fcntl_p();
	tc_net_fcntl_n();
	tc_net_fcntl_ndelay_p();
	return 0;
}
