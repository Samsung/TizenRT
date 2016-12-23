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
/// @file   tash_script.c
/// @brief  functions to excute a script

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tinyara/fs/fs_utils.h>
#include "tash_internal.h"

#define BUFF_SIZE 80
#define PATH_ARG_INDEX  1

static void show_usage_tash_script(void)
{
	printf("\nUSAGE: sh <Script File PATH>");
}

int tash_script(int argc, char **args)
{
	char *full_path;
	const char *path;
	FILE *fp;
	char buff[BUFF_SIZE + 1] = { 0, };

	if (argc != 2) {
		show_usage_tash_script();
		return ERROR;
	}

	path = (const char *)args[PATH_ARG_INDEX];
	full_path = get_fullpath(path);
	if (!full_path) {
		shdbg("TASH: script path (%s) is NULL\n", path);
		return ERROR;
	}

	fp = fopen(full_path, "r");
	if (!fp) {
		free(full_path);
		shdbg("TASH: can not open a script (%s)\n", path);
		return ERROR;
	}

	while (fgets(buff, BUFF_SIZE, fp) != NULL) {
		char *buff_p;
		buff_p = buff;

		/* remove LF, CR, EOF from buffer */
		while (*buff_p != '\0') {
			if ((*buff_p == '\r') || (*buff_p == '\n')) {
				*buff_p = '\0';
				break;
			}
			buff_p++;
		}

		tash_execute_cmdline(buff);
	}

	fclose(fp);
	free(full_path);

	return OK;
}
