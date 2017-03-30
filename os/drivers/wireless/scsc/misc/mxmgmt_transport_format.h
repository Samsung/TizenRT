/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
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
	uint8_t channel_id; /* Channel ID from mxmgr_channels */
	uint8_t payload[7]; /* Message content to store in the transport stream - user defined format */
} __packed;

#endif /* MXMGR_TRANSPORT_FORMAT_H__ */
