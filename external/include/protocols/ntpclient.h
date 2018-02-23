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
 * external/include/protocols/ntpclient.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup NTPC NTP Client
 * @brief Provides APIs for NTP(Network Time Protocol) Client
 * @ingroup NETWORK
 * @{
 */

/// @file protocols/ntpclient.h
/// @brief APIs for NTP Client

#ifndef __EXTERNAL_INCLUDE_PROTOCOLS_NTPCLIENT_H
#define __EXTERNAL_INCLUDE_PROTOCOLS_NTPCLIENT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <netinet/in.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#define MAX_NTP_SERVER_NUM                                              5
#define MIN_NTP_POLL_INTERVAL_SECS 10				/* 10 seconds */
#define MAX_NTP_POLL_INTERVAL_SECS (24 * 60 * 60)	/* 24 hours */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 * @brief This enumeration describes the state of the NTP daemon
 */
enum ntpc_daemon_e {
	NTP_NOT_RUNNING = 0,
	NTP_STARTED,
	NTP_RUNNING,
	NTP_STOP_REQUESTED,
	NTP_STOPPED
};

/**
 * @brief This enumeration describes the state of the NTP Client's Link
 */
enum ntpc_link_status_e {
	NTP_LINK_NOT_SET = 0,
	NTP_LINK_DOWN,
	NTP_LINK_UP,
};

/**
 * @brief Structure of  NTP server's connection information
 */
struct ntpc_server_conn_s {
	char *hostname;				/* host name */
	int port;					/* port */
};

/**
 * @brief Structure of  NTP server information
 */
struct ntpc_server_s {
	struct ntpc_server_conn_s conn;
	in_addr_t ipaddr;			/* ip address */
	int link;					/* link status */
};

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

/**
 * @brief ntpc_start() starts the NTP client daemon.
 *
 * @param[in] server_list the array of struct ntpc_server_conn_s (If NULL is entered, it operates as the default NTP server set in the system)
 *                                   - server_list[n]->hostname : ntp server's hostname
 *                                   - server_list[n]->port : ntp server's port number
 * @param[in] num_of_servers number of servers (If 0 is entered, it operates as the default NTP server set in the system)
 * @param[in] interval_secs polling interval seconds (If 0 is entered, it operates as the default interval set in the system)
 * @param[in] link_error_cb callback function for link error case
 * @return On success, the non-negative task ID of the NTPC daemon is returned.
 *             On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int ntpc_start(struct ntpc_server_conn_s *server_list, uint32_t num_of_servers, uint32_t interval_secs, void *link_error_cb);

/**
 * @brief ntpc_stop() stops the NTP client daemon.
 *
 * @param[in] none
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
#ifndef CONFIG_DISABLE_SIGNALS
int ntpc_stop(void);
#endif

/**
 * @brief  ntpc_get_status() gets NTP client daemon's status value.
 *
 * @param[in] none
 * @return ntpc daemon's status value is returned (please, refer to enum ntpc_daemon_e definition).
 * @since TizenRT v1.0
 */
int ntpc_get_status(void);

/**
 * @cond
 * @internal
 * @brief  ntpc_get_link_status() gets NTP client link status value.
 *
 * @param[in] none
 * @return ntpc link status value is returned (please, refer to enum ntpc_link_status_e definition).
 *
 */
int ntpc_get_link_status(void);

// @endcond

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __EXTERNAL_INCLUDE_PROTOCOLS_NTPCLIENT_H */

/** @} */// end of NTPC group
