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
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PM_LOCK_PATH						"/proc/power/domains/0/pm_lock"
#define PM_UNLOCK_PATH					"/proc/power/domains/0/pm_unlock"
#define PM_SLEEP_PATH						"/proc/power/domains/0/enter_sleep"

#define POWER_THEAD_SIZE				2048
#define POWER_THEAD_PRIORITY		255
#define PM_DAEMON_INTERVAL			15
#define PM_LOCK(x)								power_set_int(PM_LOCK_PATH, x)
#define PM_UNLOCK(x)							power_set_int(PM_UNLOCK_PATH, x)

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int power_write_buf(char *file, char *buf)
{
	int fd;
	int r;
	int ret = 0;

	fd = open(file, O_WRONLY);
	if (fd == -1)
		return -ENOENT;

	r = write(fd, buf, strlen(buf));
	if (r < 0)
		ret = -EIO;

	close(fd);

	return ret;
}

int power_set_int(char *fname, int val)
{
	char buf[8];
	int ret = 0;

	snprintf(buf, sizeof(buf), "%d", val);

	if (power_write_buf(fname, buf) != 0)
		ret = -EIO;

	return ret;
}

static pthread_addr_t power_daemon()
{
	char *state = "sleep";

	/* Release PM lock after bootup*/
	PM_UNLOCK(PM_NORMAL);
	printf("Boot completed ..Unlocked PM!!\n");

	do {
		/* TODO: Apply logic to detect activity timeout */

		/* Apply PM lock to prevent sleep*/
		PM_LOCK(PM_STANDBY);
		/* We choose 15s duration to trigger sleep */
		sleep(PM_DAEMON_INTERVAL);

		/* Release PM lock for sleep */
		PM_UNLOCK(PM_STANDBY);

		/* Trigger sleep */
		power_write_buf(PM_SLEEP_PATH, state);
	} while (1);

	return NULL;
}

static pthread_t create_power_thread(void)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = OK;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		fprintf(stderr, "failed to set pthread init(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (ret != 0) {
		fprintf(stderr, "failed to set policy(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setstacksize(&attr, POWER_THEAD_SIZE);
	if (ret != 0) {
		fprintf(stderr, "failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = POWER_THEAD_PRIORITY;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		fprintf(stderr, "failed to set sched param(%d)\n", ret);
		return -ret;
	}

	ret = pthread_create(&tid, &attr, power_daemon, NULL);
	if (ret != 0) {
		fprintf(stderr, "failed to create pthread(%d)\n", ret);
		return -ret;
	}

	pthread_setname_np(tid, "power daemon");
	pthread_detach(tid);

	return tid;
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
	pthread_t tid;

	printf("Power Daemon!!\n");
	/* Create Power Thread */
	tid = create_power_thread();
	if (tid < 0) {
		return EXIT_FAILURE;
	}

	pthread_join(tid, NULL);

	return EXIT_SUCCESS;
}
