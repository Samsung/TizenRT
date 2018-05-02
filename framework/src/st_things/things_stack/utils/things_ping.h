/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef THINGS_CLOUD_PING_H_
#define THINGS_CLOUD_PING_H_

typedef enum ping_state_e {
	PING_ST_INIT = 0,			// Ping initialization
	PING_ST_ISCLOUD = 1,		// remote client is Cloud CIserver
	PING_ST_SIGNIN = (1 << 1),	// Sign-IN complete
	PING_ST_TCPCONNECT = (1 << 2),	// TCP session connected
	PING_ST_STARTTHREAD = (1 << 3),	// Ping-Thread created
	PING_ST_DISCOVERY = (1 << 4),	// Doing discovery /oic/ping resource in Cloud
	PING_ST_REQUEST = (1 << 5),	// Doing common-ping request to Cloud
	PING_ST_INTUPDATE = (1 << 6),	// Doing interval-update request to Cloud
	PING_ST_INTUPDATED = (1 << 7),	// Success interval-update request to Cloud
	PING_ST_TIMEOUT = (1 << 8),	// Occurred Time-out with regard to any request
	PING_ALL_FLAG = 0xFFFFFFFF
} ping_state_e;

bool things_ping_init(void);

bool things_ping_terminate(void);

bool things_ping_set_mask(const char *remote_addr, uint16_t port, ping_state_e state);

bool things_ping_unset_mask(const char *remoteAddr, ping_state_e state);

#endif							/* THINGS_CLOUD_PING_H_ */
