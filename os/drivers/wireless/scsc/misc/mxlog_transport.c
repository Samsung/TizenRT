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
/** Uses */
#include "scsc_mif_abs.h"
#include "mifintrbit.h"
/** Implements */
#include "mxlog_transport.h"

#define MXLOG_TRANSPORT_BUF_LENGTH (32)
#define MXLOG_TRANSPORT_PACKET_SIZE (4)
#define SYNC_VALUE (0xA55A)

static void input_irq_handler(int irq, void *data)
{
	struct mxlog_transport *mxlog_transport = (struct mxlog_transport *)data;
	struct scsc_mif_abs *mif_abs;
	u32 header;
	char *buf;
	u32 num_bytes;
	u32 level;

	if (!mxlog_transport->channel_handler_fn) {
		/* HERE: Invalid handler, raise fault or log message */
		pr_warn("%s: mxlog_transport->channel_handler_fn==NULL\n", __func__);
		return;
	}
	/* 1st length */
	/* Clear the interrupt first to ensure we can't possibly miss one */
	mif_abs = scsc_mx_get_mif_abs(mxlog_transport->mx);
	mif_abs->irq_bit_clear(mif_abs, irq);
	/* 1st read - header only */
	while (mif_stream_read(&mxlog_transport->mif_stream, (void *)&header, sizeof(uint32_t))) {
		if (((header & 0xFFFF0000) >> 16) != SYNC_VALUE) {
			pr_err("%s: Bad sync in header: header=0x%08x\n", __func__, header);
			return;
		}
		num_bytes = header & 0x000000FF;
		if (num_bytes > 0 && num_bytes < (MXLOG_TRANSPORT_BUF_LENGTH - sizeof(uint32_t))) {
			buf = kmm_malloc(num_bytes);
			/* 2nd read - payload (msg) */
			mif_stream_read(&mxlog_transport->mif_stream, buf, num_bytes);
			level = (header & 0x0000FF00) >> 8;
			mxlog_transport->channel_handler_fn(buf, num_bytes, level, mxlog_transport->channel_handler_data);
			kmm_free(buf);
		} else {
			pr_err("%s: Bad num_bytes(%d) in header: header=0x%08x\n", __func__, num_bytes, header);
		}
	}
}

void mxlog_transport_release(struct mxlog_transport *mxlog_transport)
{
	mif_stream_release(&mxlog_transport->mif_stream);
}

void mxlog_transport_config_serialise(struct mxlog_transport *mxlog_transport, struct mxlogconf *mxlogconf)
{
	mif_stream_config_serialise(&mxlog_transport->mif_stream, &mxlogconf->stream_conf);
}

/** Public functions */
int mxlog_transport_init(struct mxlog_transport *mxlog_transport, struct scsc_mx *mx)
{
	int r;
	uint32_t mem_length = MXLOG_TRANSPORT_BUF_LENGTH;
	uint32_t packet_size = MXLOG_TRANSPORT_PACKET_SIZE;
	uint32_t num_packets;

	memset(mxlog_transport, 0, sizeof(struct mxlog_transport));
	num_packets = mem_length / packet_size;
	mxlog_transport->mx = mx;
	r = mif_stream_init(&mxlog_transport->mif_stream, MIF_STREAM_DIRECTION_IN, num_packets, packet_size, mx, input_irq_handler, mxlog_transport);
	if (r) {
		return r;
	}

	return 0;
}

void mxlog_transport_register_channel_handler(struct mxlog_transport *mxlog_transport, mxlog_channel_handler handler, void *data)
{
	mxlog_transport->channel_handler_fn = handler;
	mxlog_transport->channel_handler_data = (void *)data;
}
