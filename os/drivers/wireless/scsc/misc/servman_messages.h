/*****************************************************************************
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

#ifndef SERVMAN_MESSAGES_H__
#define SERVMAN_MESSAGES_H__

#include <scsc/scsc_mifram.h>
/**
 * Maxwell Service Management Messages.
 */
enum {
	SM_MSG_START_REQ,
	SM_MSG_START_CFM,
	SM_MSG_STOP_REQ,
	SM_MSG_STOP_CFM,
} sm_msg;

/* Transport format for service management messages across the
 * Maxwell management transport.
 */
struct sm_msg_packet {
	uint8_t service_id;
	uint8_t msg;
	scsc_mifram_ref optional_data;
} __packed;

#endif							/* SERVMAN_MESSAGES_H__ */
