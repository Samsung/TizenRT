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
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <tinyara/assertmode.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void assertmode_help(void)
{
	printf("Usage:\n");
	printf("  assertmode\n");
	printf("  assertmode [MODE]\n");
	printf("  assertmode --help\n");
	printf("To set board with \"AUTO RESET\" or \"SYSTEM HALT\" or \"TASK EXIT/TERMINATE\" on assert.\n");
	printf("\n");
	printf("MODE:\n");
	printf("  0     Auto reset on assert\n");
	printf("  1     System halt on assert\n");
	printf("  2     Task exit/terminate on assert\n");
	printf("\n");
	printf("Notes:\n");
	printf("  Running without arguments shows current status\n");
	printf("  Default depends on configuration:\n");
	printf("    CONFIG_BOARD_ASSERT_AUTORESET=y: default is auto-reset\n");
	printf("    CONFIG_BOARD_ASSERT_SYSTEM_HALT=y: default is halt\n");
	printf("\n");
	printf("Examples:\n");
	printf("  assertmode\n");
	printf("  assertmode 0\n");
	printf("  assertmode 1\n");
	printf("  assertmode 2\n");
}

static const char *assertmode_mode_string(int mode)
{
	switch (mode) {
	case ASSERT_ACTION_AUTORESET:
		return "AUTO RESET";
	case ASSERT_ACTION_HALT:
		return "SYSTEM HALT";
	case ASSERT_ACTION_NONE:
		return "TASK EXIT/TERMINATE";
	default:
		return "UNKNOWN";
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int utils_assertmode(int argc, char **args)
{
	int fd;
	int ret;
	int mode;
	struct assertmode_info_s info;

	fd = open(ASSERTMODE_DRVPATH, O_RDWR);
	if (fd < 0) {
		printf("ERROR: Failed to open %s, errno: %d\n",
		       ASSERTMODE_DRVPATH, get_errno());
		return ERROR;
	}

	if (argc == 1) {
		/* No argument: read and display current action */
		ret = ioctl(fd, ASSERTMODEIOC_GET_MODE, (unsigned long)&info);
		if (ret < 0) {
			printf("ERROR: ioctl GET_MODE failed, errno: %d\n", get_errno());
			close(fd);
			return ERROR;
		}
		/* Print current status first, then show help */
		printf("Current: %s (mode %d)\n", assertmode_mode_string(info.action), info.action);
		assertmode_help();
	} else if (argc == 2) {
		/* Check for --help option */
		if (strcmp(args[1], "--help") == 0) {
			assertmode_help();
			close(fd);
			return OK;
		}

		/* Validate that the argument is a valid number */
		{
			char *endptr;
			long val;

			set_errno(0);
			val = strtol(args[1], &endptr, 10);

			/* Check for conversion errors or invalid characters */
			if (get_errno() != 0 || *endptr != '\0' || endptr == args[1]) {
				/* Invalid input (not a number): show help */
				assertmode_help();
				close(fd);
				return ERROR;
			}

			mode = (int)val;
		}

		if (mode >= ASSERT_ACTION_AUTORESET && mode <= ASSERT_ACTION_NONE) {
			/* Set action 0, 1, or 2 */
			info.action = mode;
			ret = ioctl(fd, ASSERTMODEIOC_SET_MODE, (unsigned long)&info);
			if (ret < 0) {
				printf("ERROR: ioctl SET_MODE failed, errno: %d\n", get_errno());
				close(fd);
				return ERROR;
			}
			/* Only print result on success */
			printf("Set: %s (mode %d)\n", assertmode_mode_string(mode), mode);
		} else {
			/* Invalid mode: show help */
			assertmode_help();
			close(fd);
			return ERROR;
		}
	} else {
		/* Invalid arguments: show help */
		assertmode_help();
		close(fd);
		return ERROR;
	}

	close(fd);
	return OK;
}
