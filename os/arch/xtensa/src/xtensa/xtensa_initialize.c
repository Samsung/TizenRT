/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/****************************************************************************
 *
 *   Copyright (C) 2016-2017 Gregory Nutt. All rights reserved.
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

#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/net/telnet.h>
#include <tinyara/syslog/syslog.h>
#include <tinyara/syslog/syslog_console.h>
#include <arch/board/board.h>

#include "xtensa.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_calibratedelay
 *
 * Description:
 *   Delay loops are provided for short timing loops.  This function, if
 *   enabled, will just wait for 100 seconds.  Using a stopwatch, you can
 *   can then determine if the timing loops are properly calibrated.
 *
 ****************************************************************************/

#if defined(CONFIG_ARCH_CALIBRATION) && defined(CONFIG_DEBUG_FEATURES)
static void up_calibratedelay(void)
{
	int i;

	_warn("Beginning 100s delay\n");
	for (i = 0; i < 100; i++) {
		up_mdelay(1000);
	}

	_warn("End 100s delay\n");
}
#else
#define up_calibratedelay()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_initialize
 *
 * Description:
 *   up_initialize will be called once during OS initialization after the
 *   basic OS services have been initialized.  The architecture specific
 *   details of initializing the OS will be handled here.  Such things as
 *   setting up interrupt service routines, starting the clock, and
 *   registering device drivers are some of the things that are different
 *   for each processor and hardware platform.
 *
 *   up_initialize is called after the OS initialized but before the user
 *   initialization logic has been started and before the libraries have
 *   been initialized.  OS services and driver services are available.
 *
 ****************************************************************************/

void up_initialize(void)
{

#ifdef CONFIG_SMP
	int i;

	/* Initialize global variables */

	for (i = 0; i < CONFIG_SMP_NCPUS; i++) {
		g_current_regs[i] = NULL;
	}
#else
	CURRENT_REGS = NULL;
#endif

	/* Calibrate the timing loop */

	up_calibratedelay();


	/* Initialize the interrupt subsystem */

	xtensa_irq_initialize();

#ifdef CONFIG_ARCH_DMA
	/* Initialize the DMA subsystem if the weak function xtensa_dma_initialize
	 * has been brought into the build
	 */

#ifdef CONFIG_HAVE_WEAKFUNCTIONS
	if (xtensa_dma_initialize)
#endif
	{
		xtensa_dma_initialize();
	}
#endif

#if !defined(CONFIG_SUPPRESS_INTERRUPTS) && !defined(CONFIG_SUPPRESS_TIMER_INTS)
	/* Initialize the system timer interrupt */

	xtensa_timer_initialize();
#endif

#ifdef CONFIG_MM_IOB
	/* Initialize IO buffering */

	iob_initialize();
#endif

#if defined(CONFIG_PIPES) && CONFIG_DEV_PIPE_SIZE > 0
	/* Initialize pipe */

	pipe_initialize();
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
	/* Register devices */

#if defined(CONFIG_DEV_NULL)
	devnull_register();			/* Standard /dev/null */
#endif

#if defined(CONFIG_DEV_RANDOM)
	devrandom_register();		/* Standard /dev/random */
#endif

#if defined(CONFIG_DEV_URANDOM)
	devurandom_register();		/* Standard /dev/urandom */
#endif

#if defined(CONFIG_DEV_ZERO)
	devzero_register();			/* Standard /dev/zero */
#endif

#if defined(CONFIG_DEV_LOOP)
	loop_register();			/* Standard /dev/loop */
#endif
#endif							/* CONFIG_NFILE_DESCRIPTORS */

#if defined(CONFIG_SCHED_INSTRUMENTATION_BUFFER) && \
	defined(CONFIG_DRIVER_NOTE)
	note_register();			/* Non-standard /dev/note */
#endif

	/* Initialize the serial device driver */

#ifdef USE_SERIALDRIVER
	xtensa_serial_initialize();
#endif

	/* Initialize the console device driver (if it is other than the standard
	 * serial driver).
	 */

#if defined(CONFIG_DEV_LOWCONSOLE)
	lowconsole_init();
#elif defined(CONFIG_CONSOLE_SYSLOG)
	syslog_console_init();
#elif defined(CONFIG_RAMLOG_CONSOLE)
	ramlog_consoleinit();
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && defined(CONFIG_PSEUDOTERM_SUSV1)
	/* Register the master pseudo-terminal multiplexor device */

	(void)ptmx_register();
#endif

	/* Early initialization of the system logging device.  Some SYSLOG channel
	 * can be initialized early in the initialization sequence because they
	 * depend on only minimal OS initialization.
	 */
#ifdef CONFIG_SYSLOG_CHAR
		syslog_initialize();
#endif

#if defined(CONFIG_CRYPTO)
	/* Initialize the HW crypto and /dev/crypto */

	up_cryptoinitialize();
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && defined(CONFIG_CRYPTO_CRYPTODEV)
	devcrypto_register();
#endif

#ifndef CONFIG_NETDEV_LATEINIT
	/* Initialize the network */

	up_netinitialize();
#endif

#ifdef CONFIG_NETDEV_LOOPBACK
	/* Initialize the local loopback device */

	(void)localhost_initialize();
#endif

#ifdef CONFIG_NET_TUN
	/* Initialize the TUN device */

	(void)tun_initialize();
#endif

#ifdef CONFIG_NETDEV_TELNET
	/* Initialize the Telnet session factory */

	(void)telnet_initialize();
#endif

	/* Initialize USB -- device and/or host */

	up_usbinitialize();
	board_autoled_on(LED_IRQSENABLED);

}
