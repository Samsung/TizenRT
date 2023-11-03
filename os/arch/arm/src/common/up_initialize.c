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
 * arch/arm/src/common/up_initialize.c
 *
 *   Copyright (C) 2007-2010, 2012-2013 Gregory Nutt. All rights reserved.
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
#include <tinyara/fs/fs.h>
#include <tinyara/syslog/ramlog.h>
#include <tinyara/syslog/syslog_console.h>
#if defined(CONFIG_BLUETOOTH) && defined(CONFIG_BLUETOOTH_NULL)
#include <tinyara/bluetooth/bt_null.h>
#endif

#include <arch/board/board.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <arch/reboot_reason.h>
#endif

#include "up_arch.h"
#include "up_internal.h"

#ifdef CONFIG_NETDEV_TELNET
#include <tinyara/net/telnet.h>
#endif

#ifdef CONFIG_WATCHDOG_FOR_IRQ
#include <tinyara/arch.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

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

#if defined(CONFIG_ARCH_CALIBRATION) && defined(CONFIG_DEBUG)
static void up_calibratedelay(void)
{
	int i;
	sllwdbg("Beginning 100s delay\n");
	for (i = 0; i < 100; i++) {
		up_mdelay(1000);
	}
	sllwdbg("End 100s delay\n");
}
#else
#define up_calibratedelay()
#endif

/****************************************************************************
 * Name: up_color_intstack
 *
 * Description:
 *   Set the interrupt stack to a value so that later we can determine how
 *   much stack space was used by interrupt handling logic
 *
 ****************************************************************************/

#if defined(CONFIG_STACK_COLORATION) && CONFIG_ARCH_INTERRUPTSTACK > 3
static inline void up_color_intstack(void)
{
#ifdef CONFIG_SMP
  uint32_t *ptr = (uint32_t *)arm_intstack_alloc();
#else
	uint32_t *ptr = (uint32_t *)&g_intstackalloc;
#endif
	ssize_t size;

	for (size = (CONFIG_ARCH_INTERRUPTSTACK & ~3); size > 0; size -= sizeof(uint32_t)) {
		*ptr++ = INTSTACK_COLOR;
	}
}
#else
#define up_color_intstack()
#endif

#ifdef CONFIG_ARCH_NESTED_IRQ_STACK_SIZE
static inline void up_color_nestirqstack(void)
{
	uint32_t *ptr = (uint32_t *)&g_nestedirqstkalloc;
	ssize_t size;

	for (size = (CONFIG_ARCH_NESTED_IRQ_STACK_SIZE & ~3); size > 0; size -= sizeof(uint32_t)) {
		*ptr++ = INTSTACK_COLOR;
	}
}
#else
#define up_color_nestirqstack()
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
	/* Initialize global variables */

	current_regs = NULL;

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_init();
	lldbg("[Reboot Reason] : %d\n", up_reboot_reason_read());
#endif

	/* Calibrate the timing loop */

	up_calibratedelay();

	/* Colorize the interrupt stack */

	up_color_intstack();
	up_color_nestirqstack();

	/* Initialize the interrupt subsystem */

	up_irqinitialize();

	/* Initialize the power management subsystem.  This MCU-specific function
	 * must be called *very* early in the initialization sequence *before* any
	 * other device drivers are initialized (since they may attempt to register
	 * with the power management subsystem).
	 */

#ifdef CONFIG_PM
	up_pminitialize();
#endif

	/* Initialize the DMA subsystem if the weak function up_dmainitialize has been
	 * brought into the build
	 */

#ifdef CONFIG_ARCH_DMA
#ifdef CONFIG_HAVE_WEAKFUNCTIONS
	if (up_dmainitialize)
#endif
	{
		up_dmainitialize();
	}
#endif

	/* Initialize the system timer interrupt */

#if !defined(CONFIG_SUPPRESS_INTERRUPTS) && !defined(CONFIG_SUPPRESS_TIMER_INTS) && \
	!defined(CONFIG_SYSTEMTICK_EXTCLK)
	up_timer_initialize();

#ifdef CONFIG_WATCHDOG_FOR_IRQ
#if ((CONFIG_WATCHDOG_FOR_IRQ_INTERVAL * 1000) <= CONFIG_USEC_PER_TICK)
#error "CONFIG_WATCHDOG_FOR_IRQ_INTERVAL should be greater than CONFIG_USEC_PER_TICK"
#endif
	up_wdog_init(CONFIG_WATCHDOG_FOR_IRQ_INTERVAL);
#endif
#endif

	/* Initialize pipe */

#if defined(CONFIG_PIPES) && CONFIG_DEV_PIPE_SIZE > 0
	pipe_initialize();
#endif

	/* Register devices */

#if CONFIG_NFILE_DESCRIPTORS > 0

#if defined(CONFIG_DEV_NULL)
	devnull_register();			/* Standard /dev/null */
#endif

#ifdef CONFIG_VIDEO_NULL
	video_null_initialize("/dev/video0");	/* Standard /dev/video0 */
#endif

#if defined(CONFIG_BLUETOOTH) && defined(CONFIG_BLUETOOTH_NULL)
	btnull_register();    /* bluetooth bt_null */
#endif

#ifdef CONFIG_DEV_URANDOM
	devurandom_register();			/* /dev/urandom */
#endif

#if defined(CONFIG_DEV_ZERO)
	devzero_register();			/* Standard /dev/zero */
#endif

#endif							/* CONFIG_NFILE_DESCRIPTORS */

	/* Initialize the serial device driver */

#ifdef USE_SERIALDRIVER
	up_serialinit();
#endif

	/* Initialize the console device driver (if it is other than the standard
	 * serial driver).
	 */

#if defined(CONFIG_DEV_LOWCONSOLE)
	lowconsole_init();
#elif defined(CONFIG_SYSLOG_CONSOLE)
	syslog_console_init();
#elif defined(CONFIG_RAMLOG_CONSOLE)
	ramlog_consoleinit();
#endif

	/* Initialize the Random Number Generator (RNG)  */

#ifdef CONFIG_DEV_RANDOM
	up_rnginitialize();
#endif

	/* Initialize the system logging device */

#ifdef CONFIG_SYSLOG_CHAR
	syslog_initialize();
#endif
#ifdef CONFIG_RAMLOG_SYSLOG
	ramlog_sysloginit();
#endif

#ifdef CONFIG_NETDEV_TELNET
	/* Initialize the Telnet session factory */

	(void)telnet_initialize();
#endif

#ifdef CONFIG_ERROR_REPORT
	/* Initialize Error Reporting for network */
	error_report_init();
#endif
	/* Initialize the network */

	up_netinitialize();

	/* Initialize USB -- device and/or host */

	up_usbinitialize();

	/* Initialize the L2 cache if present and selected */

	up_l2ccinitialize();
	board_led_on(LED_IRQSENABLED);
}
