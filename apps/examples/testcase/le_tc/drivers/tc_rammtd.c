/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

/// @file tc_rammtd.c
/// @brief Test rejection of undersized RAM MTD regions
#include <tinyara/config.h>
#include <tinyara/fs/mtd.h>
#include "tc_internal.h"

static void tc_driver_rammtd_small_region(void)
{
	uint8_t buffer = 0x5a;

	/* A zero-length region is invalid regardless of the kernel erase size. */
	TC_ASSERT("reject empty RAM MTD", rammtd_initialize(&buffer, 0) == NULL);
	TC_ASSERT_EQ("preserve rejected RAM region", buffer, 0x5a);
	TC_SUCCESS_RESULT();
}

void rammtd_main(void)
{
	tc_driver_rammtd_small_region();
}
