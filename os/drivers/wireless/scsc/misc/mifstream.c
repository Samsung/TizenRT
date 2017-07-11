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
 * MIF Byte Stream (Implementation)
 */

/* Implements */
#include "scsc_mx_impl.h"
#include "mifstream.h"
#include <errno.h>

/* Uses */
#include "mifintrbit.h"

void mif_stream_config_serialise(struct mif_stream *stream, struct mxstreamconf *stream_conf)
{
	stream_conf->read_bit_idx = stream->read_bit_idx;
	stream_conf->write_bit_idx = stream->write_bit_idx;
	cpacketbuffer_config_serialise(&stream->buffer, &stream_conf->buf_conf);
}

int mif_stream_init(struct mif_stream *stream, enum MIF_STREAM_DIRECTION direction, uint32_t num_packets, uint32_t packet_size, struct scsc_mx *mx, mifintrbit_handler tohost_irq_handler, void *data)
{
	struct mifintrbit *intr;
	int r, r1, r2;

	stream->mx = mx;
	r = cpacketbuffer_init(&stream->buffer, num_packets, packet_size, mx);
	if (r) {
		return r;
	}

	intr = scsc_mx_get_intrbit(mx);
	r1 = mifintrbit_alloc_tohost(intr, tohost_irq_handler, data);
	if (r1 < 0) {
		cpacketbuffer_release(&stream->buffer);
		return r1;
	}

	r2 = mifintrbit_alloc_fromhost(intr, SCSC_MIF_ABS_TARGET_R4);
	if (r2 < 0) {
		cpacketbuffer_release(&stream->buffer);
		mifintrbit_free_tohost(intr, r1);
		return r2;
	}
	switch (direction) {
	case MIF_STREAM_DIRECTION_OUT:
		stream->read_bit_idx = r1;
		stream->write_bit_idx = r2;
		break;
	case MIF_STREAM_DIRECTION_IN:
		stream->read_bit_idx = r2;
		stream->write_bit_idx = r1;
		break;
	default:
		cpacketbuffer_release(&stream->buffer);
		mifintrbit_free_tohost(intr, r1);
		mifintrbit_free_fromhost(intr, r2, SCSC_MIF_ABS_TARGET_R4);
		return -EINVAL;
	}
	stream->direction = direction;
	return 0;
}

void mif_stream_release(struct mif_stream *stream)
{
	struct mifintrbit *intr;

	intr = scsc_mx_get_intrbit(stream->mx);
	if (stream->direction == MIF_STREAM_DIRECTION_IN) {
		mifintrbit_free_tohost(intr, stream->write_bit_idx);
		mifintrbit_free_fromhost(intr, stream->read_bit_idx, SCSC_MIF_ABS_TARGET_R4);
	} else {
		mifintrbit_free_tohost(intr, stream->read_bit_idx);
		mifintrbit_free_fromhost(intr, stream->write_bit_idx, SCSC_MIF_ABS_TARGET_R4);
	}
	cpacketbuffer_release(&stream->buffer);
}

uint32_t mif_stream_read(struct mif_stream *stream, void *buf, uint32_t num_bytes)
{
	struct scsc_mif_abs *mif_abs = scsc_mx_get_mif_abs(stream->mx);

	uint32_t num_bytes_read = cpacketbuffer_read(&stream->buffer, buf, num_bytes);

	if (num_bytes_read > 0)
		/* Signal that the read is finished to anyone interested */
	{
		mif_abs->irq_bit_set(mif_abs, stream->read_bit_idx, SCSC_MIF_ABS_TARGET_R4);
	}

	return num_bytes_read;
}

const void *mif_stream_peek(struct mif_stream *stream, const void *current_packet)
{
	return cpacketbuffer_peek(&stream->buffer, current_packet);
}

void mif_stream_peek_complete(struct mif_stream *stream, const void *packet)
{
	struct scsc_mif_abs *mif_abs = scsc_mx_get_mif_abs(stream->mx);

	cpacketbuffer_peek_complete(&stream->buffer, packet);

	/* Signal that the read is finished to anyone interested */
	mif_abs->irq_bit_set(mif_abs, stream->read_bit_idx, SCSC_MIF_ABS_TARGET_R4);
}

bool mif_stream_write(struct mif_stream *stream, const void *buf, uint32_t num_bytes)
{
	struct scsc_mif_abs *mif_abs = scsc_mx_get_mif_abs(stream->mx);

	if (!cpacketbuffer_write(&stream->buffer, buf, num_bytes)) {
		return false;
	}

	/* Kick the assigned interrupt to let others know new data is available */
	mif_abs->irq_bit_set(mif_abs, stream->read_bit_idx, SCSC_MIF_ABS_TARGET_R4);

	return true;
}

bool mif_stream_write_gather(struct mif_stream *stream, const void **bufs, uint32_t *lengths, uint32_t num_bufs)
{
	struct scsc_mif_abs *mif_abs = scsc_mx_get_mif_abs(stream->mx);

	if (!cpacketbuffer_write_gather(&stream->buffer, bufs, lengths, num_bufs)) {
		return false;
	}

	/* Kick the assigned interrupt to let others know new data is available */
	mif_abs->irq_bit_set(mif_abs, stream->write_bit_idx, SCSC_MIF_ABS_TARGET_R4);
	return true;
}

uint32_t mif_stream_block_size(struct mif_stream *stream)
{
	return cpacketbuffer_packet_size(&stream->buffer);
}

uint8_t mif_stream_read_interrupt(struct mif_stream *stream)
{
	return stream->read_bit_idx;
}

uint8_t mif_stream_write_interrupt(struct mif_stream *stream)
{
	return stream->write_bit_idx;
}
