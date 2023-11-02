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

/// @file compression_tc_main.c

/// @brief Main Function for Compression TestCase Example

#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <tinyara/os_api_test_drv.h>
#include <tinyara/compression.h>
#include "tc_common.h"
#include "tc_internal.h"

#ifdef CONFIG_TC_COMPRESS_READ
static int g_tc_fd;

int tc2_get_drvfd(void)
{
	return g_tc_fd;
}
#endif

static int g_comp_tc_fd;

int get_compfd(void)
{
	return g_comp_tc_fd;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_compression_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TC_COMPRESS_READ
	g_tc_fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	/*If FAIL : Failed to open testcase driver*/
	if (g_tc_fd < 0) {
		printf("Failed to open OS API test driver %d\n", errno);
		return ERROR;
	}

	g_comp_tc_fd = open(COMP_DRVPATH, O_WRONLY);
	/*If FAIL : Failed to open compression driver*/
	if (g_comp_tc_fd < 0) {
		printf("Failed to open compression driver %d\n", errno);
		close(g_tc_fd);
		return ERROR;
	}
	
	tc_compress_main();
	close(g_comp_tc_fd);
	close(g_tc_fd);
#endif

	return 0;
}
