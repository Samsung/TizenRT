/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * apps/system/sysinfo/sysinfo.c
 *
 *   Copyright (C) 2011 Uros Platise. All rights reserved.
 *   Author: Uros Platise <uros.platise@isotel.eu>
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

#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>
#ifdef CONFIG_BUILD_PROTECTED
#include <fcntl.h>
#else
#include <tinyara/version.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define MAX_BUF_SIZE 64
#define PROC_VERSION_PATH "/proc/version"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sysinfo(void)
{
	time_t now;
	struct tm *ptm;
	char sysinfo_str[MAX_BUF_SIZE + 1];

	printf("System Information:\n");

	/* Print OS version and Build information */
	/* just get values defined in version.h */
	printf("\tVersion: " CONFIG_VERSION_STRING "\n\tCommit Hash: %s\n", CONFIG_VERSION_BUILD);
	printf("\tBuild User: " CONFIG_VERSION_BUILD_USER "\n\tBuild Time: %s\n", CONFIG_VERSION_BUILD_TIME);

	/* Get the current time as specific format in the buffer */
	now = time(NULL);
	ptm = (struct tm *)localtime(&now);
	(void)strftime(sysinfo_str, MAX_BUF_SIZE, "%d %b %Y, %H:%M:%S", ptm);

	/* Print System Time information */
	printf("\tSystem Time: %s [s] UTC "
#ifdef CONFIG_RTC
		   "Hardware RTC Support"
#endif
		   "\n", sysinfo_str);

}

