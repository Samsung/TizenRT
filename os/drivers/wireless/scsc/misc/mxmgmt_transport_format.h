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
#ifndef MXMGR_TRANSPORT_FORMAT_H__
#define MXMGR_TRANSPORT_FORMAT_H__

/**
 * Layout of messages across the manager transport streams.
 *
 * HERE: This is a dummy definition and will be replaced
 * once more of the service management infrastructure is completed.
 */
struct mxmgr_message {
	uint8_t channel_id;			/* Channel ID from mxmgr_channels */
	uint8_t payload[7];			/* Message content to store in the transport stream - user defined format */
} __packed;

#endif							/* MXMGR_TRANSPORT_FORMAT_H__ */
