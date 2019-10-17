/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "tc_common.h"
#include "tc_internal.h"

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_messaging_main(int argc, char *argv[])
#endif
{
	if (testcase_state_handler(TC_START, "Messaging UTC") == ERROR) {
		return ERROR;
	}

	utc_messaging_recv_reply_and_cleanup_main();

	utc_messaging_send_main();

	utc_messaging_multicast_main();

	(void)testcase_state_handler(TC_END, "Messaging UTC");

	return 0;
}
