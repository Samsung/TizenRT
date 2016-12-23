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
 * system/prun/pexec_main.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include <apps/prun.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_SYSTEM_PEXEC_VARSTACKSIZE
#define CONFIG_SYSTEM_PEXEC_VARSTACKSIZE 1024
#endif

#ifndef CONFIG_SYSTEM_PEXEC_STRSTACKSIZE
#define CONFIG_SYSTEM_PEXEC_STRSTACKSIZE 128
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void show_usage(FAR const char *progname, int errcode)
{
	fprintf(stderr, "USAGE: %s [OPTIONS] <filename>\n", progname);
	fprintf(stderr, "\nWhere:\n");
	fprintf(stderr, "\t<filename> is the full path to the P-Code file to be executed\n");
	fprintf(stderr, "\nand OPTIONS include the following:\n");
	fprintf(stderr, "\t-v <varsize>: Variable memory size to use.  Default: %d\n", CONFIG_SYSTEM_PEXEC_VARSTACKSIZE);
	fprintf(stderr, "\t-s <strsize>: String stack size to use.  Default: %d\n", CONFIG_SYSTEM_PEXEC_STRSTACKSIZE);
	fprintf(stderr, "\t-h: Show this text and exit\n");
	exit(errcode);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int pexec_main(int argc, FAR char **argv)
#endif
{
	FAR char *filename = NULL;
	FAR char *endptr;
	long varsize = CONFIG_SYSTEM_PEXEC_VARSTACKSIZE;
	long strsize = CONFIG_SYSTEM_PEXEC_STRSTACKSIZE;
	long tmp;
	int option;
	int ret;

	/* Parse input parameters */

	while ((option = getopt(argc, argv, ":v:s:h")) != ERROR) {
		switch (option) {
		case 'h':
			show_usage(argv[0], EXIT_SUCCESS);
			break;

		case 'v':
			tmp = strtol(optarg, &endptr, 10);
			if (tmp < 0) {
				fprintf(stderr, "ERROR: Variable memory size out of range: %ld\n", tmp);
				show_usage(argv[0], EXIT_FAILURE);
			} else {
				varsize = tmp;
			}
			break;

		case 's':
			tmp = strtol(optarg, &endptr, 10);
			if (tmp < 0) {
				fprintf(stderr, "ERROR: String stack size out of range: %ld\n", tmp);
				show_usage(argv[0], EXIT_FAILURE);
			} else {
				strsize = tmp;
			}
			break;

		case ':':
			fprintf(stderr, "ERROR: Missing required argument\n");
			show_usage(argv[0], EXIT_FAILURE);
			break;

		default:
		case '?':
			fprintf(stderr, "ERROR: Unrecognized option\n");
			show_usage(argv[0], EXIT_FAILURE);
			break;
		}
	}

	/* There should be one final parameters remaining on the command line */

	if (optind >= argc) {
		fprintf(stderr, "ERROR: Missing required <filename> argument\n");
		show_usage(argv[0], EXIT_FAILURE);
	}

	filename = argv[optind];
	optind++;

	if (optind < argc) {
		fprintf(stderr, "ERROR: Garbage at the end of the command line\n");
		show_usage(argv[0], EXIT_FAILURE);
	}

	/* Execute the P-Code file */

	ret = prun(filename, varsize, strsize);
	if (ret < 0) {
		int errval = errno;
		DEBUGASSERT(errval > 0);

		fprintf(stderr, "ERROR: P-Code execution failed:  %d %d\n", ret, errval);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
