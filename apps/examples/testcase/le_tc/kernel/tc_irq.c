/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/// @file tc_irq.c

/// @brief Test Case Example for fin_wait API
#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <tinyara/timer.h>
#include <tinyara/irq.h>
#include "tc_internal.h"

# define TIMER_DEVNAME  "/dev/timer0"
# define INTERVAL	100

/**
 * @fn                  :tc_irq
 * @brief               :test the fin_wait using timer
 * @scenario            :fin_wait waits for kernel irq event, so here we will generate the irq using timer.
 * API's covered        :fin_wait
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */

static void tc_irq_fin_wait(void)
{
	struct timer_notify_s notify;
	notify.arg   = NULL;
	notify.pid   = (pid_t)getpid();
	char path[256];
	int ret = 0;

	snprintf(path, _POSIX_PATH_MAX, TIMER_DEVNAME);
	int fd = open(path, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = ioctl(fd, TCIOC_NOTIFICATION, (unsigned long)((uintptr_t)&notify));
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, close(fd));

	ret = ioctl(fd, TCIOC_SETTIMEOUT, INTERVAL);
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, close(fd));

	/* Start the timer */
	ret = ioctl(fd, TCIOC_START, 0);
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, close(fd));

	ret = fin_wait();
	TC_ASSERT_EQ_CLEANUP("fin_wait", ret, 0, close(fd));

	/* stop the timer */
	ret = ioctl(fd, TCIOC_STOP, 0);
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

int irq_main(void)
{
	tc_irq_fin_wait();

	return 0;
}
