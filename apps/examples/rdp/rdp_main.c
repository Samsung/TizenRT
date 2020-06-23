/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * apss/examples/rdp/rdp_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <arch/chip/amebad_nsc.h>

#include <security/security_api.h>
/****************************************************************************
 * Macros
 ****************************************************************************/

#define CONFIG_MIN_SECURE_STACKSIZE				(1024)

uint32_t no_protection_func(uint32_t data)
{
	uint32_t result;

	result = data / 5 + 3;
	result *= 2;
	result += 8;

	return result;
}

static int rdp_demo(int argc, FAR char *argv[])
{
	uint32_t rdp_result;
	uint32_t no_rdp_result;
	security_handle hnd;
	int i = 0;

	int res = security_init(&hnd);
	if (res != 0) {
		printf("security_init fail\n");
		return 0;
	}

	/*
	 * Tasks are not created with a secure context.
	 * Any task that is going to call secure functions must call up_allocate_secure_context()
	 * to allocate itself a secure context before it calls any secure function
	 */
	res = security_allocate(hnd, CONFIG_MIN_SECURE_STACKSIZE);
	if (res != 0) {
		printf("security_allocate fail\n");
		goto errout;
	}

	for (i = 0; i < 32; i++){
		rdp_result = rdp_protection_entry(i);
		no_rdp_result = rdp_no_protection_call(no_protection_func, i);

		if (rdp_result != no_rdp_result) {
			printf("rdp call fail!\n");
			printf("rdp_result = 0x%x, no_rdp_result=0x%x\n", rdp_result, no_rdp_result);
			goto errout_with_result;
		}
	}
	printf("rdp demo call succeed!\n");
errout_with_result:
	/* Frees the given secure context */
	res = security_free(hnd, tz_memory);
	if (res != 0) {
		printf("security_free fail\n");
	}
errout:
	res = security_deinit(hnd);
	if (res != 0) {
		printf("deinit fail\n");
	}

	return 0;
}

/****************************************************************************
 * rdp_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int rdp_main(int argc, char *argv[])
#endif
{
	int pid;

	printf("rdp main!!\n");

	/* Currently TZ support is on flat build only */
#ifndef CONFIG_BUILD_KERNEL
	pid = task_create("RDP DEMO", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, (main_t)rdp_demo, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("Failed to create rdp demo thread\n");
	}
#endif
	return 0;
}
