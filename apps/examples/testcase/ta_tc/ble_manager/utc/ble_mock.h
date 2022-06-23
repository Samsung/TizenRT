/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#ifdef CONFIG_VIRTUAL_BLE
#include <tinyara/net/if/ble.h>

// Set return value for an API call.
#define VBLE_CMD_SET 0x0
// Generate an event
#define VBLE_CMD_GEN_EVT 0x1

#define LWNL_ERR -1

typedef struct {
    int key;
    int value;
    int wait;
} vble_ioctl_msg_s;

#define CONTROL_VVDRIVER(cmd, key, value, wait)	\
	(void)utc_control_mock(cmd, key, value, wait)

#else
#define CONTROL_VDRIVER(cmd, key, value, wait)

#endif
