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
 *   Copyright (C) 2007, 2009-2014 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_NET_NET_H
#define __INCLUDE_NET_NET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifdef CONFIG_NET

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <semaphore.h>
#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Socket descriptors are the index into the TCB sockets list, offset by the
 * following amount. This offset is used to distinguish file descriptors from
 * socket descriptors
 */

#ifdef CONFIG_NFILE_DESCRIPTORS
#define __SOCKFD_OFFSET CONFIG_NFILE_DESCRIPTORS
#else
#define __SOCKFD_OFFSET 0
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* Data link layer type */

enum net_lltype_e {
	NET_LL_ETHERNET = 0,		/* Ethernet */
	NET_LL_SLIP,				/* Serial Line Internet Protocol (SLIP) */
	NET_LL_PPP					/* Point-to-Point Protocol (PPP) */
};

/* This defines a bitmap big enough for one bit for each socket option */

typedef uint16_t sockopt_t;

/* This defines the storage size of a timeout value.  This effects only
 * range of supported timeout values.  With an LSB in seciseconds, the
 * 16-bit maximum of 65535 corresponds to 1 hr 49 min 13.5 sec at decisecond
 * resolution.
 */

typedef uint16_t socktimeo_t;

/* This is the internal representation of a socket reference by a file
 * descriptor.
 */

struct socket {
	/** sockets currently are built on netconns, each socket has one netconn */
	struct netconn *conn;
	/** data that was left from the previous read */
	void *lastdata;
	/** offset in the data that was left from the previous read */
	uint16_t lastoffset;
	/** number of times data was received, set by event_callback(),
	    tested by the receive and select functions */
	int16_t rcvevent;
	/** number of times data was ACKed (free send buffer), set by event_callback(),
	    tested by select */
	uint16_t sendevent;
	/** error happened for this socket, set by event_callback(), tested by select */
	uint16_t errevent;
	/** last error that occurred on this socket */
	int err;
	/** counter of how many threads are waiting for this socket using select */
	int select_waiting;
};

/* This defines a list of sockets indexed by the socket descriptor */

#if CONFIG_NSOCKET_DESCRIPTORS > 0
struct socketlist {
	sem_t sl_sem;				/* Manage access to the socket list */
	struct socket sl_sockets[CONFIG_NSOCKET_DESCRIPTORS];
};

#endif

/* Callback from netdev_foreach() */
struct netif;					/* Forward reference. Defined in tinyara/net/lwip/src/include/lwip/netif.h */
typedef int (*netdev_callback_t)(FAR struct netif *dev, void *arg);

/* Semaphore based locking for non-interrupt based logic.
 *
 * net_lock_t -- Not used.  Only for compatibility
 */

typedef uint8_t net_lock_t;		/* Not really used */

/****************************************************************************
 * Public Data
 ****************************************************************************/
/****************************************************************************
 * Definitions
 ****************************************************************************/

/* Parameters for connectivity manager statistic collection.
 * Currently WIFI and Ethernet parameters being defined.
 */

/* Connectivity Manager */

/* Connectivity Manager - WiFi*/

/* Connectivity Manager - Ethernet*/
#ifdef CONFIG_ENC28J60
#ifndef CONFIG_SYSTEM_IOTJS
uint64_t eth_tx_count;
uint64_t eth_rx_count;
uint64_t eth_max_message_size;
uint64_t eth_avg_message_size;
uint64_t eth_total_message_size;
#endif
#endif
/* Connectivity Manager - Ethernet*/

/* Connectivity Manager - 802.15.4*/
///TODO
/* Connectivity Manager - 802.15.4*/

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
 * Critical section management.
 *
 *   net_lock()          - Takes the semaphore().  Implements a re-entrant mutex.
 *   net_unlock()        - Gives the semaphore().
 *   net_lockedwait()    - Like pthread_cond_wait(); releases the semaphore
 *                         momentarily to wait on another semaphore()
 *
 ****************************************************************************/

/****************************************************************************
 * Function: net_lock
 *
 * Description:
 *   Take the lock
 *
 ****************************************************************************/

net_lock_t net_lock(void);

/****************************************************************************
 * Function: net_unlock
 *
 * Description:
 *   Release the lock.
 *
 ****************************************************************************/

void net_unlock(net_lock_t flags);

/****************************************************************************
 * Function: net_timedwait
 *
 * Description:
 *   Atomically wait for sem (or a timeout( while temporarily releasing
 *   the lock on the network.
 *
 * Input Parameters:
 *   sem     - A reference to the semaphore to be taken.
 *   abstime - The absolute time to wait until a timeout is declared.
 *
 * Returned value:
 *   The returned value is the same as sem_timedwait():  Zero (OK) is
 *   returned on success; -1 (ERROR) is returned on a failure with the
 *   errno value set appropriately.
 *
 ****************************************************************************/

struct timespec;
int net_timedwait(sem_t *sem, FAR const struct timespec *abstime);

/****************************************************************************
 * Function: net_lockedwait
 *
 * Description:
 *   Atomically wait for sem while temporarily releasing lock on the network.
 *
 * Input Parameters:
 *   sem - A reference to the semaphore to be taken.
 *
 * Returned value:
 *   The returned value is the same as sem_wait():  Zero (OK) is returned
 *   on success; -1 (ERROR) is returned on a failure with the errno value
 *   set appropriately.
 *
 ****************************************************************************/

