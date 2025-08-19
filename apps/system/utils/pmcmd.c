/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 *  apps/system/utils/pmcmd.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
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
/// @file pmcmd.c
/// @brief Cursor functions.
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyara/pm/pm.h>
#include <sys/ioctl.h>
#ifdef CONFIG_TASH
#include <apps/shell/tash.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define WORK_TIME 50000000
#define SLEEP_TIME 5000

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_AMP
static void* pm_smp_test_app(void *arg) {
	int ret = -1;
	struct timeval start, cur;
	int work_time = WORK_TIME;
	int sleep_time = SLEEP_TIME;
	int delay = 0;
	if (fd < 0) {
	int pid = getpid();
	int fd = open(PM_DRVPATH, O_RDONLY);
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
		return NULL;
	}
	while (1) {
		printf("PID %d: I am pm_smp_test_app\n", pid);
		printf("PID %d: Lets do non critical work\n", pid);
		volatile count = work_time;
		while (count) {
			count--;
		}
		printf("PID %d: Lets sleep for %d ms\n", pid, sleep_time);
		system_pm_sleep(sleep_time);
	}
	close(fd);
	return NULL;
}

static int tash_pm_smp_test(int args, char *argv[]) {
	pthread_t threadid;
	pthread_attr_t attr;
	int ret;
	ret = pthread_attr_init(&attr);
	if (ret != OK) {
		printf("Failed to init thread attributes\n");
	}
	attr.affinity = 1<<atoi(argv[1]);
	ret = pthread_create(&threadid, &attr, pm_smp_test_app, NULL);
	if (ret != OK) {
		printf("Thread creation failed\n");
	}
	return 0;
}
#endif

static int tash_pm_sleep_test(int args, char *argv[]) {
	int ret = OK;
	int fd = open(PM_DRVPATH, O_RDONLY);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
        return -1;
	}
	while(ret==OK) {
	    printf("SLEEPING for %s\n", argv[1]);
		ret = ioctl(fd, PMIOC_SLEEP, atoi(argv[1]));
	}
    printf("ioctl failed with error = %d\n", get_errno());

	close(fd);
	return 0;
}

static int tash_pm_start(int argc, char *argv[])
{
	int fd = open(PM_DRVPATH, O_RDONLY);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
        return -1;
	}
    if (ioctl(fd, PMIOC_START, 0) < 0){
        printf("ioctl failed with error = %d\n", get_errno());
        close(fd);
        return -1;
    }
    printf("pm_start success!!\n");
	close(fd);
	return 0;
}

static int tash_pm_suspend(int argc, char *argv[])
{
	int fd = open(PM_DRVPATH, O_RDONLY);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
        return -1;
	}
    if (ioctl(fd, PMIOC_SUSPEND, atoi(argv[1])) < 0){
        printf("ioctl failed with error = %d\n", get_errno());
        close(fd);
        return -1;
    }
    printf("pm_suspend success!!\n");
	close(fd);
	return 0;
}

static int tash_pm_resume(int argc, char *argv[])
{
	int fd = open(PM_DRVPATH, O_RDONLY);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
        return -1;
	}
	if (ioctl(fd, PMIOC_RESUME, atoi(argv[1])) < 0){
        printf("ioctl failed with error = %d\n", get_errno());
        close(fd);
        return -1;
    }
    printf("pm_resume success!!\n");
	close(fd);
	return 0;
}


static int tash_pm_domain_register(int argc, char *argv[]) {
	int fd = open(PM_DRVPATH, O_RDONLY);
	pm_domain_arg_t domain_arg;
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n",PM_DRVPATH, fd);
        return -1;
	}
    domain_arg.domain_name = argv[1];
	domain_arg.domain_id = -1;

	if (ioctl(fd, PMIOC_DOMAIN_REGISTER, &domain_arg) < 0) {
        printf("ioctl failed with error = %d\n", get_errno());
        close(fd);
        return -1;
    }
	printf("DomainID is %d\n", domain_arg.domain_id);
	close(fd);
    return 0;
}

const static tash_cmdlist_t pm_utilcmds[] = {
	{"pm_start"          , tash_pm_start          , TASH_EXECMD_SYNC},
	{"pm_suspend"        , tash_pm_suspend        , TASH_EXECMD_SYNC},
	{"pm_resume"         , tash_pm_resume         , TASH_EXECMD_SYNC},
	{"pm_domain_register", tash_pm_domain_register, TASH_EXECMD_SYNC},
	{NULL                , NULL                   , 0               }
};

const static tash_cmdlist_t pm_appcmds[] = {
#ifdef CONFIG_AMP
	{"pm_smp_test"  , tash_pm_smp_test  , TASH_EXECMD_ASYNC},
#endif
	{"pm_sleep_test", tash_pm_sleep_test, TASH_EXECMD_ASYNC},
	{NULL           , NULL              , 0               }
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void pm_register_utilcmds(void)
{
	tash_cmdlist_install(pm_utilcmds);
}

void pm_register_appcmds(void)
{
	tash_cmdlist_install(pm_appcmds);
}
