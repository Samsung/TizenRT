/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * either express or implied.
 * See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <tinyara/cpu_state.h>
#include <tinyara/cpu_driver.h>
#include <apps/shell/tash.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

#define CPU1_DEVICE_PATH "/dev/cpu1"

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int cputest_cpu1_on(int argc, char *argv[]);
static int cputest_cpu1_off(int argc, char *argv[]);
static int cputest_cpu_status(int argc, char *argv[]);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int cputest_cpu1_on(int argc, char *argv[])
{
	int fd;
	cpu_state_t state;

	if (sched_getcpu() > 0) {
		printf("can only powerup cpu1 from cpu0!\n");
		return 0;
	}

	printf("powering up cpu1!\n");

	fd = open(CPU1_DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d errno = %d\n", CPU1_DEVICE_PATH, fd, errno);
		return 0;
	}

	if (ioctl(fd, IOC_CPU_ENABLE, 1) < 0) {
		printf("ioctl failed errno =  %d\n", errno);
		close(fd);
		return 0;
	}
	printf("cpu1 successfully enabled\n");

	if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
		printf("IOC_CPU_GET_STATE ioctl failed for %s : %d\n", CPU1_DEVICE_PATH, errno);
		close(fd);
		return 0;
	}

	printf("state: %d (0=running,1=hotplug,2=wakefromsleep,3=halt)\n", state);
	close(fd);
	return 0;
}

static int cputest_cpu1_off(int argc, char *argv[])
{
	int fd;
	cpu_state_t state;

	if (sched_getcpu() > 0) {
		printf("can only powerdown cpu1 from cpu0!\n");
		return 0;
	}
	printf("powering down cpu1!\n");

	fd = open(CPU1_DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s file with fd = %d errno = %d\n", CPU1_DEVICE_PATH, fd, errno);
		return 0;
	}

	if (ioctl(fd, IOC_CPU_DISABLE, 1) < 0) {
		printf("ioctl failed errno =  %d\n", errno);
		close(fd);
		return 0;
	}
	printf("cpu1 successfully disabled\n");

	if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
		printf("IOC_CPU_GET_STATE ioctl failed for %s : %d\n", CPU1_DEVICE_PATH, errno);
		close(fd);
		return 0;
	}
	printf("state: %d (0=running,1=hotplug,2=wakefromsleep,3=halt)\n", state);
	close(fd);
	return 0;
}

static int cputest_cpu_status(int argc, char *argv[])
{
	int fd;
	char devname[16];
	cpu_state_t state;

	for (u8 i = 1; i < CONFIG_SMP_NCPUS; i++) {
		snprintf(devname, sizeof(devname), "/dev/cpu%d", i);

		fd = open(devname, O_RDWR);
		if (fd < 0) {
			printf("Cannot open %s file with fd = %d errno = %d\n", devname, fd, errno);
			return 0;
		}

		if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
			printf("IOC_CPU_GET_STATE ioctl failed for %s : %d\n", devname, errno);
			close(fd);
			return 0;
		}

		printf("cpu state for cpu%d: %d\n", i, state);
		close(fd);
	}
	return 0;
}

const static tash_cmdlist_t cputest_cmds[] = {
	{"cpu1on",	cputest_cpu1_on,	TASH_EXECMD_ASYNC},
	{"cpu1off",	cputest_cpu1_off,	TASH_EXECMD_ASYNC},
	{"cpustatus",	cputest_cpu_status,	TASH_EXECMD_ASYNC},
	{NULL,		NULL,		0}
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void cputest_register_cmds(void)
{
	/* Register the CPU test commands during system initialization */
	tash_cmdlist_install(cputest_cmds);
}
