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
 * @file write_remove_test.c
 * @brief This is test application for smart filesystem.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include <tinyara/fs/fs_utils.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define WRITE_DATA_SIZE 1024
/****************************************************************************
 * Private data
 ****************************************************************************/
struct pthread_arg {
	int argc;
	char **argv;
};

static int make_file(char *filename)
{
	FILE *fp;
	int i;

	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("Cannot make file %s\n", filename);
		return -1;
	}

	for (i = 0; i < WRITE_DATA_SIZE; i++) {
		if (fwrite(&i, 4, 1, fp) < 0) {
			printf("Cannot write file %d\n", errno);
			fclose(fp);
			return -1;
		}
	}

	if (fclose(fp) != 0) {
		printf("Cannot close file %s\n", filename);
		return -1;
	}
	return OK;
}

int wr_commands(void *args)
{
	char **argv;
	int repeat;
	int count;
	int ret;
	char path[100] = "/mnt/wr_test";

	argv = ((struct pthread_arg *)args)->argv;

	repeat = atoi(argv[1]);
	printf("repeat = %d\n", repeat);

	printf("\n");

	for (count = 1; count != repeat; count++) {
		printf("\r%d", count);

		//write file
		if (make_file(path) < 0) {
			return -1;
		}
		//remove file
		ret = unlink(path);
		if (ret < 0) {
			printf("unlink fail [%d]\n", ret);
			return -1;
		}
	}
	printf("\ntest complete!\n");

	return 0;
}

void usage_write_remove(void)
{
	printf("Usage : \n");
	printf("write and remove test for filesystem(smartfs) \n");
	printf("ex) write_remove 10\n");
}

/****************************************************************************
 * fs_command_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int write_remove_test_main(int argc, char **argv)
#endif
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	struct pthread_arg args;

	args.argc = argc;
	args.argv = argv;

	if (argc != 2) {
		usage_write_remove();
		return 0;
	}

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
	r = pthread_create(&tid, &attr, (pthread_startroutine_t) wr_commands, (void *)&args);
	if (r != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;

}
