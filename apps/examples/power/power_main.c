/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 * Author: Nitish Ambastha <nitish.a@samsung.com>
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
#include <tinyara/pm/pm.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PM_DRVPATH "/dev/pm"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int is_running;


static int pm_sleep_test(void *args)
{
	printf("pm sleep thread start (time: %d)\n", (int)args);

	int fd = open(PM_DRVPATH, O_WRONLY);
	if (fd < 0) {
		printf("Fail to open pm sleep(errno %d)", get_errno());
		return -1;
	}

	while (is_running) {
		if (ioctl(fd, PMIOC_SLEEP, (int)args) < 0) {
			printf("Fail to pm sleep(errno %d)\n", get_errno());
			close(fd);
			return -1;
		}
	}
	close(fd);
	return 0;
}

static void _pm_suspend(char *name)
{
	pm_domain_arg_t domain_arg;
	int domain_id;
	int fd = open(PM_DRVPATH, O_WRONLY);
	if (fd < 0) {
		printf("Fail to open pm(errno %d)", get_errno());
		return;
	}
	domain_arg.domain_name = name;
	if (ioctl(fd, PMIOC_DOMAIN_REGISTER, &domain_arg) < 0) {
		printf("Fail to register pm domain(errno %d)", get_errno());
		close(fd);
		return;
	}
	domain_id = domain_arg.domain_id;

	if(ioctl(fd, PMIOC_SUSPEND, domain_id) < 0) {
		printf("Fail to suspend(errno %d)\n", get_errno());
	}
	close(fd);
}

static void _pm_resume(char *name)
{
	pm_domain_arg_t domain_arg;
	int domain_id;
	int fd = open(PM_DRVPATH, O_WRONLY);
	if (fd < 0) {
		printf("Fail to open pm(errno %d)", get_errno());
		return;
	}
	domain_arg.domain_name = name;
	if (ioctl(fd, PMIOC_DOMAIN_REGISTER, &domain_arg) < 0) {
		printf("Fail to register pm domain(errno %d)", get_errno());
		close(fd);
		return;
	}
	domain_id = domain_arg.domain_id;

	if(ioctl(fd, PMIOC_RESUME, domain_id) < 0) {
		printf("Fail to resume(errno %d)\n", get_errno());
	}
	close(fd);
}

static int pm_suspend_resume_test(void)
{
	printf("pm supend resume repeat test start\n");
	pm_domain_arg_t domain_arg;
	int domain_id;
	int test_count = 0;
	int fd = open(PM_DRVPATH, O_WRONLY);
	if (fd < 0) {
		printf("Fail to open pm(errno %d)", get_errno());
		return -1;
	}

	domain_arg.domain_name = "SUSPEND_TEST";
	if (ioctl(fd, PMIOC_DOMAIN_REGISTER, &domain_arg) < 0) {
		printf("Fail to register pm domain(errno %d)", get_errno());
		close(fd);
		return -1;
	}
	domain_id = domain_arg.domain_id;

	int count = 0;
	while (is_running) {
		if (count == 0) {
			test_count++;
			printf("call suspend(%d)\n", test_count);
			_pm_suspend("SUSPEND_TEST");
		} else if (count == 2) {
			printf("call resume(%d)\n", test_count);
			_pm_resume("SUSPEND_TEST");
		}
		count = (count + 1) % 20;
		sleep(1);
	}

	close(fd);
	return 0;
}

