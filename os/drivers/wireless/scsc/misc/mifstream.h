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
 * MIF stream (Interface)
 *
 * Provides a one-way communication mechanism between two points. The consumer side
 * will be notified via an interrupt when the producer side writes data to the
 * stream, and likewise the producer will be notified when the consumer has read
 * data from the stream.
 *
 * It is expected that the data sent across the stream consists of fixed-size
 * packets, and that the underlying storage mechanism is initialised to use a packet size
 * that is at least as large as the largest message size. If this is not the case,
 * callers are responsible for handling reading of partial messages from the stream
 * in multiples of the packet size.
 */

#ifndef MIFSTREAM_H__
#define MIFSTREAM_H__

/** Uses */
#include "cpacket_buffer.h"
#include "mifintrbit.h"

enum MIF_STREAM_DIRECTION {
	MIF_STREAM_DIRECTION_IN,
	MIF_STREAM_DIRECTION_OUT,
	MIF_STREAM_DIRECTION_INVALID,
};

struct mif_stream;

/**
 * Initialises MIF Stream state.
 */
int mif_stream_init(struct mif_stream *stream, enum MIF_STREAM_DIRECTION direction, uint32_t num_packets, uint32_t packet_size, struct scsc_mx *mx, mifintrbit_handler tohost_irq_handler, void *data);
/**
 * Initialises MIF Output Stream state.
 */
void mif_stream_release(struct mif_stream *stream);
/**
 * Reads the given number of bytes from the MIF stream, copying them
 * to the provided address. This removes the read data from the stream.
 *
 * Returns the number of bytes read.
 */
uint32_t mif_stream_read(struct mif_stream *stream, void *buf, uint32_t num_bytes);

/**
 * Returns a pointer to the next packet of data within the stream, without
 * removing it. This can be used to processss data in place without needing to
 * copy it first.
 *
 * If multiple packets are present these can be read in turn by setting the value
 * of current_packet to the returned value from the previous call to mif_stream_peek.
 * Each time the returned pointer will advance in the stream by mif_stream_block_size()
 * bytes.
 *
 * Callers cannot assume that multiple calls to mif_stream_peek() will return
 * consecutive addresses.
 *
 * mif_stream_peek_complete must be called to remove the packet(s) from the stream.
 *
 * Returns a pointer to the beginning of the packet to read, or NULL if there is no
 * packet to process.
 *
 * Example use:
 *   // Get the first data packet
 *   void *current_packet = mif_stream_peek( buffer, NULL );
 *   void *last_packet = NULL;
 *   while( current_packet != NULL )
 *   {
 *      // Process data packet
 *      ...
 *
 *      // Get the next data packet
 *      last_packet = current_packet;
 *      current_packet = mif_stream_peek( buffer, current_packet );
 *   }
 *
 *   // Remove all processed packets from the stream
 *   if( last_packet != NULL )
 *   {
 *      mif_stream_peek( buffer, last_packet );
 *   }
 */
const void *mif_stream_peek(struct mif_stream *stream, const void *current_packet);

/**
 * Removes all packets from the stream up to and including the given
 * packet.
 *
 * This must be called after using mif_stream_peek to indicate that packet(s)
 * can be removed from the stream.
 */
void mif_stream_peek_complete(struct mif_stream *stream, const void *packet);

/**
 * Writes the given number of bytes to the MIF stream.
 *
 * Returns true if the block was written, false if there is not enough
 * free space in the buffer for the data.
 */
bool mif_stream_write(struct mif_stream *stream, const void *buf, uint32_t num_bytes);

/**
 * Writes a set of non-contiguous data blocks to the MIF stream
 * as a contiguous set.
 *
 * Returns true if the blocks were written, false if there is not enough
 * free space in the buffer for the block.
 */
bool mif_stream_write_gather(struct mif_stream *stream, const void **bufs, uint32_t *lengths, uint32_t num_bufs);

/**
 * Returns the size in bytes of each individual block within the stream.
 *
 * When reading data from the stream using mif_stream_read or mif_stream_peek
 * this value is the amount of data
 */
uint32_t mif_stream_block_size(struct mif_stream *stream);

/**
 * Returns the interrupt number that will be triggered by reads from the stream
 */
uint8_t mif_stream_read_interrupt(struct mif_stream *stream);

/**
 * Returns the interrupt number that will be triggered by writes to the stream
 */
uint8_t mif_stream_write_interrupt(struct mif_stream *stream);

/*
 * Initialises the stream's part of the configuration area
 */
void mif_stream_config_serialise(struct mif_stream *stream, struct mxstreamconf *stream_conf);
/**
 * MIF Packet Stream Descriptor.
 */
struct mif_stream {
	struct scsc_mx *mx;
	struct cpacketbuffer buffer;

	/** MIF interrupt bit index, one in each direction */
	uint8_t read_bit_idx;
	uint8_t write_bit_idx;
	enum MIF_STREAM_DIRECTION direction;
};

#endif							/* MIFSTREAM_H__ */
