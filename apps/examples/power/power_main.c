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
#define POWER_THREAD_SIZE				2048
#define POWER_THREAD_PRIORITY				255
#define PM_DAEMON_SLEEP_INTERVAL			5	//Seconds
#define PM_WAKEUP_TIMER_DURATION			5000000	//Microseconds

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* PM_UNLOCK request is always accompanied by a timer interval duration in microseconds.
 * A timer interrupt is scheduled to wake up the board after this duration.
 * Replace the timer interval duration with 0 to avoid the use of the timer interrupt.
 */
static pthread_addr_t power_daemon()
{
	int ret = 0;
	int fd_lock, fd_unlock;

	/* Release PM lock after bootup*/
	fd_unlock = open(PM_UNLOCK_PATH, O_WRONLY);
	if (fd_unlock < 0) {
		printf("Failed to open PM_UNLOCK file, ret: %d\n", fd_unlock);
		return NULL;
	}
	fd_lock = open(PM_LOCK_PATH, O_WRONLY);
	if (fd_lock < 0) {
		printf("Failed to open PM_LOCK file, ret: %d\n", fd_lock);
		close(fd_unlock);
		return NULL;
	}

	ret = write(fd_unlock, NULL, PM_WAKEUP_TIMER_DURATION);
	if (ret < 0) {
		printf("Failed to unlock PM state transition, ret: %d\n", ret);
		goto errout_with_fd;
	}
	printf("Boot completed ..Unlocked PM!!\n");

	do {
		printf("Locking PM state, transition not expected during APP sleep\n");
		/* Apply PM lock to prevent sleep*/
		ret = write(fd_lock, NULL, 0);
		if (ret < 0) {
			printf("Failed to lock PM state transition, ret: %d\n", ret);
			goto errout_with_fd;
		}

		/* Trigger APP sleep, but we do not expect PM state transition */
		sleep(PM_DAEMON_SLEEP_INTERVAL);
		printf("1st APP sleep complete\n\n");

		printf("Unlocking PM state, transition expected during APP sleep\n");
		/* Release PM lock for sleep */
		ret = write(fd_unlock, NULL, PM_WAKEUP_TIMER_DURATION);
		if (ret < 0) {
			printf("Failed to unlock PM state transition, ret: %d\n", ret);
			goto errout_with_fd;
		}

		/* Trigger APP sleep, this time we expect PM state transition */
		sleep(PM_DAEMON_SLEEP_INTERVAL);
		printf("2nd APP sleep complete\n\n");

		printf("##########################################################\n\n");
	} while (1);

errout_with_fd:
	close(fd_unlock);
	close(fd_lock);
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

	ret = pthread_attr_setstacksize(&attr, POWER_THREAD_SIZE);
	if (ret != 0) {
		fprintf(stderr, "failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = POWER_THREAD_PRIORITY;
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

	printf("Power Daemon Start!!\n");
	/* Create Power Thread */
	tid = create_power_thread();
	if (tid < 0) {
		return EXIT_FAILURE;
	}

	pthread_join(tid, NULL);

	return EXIT_SUCCESS;
}
