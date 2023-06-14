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

#ifndef AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_BUFFER_H
#define AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_BUFFER_H

#include "audio_hw_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AudioBuffer {
	size_t read_ptr;
	size_t write_ptr;
	size_t size_remain;
	size_t capacity;
	char *raw_data;
} AudioBuffer;

AudioBuffer *ameba_audio_stream_buffer_create(void);
void   ameba_audio_stream_buffer_release(AudioBuffer *buffer);
void   ameba_audio_stream_buffer_alloc(AudioBuffer *buffer, size_t capacity);
size_t ameba_audio_stream_buffer_get_remain_size(AudioBuffer *buffer);
size_t ameba_audio_stream_buffer_get_available_size(AudioBuffer *buffer);
size_t ameba_audio_stream_buffer_get_buffer_capacity(AudioBuffer *buffer);
size_t ameba_audio_stream_buffer_write_in_noirq_mode(AudioBuffer *buffer, const void *data, size_t bytes, uint32_t period_bytes);
size_t ameba_audio_stream_buffer_write(AudioBuffer *buffer, const void *data, size_t bytes);
size_t ameba_audio_stream_buffer_read(AudioBuffer *buffer, void *data, size_t bytes, uint32_t mode);
size_t ameba_audio_stream_buffer_get_tx_readptr(AudioBuffer *buffer);
size_t ameba_audio_stream_buffer_get_rx_writeptr(AudioBuffer *buffer);
void   ameba_audio_stream_buffer_update_rx_writeptr(AudioBuffer *buffer, size_t bytes);
void   ameba_audio_stream_buffer_update_tx_readptr(AudioBuffer *buffer, size_t bytes);
void   ameba_audio_stream_buffer_flush(AudioBuffer *buffer);

#ifdef __cplusplus
}
#endif

#endif


