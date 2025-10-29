// Copyright 2020-2025 Beken
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
#include <components/usb_types.h>

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

typedef void (*frame_callback_t)(frame_buffer_t *frame);

typedef enum {
	UVC_DISCONNECT_NORMAL,
	UVC_DISCONNECT_ABNORMAL,
	UVC_CONNECTED,
	UVC_WORKING_START,
	UVC_WORKING_STOP,
	UVC_STATE_UNKNOW,
} uvc_state_t;

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
#ifdef CONFIG_GENERAL_DMA
	dma_isr_t dma_rx_handler;
	uint8_t  dma_channel;
#endif
} uvc_desc_t;

typedef void (*uvc_device_info_t)(bk_uvc_device_brief_info_t *info, uvc_state_t state);

typedef struct
{
	media_camera_device_t device;

	frame_buffer_control_t jpeg_cb;
	frame_buffer_control_t h264_cb;
	frame_buffer_control_t display_cb;
	void (*uvc_connect_state_change_cb)(uvc_state_t state);
	void (*user_cb)(uint8_t *data, uint32_t length);
} uvc_camera_config_t;


#ifdef __cplusplus
}
#endif
