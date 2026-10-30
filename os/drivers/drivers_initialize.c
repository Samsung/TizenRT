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
 * drivers/drivers_initialize.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/clk/clk_provider.h>
#if defined(CONFIG_CRYPTO)
#  include <tinyara/crypto/crypto.h>
#endif
#include <tinyara/drivers/drivers.h>
#include <tinyara/drivers/rpmsgdev.h>
#include <tinyara/drivers/rpmsgblk.h>
#include <tinyara/fs/loop.h>
#include <tinyara/fs/smart.h>
#include <tinyara/fs/loopmtd.h>
#if defined(CONFIG_UINPUT_TOUCH) || defined(CONFIG_UINPUT_BUTTONS) || \
    defined(CONFIG_UINPUT_KEYBOARD)
#  include <tinyara/input/uinput.h>
#endif
#include <tinyara/mtd/mtd.h>
#if defined(CONFIG_NET_LOOPBACK)
#  include <tinyara/net/loopback.h>
#endif
#if defined(CONFIG_NET_RPMSG_DRV_SERVER)
#  include <tinyara/net/rpmsgdrv.h>
#endif
#if defined(CONFIG_NET_TUN)
#  include <tinyara/net/tun.h>
#endif
#include <tinyara/net/telnet.h>
#include <tinyara/note/note_driver.h>
#include <tinyara/pci/pci.h>
#if defined(CONFIG_PM)
#  include <tinyara/power/pm.h>
#endif
#if defined(CONFIG_REGULATOR_RPMSG)
#  include <tinyara/power/regulator.h>
#endif
#include <tinyara/reset/reset-controller.h>
#include <tinyara/segger/rtt.h>
#if defined(CONFIG_USENSOR) || defined(CONFIG_SENSORS_RPMSG) || \
    defined(CONFIG_SENSORS_MONITOR)
#  include <tinyara/sensors/sensor.h>
#endif
#include <tinyara/serial/pty.h>
#include <tinyara/serial/uart_hostfs.h>
#include <tinyara/serial/uart_ram.h>
#include <tinyara/syslog/syslog.h>
#include <tinyara/syslog/syslog_console.h>
#include <tinyara/thermal.h>
#include <tinyara/timers/ptp_clock_dummy.h>
#include <tinyara/timers/capture.h>
#include <tinyara/trace.h>
#if defined(CONFIG_NET_USRSOCK_RPMSG_SERVER)
#  include <tinyara/usrsock/usrsock_rpmsg.h>
#endif
#include <tinyara/vhost/vhost.h>
#include <tinyara/virtio/virtio.h>
#include <tinyara/drivers/optee.h>
#include <tinyara/usb/usbhost.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Check if only one console device is selected.
 * If you get this error, search your .config file for CONSOLE_XXX_CONSOLE
 * options and remove what is not needed.
 */

#if (defined(CONFIG_LWL_CONSOLE) + defined(CONFIG_SERIAL_CONSOLE) + \
     defined(CONFIG_CDCACM_CONSOLE) + defined(CONFIG_PL2303_CONSOLE) + \
     defined(CONFIG_SERIAL_RTT_CONSOLE) + defined(CONFIG_RPMSG_UART_CONSOLE) + \
     defined(CONFIG_RPMSG_UART_RAW_CONSOLE) ) > 1
#  error More than one console driver selected. Check your configuration !
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: drivers_early_initialize
 *
 * Description:
 *   drivers_early_initialize will be called once before OS initialization
 *   when no system resource is ready to use.
 *
 *   drivers_early_initialize serves the purpose of bringing up drivers as
 *   early as possible, so they can be used even during OS initialization.
 *   It must not rely on any system resources, such as heap memory.
 *
 ****************************************************************************/

void drivers_early_initialize(void)
{
#ifdef CONFIG_DRIVERS_NOTE
	note_early_initialize();
#endif
}

/****************************************************************************
 * Name: drivers_initialize
 *
 * Description:
 *   drivers_initialize will be called once during OS initialization after
 *   the basic OS services have been initialized.
 *
 *   drivers_initialize is called after the OS initialized but before the
 *   user initialization logic has been started and before the libraries
 *   have been initialized.  OS services and driver services are available.
 *
 ****************************************************************************/

