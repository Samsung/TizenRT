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
* @testcase              : tc_netbuf_alloc_p
* @brief                 : allocate memory to the given netbuf
* @scenario              : none
* @apicovered            : netbuf_alloc()
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_netbuf_alloc_p(void)
{
	struct netbuf buf;
	char *result;
	result = netbuf_alloc(&buf, sizeof(struct netbuf));

	TC_ASSERT_NEQ("netbuf_alloc", result, NULL);
	TC_SUCCESS_RESULT();

	pbuf_free(buf.p);
	buf.ptr = NULL;
}

/****************************************************************************
 * Name: netbuf_alloc_main()
 ****************************************************************************/
int netbuf_alloc_main(void)
{
	tc_netbuf_alloc_p();
	return 0;
}
