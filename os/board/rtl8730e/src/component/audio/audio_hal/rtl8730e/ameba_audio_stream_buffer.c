/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ameba_audio_stream_buffer.h"

#include "ameba_audio_stream_utils.h"
#include "ameba_audio_types.h"
#include "ameba_soc.h"
#include "platform_stdlib.h"

AudioBuffer *ameba_audio_stream_buffer_create(void)
{
	AudioBuffer *buffer;
	buffer = (AudioBuffer *)ameba_audio_gdma_calloc(1, sizeof(AudioBuffer));
	if (!buffer) {
		HAL_AUDIO_ERROR("[AmebaAudioRbuf] calloc AudioRBuffer fail");
		return NULL;
	}
	buffer->read_ptr = 0;
	buffer->write_ptr = 0;
	buffer->size_remain = 0;
	buffer->capacity = 0;
	buffer->raw_data = NULL;
	return buffer;
}

void ameba_audio_stream_buffer_release(AudioBuffer *buffer)
{
	if (buffer->raw_data != NULL) {
		ameba_audio_gdma_free(buffer->raw_data);
		buffer->raw_data = NULL;
	}

	if (buffer) {
		ameba_audio_gdma_free(buffer);
	}
}


void ameba_audio_stream_buffer_alloc(AudioBuffer *buffer, size_t capacity)
{

	buffer->raw_data = (char *)ameba_audio_gdma_calloc(capacity, sizeof(char));
	if (!(buffer->raw_data)) {
		HAL_AUDIO_ERROR("[AmebaAudioRbuf] calloc raw_data fail");
		return;
	}

	buffer->capacity = capacity;
}

size_t ameba_audio_stream_buffer_get_buffer_capacity(AudioBuffer *buffer)
{
	return buffer->capacity;
}

/*buffer bytes remain in buffer to be consumed*/
size_t ameba_audio_stream_buffer_get_remain_size(AudioBuffer *buffer)
{
	return buffer->size_remain;
}

/*buffer bytes available in buffer to be written*/
size_t ameba_audio_stream_buffer_get_available_size(AudioBuffer *buffer)
{
	if (buffer->size_remain == buffer->capacity || buffer->capacity == 0) {
		return 0;
	}

	return (buffer->write_ptr < buffer->read_ptr) ? (buffer->read_ptr - buffer->write_ptr) : (buffer->capacity - (buffer->write_ptr - buffer->read_ptr));
}

void ameba_audio_stream_buffer_flush(AudioBuffer *buffer)
{
	buffer->read_ptr = 0;
	buffer->write_ptr = 0;
	buffer->size_remain = 0;
}

/*write to buffer by hal*/
size_t ameba_audio_stream_buffer_write_in_noirq_mode(AudioBuffer *buffer, const void *data, size_t bytes, uint32_t period_bytes)
{
	if (bytes == 0 || buffer->capacity == 0) {
		return 0;
	}

	size_t capacity = buffer->capacity;
	size_t bytes_to_write = bytes;
	size_t tx_addr;

	if (bytes_to_write <= capacity - buffer->write_ptr) {
		tx_addr = (size_t)buffer->raw_data + buffer->write_ptr;
		memcpy(buffer->raw_data + buffer->write_ptr, (u8 *)data, bytes_to_write);
		DCache_CleanInvalidate((uint32_t)tx_addr, (period_bytes + CACHE_LINE_SIZE));
		buffer->write_ptr += bytes_to_write;
		if (buffer->write_ptr == capacity) {
			buffer->write_ptr = 0;
		}
	} else {
		size_t size_1 = capacity - buffer->write_ptr;
		tx_addr = (size_t)buffer->raw_data + buffer->write_ptr;
		memcpy(buffer->raw_data + buffer->write_ptr, (u8 *)data, size_1);
		size_t size_2 = bytes_to_write - size_1;
		memcpy(buffer->raw_data, (u8 *)data + size_1, size_2);
		DCache_CleanInvalidate((uint32_t)buffer->raw_data, ((uint32_t)capacity + CACHE_LINE_SIZE));
		buffer->write_ptr = size_2;
	}

	buffer->size_remain += bytes_to_write;

	return bytes_to_write;
}

