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
/// @file   tash_sleep.c
/// @brief  functions to sleep

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "tash_internal.h"

int tash_sleep(int argc, char **args)
{
	char *endptr;
	long secs;

	if (argc != 2 || args[1] == NULL) {
		shdbg("%s: argument invalid\n", args[0]);
		return ERROR;
	}

	secs = strtol(args[1], &endptr, 0);
	if (!secs || endptr == args[1] || *endptr != '\0') {
		shdbg("%s: argument invalid\n", args[0]);
		return ERROR;
	}

	sleep(secs);
	return OK;
}
