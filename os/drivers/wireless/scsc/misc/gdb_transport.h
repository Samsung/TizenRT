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

/**
 * Maxwell gdb transport (Interface)
 *
 * Provides bi-directional communication between the firmware and the
 * host.
 *
 * This interface also provides a utility method for sending messages across
 * the stream.
 */

#ifndef GDB_TRANSPORT_H__
#define GDB_TRANSPORT_H__

/** Uses */
#include <sys/types.h>
#include "mifstream.h"

#define GDB_TRANSPORT_BUF_LENGTH (128)

struct gdb_transport;

enum gdb_transport_enum {
	GDB_TRANSPORT_R4 = 0,
	GDB_TRANSPORT_M4,
};
/**
 * Transport channel callback handler. This will be invoked each time a message on a channel is
 * received. Handlers may perform work within
 * their callback implementation, but should not block.
 *
 * Note that the message pointer passed is only valid for the duration of the function call.
 */
typedef void (*gdb_channel_handler)(const void *message, size_t length, void *data);

/**
 * Sends a message to the AP across the given channel.
 *
 * This function is safe to call from any RTOS thread.
 */
void gdb_transport_send(struct gdb_transport *gdb_transport, void *message, uint32_t message_length);

/**
 * Initialises the maxwell management transport and configures the necessary
 * interrupt handlers. Called once during boot.
 */
int gdb_transport_init(struct gdb_transport *gdb_transport, struct scsc_mx *mx, enum gdb_transport_enum type);
void gdb_transport_release(struct gdb_transport *gdb_transport);

/*
 *  Initialises the configuration area incl. Maxwell Infrastructure Configuration,
 * MIF Management Transport Configuration and  MIF Management Stream Configuration.
 */
void gdb_transport_config_serialise(struct gdb_transport *gdb_transport, struct mxtransconf *trans_conf);
void gdb_transport_set_error(struct gdb_transport *gdb_transport);

struct gdb_transport {
	struct scsc_mx *mx;
	struct mif_stream mif_istream;
	struct mif_stream mif_ostream;
	/** Registered channel handlers for messages coming from the AP for each channel */
	gdb_channel_handler channel_handler_fn;
	void *channel_handler_data;
	pthread_mutex_t channel_handler_mutex;
	/* Transport processor type  */
	enum gdb_transport_enum type;
};

struct gdb_transport_client {
	char *name;
	void (*probe)(struct gdb_transport_client *gdb_client, struct gdb_transport *gdb_transport, char *dev_uid);
	void (*remove)(struct gdb_transport_client *gdb_client, struct gdb_transport *gdb_transport);
};

int gdb_transport_register_client(struct gdb_transport_client *gdb_client);
void gdb_transport_unregister_client(struct gdb_transport_client *gdb_client);
void gdb_transport_register_channel_handler(struct gdb_transport *gdb_transport, gdb_channel_handler handler, void *data);
void gdb_transport_register_char_device(struct scsc_mx *mx, struct gdb_transport **gdb_transport_handler);
#endif							/* GDB_TRANSPORT_H__ */
