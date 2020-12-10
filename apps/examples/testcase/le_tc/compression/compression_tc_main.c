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
#include <tinyara/kernel_test_drv.h>
#include "tc_common.h"
#include "tc_internal.h"

static int g_tc_fd;

int tc2_get_drvfd(void)
{
	return g_tc_fd;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_compression_main(int argc, char *argv[])
#endif
{
	g_tc_fd = open(KERNEL_TEST_DRVPATH, O_WRONLY);

	/*If FAIL : Failed to open testcase driver*/
	TC_ASSERT_GEQ("open", g_tc_fd, 0);

#ifdef CONFIG_TC_LZMA
	lzma_main();
#endif

#ifdef CONFIG_TC_COMPRESS_READ
	tc_compress_read_main();
#endif

	close(g_tc_fd);
	return 0;
}
