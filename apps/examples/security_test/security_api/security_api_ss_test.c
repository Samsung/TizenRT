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
 * either express or implied. SEC the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <security/security_ss.h>
#include "security_api_utils.h"
/*
 * Test Secure Storage APIs
 */
#define TEST_SS_PATH "ss/03"


void
test_securestorage(void)
{
	int i;
	unsigned int storage_size;
	unsigned int count;
	security_storage_list list;
	security_handle hnd;
	security_data input = {NULL, 0};
	security_data output = {NULL, 0};
	input.data = "1234567890123456";
	input.length = 16;

	printf("  . SEC Initialize ...\n");

	if (0 != security_init(&hnd)) {
		printf("Fail\n	! security_init\n");
		return;
	}
	printf("ok\n");

	printf("  . SEC Write Secure Storage ...\n");

	if (0 != ss_write_secure_storage(hnd, TEST_SS_PATH, 0, &input)) {
		printf("Fail\n	! ss_write_secure_storage\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("input", input.data, input.length);

	printf("  . SEC Get Size of Secure Storage ...\n");

	if (0 != ss_get_size_secure_storage(hnd, TEST_SS_PATH, &storage_size)) {
		printf("Fail\n	! ss_get_size_secure_storage\n");
		goto exit;
	}
	printf("ok\n");
	printf("storage_test size : %d\n", storage_size);

	printf("  . SEC Read Secure Storage ...\n");

	if (0 != ss_read_secure_storage(hnd, TEST_SS_PATH, 0, &output)) {
		printf("Fail\n	! ss_read_secure_storage\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer(TEST_SS_PATH, output.data, output.length);

	printf("  . SEC Get Secure Storage List ...\n");

	if (0 != ss_get_list_secure_storage(hnd, &count, &list)) {
		printf("Fail\n	! ss_get_list_secure_storage\n");
		goto exit;
	}
	printf("ok\n");
	printf("[%20s] [%8s]\n", "FILE NAME", "FILE ATTR");
	for (i = 0; i < count; i++) {
		printf("[%20s] [%08x]\n", list[i].name, list[i].attr);
	}

	printf("  . SEC Delete secure storage ...\n");

	if (0 != ss_delete_secure_storage(hnd, TEST_SS_PATH)) {
		printf("Fail\n	! ss_delete_secure_storage\n");
		goto exit;
	}
	printf("ok\n");

exit:
	free_security_data(&output);
	if (count > 0)
		free(list);

	printf("  . SEC Deinitialize ...\n");
	security_deinit(hnd);
	printf("ok\n");

	return;
}

