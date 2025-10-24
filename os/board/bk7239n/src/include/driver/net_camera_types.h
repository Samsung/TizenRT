// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_err.h>
#include <driver/psram_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int (*fb_init) (fb_type_t type); /**< init jpeg psram buffer */
	int (*fb_deinit) (fb_type_t type); /**< deinit jpeg psram buffer */
	void (*fb_clear) (fb_type_t type); /**< clear jpeg psram buffer */
	void (*fb_complete) (frame_buffer_t* buffer);  /**< dvp notify upper layer a complete frame ready */
	frame_buffer_t* (*fb_malloc)(fb_type_t type, uint32_t size); /**< malloc jpeg psram buffer */
	void (*fb_free)(frame_buffer_t* buffer);
	void (*receive_jpeg)(void *data, uint32_t length);
} net_camera_config_t;

typedef struct {
	/// the video data receive complete
	beken_semaphore_t aready_semaphore;
	/// frame_buffer
	frame_buffer_t *frame;
	/// recoder the buff ptr of every time receive video packte
	uint8_t *buf_ptr;
	/// video buff receive state
	uint8_t start_buf;
	/// dma id for memcpy for sram -> sram
	uint8_t dma_id;
	/// dma id for memcpy sram - >psram
	uint8_t dma_psram;
	/// the packet count of one frame
	uint32_t frame_pkt_cnt;
} net_camera_buffer_t;


#ifdef __cplusplus
}
#endif

