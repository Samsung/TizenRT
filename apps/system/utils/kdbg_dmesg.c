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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#if defined(CONFIG_SYSLOG) && defined(CONFIG_RAMLOG_SYSLOG)
#include <tinyara/syslog/ramlog.h>

#if CONFIG_STDIO_BUFFER_SIZE > 0
#define BUFFER_LEN  CONFIG_STDIO_BUFFER_SIZE
#else
#define BUFFER_LEN  32
#endif
#endif

int kdbg_dmesg(int argc, char **args)
{
#if defined(CONFIG_SYSLOG) && defined(CONFIG_RAMLOG_SYSLOG)
	const char *relpath = CONFIG_SYSLOG_DEVPATH;
	char *buffer = NULL;
	FILE *fp;

	fp = fopen(relpath, "r");
	if (fp == NULL) {
		printf("Failed to open : %s\n", relpath);
		return ERROR;
	}

	buffer = (char *)malloc(BUFFER_LEN * sizeof(char));
	if (buffer == NULL) {
		printf("Failed to malloc\n");
		fclose(fp);
		return ERROR;
	}

	while (fgets(buffer, BUFFER_LEN, fp)) {
		printf("%s", buffer);
	}
	printf("\n");

	fclose(fp);
	free(buffer);
	return OK;
#else
	printf("Not exist. RAM logging was not operated.\n");
	printf("You should turn on RAM logging feature.\n");
	return OK;
#endif
}
