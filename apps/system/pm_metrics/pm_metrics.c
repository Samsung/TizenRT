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
	if (argc >= 2 && !strncmp(argv[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}
	if (!argv[1]) {
		printf("Unable to start PM Monitoring cause time argument not provided\n");
		goto usage;
	}
	int fd = open("/dev/pm", O_RDONLY);
	if (fd < 0) {
		printf("Cannot open /dev/pm file with fd = %d\n", fd);
		return ERROR;
	}

	if (ioctl(fd, PMIOC_METRICS, atoi(argv[1])) < 0) {
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
