/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <apps/shell/tash.h>
#include <tinyara/logm.h>
#include "logm.h"

static int logm_tash(int argc, char **args);

const static tash_cmdlist_t logm_tashmds[] = {
	{"logm", logm_tash, TASH_EXECMD_SYNC},
	{NULL, NULL, 0}
};

void logm_register_tashcmds(void)
{
	tash_cmdlist_install(logm_tashmds);
}

static void logm_usage(void)
{
	fprintf(stdout, "[LOGM USAGE]\n");
	fprintf(stdout, "usage: logm [-b <BUFSIZE>] [-i <TIME>]\n");

	fprintf(stdout, "options:\n");
	fprintf(stdout, "    -b BUFSIZE\n");
	fprintf(stdout, "        Set logm buffer size (bytes)\n");
	fprintf(stdout, "    -i TIME\n");
	fprintf(stdout, "        Set buffer flusing interval (ms)\n");

}

static void logm_info(void)
{
	int bufsize;
	int interval;

	logm_get_values(LOGM_BUFSIZE, &bufsize);
	logm_get_values(LOGM_INTERVAL, &interval);

	fprintf(stdout, "[LOGM CONFIGURATIONS]\n");
	fprintf(stdout, "  Buffer size : %d (bytes)\n", bufsize);
	fprintf(stdout, "  Flusing interval : %d (ms)\n", interval);
}

static int logm_tash(int argc, char **args)
{
	int opt;

	if (argc < 2) {
		/* TASH>> logm */
		/* show current logm configurations */
		logm_info();
		return 0;
	}

	if (strcmp(args[1], "--help") == 0) {
		/* TASH>> logm --help */
		/* show usage of logm commands */
		logm_usage();
		return 0;
	}

	/*
	 * -b [bufsize] : set buffer size (bytes)
	 * -i [time] : set buffer flushing interval (ms)
	 */
	while ((opt = getopt(argc, args, "b:i:")) != -1) {
		switch (opt) {
		case 'b':
			/* TASH>> logm -b 10240 */
			/* set buffer size as 10240 (=10KB) */
			if (optarg != NULL && atoi(optarg) > 0) {
				logm_set_values(LOGM_BUFSIZE, atoi(optarg));
				LOGM_STATUS_SET(LOGM_BUFFER_RESIZE_REQ);
			}
			break;
		case 'i':
			/* TASH>> logm -i 1000 */
			/* sets interval for flushing buffer as 1000ms (=1sec) */
			if (optarg != NULL && atoi(optarg) > 0) {
				logm_set_values(LOGM_INTERVAL, atoi(optarg));
			}
			break;
		default:
			logm_usage();
			return 0;
		}
	}

	return 0;					//Just to make the compiler happy now
}
