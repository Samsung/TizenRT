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
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Includes some definitions that a compatible with the LGPL GNU C Library
 * header file of the same name.
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

#ifndef __INCLUDE_NET_PHY_H
#define __INCLUDE_NET_PHY_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Prerequisites: CONFIG_DISABLE_SIGNALS must not be defined */

#ifdef CONFIG_DISABLE_SIGNALS
#undef CONFIG_ARCH_PHY_INTERRUPT
#endif

/* Maximum number of phy_notify clients */

#ifndef CONFIG_PHY_NOTIFICATION_NCLIENTS
#define CONFIG_PHY_NOTIFICATION_NCLIENTS 1
#endif

/* Maximum length of on interface device name (excluding NULL termination) */

#ifndef CONFIG_PHY_NOTIFICATION_MAXINTFLEN
#define CONFIG_PHY_NOTIFICATION_MAXINTFLEN 4
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
 * Function: phy_notify_subscribe
 *
 * Description:
 *   Setup up to deliver signals to the task identified by 'pid' when
 *   there is any change indicated by an interrupt from the PHY associated
 *   with 'intf'
 *
 *   NOTE: This function is intended to be called only from an Ethernet
 *   driver in support of the SIOCMIISIG ioctl command.  It should never
 *   by called directly by application logic.
 *
 * Parameters:
 *   intf  - Provides the name of the network interface, for example, "eth0".
 *           The length of intf must not exceed 4 bytes (excluding NULL
 *           terminator).  Configurable with CONFIG_PHY_NOTIFICATION_MAXINTFLEN.
 *   pid   - Identifies the task to receive the signal.  The special value
 *           of zero means to use the pid of the current task.
 *   signo - This is the signal number to use when notifying the task.
 *   arg   - An argument that will accompany the notification signal.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_PHY_INTERRUPT
int phy_notify_subscribe(FAR const char *intf, pid_t pid, int signo, FAR void *arg);
#endif

/****************************************************************************
 * Function: phy_notify_unsubscribe
 *
 * Description:
 *   Stop the deliver of signals for events from the PHY associated with
 *   'intf' to the task identified by 'pid'
 *
 *   NOTE: This function is intended to be called only from an Ethernet
 *   driver in support of the SIOCMIISIG ioctl command.  It should never
 *   by called directly by application logic.
 *
 * Parameters:
 *   intf  - Provides the name of the network interface, for example, "eth0".
 *           The length of 'intf' must not exceed 4 bytes (excluding NULL
 *           terminator).  Configurable with CONFIG_PHY_NOTIFICATION_MAXINTFLEN.
 *   pid   - Identifies the task that was receiving notifications.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_PHY_INTERRUPT
int phy_notify_unsubscribe(FAR const char *intf, pid_t pid);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_NET_PHY_H */
