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
/// @file   tash_usleep.c
/// @brief  function to usleep

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "tash_internal.h"

/*
 *	Tash command to sleep for micro seconds
 */

int tash_usleep(int argc, char **args)
{
	char *endptr;
	long usecs;

	usecs = strtol(args[1], &endptr, 0);
	if (!usecs || endptr == args[1] || *endptr != '\0') {
		shdbg("%s: argument invalid\n", args[0]);
		return ERROR;
	}

	usleep(usecs);

	return OK;
}
