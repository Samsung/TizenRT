/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char **argv)
{
	struct timeval tv;
	struct tm *ptm = NULL;
	char tformat[32] = { 0 };

	/* stdout and stderr stream test */

	fprintf(stdout, "%15s: Hello, World!! on stdout\n", argv[0]);
	fprintf(stderr, "%15s: Hello, World!! on stderr\n", argv[0]);

	while (1) {
		/* Get current time of day */
		if (gettimeofday(&tv, NULL)) {
			printf("gettimeofday failed with errno = %d\n", errno);
			exit(EXIT_FAILURE);
		}

		/* Format date & time */
		ptm = localtime(&tv.tv_sec);
		if (!ptm) {
			printf("localtime failed\n");
			exit(EXIT_FAILURE);
		}

		if (strftime(tformat, sizeof(tformat), "%H:%M:%S", ptm) == 0) {
			printf("strftime failed\n");
			exit(EXIT_FAILURE);
		}

		printf("%s \talive at %s.%03ld\n", argv[0], tformat, tv.tv_usec / 1000);
		sleep(10);
	}

	return 0;
}
