/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/// @file tc_sysdbg.c

/// @brief Test Case Example for Debug API

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <tinyara/debug/sysdbg.h>
#include "tc_internal.h"

#define BUFLEN 32

#ifdef CONFIG_TC_KERNEL_SYSTEM_DEBUG
/**
 * @fn                  :tc_debug_sysdbg_test
 * @brief               :fops to read data buffer passed from user space
 * @scenario            :fops to read data buffer passed from user space, return value should be equal to 0
 * API's covered        :sysdbg_open, sysdbg_close, tc_debug_sysdbg_write
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_debug_sysdbg_test(void)
{
	int ret;
	char buffer[BUFLEN];

	int fd = open(SYSDBG_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	strncpy(buffer, "enable_monitor", 14);
	ret = write(fd, buffer, sizeof(buffer));
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	strncpy(buffer, "read", 4);
	ret = write(fd, buffer, sizeof(buffer));
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	strncpy(buffer, "dump_stack", 10);
	ret = write(fd, buffer, sizeof(buffer));
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	strncpy(buffer, "dump_allstack", 13);
	ret = write(fd, buffer, sizeof(buffer));
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	strncpy(buffer, "disable_monitor", 15);
	ret = write(fd, buffer, sizeof(buffer));
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	close(fd);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: debug_main
 ****************************************************************************/

int debug_main(void)
{
	tc_debug_sysdbg_test();

	return 0;
}

#endif		// CONFIG_TC_KERNEL_SYSTEM_DEBUG

