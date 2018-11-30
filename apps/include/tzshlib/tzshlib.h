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
 * apps/include/tzshlib/tzshlib.h
 *
 *   Copyright (C) 2011, 2013, 2016-2017 Gregory Nutt. All rights reserved.
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

#ifndef __APPS_INCLUDE_TZSHLIB_TZSHLIB_H
#define __APPS_INCLUDE_TZSHLIB_TZSHLIB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* If a USB device is selected for the TZSH console then we need to handle some
 * special start-up conditions.
 */

#undef HAVE_USB_CONSOLE
#if defined(CONFIG_USBDEV)

/* Check for a PL2303 serial console.  Use console device "/dev/console". */

#if defined(CONFIG_PL2303) && defined(CONFIG_PL2303_CONSOLE)
#define HAVE_USB_CONSOLE 1

/* Check for a CDC/ACM serial console.  Use console device "/dev/console". */

#elif defined(CONFIG_CDCACM) && defined(CONFIG_CDCACM_CONSOLE)
#define HAVE_USB_CONSOLE 1

/* Check for a generic USB console.  In this case, the USB console device
 * must be provided in CONFIG_TZSH_USBCONDEV.
 */

#elif defined(CONFIG_TZSH_USBCONSOLE)
#define HAVE_USB_CONSOLE 1
#endif
#endif

#if CONFIG_RR_INTERVAL > 0
#define SCHED_TZSH SCHED_RR
#else
#define SCHED_TZSH SCHED_FIFO
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_initialize
 *
 * Description:
 *   This interface is used to initialize the NuttShell (TZSH).
 *   tzsh_initialize() should be called one during application start-up prior
 *   to executing either tzsh_consolemain() or tzsh_telnetstart().
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void tzsh_initialize(void);

/****************************************************************************
 * Name: tzsh_consolemain
 *
 * Description:
 *   This interfaces maybe to called or started with task_start to start a
 *   single an TZSH instance that operates on stdin and stdout.  This
 *   function does not return.
 *
 *   This function handles generic /dev/console character devices, or
 *   special USB console devices.  The USB console requires some special
 *   operations to handle the cases where the session is lost when the
 *   USB device is unplugged and restarted when the USB device is plugged
 *   in again.
 *
 * Input Parameters:
 *   Standard task start-up arguments.  These are not used.  argc may be
 *   zero and argv may be NULL.
 *
 * Returned Values:
 *   This function does not normally return.  exit() is usually called to
 *   terminate the TZSH session.  This function will return in the event of
 *   an error.  In that case, a non-zero value is returned (EXIT_FAILURE=1).
 *
 ****************************************************************************/

int tzsh_consolemain(int argc, char *argv[]);

/****************************************************************************
 * Name: tzsh_telnetstart
 *
 * Description:
 *   tzsh_telnetstart() starts the Telnet daemon that will allow multiple
 *   TZSH connections via Telnet.  This function returns immediately after
 *   the daemon has been started.
 *
 * Input Parameters:
 *   family - Provides the IP family to use by the server.  May be either
 *     AF_INET or AF_INET6.  This is needed because both both may be
 *     enabled in the configuration.
 *
 *   All of the other properties of the Telnet daemon are controlled by
 *   NuttX configuration settings.
 *
 * Returned Values:
 *   The task ID of the Telnet daemon was successfully started.  A negated
 *   errno value will be returned on failure.
 *
 ****************************************************************************/

int tzsh_telnetstart(sa_family_t family);

/****************************************************************************
 * Name: platform_motd
 *
 * Description:
 *   If CONFIG_TZSH_PLATFORM_MOTD is defined, then platform-specific logic
 *   must provide this function in order to obtain the Message of the Day
 *   (MOTD)
 *
 * Input Parmeters:
 *   buffer - A caller allocated buffer in which to receive the MOTD
 *   buflen - The length in bytes of the caller allocated buffer
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_TZSH_PLATFORM_MOTD
void platform_motd(FAR char *buffer, size_t buflen);
#endif

/****************************************************************************
 * Name: platform_user_verify
 *
 * Description:
 *   If CONFIG_TZSH_LOGIN_PLATFORM is defined, then platform-specific logic
 *   must provide this function in order verify user credentials as part of
 *   the login process.
 *
 * Input Parmeters:
 *   username/password - User credentials to be verified.
 *
 * Returned value:
 *   1 - The user credentials are verified
 *   0 - The user credentials are incorrect
 *  <0 - An error occurred.  The returned value is a negated errno number.
 *
 ****************************************************************************/

#ifdef CONFIG_TZSH_LOGIN_PLATFORM
int platform_user_verify(FAR const char *username, FAR const char *password);
#endif

/****************************************************************************
 * Name: tzsh_system
 *
 * Description:
 *   This is the TZSH-specific implementation of the standard system()
 *   command.
 *
 *   NOTE: This assumes that other TZSH instances have previously ran and so
 *   common TZSH logic is already initialized.
 *
 * Input Parameters:
 *   Standard task start-up arguments.  Expects argc == 2 with argv[1] being
 *   the command to execute
 *
 * Returned Values:
 *   EXIT_SUCCESS or EXIT_FAILURE
 *
 ****************************************************************************/

int tzsh_system(int argc, char *argv[]);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __APPS_INCLUDE_TZSHLIB_TZSHLIB_H */
