/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an
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
#include <sched.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tinyara/cpu_driver.h>
#include <apps/shell/tash.h>
#include <apps/system/cpucontrol.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

const char *cpu_states[] = {
	"running",			//0
	"offline",			//1
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/


static int cpucontrol_cpu_on(int argc, char *argv[]);
static int cpucontrol_cpu_off(int argc, char *argv[]);
static int cpucontrol_cpu_status(int argc, char *argv[]);
static int cpucontrol_help(int argc, char *argv[]);
static int cpucontrol_main(int argc, char *argv[]);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int cpucontrol_cpu_on(int argc, char *argv[])
{
	int fd, state, cpu_num;
	char devname[16];
	int ncpus = sched_getcpucount();

	if (argc < 3) {
		printf("Usage: cpuctrl on <cpu_id>\n");
		printf("Example: cpuctrl on 1\n");
		return -EINVAL;
	}

	cpu_num = atoi(argv[2]);
	if (cpu_num <= 0 || cpu_num >= ncpus) {
		printf("Invalid CPU ID %d. Valid range: 1-%d\n", cpu_num, ncpus - 1);
		return -EINVAL;
	}

	printf("powering up cpu%d!\n", cpu_num);
	snprintf(devname, sizeof(devname), "/dev/cpu%d", cpu_num);
	fd = open(devname, O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s: errno = %d\n", devname, get_errno());
		return -get_errno();
	}

	if (ioctl(fd, IOC_CPU_ENABLE, 1) < 0) {
		printf("IOC_CPU_ENABLE ioctl failed for %s : errno =  %d\n", devname, get_errno());
		close(fd);
		return -get_errno();
	}
	printf("cpu%d successfully enabled\n", cpu_num);

	if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
		printf("IOC_CPU_GET_STATE ioctl failed for %s : errno = %d\n", devname, get_errno());
		close(fd);
		return -get_errno();
	}

	printf("state: %d (%s)\n", state, cpu_states[state]);
	close(fd);
	return 0;
}

static int cpucontrol_cpu_off(int argc, char *argv[])
{
	int fd, state, cpu_num;
	char devname[16];
	int ncpus = sched_getcpucount();

	if (argc < 3) {
		printf("Usage: cpuctrl off <cpu_id>\n");
		printf("Example: cpuctrl off 1\n");
		return -EINVAL;
	}

	cpu_num = atoi(argv[2]);
	if (cpu_num <= 0 || cpu_num >= ncpus) {
		printf("Invalid CPU ID %d. Valid range: 1-%d\n", cpu_num, ncpus - 1);
		return -EINVAL;
	}

	printf("powering down cpu%d!\n", cpu_num);
	snprintf(devname, sizeof(devname), "/dev/cpu%d", cpu_num);
	fd = open(devname, O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s: errno = %d\n", devname, get_errno());
		return -get_errno();
	}

	if (ioctl(fd, IOC_CPU_DISABLE, 1) < 0) {
		printf("IOC_CPU_DISABLE ioctl failed for %s : errno =  %d\n", devname, get_errno());
		close(fd);
		return -get_errno();
	}
	printf("cpu%d successfully disabled\n", cpu_num);

	if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
		printf("IOC_CPU_GET_STATE ioctl failed for %s : errno = %d\n", devname, get_errno());
		close(fd);
		return -get_errno();
	}
	printf("state: %d (%s)\n", state, cpu_states[state]);
	close(fd);
	return 0;
}

static int cpucontrol_cpu_status(int argc, char *argv[])
{
	int fd, state;
	char devname[16];
	int ncpus = sched_getcpucount();

	for (u8 i = 1; i < ncpus; i++) {
		snprintf(devname, sizeof(devname), "/dev/cpu%d", i);

		fd = open(devname, O_RDWR);
		if (fd < 0) {
			printf("Cannot open %s: errno = %d\n", devname, get_errno());
			continue;
		}

		if (ioctl(fd, IOC_CPU_GET_STATE, &state) < 0) {
			printf("IOC_CPU_GET_STATE ioctl failed for %s : errno = %d\n", devname, get_errno());
			close(fd);
			continue;
		}

		printf("cpu state for cpu%d: %d (%s)\n", i, state, cpu_states[state]);
		close(fd);
	}
	return 0;
}

static int cpucontrol_help(int argc, char *argv[])
{
	printf("CPU Control Commands:\n");
	printf("  cpuctrl on <cpu_id>   - Power on specified CPU\n");
	printf("  cpuctrl off <cpu_id>  - Power off specified CPU\n");
	printf("  cpuctrl status        - Show status of all secondary CPUs\n");
	printf("  cpuctrl help          - Show this help message\n");
	printf("\nExamples:\n");
	printf("  cpuctrl on 1          - Power on CPU1\n");
	printf("  cpuctrl off 1         - Power off CPU1\n");
	printf("  cpuctrl status        - Show all CPU states\n");
	printf("\nNote: CPU control operations can only be performed from CPU0\n");
	return 0;
}

static int cpucontrol_main(int argc, char *argv[])
{
	if (argc < 2) {
		return cpucontrol_help(argc, argv);
	}

	if (strncmp(argv[1], "on", 3) == 0) {
		return cpucontrol_cpu_on(argc, argv);
	} else if (strncmp(argv[1], "off", 4) == 0) {
		return cpucontrol_cpu_off(argc, argv);
	} else if (strncmp(argv[1], "help", 5) == 0) {
		return cpucontrol_help(argc, argv);
	} else if (strncmp(argv[1], "status", 7) == 0) {
		return cpucontrol_cpu_status(argc, argv);
	} else {
		printf("Unknown subcommand: %s\n", argv[1]);
		return cpucontrol_help(argc, argv);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void cpucontrol_register_cmds(void)
{
	/* Register the CPU control command during system initialization */
	tash_cmd_install("cpuctrl", cpucontrol_main, TASH_EXECMD_ASYNC);
}
