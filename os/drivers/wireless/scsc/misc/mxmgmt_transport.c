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
 * Maxwell management transport (implementation)
 */

/** Implements */
#include "mxmgmt_transport.h"
#include <errno.h>

/** Uses */
//#include <tinyara/module.h>
#include "mxmgmt_transport_format.h"
#include "mifintrbit.h"
#include "debug_scsc.h"

/* Flag that an error has occurred so the I/O thread processing should stop */
void mxmgmt_transport_set_error(struct mxmgmt_transport *mxmgmt_transport)
{
	pr_warn("%s: I/O thread processing is suspended\n", __func__);

	mxmgmt_transport->mxmgmt_thread.block_thread = 1;
}

/** MIF Interrupt handler for writes made to the AP */
static void input_irq_handler(int irq, void *data)
{
	struct mxmgmt_transport *mxmgmt_transport = (struct mxmgmt_transport *)data;
	struct mxmgmt_thread *th = &mxmgmt_transport->mxmgmt_thread;
	struct scsc_mif_abs *mif_abs;

	/* Clear the interrupt first to ensure we can't possibly miss one */
	mif_abs = scsc_mx_get_mif_abs(mxmgmt_transport->mx);
	mif_abs->irq_bit_clear(mif_abs, irq);
	sem_post(&th->selectsem);
}

/** MIF Interrupt handler for acknowledging writes made by the AP */
static void output_irq_handler(int irq, void *data)
{
	struct scsc_mif_abs *mif_abs;
	struct mxmgmt_transport *mxmgmt_transport = (struct mxmgmt_transport *)data;

	/* Clear the interrupt first to ensure we can't possibly miss one */
	/* The FW read some data from the output stream.
	 * Currently we do not care, so just clear the interrupt. */
	mif_abs = scsc_mx_get_mif_abs(mxmgmt_transport->mx);
	mif_abs->irq_bit_clear(mif_abs, irq);

	/* The driver doesn't use the ack IRQ, so mask it from now on,
	 * otherwise we may get spurious host-wakes.
	 */
	mif_abs->irq_bit_mask(mif_abs, irq);
}

static void thread_wait_until_stopped(struct mxmgmt_transport *mxmgmt_transport)
{
	struct mxmgmt_thread *th = &mxmgmt_transport->mxmgmt_thread;

	/*
	 * kthread_stop() cannot handle the th exiting while
	 * kthread_should_stop() is false, so sleep until kthread_stop()
	 * wakes us up.
	 */
	SLSI_INFO_NODEV("%s waiting for the stop signal.\n", th->name);
	th->task = -1;
	SLSI_INFO_NODEV("%s exiting....\n", th->name);
}

/**
 * A thread that forwards messages sent across the transport to
 * the registered handlers for each channel.
 */
static pthread_addr_t mxmgmt_thread_function(pthread_addr_t arg)
{
	struct mxmgmt_transport *mxmgmt_transport = (struct mxmgmt_transport *)arg;
	struct mxmgmt_thread *th = &mxmgmt_transport->mxmgmt_thread;
	const struct mxmgr_message *current_message;
	int ret = OK;

	while (th->wakeup_flag) {
		ret = sem_wait(&th->selectsem);
		if (ret != OK) {
			sem_post(&th->selectsem);
			return 0;
		}

		/* Forward each pending message to the applicable channel handler */
		current_message = mif_stream_peek(&mxmgmt_transport->mif_istream, NULL);
		while (current_message != NULL) {
			pthread_mutex_lock(&mxmgmt_transport->channel_handler_mutex);
			if (current_message->channel_id < MMTRANS_NUM_CHANNELS && mxmgmt_transport->channel_handler_fns[current_message->channel_id]) {
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
				pr_debug("%s: Calling handler for channel_id: %d\n", __func__, current_message->channel_id);
#endif
				(*mxmgmt_transport->channel_handler_fns[current_message->channel_id])(current_message->payload, mxmgmt_transport->channel_handler_data[current_message->channel_id]);
			} else {
				/* HERE: Invalid channel or no handler, raise fault or log message */
				pr_warn("%s: Invalid channel or no handler channel_id: %d\n", __func__, current_message->channel_id);
			}

			pthread_mutex_unlock(&mxmgmt_transport->channel_handler_mutex);
			/* Remove the current message from the buffer before processing the next
			 * one in case it generated another message, otherwise it's possible we
			 * could run out of space in the stream before we get through all the messages. */
			mif_stream_peek_complete(&mxmgmt_transport->mif_istream, current_message);
			current_message = mif_stream_peek(&mxmgmt_transport->mif_istream, NULL);
		}
	}

	return 0;
}

