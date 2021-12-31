/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#pragma once

typedef struct {
	int key;
	int value;
	int wait;
} vwifi_ioctl_msg_s;

/*
 * command in trwifi_result_e
 */
// Set return result else.
#define VWIFI_CMD_SET 0x1001
// Generate an event immediately
#define VWIFI_CMD_GEN_EVT 0x1002
// Register an event to generate it when a vdev receives a request.
// This command needs in this situation as request deinit to the vdev
// while vdev is in sta connected state. because wifi_manager waits
// disconnect event from a vdev however deinit is synchronous call
// user can't generate fake disconnect event.
#define VWIFI_CMD_GEN_EVT_FUNC 0x1003

/*
 * key
 */
// command: VWIFI_CMD_SET
// this'll set return value of vdev.
#define VWIFI_KEY_RESULT 1

// command: VWIFI_CMD_GEN_EVT, VWIFI_CMD_GEN_EVT_FUNC
// it must not be overlapped lwnl event type
#define VWIFI_PKT_DHCPS_EVT 0x1000

// Command: VWIFI_CMD_GEN_EVT or VWIFI_CMD_GEN_EVT_FUNC
// key: LWNL_EVT_STA_DISCONNECTED
// value: reason code