/*write to buffer by hal*/
size_t ameba_audio_stream_buffer_write(AudioBuffer *buffer, const void *data, size_t bytes)
{
	if (bytes == 0 || buffer->size_remain >= buffer->capacity || buffer->capacity == 0) {
		return 0;
	}

	size_t capacity = buffer->capacity;
	size_t bytes_to_write = (bytes <= (capacity - buffer->size_remain)) ? bytes : (capacity - buffer->size_remain);

	if (bytes_to_write <= capacity - buffer->write_ptr) {
		memcpy(buffer->raw_data + buffer->write_ptr, (u8 *)data, bytes_to_write);
		buffer->write_ptr += bytes_to_write;
		if (buffer->write_ptr == capacity) {
			buffer->write_ptr = 0;
		}
	} else {
		size_t size_1 = capacity - buffer->write_ptr;
		memcpy(buffer->raw_data + buffer->write_ptr, (u8 *)data, size_1);
		size_t size_2 = bytes_to_write - size_1;
		memcpy(buffer->raw_data, (u8 *)data + size_1, size_2);
		buffer->write_ptr = size_2;
	}

	buffer->size_remain += bytes_to_write;

	HAL_AUDIO_PVERBOSE("after write,rp:%d,wp:%d,bytes_to_write:%d,size_remain:%d", buffer->read_ptr, buffer->write_ptr, bytes_to_write,
					   buffer->size_remain);

	return bytes_to_write;
}

/*read buffer by gdma, update readptr here*/
void ameba_audio_stream_buffer_update_tx_readptr(AudioBuffer *buffer, size_t bytes)
{
	if (bytes == 0) {
		return;
	}

	size_t capacity = buffer->capacity;
	size_t bytes_to_read = (bytes <= buffer->size_remain) ? bytes : buffer->size_remain;

	if (bytes_to_read <= capacity - buffer->read_ptr) {
		buffer->read_ptr += bytes_to_read;
		if (buffer->read_ptr == capacity) {
			buffer->read_ptr = 0;
		}
	} else {
		size_t size_1 = capacity - buffer->read_ptr;
		size_t size_2 = bytes_to_read - size_1;
		buffer->read_ptr = size_2;
	}

	HAL_AUDIO_PVERBOSE("gdma up rd done,rp:%d,wp:%d", buffer->read_ptr, buffer->write_ptr);

	buffer->size_remain -= bytes_to_read;
}

size_t ameba_audio_stream_buffer_get_tx_readptr(AudioBuffer *buffer)
{
	return buffer->read_ptr;
}

/*write to buffer by gdma, update writeptr here*/
void ameba_audio_stream_buffer_update_rx_writeptr(AudioBuffer *buffer, size_t bytes)
{
	if (bytes == 0 || buffer->size_remain >= buffer->capacity || buffer->capacity == 0) {
		return;
	}

	if ((buffer->write_ptr + bytes) > buffer->capacity) {
		HAL_AUDIO_ERROR("write ptr out of range!");
		buffer->write_ptr = 0;
	}

	if ((buffer->write_ptr + bytes) == buffer->capacity) {
		buffer->write_ptr = 0;
	} else {
		buffer->write_ptr += bytes;
	}

	buffer->size_remain += bytes;
	HAL_AUDIO_CVERBOSE("gdma up wr done,rp:%d,wp:%d, buffer->size_remain:%d", buffer->read_ptr, buffer->write_ptr, buffer->size_remain);
}

/*read buffer by hal*/
size_t ameba_audio_stream_buffer_read(AudioBuffer *buffer, void *data, size_t bytes, uint32_t mode)
{
	if (bytes == 0) {
		return 0;
	}

	size_t capacity = buffer->capacity;
	size_t bytes_to_read;
	if (mode == AMEBA_AUDIO_DMA_NOIRQ_MODE) {
		bytes_to_read = bytes;
	} else {
		bytes_to_read = (bytes <= buffer->size_remain) ? bytes : buffer->size_remain;
	}

	if (bytes_to_read <= capacity - buffer->read_ptr) {
		/*must add cache clean invalidate here. otherwize buf read may remain dirty.*/
		DCache_CleanInvalidate((uint32_t)buffer->raw_data + (uint32_t)buffer->read_ptr, bytes_to_read);
		memcpy(data, buffer->raw_data + buffer->read_ptr, bytes_to_read);
		buffer->read_ptr += bytes_to_read;
		if (buffer->read_ptr == capacity) {
			buffer->read_ptr = 0;
		}
	} else {
		size_t size_1 = capacity - buffer->read_ptr;
		/*must add cache clean invalidate here. otherwize buf read may remain dirty.*/
		DCache_CleanInvalidate((uint32_t)buffer->raw_data + (uint32_t)buffer->read_ptr, size_1);
		memcpy((u8 *)data, buffer->raw_data + buffer->read_ptr, size_1);
		size_t size_2 = bytes_to_read - size_1;
		DCache_CleanInvalidate((uint32_t)buffer->raw_data, size_2);
		memcpy((u8 *)data + size_1, buffer->raw_data, size_2);
		buffer->read_ptr = size_2;
	}

	buffer->size_remain -= bytes_to_read;
	HAL_AUDIO_CVERBOSE("after read,rp:%u,wp:%u, buffer->size_remain:%u", buffer->read_ptr, buffer->write_ptr, buffer->size_remain);
	return bytes_to_read;
}

size_t ameba_audio_stream_buffer_get_rx_writeptr(AudioBuffer *buffer)
{
	return buffer->write_ptr;
}

