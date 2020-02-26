/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <errno.h>
#include <stdbool.h>
#include <sys/mount.h>
#endif
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>

#include "utils_proc.h"

#define PM_BUFLEN 256
#define PM_INFO_MAX 100

static char* pm_state[5] = {
	"NORMAL",
	"IDLE",
	"STANDBY",
	"SLEEP",
	"COUNT"
};

static void print_line(int loop)
{
	int i;

	printf("\n-------------------------------------");
	for (i = 0; i < loop; i++) {
		printf("----------------------");
	}
}

static void pm_print_values(char *buf)
{
	int i;
	int j;
	bool is_first = true;
	int driver_cnt;
	pm_data pm_info[PM_INFO_MAX];

	pm_info[0] = buf;
	for (i = 0; strncmp(pm_info[i], "eof", strlen("eof") + 1) != 0; i++) {
		pm_info[i] = strtok_r(pm_info[i], " ", &pm_info[i + 1]);
	}

	printf("\n STATE  | STAY(SEC) |");
	for (i = 0; i < 4; i++) {
		printf("\n---------------------");
		printf("\n%7s | %9s |", pm_state[i], pm_info[i]);
	}
	printf("\n---------------------\n\n\n");

	for (i = 5; strncmp(pm_info[i], "eof", strlen("eof") + 1) != 0; i += j + 4) {
		driver_cnt = atoi(pm_info[i]);
		if (is_first) {
			printf(" STATE  | STAY(SEC) | WAKEUP REASON ");
			for (j = 0; j < driver_cnt; j++) {
				printf("| drv_name , activity ");
			}
			printf("|");
			is_first = false;
		}
		print_line(driver_cnt);

		printf("\n%7s | %9s | %13s |", pm_state[atoi(pm_info[i + 1])], pm_info[i + 2], pm_info[i + 3]);
		for (j = 0; j < (driver_cnt * 2); j += 2) {
			printf(" %8s , %8s |", pm_info[i + 4 + j], pm_info[i + 5 + j]);
		}
	}

	if (!is_first) {
		print_line(driver_cnt);
	}
	printf("\n\nCurrent State : %s\n", pm_state[atoi(pm_info[4])]);
	printf("\n");
}

static int pminfo_read_proc(void)
{
	int ret;
	char *filepath;
	char buf[PM_BUFLEN];

	asprintf(&filepath, "%s/%s", PROCFS_MOUNT_POINT, "pminfo");
	ret = utils_readfile(filepath, buf, PM_BUFLEN, pm_print_values);
	if (ret != OK) {
		printf("Failed to read %s\n", filepath);
	}
	free(filepath);
	return ret;
}

int utils_pminfo(int argc, char **args)
{
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	int ret;
	bool is_mounted;

	is_mounted = false;

	/* Mount Procfs to use */
	ret = mount(NULL, PROCFS_MOUNT_POINT, PROCFS_FSTYPE, 0, NULL);
	if (ret == ERROR) {
		if (errno == EEXIST) {
			is_mounted = true;
		} else {
			printf("Failed to mount procfs : %d\n", errno);
			return ERROR;
		}
	}
#endif

	/* Print information for each task/thread */
	pminfo_read_proc();

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	return OK;
}
