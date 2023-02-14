/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>

int mem_leak_checker_main(int argc, char **argv)
{
	int ret;

#ifdef CONFIG_APP_BINARY_SEPARATION
	if (argc != 2) {
#else 
	if (argc != 1) {
#endif
		printf("Fail to launch MEMORY LEAK CHECKER : Invalid arguments.\n");
		goto usage;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	ret = prctl(PR_MEM_LEAK_CHECKER, getpid(), argv[1]);
#else
	ret = prctl(PR_MEM_LEAK_CHECKER, getpid(), NULL);
#endif
	if (ret < 0) {
		printf("Fail to launch MEMORY LEAK CHECKER.\n");
		return ERROR;
	}

	return OK;

usage:
#ifdef CONFIG_APP_BINARY_SEPARATION
	printf("\nUsage: mem_leak [TARGET]\n");
	printf("\nTargets:\n");
	printf(" kernel         Check memory leak from kernel threads.\n");
	printf(" app1            Check memory leak from app1 Binary.\n");
	printf(" app2            Check memory leak from app2 Binary.\n");
#else
	printf("\nUsage: mem_leak\n");
#endif
	return ERROR;
}
