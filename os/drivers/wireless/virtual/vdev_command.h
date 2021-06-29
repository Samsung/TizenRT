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

/*  command */
#define VWIFI_CMD_SET 0x1001
#define VWIFI_CMD_GEN_EVT 0x1002
#define VWIFI_CMD_GEN_EVT_FUNC 0x1003

/*  key */
#define VWIFI_KEY_RESULT 1

/*  value */
