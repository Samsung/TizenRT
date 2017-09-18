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
#include <net/if.h>

#include "tc_internal.h"

/**
* @testcase				: tc_netbuf_alloc_p
* @brief				:
* @scenario				:
* @apicovered			: netbuf_alloc()
* @precondition			:
* @postcondition		:
*/
static void tc_netbuf_alloc_p(void)
{
	struct netbuf *buf = NULL;
	int result = 0;

	buf = netbuf_alloc(buf, sizeof(struct netbuf));
	if (buf) {
		result = 1;
		netbuf_free(buf);
	}

	TC_ASSERT_NEQ("netbuf_alloc", result, 0)
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: lwip_read_write()
 ****************************************************************************/
int netbuf_alloc_main(void)
{
	tc_netbuf_alloc_p();
	return 0;
}
