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

/// @file tc_net_lwip_strerr.c
/// @brief Test Case Example for lwip_strerr() API
#include <net/lwip/err.h>

#include "tc_internal.h"

#define ERR_NO_TEST1	0
#define ERR_NO_TEST2	20
/**
* @testcase            : tc_net_lwip_strerr
* @brief               : to print error messages
* @scenario            : for debugging
* @apicovered          : lwip_strerr
* @precondition        :
* @postcondition       :
*/
static void tc_net_lwip_strerr(int errCount)
{
	int ret;
	err_t err;
	char *comp[] = { "Ok.", "Out of memory error.", "Buffer error.", "Timeout.", "Routing problem.",
					 "Operation in progress.", "Illegal value.", "Operation would block.", "Address in use.",
					 "Already connected.", "Connection aborted.", "Connection reset.", "Connection closed.",
					 "Not connected.", "Illegal argument.", "Low-level netif error."
				   };

	do {
		switch (errCount) {
		case 0:
			err = ERR_OK;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 1:
			err = ERR_MEM;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 2:
			err = ERR_BUF;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 3:
			err = ERR_TIMEOUT;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 4:
			err = ERR_RTE;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 5:
			err = ERR_INPROGRESS;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 6:
			err = ERR_VAL;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 7:
			err = ERR_WOULDBLOCK;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 8:
			err = ERR_USE;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 9:
			err = ERR_ISCONN;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 10:
			err = ERR_ABRT;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 11:
			err = ERR_RST;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 12:
			err = ERR_CLSD;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 13:
			err = ERR_CONN;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 14:
			err = ERR_ARG;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		case 15:
			err = ERR_IF;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			break;
		default:
			err = errCount;
			ret = strcmp(comp[errCount], lwip_strerr(err));
			TC_ASSERT_EQ("strerr", ret, ZERO);
			TC_SUCCESS_RESULT();
			break;
		}
		errCount++;
	} while (errCount <= 15);

	if (errCount <= 16) {
		TC_SUCCESS_RESULT();
	}
}

/****************************************************************************
 * Name: lwip_strerr()
 ****************************************************************************/
int net_lwip_strerr_main(void)
{
	tc_net_lwip_strerr(ERR_NO_TEST1);
	tc_net_lwip_strerr(ERR_NO_TEST2);
	return 0;
}
