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
#ifndef AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_CAPTURE_H
#define AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_CAPTURE_H

#include "ameba_audio_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _CaptureStream {
	Stream stream;
	uint32_t channel;
	uint32_t frame_size;
	bool restart_by_read;
	int capture_type;
// member below for channel > 4
	AudioBuffer *extra_rbuffer;
	GdmaCallbackData *extra_gdma_struct;
	uint32_t extra_channel;
	uint32_t extra_frame_size;
	bool extra_restart_by_read;
	uint32_t extra_gdma_cnt;
	uint32_t extra_gdma_irq_cnt;
	bool extra_sem_need_post;
	bool gdma_need_stop;
	_sema extra_sem;
} CaptureStream;

Stream *ameba_audio_stream_rx_init(StreamConfig config);
void ameba_audio_stream_rx_start(Stream *stream);
int  ameba_audio_stream_rx_read(Stream *stream, void *data, uint32_t bytes);
void ameba_audio_stream_rx_close(Stream *stream);

#ifdef __cplusplus
}
#endif

#endif