int net_lockedwait(sem_t *sem);

/****************************************************************************
 * Function: net_setipid
 *
 * Description:
 *   This function may be used at boot time to set the initial ip_id.
 *
 * Assumptions:
 *
 ****************************************************************************/

void net_setipid(uint16_t id);

/****************************************************************************
 * Name: net_checksd
 *
 * Description:
 *   Check if the socket descriptor is valid for the provided TCB and if it
 *   supports the requested access.  This trivial operation is part of the
 *   fdopen() operation when the fdopen() is performed on a socket descriptor.
 *   It simply performs some sanity checking before permitting the socket
 *   descriptor to be wrapped as a C FILE stream.
 *
 ****************************************************************************/

int net_checksd(int fd, int oflags);

/****************************************************************************
 * Name: net_setup
 *
 * Description:
 *   This is called from the OS initialization logic at power-up reset in
 *   order to configure networking data structures.  This is called prior
 *   to platform-specific driver initialization so that the networking
 *   subsystem is prepared to deal with network driver initialization
 *   actions.
 *
 *   Actions performed in this initialization phase assume that base OS
 *   facilities such as semaphores are available but this logic cannot
 *   depend upon OS resources such as interrupts or timers which are not
 *   yet available.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_setup(void);

/****************************************************************************
 * Name: net_initialize
 *
 * Description:
 *   This is called from the OS initialization logic at power-up reset in
 *   order to configure the networking subsystem.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_initialize(void);

/****************************************************************************
 * Name:
 *
 * Description:
 *   Initialize a list of sockets for a new task
 *
 * Input Parameters:
 *   list -- A reference to the pre-alloated socket list to be initialized.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_initlist(FAR struct socketlist *list);

/****************************************************************************
 * Name: net_releaselist
 *
 * Description:
 *   Release resources held by the socket list
 *
 * Input Parameters:
 *   list -- A reference to the pre-allocated socket list to be un-initialized.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_releaselist(FAR struct socketlist *list);

/****************************************************************************
 * Function: net_close
 *
 * Description:
 *   Performs the close operation on socket descriptors
 *
 * Parameters:
 *   sockfd   Socket descriptor of socket
 *
 * Returned Value:
 *   0 on success; -1 on error with errno set appropriately.
 *
 * Assumptions:
 *
 ****************************************************************************/

int net_close(int sockfd);

/****************************************************************************
 * Name: netdev_ioctl
 *
 * Description:
 *   Perform network device specific operations.
 *
 * Parameters:
 *   sockfd   Socket descriptor of device
 *   cmd      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Return:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   On a failure, -1 is returned with errno set appropriately
 *
 *   EBADF
 *     'sockfd' is not a valid descriptor.
 *   EFAULT
 *     'arg' references an inaccessible memory area.
 *   ENOTTY
 *     'cmd' not valid.
 *   EINVAL
 *     'arg' is not valid.
 *   ENOTTY
 *     'sockfd' is not associated with a network device.
 *   ENOTTY
 *      The specified request does not apply to the kind of object that the
 *      descriptor 'sockfd' references.
 *
 ****************************************************************************/

int netdev_ioctl(int sockfd, int cmd, unsigned long arg);

/****************************************************************************
 * Function: net_dupsd
 *
 * Description:
 *   Clone a socket descriptor to an arbitray descriptor number.  If file
 *   descriptors are implemented, then this is called by dup() for the case
 *   of socket file descriptors.  If file descriptors are not implemented,
 *   then this function IS dup().
 *
 ****************************************************************************/

int net_dupsd(int sockfd);

/****************************************************************************
 * Function: net_dupsd2
 *
 * Description:
 *   Clone a socket descriptor to an arbitray descriptor number.  If file
 *   descriptors are implemented, then this is called by dup2() for the case
 *   of socket file descriptors.  If file descriptors are not implemented,
 *   then this function IS dup2().
 *
 ****************************************************************************/

int net_dupsd2(int sockfd1, int sockfd2);

/****************************************************************************
 * Function: net_clone
 *
 * Description:
 *   Performs the low level, common portion of net_dupsd() and net_dupsd2()
 *
 ****************************************************************************/

int net_clone(FAR struct socket *psock1, FAR struct socket *psock2);

/****************************************************************************
 * Name: net_vfcntl
 *
 * Description:
 *   Performs fcntl operations on socket
 *
 * Input Parameters:
 *   sockfd - Socket descriptor of the socket to operate on
 *   cmd    - The fcntl command.
 *   ap     - Command-specific arguments
 *
 * Returned Value:
 *   Zero (OK) is returned on success; -1 (ERROR) is returned on failure and
 *   the errno value is set appropriately.
 *
 ****************************************************************************/

int net_vfcntl(int sockfd, int cmd, va_list ap);

/****************************************************************************
 * Function: netdev_foreach
 *
 * Description:
 *   Enumerate each registered network device.
 *
 *   NOTE: netdev semaphore held throughout enumeration.
 *
 * Parameters:
 *   callback - Will be called for each registered device
 *   arg      - User argument passed to callback()
 *
 * Returned Value:
 *  0:Enumeration completed 1:Enumeration terminated early by callback
 *
 * Assumptions:
 *  Called from normal user mode
 *
 ****************************************************************************/

int netdev_foreach(netdev_callback_t callback, void *arg);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_NET */
#endif							/* __INCLUDE_NET_NET_H */
