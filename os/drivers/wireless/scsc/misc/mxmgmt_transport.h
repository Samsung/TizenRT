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
 * mx140 management transport (Interface)
 *
 * Provides bi-directional communication between the firmware and the
 * host. Messages sent across the transport are divided into a
 * number of channels, with each channel having its own dedicated handler.
 *
 * This interface also provides a utility method for sending messages across
 * the stream.
 */

#ifndef MXMANAGEMENT_TRANSPORT_H__
#define MXMANAGEMENT_TRANSPORT_H__

/** Uses */
//#include <sys/wait.h>
#include <sys/types.h>

#include <tinyara/kthread.h>
#include "mifstream.h"

struct mxmgmt_transport;

/**
 * The various channels that can send messages
 * across the transport.
 *
 * Channel IDs are limited to one byte.
 */
enum mxmgr_channels {
	MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT = 0,
	MMTRANS_CHAN_ID_SERVICE_MANAGEMENT = 1,
	MMTRANS_CHAN_ID_MAXWELL_LOGGING = 2,
	MMTRANS_NUM_CHANNELS = 3
};

/**
 * Transport channel callback handler. This will be invoked each time a message on a channel is
 * received in the context of the transport stream's thread. Handlers may perform work within
 * their callback implementation, but should not block.
 *
 * Note that the message pointer passed is only valid for the duration of the function call.
 */
typedef void (*mxmgmt_channel_handler)(const void *message, void *data);

/**
 * Registers the callback function that will be invoked to handle data coming in from the AP
 * for the given channel.
 */
void mxmgmt_transport_register_channel_handler(struct mxmgmt_transport *mxmgmt_transport, enum mxmgr_channels channel_id, mxmgmt_channel_handler handler, void *data);

/**
 * Sends a message to the AP across the given channel.
 *
 * This function is safe to call from any RTOS thread.
 */
void mxmgmt_transport_send(struct mxmgmt_transport *mxmgmt_transport, enum mxmgr_channels channel_id, void *message, uint32_t message_length);

/**
 * Initialises the maxwell management transport and configures the necessary
 * interrupt handlers. Called once during boot.
 */
int mxmgmt_transport_init(struct mxmgmt_transport *mxmgmt_transport, struct scsc_mx *mx);
void mxmgmt_transport_release(struct mxmgmt_transport *mxmgmt_transport);

/*
 *  Initialises the configuration area incl. Maxwell Infrastructure Configuration,
 * MIF Management Transport Configuration and  MIF Management Stream Configuration.
 */
void mxmgmt_transport_config_serialise(struct mxmgmt_transport *mxmgmt_transport, struct mxtransconf *trans_conf);
void mxmgmt_transport_set_error(struct mxmgmt_transport *mxmgmt_transport);

#define MXMGMT_THREAD_NAME_MAX_LENGTH 32
struct mxmgmt_thread {
	//struct task_struct *task;
	//pthread_t workertid;                  /* Handle for the worker thread */
	pthread_t selecttid;		/* Handle for the select thread */
	int task;
	char name[MXMGMT_THREAD_NAME_MAX_LENGTH];
	int prio;
	//struct completion  completion;
	pthread_barrier_t barrier;
	unsigned int wakeup_flag;
	/*
	 * Use it to block the I/O thread when
	 * an error occurs.
	 */
	int block_thread;
	sem_t selectsem;			/* Used to sleep the select thread */
};

struct mxmgmt_transport {
	struct scsc_mx *mx;
	struct mif_stream mif_istream;
	struct mif_stream mif_ostream;
	struct mxmgmt_thread mxmgmt_thread;
	/** Registered channel handlers for messages coming from the AP for each channel */
	mxmgmt_channel_handler channel_handler_fns[MMTRANS_NUM_CHANNELS];
	void *channel_handler_data[MMTRANS_NUM_CHANNELS];
	pthread_mutex_t channel_handler_mutex;
};

#endif							/* MXMANAGEMENT_TRANSPORT_H__ */
