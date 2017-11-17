/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
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
/// @file   tash_init.c
/// @brief  functions to initialize TinyAra SHell (TASH)

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#if defined(CONFIG_CDCACM_CONSOLE)
#include <tinyara/usb/cdcacm.h>
#endif
#include "tash_internal.h"

#ifdef CONFIG_TASH
/** @brief TASH non FTDI usb console init
 *  @ingroup tash
 */
#if defined(CONFIG_CDCACM_CONSOLE)
static int tash_nonftdi_usbconinit(void)
{
	int ret = OK;
	int nlc;
	int fd;
	char inch;
	ssize_t nbytes;

#if defined(CONFIG_CDCACM)
#define CONFIG_TASH_USBCONSOLE "/dev/ttyACM0"
#define CONFIG_USBDEV_MINOR 0
	ret = cdcacm_initialize(CONFIG_USBDEV_MINOR, NULL);
	/* For future
	   if (ret != 0)
	   RETRY logic
	 */
#endif

 restart:
	do {
		/* Try to open the console */

		fd = open(CONFIG_TASH_USBCONSOLE, O_RDWR);
		if (fd < 0) {
			/* ENOTCONN means that the USB device is not yet connected. Anything
			 * else is bad.
			 */

			//DEBUGASSERT(errno == ENOTCONN);

			/* Sleep a bit and try again */

			sleep(2);
		}
	} while (fd < 0);

	nlc = 0;
	do {
		/* Read one byte */

		inch = 0;
		nbytes = read(fd, &inch, 1);

		/* Is it a carriage return (or maybe a newline)? */

		if (nbytes == 1 && (inch == '\n' || inch == '\r')) {
			/* Yes.. increment the count */

			nlc++;
		} else {
			/* No.. Reset the count.  We need to see 3 in a row to continue. */

			nlc = 0;

			/* If a read error occurred (nbytes < 0) or an end-of-file was
			 * encountered (nbytes == 0), then close the driver and start
			 * over.
			 */

			if (nbytes <= 0) {
				(void)close(fd);
				goto restart;
			}
		}
	} while (nlc < 2);

	(void)fclose(stdin);
	(void)fclose(stdout);
	(void)fclose(stderr);
	/* Dup the fd to create standard fd 0-2 */
	(void)dup2(fd, 0);
	(void)dup2(fd, 1);
	(void)dup2(fd, 2);
	/* fdopen to get the stdin, stdout and stderr streams. The following logic depends
	 * on the fact that the library layer will allocate FILEs in order.  And since
	 * we closed stdin, stdout, and stderr above, that is what we should get.
	 *
	 * fd = 0 is stdin  (read-only)
	 * fd = 1 is stdout (write-only, append)
	 * fd = 2 is stderr (write-only, append)
	 */
	(void)fdopen(0, "r");
	(void)fdopen(1, "a");
	(void)fdopen(2, "a");

	if (fd > 2) {
		close(fd);
	}

	return ret;
}
#endif

int tash_init(void)
{
	int ret = OK;

	tash_register_basic_cmds();
#if defined(CONFIG_CDCACM_CONSOLE)
	if (tash_nonftdi_usbconinit() != 0) {
		ret = ERROR;
	}
#endif

	return ret;
}
#endif							/* CONFIG_TASH */
