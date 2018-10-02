/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/security_sdk/security_sdk_random.c
 *
 *   Copyright (C) 2018 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: Youngdae Oh <yd.oh@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <security_sdk_test.h>

int security_sdk_random(void)
{
	artik_security_module *security = NULL;
	int test_result = 0;
	artik_security_handle handle;
	artik_error err = S_OK;
	unsigned char *rand_bytes = NULL;
	int num_bytes = 32;
	int i = 0;

	fprintf(stderr, "--------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get random number\n");
	fprintf(stderr, "--------------------------------------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		test_result++;
		goto exit;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		test_result++;
		goto exit;
	}

	err = security->get_random_bytes(handle, num_bytes, &rand_bytes);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get random bytes (err=%d)\n", err);
		free(rand_bytes);
		test_result++;
		goto exit;
	}

	for (i = 0; i < num_bytes; i++) {
		fprintf(stdout, "%02x ", rand_bytes[i]);
	}

	fprintf(stdout, "\n");

exit:
	if (rand_bytes) {
		free(rand_bytes);
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}

