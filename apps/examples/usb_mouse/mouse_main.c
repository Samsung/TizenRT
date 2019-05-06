/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * examples/usb_mouse/mouse_main.c
 *
 *   Copyright (C) 2011, 2014-2025 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/boardctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/input/mouse.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MOUSE_DEVPATH "/dev/mouse0"

/****************************************************************************
 * Name: mouse_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mouse_main(int argc, char *argv[])
#endif
{
	struct mouse_report_s sample;
	ssize_t nbytes;
	int fd;
	int errval = 0;

	/* If this example is configured as an NX add-on, then limit the number of
    * samples that we collect before returning.  Otherwise, we never return
    */

	/* Open the touchscreen device for reading */

	printf("mouse_main: Opening %s\n", MOUSE_DEVPATH);
	fd = open(MOUSE_DEVPATH, O_RDONLY);
	if (fd < 0) {
		printf("mouse_main: open %s failed: %d\n",
			   MOUSE_DEVPATH, errno);
		errval = 2;
		goto errout_with_tc;
	}

	/* Now loop the appropriate number of times, displaying the collected
    * touchscreen samples.
    */

	for (;;) {
		/* Flush any output before the loop entered or from the previous pass
      * through the loop.
      */

		fflush(stdout);

		/* Read one sample */

		printf("Reading...\n");
		nbytes = read(fd, &sample, sizeof(struct mouse_report_s));
		printf("Bytes read: %d\n", nbytes);

		/* Handle unexpected return values */

		if (nbytes < 0) {
			errval = errno;
			if (errval != EINTR) {
				printf("mouse_main: read %s failed: %d\n",
					   MOUSE_DEVPATH, errval);
				errval = 3;
				goto errout_with_dev;
			}

			printf("mouse_main: Interrupted read...\n");
		} else if (nbytes != sizeof(struct mouse_report_s)) {
			printf("mouse_main: Unexpected read size=%d, expected=%d, Ignoring\n",
				   nbytes, sizeof(struct mouse_report_s));
		}

		/* Print the sample data on successful return */

		else {
			printf("Sample     :\n");
			printf("   buttons : %02x\n", sample.buttons);
			printf("         x : %d\n", sample.x);
			printf("         y : %d\n", sample.y);
#ifdef CONFIG_MOUSE_WHEEL
			printf("     wheel : %d\n", sample.wheel);
#endif
		}
	}

errout_with_dev:
	close(fd);

errout_with_tc:
	printf("Terminating!\n");
	fflush(stdout);
	return errval;
}
