/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

/**
 * Maxwell mxlog transport (Interface)
 *
 * Provides communication between the firmware and the host.
 *
 */

#ifndef MXLOG_TRANSPORT_H__
#define MXLOG_TRANSPORT_H__

/** Uses */
#include <tinyara/kthread.h>
#include <tinyara/mm/mm.h>
#include "mifstream.h"

struct mxlog_transport;

/**
 * Transport channel callback handler. This will be invoked each time a message on a channel is
 * received. Handlers may perform work within
 * their callback implementation, but should not block.
 *
 * Note that the message pointer passed is only valid for the duration of the function call.
 */
typedef void (*mxlog_channel_handler)(const void *message, size_t length, u32 level, void *data);

/**
 * Initialises the maxwell management transport and configures the necessary
 * interrupt handlers.
 */
int mxlog_transport_init(struct mxlog_transport *mxlog_transport, struct scsc_mx *mx);
void mxlog_transport_release(struct mxlog_transport *mxlog_transport);
/*
 *  Initialises the configuration area incl. Maxwell Infrastructure Configuration,
 * MIF Management Transport Configuration and  MIF Management Stream Configuration.
 */
void mxlog_transport_config_serialise(struct mxlog_transport *mxlog_transport, struct mxlogconf *mxlogconf);
void mxlog_transport_register_channel_handler(struct mxlog_transport *mxlog_transport, mxlog_channel_handler handler, void *data);
void mxlog_transport_set_error(struct mxlog_transport *mxlog_transport);

struct mxlog_transport {
	struct scsc_mx *mx;
	struct mif_stream mif_stream;
	mxlog_channel_handler channel_handler_fn;
	void *channel_handler_data;
};

#endif							/* MXLOG_TRANSPORT_H__ */
