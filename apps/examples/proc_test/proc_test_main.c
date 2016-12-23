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
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PROC_MOUNTPOINT "/proc"

#define PROC_BUFFER_LEN 128
#define TASK_STACK_SIZE 512

#define INTERVAL_UPDATE_SEC 2

typedef int (*direntry_handler_t)(FAR const char *dirpath,
								  FAR struct dirent *entryp,
								  FAR void *pvarg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
char g_proc_iobuffer[PROC_BUFFER_LEN];

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)
int proc_read_entry(direntry_handler_t handler)
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

		printf("");

		/* Call the handler with this directory entry */
		if (handler(dirpath, entryp, NULL) < 0) {
			/* The handler reported a problem */
			ret = ERROR;
			break;
		}
	}
	closedir(dirp);
	printf("===================================\n");
	return ret;
}

int readfile(FAR const char *filepath)
{
	int fd;
	ssize_t nread;

	nread = 0;

	/* Open the file */
	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		printf("Open error : %s\n", filepath);
		return ERROR;
	}

	/* Initialize buffer for read operation */
	memset(g_proc_iobuffer, 0, PROC_BUFFER_LEN);

	nread = read(fd, g_proc_iobuffer, PROC_BUFFER_LEN - 1);
	if (nread < 0) {
		/* Read error */
		int errcode = errno;
		DEBUGASSERT(errcode > 0);
		close(fd);
		return ERROR;
	}

	g_proc_iobuffer[nread] = '\0';

	/* Close the file and return. */
	close(fd);
	return nread;
}

static int proc_pid(FAR const char *dirpath, FAR struct dirent *entryp, FAR void *pvarg)
{
	FAR char *filepath;
	int ret;
	int i;

	/* Task/thread entries in the /proc directory will all be (1) directories with
	 * (2) all numeric names.
	 */
	if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
		/* Not a directory... skip this entry */
		return OK;
	}

	/* Check each character in the name */
	for (i = 0; i < NAME_MAX && entryp->d_name[i] != '\0'; i++) {
		if (!isdigit(entryp->d_name[i])) {
			/* Name contains something other than a numeric character */
			return OK;
		}
	}

	/* Get the task status */

	printf("[TASK %s] \n", entryp->d_name);

	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/status", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("%s\n", g_proc_iobuffer);
	}
#ifdef CONFIG_SCHED_CPULOAD

	/* Get the CPU load */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/loadavg", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("loadavg : %s\n", g_proc_iobuffer);
	}
#endif

	/* Read the task/tread command line */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/cmdline", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("cmdline  : %s", g_proc_iobuffer);
	}

	/* Read the stack */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/stack", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("%s\n", g_proc_iobuffer);
	}

	/* Read the Group status */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/group/status", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("Group status :\n");
		printf("%s\n", g_proc_iobuffer);
	}

	/* Read the Group fd */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/group/fd", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("Group File decriptor :\n");
		printf("%s\n", g_proc_iobuffer);
	}

	return OK;
}

#ifdef CONFIG_SCHED_CPULOAD
static int proc_cpu(FAR const char *dirpath, FAR struct dirent *entryp, FAR void *pvarg)
{
	FAR char *filepath;
	int ret;
	int i;

	/* Task/thread entries in the /proc directory will all be (1) directories
	 * with (2) all numeric names.
	 */
	if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
		/* Not a directory... skip this entry */
		return OK;
	}

	/* Check each character in the name */
	for (i = 0; i < NAME_MAX && entryp->d_name[i] != '\0'; i++) {
		if (!isdigit(entryp->d_name[i])) {
			/* Name contains something other than a numeric character */
			return OK;
		}
	}

	/* Read the CPU load */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/loadavg", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("%6s  ", g_proc_iobuffer);
	}

	/* Read the task/tread command line */
	filepath = NULL;
	ret = asprintf(&filepath, "%s/%s/cmdline", dirpath, entryp->d_name);
	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("%s ", g_proc_iobuffer);
	}

	return OK;
}
#endif

#if defined(CONFIG_SCHED_CPULOAD) && !defined(CONFIG_FS_PROCFS_EXCLUDE_CPULOAD)
int proc_cpuload_test()
{
	char *filepath = NULL;
	int ret;

	ret = asprintf(&filepath, "%s/cpuload", PROC_MOUNTPOINT);

	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("======= CPU load Information =======\n");
		printf("CPU load : %s\n", g_proc_iobuffer);
		printf("===================================\n");
	}
	return OK;
}
#endif

#ifndef CONFIG_FS_PROCFS_EXCLUDE_UPTIME
int proc_uptime_test()
{
	char *filepath = NULL;
	int ret;

	ret = asprintf(&filepath, "%s/uptime", PROC_MOUNTPOINT);

	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("======= Uptime Information =======\n");
		printf("uptime : %s\n", g_proc_iobuffer);
		printf("===================================\n");
	}
	return OK;
}
#endif

#ifndef CONFIG_FS_PROCFS_EXCLUDE_VERSION
int proc_version_test()
{
	char *filepath = NULL;
	int ret;

	ret = asprintf(&filepath, "%s/version", PROC_MOUNTPOINT);

	if (ret == ERROR) {
		printf("Error : asprintf\n");
		return ERROR;
	}
	ret = readfile(filepath);
	if (ret >= 0) {
		printf("======= Version Information =======\n");
		printf("%s\n", g_proc_iobuffer);
		printf("===================================\n");
	}
	return OK;
}
#endif
#endif
static int my_task(int argc, char *argv[])
{
	while (1) {
		sleep(10);
	}
	return 0;
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
	FAR const char *filesystem = NULL;
	int ret;

	printf("Proc Test START!!\n");

	ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
	if (ret >= 0) {
		printf("Error : Failed to mount procfs\n");
	}

	int pid;
	pid = task_create("mytask", SCHED_PRIORITY_DEFAULT, TASK_STACK_SIZE,
					  my_task, (char *const *)NULL);
	if (pid == ENOMEM || pid < 0) {
		printf("task create failed : %d\n", pid);
	} else {
		printf("task create succeed : %d\n", pid);
	}

	while (1) {
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)
#ifndef CONFIG_FS_PROCFS_EXCLUDE_PROCESS
		proc_read_entry(proc_pid);
#ifdef CONFIG_SCHED_CPULOAD
		proc_read_entry(proc_cpu);
#endif
#endif
#if defined(CONFIG_SCHED_CPULOAD) && !defined(CONFIG_FS_PROCFS_EXCLUDE_CPULOAD)
		proc_cpuload_test();
#endif
#ifndef CONFIG_FS_PROCFS_EXCLUDE_UPTIME
		proc_uptime_test();
#endif
#ifndef CONFIG_FS_PROCFS_EXCLUDE_VERSION
		proc_version_test();
#endif
#endif
		sleep(INTERVAL_UPDATE_SEC);
	}

	return 0;
}
