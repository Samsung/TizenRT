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
#include <netinet/in.h>

#include "tc_internal.h"

#define COUNT1 5

/**
* @testcase				: tc_net_mem_trim_p
* @brief				:
* @scenario				:
* @apicovered			: mem_trim()
* @precondition			:
* @postcondition		:
*/
void tc_net_mem_trim_p(void *buf)
{
	int result = 0;

	if (buf) {
		buf = (struct netbuf *)mem_trim(buf, 2 * sizeof(struct netbuf));
		if (buf) {
			result = 1;
		}
		TC_ASSERT_GEQ("mem_calloc", result, 1);
		TC_SUCCESS_RESULT();
	}
}

/**
* @testcase				: tc_net_mem_calloc_p
* @brief				:
* @scenario				:
* @apicovered			: mem_calloc()
* @precondition			:
* @postcondition		:
*/
void tc_net_mem_calloc_p(void)
{
	struct netbuf *p1 = NULL;
	int result = 0;

	p1 = (struct netbuf *)mem_calloc(COUNT1, sizeof(struct netbuf));

	if (p1) {
		result = 1;
		tc_net_mem_trim_p(p1);
		mem_free(p1);
	}
	TC_ASSERT_GEQ("mem_calloc", result, 1);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: mem_calloc
 ****************************************************************************/
int net_mem_allocate_main(void)
{
	tc_net_mem_calloc_p();
	return 0;
}