void drivers_initialize(void)
{
	drivers_trace_begin();

	/* Register devices */

	syslog_initialize();

#ifdef CONFIG_SERIAL_RTT
	serial_rtt_initialize();
#endif

#if defined(CONFIG_DEV_NULL)
	devnull_register();   /* Standard /dev/null */
#endif

#if defined(CONFIG_DEV_RANDOM)
	devrandom_register(); /* Standard /dev/random */
#endif

#if defined(CONFIG_DEV_URANDOM)
	devurandom_register();   /* Standard /dev/urandom */
#endif

#if defined(CONFIG_DEV_ZERO)
	devzero_register();   /* Standard /dev/zero */
#endif

#ifdef CONFIG_DEV_MEM
	devmem_register();
#endif

#if defined(CONFIG_DEV_LOOP)
	loop_register();      /* Standard /dev/loop */
#endif

#if defined(CONFIG_DEV_ASCII)
	devascii_register();  /* Non-standard /dev/ascii */
#endif

#if defined(CONFIG_DRIVERS_NOTE)
	note_initialize();    /* Non-standard /dev/note */
#endif

#if defined(CONFIG_CLK_RPMSG)
	clk_rpmsg_server_initialize();
#endif

#if defined(CONFIG_REGULATOR_RPMSG)
	regulator_rpmsg_server_init();
#endif

#if defined(CONFIG_RESET_RPMSG)
	reset_rpmsg_server_init();
#endif

	/* Initialize the serial device driver */

#ifdef CONFIG_RPMSG_UART
	rpmsg_serialinit();
#endif

#ifdef CONFIG_RPMSG_UART_RAW
	rpmsg_serialrawinit();
#endif

#ifdef CONFIG_RAM_UART
	ram_serialinit();
#endif

	/* Initialize the console device driver (if it is other than the standard
	 * serial driver).
	 */

#if defined(CONFIG_LWL_CONSOLE)
	lwlconsole_init();
#elif defined(CONFIG_CONSOLE_SYSLOG)
	syslog_console_init();
#endif

#ifdef CONFIG_UART_HOSTFS
	uart_hostfs_init();
#endif

#ifdef CONFIG_PSEUDOTERM_SUSV1
	/* Register the master pseudo-terminal multiplexor device */

	ptmx_register();
#endif

#if defined(CONFIG_CRYPTO)
	/* Initialize the HW crypto and /dev/crypto */

	up_cryptoinitialize();
#endif

#ifdef CONFIG_CRYPTO_CRYPTODEV
	devcrypto_register();
#endif

#ifdef CONFIG_UINPUT_TOUCH
	uinput_touch_initialize();
#endif

#ifdef CONFIG_UINPUT_BUTTONS
	uinput_button_initialize();
#endif

#ifdef CONFIG_UINPUT_KEYBOARD
	uinput_keyboard_initialize();
#endif

#ifdef CONFIG_NET_LOOPBACK
	/* Initialize the local loopback device */

	localhost_initialize();
#endif

#ifdef CONFIG_NET_TUN
	/* Initialize the TUN device */

	tun_initialize();
#endif

#ifdef CONFIG_NETDEV_TELNET
	/* Initialize the Telnet session factory */

	telnet_initialize();
#endif

#ifdef CONFIG_USENSOR
	usensor_initialize();
#endif

#ifdef CONFIG_SENSORS_RPMSG
	sensor_rpmsg_initialize();
#endif

#ifdef CONFIG_SENSORS_MONITOR
	sensor_monitor_initialize();
#endif

#ifdef CONFIG_DEV_RPMSG_SERVER
	rpmsgdev_server_init();
#endif

#ifdef CONFIG_BLK_RPMSG_SERVER
	rpmsgblk_server_init();
#endif

#ifdef CONFIG_RPMSGMTD_SERVER
	rpmsgmtd_server_init();
#endif

#ifdef CONFIG_NET_USRSOCK_RPMSG_SERVER
	/* Initialize the user socket rpmsg server */

	usrsock_rpmsg_server_initialize();
#endif

#ifdef CONFIG_NET_RPMSG_DRV_SERVER
	/* Initialize the net rpmsg default server */

	net_rpmsg_drv_server_init();
#endif

#ifdef CONFIG_SMART_DEV_LOOP
	smart_loop_register_driver();
#endif

#ifdef CONFIG_MTD_LOOP
	mtd_loop_register();
#endif

#ifdef CONFIG_USBHOST_WAITER
	usbhost_drivers_initialize();
#endif

#if defined(CONFIG_PCI) && !defined(CONFIG_PCI_LATE_DRIVERS_REGISTER)
	pci_register_drivers();
#endif

#ifdef CONFIG_DRIVERS_VIRTIO
	virtio_register_drivers();
#endif

#ifdef CONFIG_DRIVERS_VHOST
	vhost_register_drivers();
#endif

#ifndef CONFIG_DEV_OPTEE_NONE
	optee_register();
#endif

#ifdef CONFIG_THERMAL
	thermal_init();
#endif

#ifdef CONFIG_PTP_CLOCK_DUMMY
	ptp_clock_dummy_initialize(0);
#endif

#ifdef CONFIG_FAKE_CAPTURE
	fake_capture_initialize(2);
#endif

	drivers_trace_end();
}
