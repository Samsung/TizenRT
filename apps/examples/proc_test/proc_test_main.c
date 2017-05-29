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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PROC_MOUNTPOINT "/proc"

#define PROC_BUFFER_LEN 128
#define PROC_FILEPATH_LEN CONFIG_PATH_MAX
#define TASK_STACK_SIZE 512
#define INTERVAL_UPDATE_SEC 10

typedef int (*direntry_handler_t)(FAR const char *dirpath,
								  FAR struct dirent *entryp,
								  FAR void *pvarg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

const char *g_proc_pid_entries[] = {
	"status",
	"cmdline",
#ifdef CONFIG_SCHED_CPULOAD
	"loadavg",
#endif
	"stack",
	"group/status",
	"group/fd",
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

int foreach_proc_entry(direntry_handler_t handler)
{
	DIR *dirp;
	FAR const char *dirpath = PROC_MOUNTPOINT;
	int ret = OK;

	/* Open the directory */
	dirp = opendir(dirpath);

	if (!dirp) {
		/* Failed to open the directory */
		printf("Error : no such %s: %s\n", "directory", dirpath);
		return ERROR;
	}

	/* Read each directory entry */
	for (;;) {
		struct dirent *entryp = readdir(dirp);
		if (!entryp) {
			/* Finished with this directory */
			break;
		}

		/* Call the handler with this directory entry */
		if (handler(dirpath, entryp, NULL) < 0) {
			/* The handler reported a problem */
			ret = ERROR;
			break;
		}
	}
	closedir(dirp);
	return ret;
}

int readfile(FAR const char *filepath)
{
	int fd;
	ssize_t nread;
	char buffer[PROC_BUFFER_LEN];

	nread = 0;

	/* Open the file */
	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		printf("Failed to open %s\n", filepath);
		return ERROR;
	}

	/* Initialize buffer for read operation */
	memset(buffer, 0, PROC_BUFFER_LEN);

	do {
		nread = read(fd, buffer, PROC_BUFFER_LEN - 1);
		if (nread < 0) {
			/* Read error */
			printf("Failed to read : %d\n", errno);
			close(fd);
			return ERROR;
		}
		buffer[nread] = '\0';
		printf("%s", buffer);
	} while (nread == PROC_BUFFER_LEN - 1);

	printf("\n");

	/* Close the file and return. */
	close(fd);
	return nread;
}

static int read_proc_entry(FAR const char *dirpath, FAR struct dirent *entryp, FAR void *pvarg)
{
	/* This function is for showing all file entries of /proc like uptime, version */
	int ret;
	char filepath[PROC_FILEPATH_LEN];

	if (DIRENT_ISDIRECTORY(entryp->d_type)) {
		/* Is a directory... skip this entry */
		return OK;
	}

	snprintf(filepath, PROC_FILEPATH_LEN, "%s/%s", PROC_MOUNTPOINT, entryp->d_name);

	printf("%s : ", entryp->d_name);

	ret = readfile(filepath);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}
	printf("----------------------------------------\n");

	return OK;
}

static int read_pid_entry(FAR const char *dirpath, FAR struct dirent *entryp, FAR void *pvarg)
{
	/* This function is for showing all file entries of /proc/[pid] */
	int ret;
	int i;
	char filepath[PROC_FILEPATH_LEN];

	if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
		/* Not a directory... skip this entry */
		return OK;
	}

	for (i = 0; i < NAME_MAX && entryp->d_name[i] != '\0'; i++) {
		if (!isdigit(entryp->d_name[i])) {
			/* Name contains something other than a numeric character */
			return OK;
		}
	}

	printf("[TASK %s] \n", entryp->d_name);

	for (i = 0; i < sizeof(g_proc_pid_entries) / sizeof(g_proc_pid_entries[0]); i++) {
		/* Read values from entries of /proc/[pid] */
		snprintf(filepath, PROC_FILEPATH_LEN, "%s/%s/%s", PROC_MOUNTPOINT, entryp->d_name, g_proc_pid_entries[i]);
		ret = readfile(filepath);
		if (ret < 0) {
			printf("Failed to read %s\n", filepath);
			return ERROR;
		}
	}
	printf("----------------------------------\n");

	return OK;
}

/****************************************************************************
 * proc_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int proc_test_main(int argc, char *argv[])
#endif
{
	int ret;

	printf("Proc Test START!!\n");

	ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
	if (ret == ERROR && errno != EEXIST) {
		printf("Failed to mount procfs : %d\n", errno);
		return 0;
	}

	while (1) {
		foreach_proc_entry(read_proc_entry);
#ifndef CONFIG_FS_PROCFS_EXCLUDE_PROCESS
		foreach_proc_entry(read_pid_entry);
#endif
		sleep(INTERVAL_UPDATE_SEC);
	}

	return 0;
}
