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

#include <driver/media_types.h>
#include <driver/dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_UVC_CONFIG_ERROR    (BK_ERR_UVC_BASE)
#define BK_UVC_NO_MEMORY       (BK_ERR_UVC_BASE - 1)
#define BK_UVC_NO_RESOURCE     (BK_ERR_UVC_BASE - 2)
#define BK_UVC_PPI_ERROR       (BK_ERR_UVC_BASE - 3)
#define BK_UVC_DISCONNCT       (BK_ERR_UVC_BASE - 4)
#define BK_UVC_NO_RESPON       (BK_ERR_UVC_BASE - 5)
#define BK_UVC_DMA_ERROR       (BK_ERR_UVC_BASE - 6)
#define BK_UVC_NOT_PERMIT      (BK_ERR_UVC_BASE - 7)

#define USB_UVC_FIFO_ADDR      (0x46002024)
#define USB_UVC_HEAD_LEN       12
#define UVC_PPI_TYPE_CNT       15

typedef enum {
	UVC_DISCONNECT_NORMAL,
	UVC_DISCONNECT_ABNORMAL,
	UVC_CONNECTED,
	UVC_WORKING_START,
	UVC_WORKING_STOP,
	UVC_STATE_UNKNOW,
} uvc_state_t;

typedef enum {
	UVC_SET_PARAM,
	UVC_START,
	UVC_STOP,
	UVC_EOF,
	UVC_CAPTURE,
	UVC_EXIT,
}uvc_msg_type_t;

typedef struct {
	uint8_t  type;
	uint32_t data;
}uvc_msg_t;

typedef struct {
	uint16_t fps;
	uint16_t width;
	uint16_t height;
}uvc_camera_device_t;

typedef struct {
	uint8_t *rxbuf;
	uint8_t mem_status;
	uint8_t capture_enable;

	void (*node_full_handler)(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len);
	void (*data_end_handler)(void);

	uint32_t rxbuf_len;
	uint32_t rx_read_len;
	uint32_t node_len;
	uint32_t frame_id;
	uint32_t frame_len;

	void (*end_frame_handler)(void *buf, uint32_t frame_len);
	void (*frame_capture_handler)(void *buf, uint32_t frame_len);
#if CONFIG_GENERAL_DMA
	dma_isr_t dma_rx_handler;
	uint8_t  dma_channel;
#endif
} uvc_desc_t;

typedef struct {
	media_camera_type_t type;
	uvc_camera_device_t device;
	void (*connect_state_change_cb)(uvc_state_t state);
	void (*uvc_packet_rx_cb)(uint8_t *data, uint32_t length);
} uvc_config_t;

typedef struct
{
	uvc_config_t *uvc_config;
	int (*fb_init)(media_ppi_t max_ppi);
	int (*fb_deinit) (void);
	void (*fb_complete)(frame_buffer_t* buffer);
	frame_buffer_t* (*fb_malloc)(void);
	void (*fb_free)(frame_buffer_t* buffer);
	void (*uvc_connect_state_change_cb)(uvc_state_t state);
} uvc_camera_config_t;


#ifdef __cplusplus
}
#endif
