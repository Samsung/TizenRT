/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <tinyara/pm/pm.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int pm_metrics_main(int argc, char *argv[])
#endif
{
	char *last_ch;
	int metric_time;
	if (argc != 2 || !strncmp(argv[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}
	metric_time = (int)strtol(argv[1], &last_ch, 10);
	if ((*last_ch != '\0') || (metric_time < 0)) {
		printf("'%s' is not a valid argument\n", argv[1]);
		goto usage;
	}
	int fd = open(PM_DRVPATH, O_RDONLY);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d\n", PM_DRVPATH, fd);
		return ERROR;
	}

	if (ioctl(fd, PMIOC_METRICS, metric_time) < 0) {
		printf("Unable to perform PM Monitoring, error = %d\n", get_errno());
		close(fd);
		return ERROR;
	} 
	close(fd);
	return OK;
usage:
	printf("\nUsage: pm_metrics <TIME>\n");
	printf("Display PM metrics information after analysing pm activity for specified time\n");
	printf("\n TIME    Duration in milliseconds to run PM Metrics\n");
	return ERROR;
}
