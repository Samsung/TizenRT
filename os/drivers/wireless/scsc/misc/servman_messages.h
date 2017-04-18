/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
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