static int mxmgmt_thread_start(struct mxmgmt_transport *mxmgmt_transport)
{
	struct mxmgmt_thread *th = &mxmgmt_transport->mxmgmt_thread;

	if (th->task == 0) {
		pr_warn("%s: %s thread already started\n", __func__, th->name);
		return 0;
	}

	/* Initialise thread structure */
	th->block_thread = 1;
	th->wakeup_flag = 1;
	snprintf(th->name, MXMGMT_THREAD_NAME_MAX_LENGTH, "mxmgmt_thread");

	sem_init(&th->selectsem, 0, 0);
	th->task = pthread_create(&th->selecttid, NULL, (pthread_startroutine_t)mxmgmt_thread_function, (pthread_addr_t)mxmgmt_transport);
	if (th->task != 0) {
		return -th->task;
	}

	pthread_setname_np(th->selecttid, "WLAN Driver mxmgmt");

	return 0;
}

static void mgmt_thread_stop(struct mxmgmt_transport *mxmgmt_transport)
{
	struct mxmgmt_thread *th = &mxmgmt_transport->mxmgmt_thread;

	SLSI_INFO_NODEV("Stopping %s mgmt_thread\n", th->name);
	th->wakeup_flag = 0;
	th->task = -1;
	pthread_cancel(th->selecttid);
	/* pthread_create does a pjoin allocation that seems to be freed in pthread_detach */
	pthread_detach(th->selecttid);
}

void mxmgmt_transport_release(struct mxmgmt_transport *mxmgmt_transport)
{
	mgmt_thread_stop(mxmgmt_transport);
	mif_stream_release(&mxmgmt_transport->mif_istream);
	mif_stream_release(&mxmgmt_transport->mif_ostream);
}

void mxmgmt_transport_config_serialise(struct mxmgmt_transport *mxmgmt_transport, struct mxtransconf *trans_conf)
{
	mif_stream_config_serialise(&mxmgmt_transport->mif_istream, &trans_conf->to_ap_stream_conf);
	mif_stream_config_serialise(&mxmgmt_transport->mif_ostream, &trans_conf->from_ap_stream_conf);
}

/** Public functions */
int mxmgmt_transport_init(struct mxmgmt_transport *mxmgmt_transport, struct scsc_mx *mx)
{
	/*
	 * Initialising a buffer of 1 byte is never legitimate, do not allow it.
	 * The memory buffer length must be a multiple of the packet size.
	 */
#define MEM_LENGTH 32

	int r;
	uint32_t mem_length = MEM_LENGTH;
	uint32_t packet_size = sizeof(struct mxmgr_message);
	uint32_t num_packets;
	struct mxmgmt_thread *th = NULL;

	memset(mxmgmt_transport, 0, sizeof(struct mxmgmt_transport));

	num_packets = mem_length / packet_size;
	pthread_mutex_init(&mxmgmt_transport->channel_handler_mutex, NULL);
	mxmgmt_transport->mx = mx;
	r = mif_stream_init(&mxmgmt_transport->mif_istream, MIF_STREAM_DIRECTION_IN, num_packets, packet_size, mx, input_irq_handler, mxmgmt_transport);
	if (r) {
		return r;
	}
	r = mif_stream_init(&mxmgmt_transport->mif_ostream, MIF_STREAM_DIRECTION_OUT, num_packets, packet_size, mx, output_irq_handler, mxmgmt_transport);
	if (r) {
		mif_stream_release(&mxmgmt_transport->mif_istream);
		return r;
	}
	th = &mxmgmt_transport->mxmgmt_thread;
	th->task = -1;				// make sure initial value is set correctly

	r = mxmgmt_thread_start(mxmgmt_transport);
	if (r) {
		mif_stream_release(&mxmgmt_transport->mif_istream);
		mif_stream_release(&mxmgmt_transport->mif_ostream);
		return r;
	}
	return 0;
}

void mxmgmt_transport_register_channel_handler(struct mxmgmt_transport *mxmgmt_transport, enum mxmgr_channels channel_id, mxmgmt_channel_handler handler, void *data)
{
	if (channel_id >= MMTRANS_NUM_CHANNELS) {
		pr_err("%s: Invalid channel id: %d\n", __func__, channel_id);
		return;
	}
	pthread_mutex_lock(&mxmgmt_transport->channel_handler_mutex);
	mxmgmt_transport->channel_handler_fns[channel_id] = handler;
	mxmgmt_transport->channel_handler_data[channel_id] = data;
	pthread_mutex_unlock(&mxmgmt_transport->channel_handler_mutex);
}

void mxmgmt_transport_send(struct mxmgmt_transport *mxmgmt_transport, enum mxmgr_channels channel_id, void *message, uint32_t message_length)
{
	struct mxmgr_message transport_msg = {.channel_id = channel_id };

	const void *bufs[2] = { &transport_msg.channel_id, message };
	uint32_t buf_lengths[2] = { sizeof(transport_msg.channel_id), message_length };

	mif_stream_write_gather(&mxmgmt_transport->mif_ostream, bufs, buf_lengths, 2);
}
