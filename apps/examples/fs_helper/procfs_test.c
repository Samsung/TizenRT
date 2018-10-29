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
/**
 * @file procfs_test.c
 * @brief This is test application for proc file system.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/mount.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BUFFER_SIZE 1024
/****************************************************************************
 * Private data
 ****************************************************************************/
struct pthread_arg {
	int argc;
	char **argv;
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static int read_file(const char *srcpath)
{
	int nbytesread;
	int rdfd;
	static char buffer[BUFFER_SIZE];

	/* Open the source file for reading */

	rdfd = open(srcpath, O_RDONLY);
	if (rdfd < 0) {
		printf("ERROR: Failed to open %s for reading: %s\n", srcpath, strerror(errno));
		return -1;
	}

	/* read the file */

	for (;;) {
		do {
			nbytesread = read(rdfd, buffer, BUFFER_SIZE);
			if (nbytesread == 0) {
				/* End of file */

				close(rdfd);
				return OK;
			} else if (nbytesread < 0) {
				/* EINTR is not an error (but will still stop the copy) */

				printf("ERROR: Read failure: %s\n", strerror(errno));
				return -1;
			}
			printf("read file string[%s]", buffer);

		} while (nbytesread <= 0);
	}
	close(rdfd);
	return OK;
}

int procfs_commands(void *args)
{
	char filename[100] = "/proc/0/status";
	int ret;

	ret = read_file(filename);
	if (ret < 0) {
		printf("procfs error!\n");
	} else {
		printf("success procfs test!\n");
	}
	if (umount("/proc") < 0) {
		printf("umount procfs fail\n");
		return -1;
	}
	return 0;
}

/****************************************************************************
 * fs_command_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int procfs_test_main(int argc, char **argv)
#endif
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	struct pthread_arg args;

	args.argc = argc;
	args.argv = argv;

	/* Initialize the attribute variable */
	r = pthread_attr_init(&attr);
	if (r != 0) {
		printf("%s: pthread_attr_init failed, status=%d\n", __func__, r);
	}

	/* 1. set a priority */
	sparam.sched_priority = 100;
	r = pthread_attr_setschedparam(&attr, &sparam);
	if (r != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
	}

	r = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (r != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
	}

	/* 2. set a stacksize */
	r = pthread_attr_setstacksize(&attr, 51200);
	if (r != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
	}

	/* 3. create pthread with entry function */
	r = pthread_create(&tid, &attr, (pthread_startroutine_t)procfs_commands, (void *)&args);
	if (r != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}
