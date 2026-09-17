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
#include <stdlib.h>
#include <string.h>
#include "tc_internal.h"

static void tc_driver_rammtd_small_region(void)
{
	size_t sizes[] = {0, 1, CONFIG_RAMMTD_ERASESIZE - 1};
	uint8_t *buffer = malloc(CONFIG_RAMMTD_ERASESIZE);
	size_t i;
	size_t j;
	uint8_t marker = CONFIG_RAMMTD_ERASESTATE ^ 0xff;

	TC_ASSERT("RAM MTD buffer", buffer != NULL);
	for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
		if (sizes[i] >= CONFIG_RAMMTD_ERASESIZE) {
			continue;
		}
		memset(buffer, marker, CONFIG_RAMMTD_ERASESIZE);
		TC_ASSERT_CLEANUP("reject undersized RAM MTD", rammtd_initialize(buffer, sizes[i]) == NULL, free(buffer));
		for (j = 0; j < CONFIG_RAMMTD_ERASESIZE; j++) {
			TC_ASSERT_EQ_CLEANUP("preserve rejected RAM region", buffer[j], marker, free(buffer));
		}
	}
	free(buffer);
	TC_SUCCESS_RESULT();
}

void rammtd_main(void)
{
	tc_driver_rammtd_small_region();
}