static int start_pm_test(int argc, char *argv[])
{
	pthread_t suspend_resume_tid = 0;
	pthread_t pm_sleep_tid = 0;
	int ret;
	int fd;
	bool suspend_resume_test = false;
	bool timed_wakeup_test = false;
	int timed_wakeup_time = 100;  // 100 ms

	printf("######################### PM LONG TERM TEST START #########################\n");

	fd = open(PM_DRVPATH, O_WRONLY);
	if (fd < 0) {
		printf("Fail to open pm start(errno %d)", get_errno());
		return -1;
	}

	if(ioctl(fd, PMIOC_START, 0) < 0) {
		printf("Fail to pm start(errno %d)\n", get_errno());
		close(fd);
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--lock-test", 12) == 0 || strncmp(argv[i], "-l", 3) == 0) {
			suspend_resume_test = true;

		} else if (strncmp(argv[i], "--timed-wakeup", 15) == 0 || strncmp(argv[i], "-t", 3) == 0) {
			timed_wakeup_test = true;
			if (i + 1 < argc && argv[i + 1][0] != '-') {
				timed_wakeup_time = atoi(argv[++i]);
			}
		}
	}

	if (suspend_resume_test) {
		if (pthread_create(&suspend_resume_tid, NULL, (pthread_startroutine_t)pm_suspend_resume_test, NULL) < 0) {
			printf("Failed to create suspend test pthread(%d):\n", get_errno());
			close(fd);
			return -1;
		}
	}

	if (timed_wakeup_test) {
		if (pthread_create(&pm_sleep_tid, NULL, (pthread_startroutine_t)pm_sleep_test, (void*)timed_wakeup_time) < 0) {
			printf("Failed to create pm sleep pthread(%d):\n", get_errno());
			if (suspend_resume_test) {
				pthread_cancel(suspend_resume_tid);
			}
			close(fd);
			return -1;
		}
		pthread_setname_np(pm_sleep_tid, "pm_sleep_test");
	}

	if (suspend_resume_test) {
		ret = pthread_join(suspend_resume_tid, NULL);
		if (ret != 0) {
			printf("Fail to join suspend_resume_tid thread(%d):\n", ret);
		}
	}

	if (timed_wakeup_test) {
		ret = pthread_join(pm_sleep_tid, NULL);
		if (ret != 0) {
			printf("Fail to join pm_sleep_tid thread(%d):\n", ret);
		}
	}

	close(fd);
	printf("######################### PM LONG TERM TEST END #########################\n");

	return 0;
}

static void help_func(void)
{
	printf("usage: power <command> \n\n");
	printf("The example power is intended for testing power management.\n");
	printf("\n");
	printf("These are power management test commands:\n");
	printf("\n");
	printf("The basic test involves transitioning the chipset into a power-saving mode (sleep) and subsequently waking it up.\n");
	printf("and it is testing suspending of power managemenet operation and resuming for block ender chipset sleep mode.\n");
	printf("   start [options]\t\t Start power management test\n");
	printf("   stop           \t\t stop power management test\n");
	printf("   options: -l, --lock-test              \t\t Test pm suspend/resume API\n");
	printf("            -t, --timed-wakeup [time(ms)]\t\t Test timed wakeup (default 100ms)\n");
	printf("\n");
	printf("start and stop are used to control the power management test.\n");
	printf("   suspend <name>\t\t Suspend power management test\n");
	printf("   resume  <name>\t\t Start power management test\n");
	printf("\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int power_main(int argc, char *argv[])
#endif
{
	int pid;

	if (argc < 2 || strncmp(argv[1], "help", 5) == 0) {
		help_func();
		return 0;
	}

	if (strncmp(argv[1], "start", 6) == 0) {
		if (is_running) {
			printf("power test is already running\n");
			return 0;
		}

		is_running = true;

		pid = task_create("start_pm_test", 100, 1024, start_pm_test, argv + 1);
		if (pid < 0) {
			printf("Fail to create start_pm_test task(errno %d)\n", get_errno());
			is_running = false;
			return -1;
		}


	} else if (strncmp(argv[1], "stop", 5) == 0) {
		if (!is_running) {
			printf("power test is not running\n");
			return 0;
		}

		is_running = false;

	} else if (strncmp(argv[1], "suspend", 8) == 0 && argc == 3) {
		_pm_suspend(argv[2]);
		printf("Done pm suspend domain: %s\n", argv[2]);
	} else if (strncmp(argv[1], "resume", 7) == 0 && argc == 3) {
		_pm_resume(argv[2]);
		printf("Done pm resume domain: %s\n", argv[2]);
	} else {
		help_func();
	}

	return EXIT_SUCCESS;
}
