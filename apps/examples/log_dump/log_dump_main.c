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
 * log_dump_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int log_dump_main(int argc, char *argv[])
#endif
{
	int ret = 1;
	char buf[READ_BUFFER_SIZE];
	int fd = OPEN_LOGDUMP();

	if (fd < 0) {
		printf("Please ensure that procfs is mounted\n");
		return -1;
	}

	/* Log dump starts automatically from boot up.
	 * To test start, intentionally add to stop. */
	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("Failed to stop log dump, errno %d\n", get_errno());
		goto errout;
	}

	printf("This Log Should NOT be saved!!!\n");
	sleep(1);

	printf("\n*********************   LOG DUMP START  *********************\n");

	if (START_LOGDUMP_SAVE(fd) < 0)	{
		printf("Failed to start log dump, errno %d\n", get_errno());
		goto errout;
	}

	printf("This Log Should be saved!!!\n");
	sleep(1);

	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("Failed to stop log dump, errno %d\n", get_errno());
		goto errout;
	}

	while (ret > 0) {
		ret = READ_LOGDUMP(fd, buf, sizeof(buf));
		for (int i = 0; i < ret; i++) {
			printf("%c", buf[i]);
		}
	}

        printf("\n*********************   LOG DUMP END  ***********************\n");

	CLOSE_LOGDUMP(fd);

	return 0;

errout:
	CLOSE_LOGDUMP(fd);
	return -1;
}
