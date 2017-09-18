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

#include "tc_internal.h"

/**
* @testcase				: tc_do_netif_api_netif_add
* @brief				:
* @scenario				:
* @apicovered			: do_netifapi_netif_add and netifapi_netif_add
* @precondition			:
* @postcondition		:
*/
static void tc_do_netifapi_netif_add_n(void)
{
	struct netifapi_msg_msg *msg = NULL;

	do_netifapi_netif_add(msg);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name lwip_netifapi
 ****************************************************************************/
int net_lwip_netifapi_main(void)
{
	tc_do_netifapi_netif_add_n();
	return 0;
}

