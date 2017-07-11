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
 * Circular buffer backed packet stream (Interface)
 */

#ifndef CPACKET_BUFFER_H__
#define CPACKET_BUFFER_H__

/* Uses */
#include <sys/types.h>
#include "scsc_mx_impl.h"
#include "mxconf.h"

struct cpacketbuffer;

/**
 * Initialises the circular buffer.
 * The memory buffer length must be a multiple of the packet size.
 */
int cpacketbuffer_init(struct cpacketbuffer *buffer, uint32_t num_packets, uint32_t packet_size, struct scsc_mx *mx);
void cpacketbuffer_release(struct cpacketbuffer *buffer);

/**
 * Reads the gven amount of data from the buffer, copying it to the provided address.
 * This automatically removes the read data from the buffer.
 *
 * If the amount of data requested is not a multiple of the packet size
 * only the requested number of bytes will be read, but the partially read packet
 * will still be removed from the buffer.
 *
 * Returns true if the packet was read.
 */
uint32_t cpacketbuffer_read(struct cpacketbuffer *buffer, void *buf, uint32_t num_bytes);

/**
 * Returns a pointer to the next packet of data within the buffer, without
 * removing it. This can be used to processss data in place without needing to
 * copy it first.
 *
 * If multiple packets are present these can be read in turn by setting the value
 * of current_packet to the returned value from the previous call to cpacketbuffer_peek.
 *
 * cpacketbuffer_peek_complete must be called to remove the packet(s) from the buffer.
 *
 * Returns a pointer to the beginning of the packet to read, or NULL if there is no
 * packet to process.
 *
 * Example use:
 *   // Get the first data packet
 *   void *current_packet = cpacketbuffer_peek( buffer, NULL );
 *   void *last_packet = NULL;
 *   while( current_packet != NULL )
 *   {
 *      // Process data packet
 *      ...
 *
 *      // Get the next data packet
 *      last_packet = current_packet;
 *      current_packet = cpacketbuffer_peek( buffer, current_packet );
 *   }
 *
 *   // Remove all processed packets from the buffer
 *   if( last_packet != NULL )
 *   {
 *      cpacketbuffer_peek_complete( buffer, last_packet );
 *   }
 */
const void *cpacketbuffer_peek(struct cpacketbuffer *buffer, const void *current_packet);

/**
 * Removes all packets from the buffer up to and including the given
 * packet.
 *
 * This must be called after using cpacketbuffer_peek to indicate that packet(s)
 * can be removed from the buffer.
 */
void cpacketbuffer_peek_complete(struct cpacketbuffer *buffer, const void *packet);

/**
 * Writes a number of bytes to the buffer. This will always use up whole packets in the buffer
 * even if the amount of data written is not an exact multiple of the packet size.
 *
 * Returns true if the data was written, false if there is not enough free space in the buffer.
 */
bool cpacketbuffer_write(struct cpacketbuffer *buffer, const void *buf, uint32_t num_bytes);

/**
 * Writes a set of non-contiguous data blocks to the buffer as a contiguous set.
 * This will always use up whole packets even if the
 * amount of data written is not an exact multiple of the packet size.
 *
 * Returns true if the blocks were written, false if there is not enough
 * free space in the buffer for all the blocks.
 */
bool cpacketbuffer_write_gather(struct cpacketbuffer *buffer, const void **bufs, uint32_t *num_bytes, uint32_t num_bufs);

/**
 * Returns the number of free packets in the buffer.
 */
uint32_t cpacketbuffer_free_space(const struct cpacketbuffer *buffer);

/**
 * Returns true if the buffer is empty.
 */
bool cpacketbuffer_is_empty(const struct cpacketbuffer *buffer);

/**
 * Returns true if the buffer is full.
 */
bool cpacketbuffer_is_full(const struct cpacketbuffer *buffer);

/**
 * Returns the packet size the buffer was initialised with. This is the same value
 * as the packet_size argument passed to cpacketbuffer_init().
 */
uint32_t cpacketbuffer_packet_size(const struct cpacketbuffer *buffer);

void cpacketbuffer_config_serialise(const struct cpacketbuffer *buffer, struct mxcbufconf *buf_conf);
/**
 * Buffer context object.
 */
struct cpacketbuffer {
	struct scsc_mx *mx;
	void *buffer;				/* Buffer location */
	uint32_t num_packets;		/* Total number of packets that can be stored in the buffer */
	uint32_t packet_size;		/* Size of each individual packet within the buffer */

	/** Pointers to 32bit R/W indexes - these should point to uint32_ts */
	uint32_t *read_index;		/* Pointer to the location of the read index, which
								 *     contains the index of the next packet to read. */
	uint32_t *write_index;		/* Pointer to the location of the write index, which
								 *     contains the index after the last packet written. */
};

#endif							/* CPACKET_BUFFER_H__ */
