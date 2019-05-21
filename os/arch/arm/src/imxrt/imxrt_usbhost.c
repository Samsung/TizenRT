/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
/************************************************************************************
 * arch/arm/src/imxrt_usbhost.c
 *
 *   Copyright (C) 2013, 2015-2017 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/kthread.h>
#include <tinyara/usb/usbdev.h>
#include <tinyara/usb/usbhost.h>
#include <tinyara/usb/usbdev_trace.h>

#include "up_arch.h"
#include "chip.h"
#include "imxrt_gpio.h"
#include "chip/imxrt_pinmux.h"
#include "imxrt_ehci.h"
#include "imxrt_log.h"

#ifdef HAVE_USBHOST

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef CONFIG_USBHOST_DEFPRIO
#define CONFIG_USBHOST_DEFPRIO 50
#endif

#ifndef CONFIG_USBHOST_STACKSIZE
#ifdef CONFIG_USBHOST_HUB
#define CONFIG_USBHOST_STACKSIZE 1536
#else
#define CONFIG_USBHOST_STACKSIZE 1024
#endif
#endif

/************************************************************************************
 * Private Data
 ************************************************************************************/
/* Retained device driver handle */

static struct usbhost_connection_s *g_ehciconn;

/* Overcurrent interrupt handler */

#if 0							/* Not yet implemented */
static xcpt_t g_ochandler;
#endif

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Name: ehci_waiter
 *
 * Description:
 *   Wait for USB devices to be connected to the EHCI root hub.
 *
 ************************************************************************************/

static int ehci_waiter(int argc, char *argv[])
{
	FAR struct usbhost_hubport_s *hport;

	udbg("ehci_waiter:  Running\n");
	for (;;) {
		/* Wait for the device to change state */

		DEBUGVERIFY(CONN_WAIT(g_ehciconn, &hport));
		udbg("ehci_waiter: %s\n", hport->connected ? "connected" : "disconnected");

		/* Did we just become connected? */

		if (hport->connected) {
			/* Yes.. enumerate the newly connected device */

			(void)CONN_ENUMERATE(g_ehciconn, hport);
		}
	}

	/* Keep the compiler from complaining */

	return 0;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: imxrt_usbhost_bootinitialize
 *
 * Description:
 *   Called from imxrt_boardinitialize very early in inialization to setup USB
 *   host-related GPIO pins for the LPC-H3131 board.
 *
 *   SIGNAL      GPIO
 *   ----------- -------
 *   #OTG_PWR_E  GPIO19
 *   #OTG_OVRCR  GPIO20
 *
 ************************************************************************************/

void weak_function imxrt_usbhost_bootinitialize(void)
{
	/* Configure output pin to drive VBUS power (initial state: power off) */

	imxrt_config_gpio(GPIO_USB_OTG1_PWR_1);

	/* Configure input pin to detect overrcurrent errors */

	imxrt_config_gpio(GPIO_USB_OTG1_OC_1);

	/* Configure to receive interrupts on the overrcurrent input pin */
#warning Missing logic
}

/***********************************************************************************
 * Name: imxrt_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host functionality.
 *   This function will start a thread that will monitor for device
 *   connection/disconnection events.
 *
 ***********************************************************************************/

int imxrt_usbhost_initialize(void)
{
	pid_t pid;

	/* Then get an instance of the USB EHCI interface. */

	g_ehciconn = imxrt_ehci_initialize(0);
	if (!g_ehciconn) {
		IMXLOG("ERROR: imxrt_ehci_initialize failed\n");
		return -ENODEV;
	}

	/* Start a thread to handle device connection. */
	pid = kernel_thread("EHCI Monitor", CONFIG_USBHOST_DEFPRIO, CONFIG_USBHOST_STACKSIZE, (main_t)ehci_waiter, (FAR char *const *)NULL);
	if (pid < 0) {
		IMXLOG("ERROR: Failed to create ehci_waiter task\n");
		return pid;
	}

	return OK;
}

/***********************************************************************************
 * Name: imxrt_usbhost_vbusdrive
 *
 * Description:
 *   Enable/disable driving of VBUS 5V output.  This function must be provided by
 *   each platform that implements the OHCI or EHCI host interface
 *
 * Input Parameters:
 *   rhport - Selects root hub port to be powered host interface. Since the LPC31
 *     has only a downstream port, zero is the only possible value for this
 *     parameter.
 *   enable - true: enable VBUS power; false: disable VBUS power
 *
 * Returned Value:
 *   None
 *
 ***********************************************************************************/

void imxrt_usbhost_vbusdrive(int rhport, bool enable)
{
	udbg("RHPort%d: enable=%d\n", rhport + 1, enable);

	/* The LPC3131 has only a single root hub port */

#ifdef NEED_REFERENCE 
	if (rhport == 0) {
		/* Then enable or disable VBUS power */

		if (enable) {
			/* Enable the Power Switch by driving the enable pin low */

			imxrt_gpio_write(GPIO_USB_OTG1_PWR_1, GPIO_OUTPUT_ZERO);
		} else {
			/* Disable the Power Switch by driving the enable pin high */

			imxrt_gpio_write(GPIO_USB_OTG1_PWR_1, GPIO_OUTPUT_ONE);
		}
	}
#endif
}

#endif							/* HAVE_USBHOST */
