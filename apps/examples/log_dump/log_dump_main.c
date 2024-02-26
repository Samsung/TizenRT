/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyara/log_dump/log_dump.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define READ_BUFFER_SIZE	1024	/* user configurable read size */

/****************************************************************************
 * Private functions
 * **************************************************************************/

static void show_usage(void)
{
	printf("\nUsage: log_dump [N]\n");
	printf("Fetches the last N nodes of log dump\n");
	printf("\n  N    The number of nodes to be fetched from the last (newly saved), N > 0\n");
	printf("       If N is not provided then we fetch all the saved nodes\n");
}

/****************************************************************************
 * log_dump_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int log_dump_main(int argc, char *argv[])
#endif
{
	if (argc > 2) {
		show_usage();
		goto errout;
	}

	int ret = 1;
	char buf[READ_BUFFER_SIZE];
	int fd = OPEN_LOGDUMP();

	if (fd < 0) {
		printf("log_dump_main : Please ensure that procfs is mounted\n");
		goto errout;
	}

	/* Log dump starts automatically from boot up.
	 * To test start, intentionally add to stop. */
	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("log_dump_main : Failed to stop log dump, errno %d\n", get_errno());
		goto errout;
	}

	if (argc == 2) {
		int N = strtol(argv[1], NULL, 10);
		if (N < 1) {
			show_usage();
			goto errout;
		}
		/* This is similar to a file seek command. So when N is given
		 * we move the start of the reading pointer to the Nth block
		 * from the end. If N is not provided, we don't change the
		 * reading pointer so we fetch all the saved blocks */
		if (LOG_DUMP_SEEK(fd, N)) {
			printf("log_dump_main : Failed to set the number of blocks to be fetched, errno : %d\n", get_errno());
			goto errout;
		}
	}

	printf("log_dump_main : This Log Should NOT be saved!!!\n");
	sleep(1);

	printf("log_dump_main : \n*********************   LOG DUMP START  *********************\n");

	if (START_LOGDUMP_SAVE(fd) < 0)	{
		printf("log_dump_main : Failed to start log dump, errno %d\n", get_errno());
		goto errout;
	}

	printf("log_dump_main : This Log Should be saved!!!\n");
	sleep(1);

	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("log_dump_main : Failed to stop log dump, errno %d\n", get_errno());
		goto errout;
	}

	while (ret > 0) {
		ret = READ_LOGDUMP(fd, buf, sizeof(buf));
		for (int i = 0; i < ret; i++) {
			printf("%c", buf[i]);
		}
	}

        printf("log_dump_main : \n*********************   LOG DUMP END  ***********************\n");

	CLOSE_LOGDUMP(fd);

	return 0;

errout:
	CLOSE_LOGDUMP(fd);
	return -1;
}
