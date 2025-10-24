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
#include <os/str.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief define all image resolution, unit pixel
 * @{
 */

#define PIXEL_240   (240)
#define PIXEL_272   (272)
#define PIXEL_288   (288)
#define PIXEL_320   (320)
#define PIXEL_400   (400)
#define PIXEL_454   (454)
#define PIXEL_480   (480)
#define PIXEL_576   (576)
#define PIXEL_600   (600)
#define PIXEL_640   (640)
#define PIXEL_720   (720)
#define PIXEL_800   (800)
#define PIXEL_854   (854)
#define PIXEL_864   (864)
#define PIXEL_1024 (1024)
#define PIXEL_1200 (1200)
#define PIXEL_1280 (1280)
#define PIXEL_1600 (1600)

typedef enum {
	MSTATE_TURN_OFF,
	MSTATE_TURNING_OFF,
	MSTATE_TURNING_ON,
	MSTATE_TURN_ON,
} media_state_t;

typedef enum {
    ROTATE_NONE = 0,
    ROTATE_90, /**< Image rotaged 90 degress*/
    ROTATE_180, /**< No support yet, reserved for the future*/
    ROTATE_270, /**< Image rotaged 270 degress*/
    ROTATE_YUV2RGB565,
} media_rotate_t;

typedef enum
{
	PPI_DEFAULT     = 0,
	PPI_320X240     = (PIXEL_320 << 16) | PIXEL_240,
	PPI_320X480     = (PIXEL_320 << 16) | PIXEL_480,
	PPI_400X400     = (PIXEL_400 << 16) | PIXEL_400,
	PPI_454X454     = (PIXEL_454 << 16) | PIXEL_454,
	PPI_480X272     = (PIXEL_480 << 16) | PIXEL_272,
	PPI_480X320     = (PIXEL_480 << 16) | PIXEL_320,
	PPI_480X480     = (PIXEL_480 << 16) | PIXEL_480,
	PPI_640X480     = (PIXEL_640 << 16) | PIXEL_480,
	PPI_480X800     = (PIXEL_480 << 16) | PIXEL_800,
	PPI_480X854     = (PIXEL_480 << 16) | PIXEL_854,
	PPI_720X288     = (PIXEL_720 << 16) | PIXEL_288,
	PPI_720X576     = (PIXEL_720 << 16) | PIXEL_576,
	PPI_800X480     = (PIXEL_800 << 16) | PIXEL_480,
	PPI_864X480     = (PIXEL_864 << 16) | PIXEL_480,
	PPI_800X600     = (PIXEL_800 << 16) | PIXEL_600,
	PPI_1024X600    = (PIXEL_1024 << 16) | PIXEL_600,
	PPI_1280X720    = (PIXEL_1280 << 16) | PIXEL_720,
	PPI_1600X1200   = (PIXEL_1600 << 16) | PIXEL_1200,
} media_ppi_t;

typedef enum
{
	PPI_CAP_UNKNOW 		= 0,
	PPI_CAP_320X240     = (1 << 0), /**< 320 * 240 */
	PPI_CAP_320X480     = (1 << 1), /**< 320 * 480 */
	PPI_CAP_480X272     = (1 << 2), /**< 480 * 272 */
	PPI_CAP_480X320     = (1 << 3), /**< 480 * 320 */
	PPI_CAP_640X480     = (1 << 4), /**< 640 * 480 */
	PPI_CAP_480X800     = (1 << 5), /**< 480 * 800 */
	PPI_CAP_800X480     = (1 << 6), /**< 800 * 480 */
	PPI_CAP_800X600     = (1 << 7), /**< 800 * 600 */
	PPI_CAP_864X480     = (1 << 8), /**< 864 * 480 */
	PPI_CAP_1024X600    = (1 << 9), /**< 1024 * 600 */
	PPI_CAP_1280X720    = (1 << 10), /**< 1280 * 720 */
	PPI_CAP_1600X1200   = (1 << 11), /**< 1600 * 1200 */
	PPI_CAP_480X480     = (1 << 12), /**< 480 * 480 */
	PPI_CAP_720X288     = (1 << 13), /**< 720 * 288 */
	PPI_CAP_720X576     = (1 << 14), /**< 720 * 576 */
	PPI_CAP_480X854     = (1 << 15), /**< 480 * 854 */
} media_ppi_cap_t;

/** rgb lcd input data format, data save in mem is little endian, like VUYY format is [bit31-bit0] is [V U Y Y]*/
typedef enum {
	PIXEL_FMT_UNKNOW,
	PIXEL_FMT_DVP_JPEG,
	PIXEL_FMT_DVP_H264,
	PIXEL_FMT_UVC_JPEG,
	PIXEL_FMT_UVC_H264,
	PIXEL_FMT_RGB565,          /**< input data format is rgb565(big endian), high pixel is bit[31-16], low pixel is bit[15-0] (PIXEL BIG ENDIAN)*/
	PIXEL_FMT_RGB565_LE,          /**< input data format is rgb565(big endian), high pixel is bit[15-0], low pixel is bit[31-16] (PIXEL little ENDIAN)*/
	PIXEL_FMT_YUYV,             /**< input data is yuyv format, diaplay module internal may convert to rgb565 data*/
	PIXEL_FMT_UYVY,
	PIXEL_FMT_YYUV,            /**< input data is yyuv format */
	PIXEL_FMT_UVYY,            /**< input data is uvyy format*/
	PIXEL_FMT_VUYY,            /**< input data is vuyy format */
} pixel_format_t;

typedef enum {
	MEDIA_DVP_MJPEG,
	MEDIA_DVP_H264_WIFI_TRANSFER,
	MEDIA_DVP_H264_USB_TRANSFER,
	MEDIA_DVP_H264_ENC_LCD,
	MEDIA_DVP_H264_LOCAL,
	MEDIA_DVP_YUV,
	MEDIA_DVP_MIX,
	MEDIA_UVC_MJPEG,
	MEDIA_UVC_H264,
	MEDIA_CAMERA_UNKNOW,
} media_camera_type_t;

typedef enum
{
	FPS0    = 0,        /**< 0fps */
	FPS5    = (1 << 0), /**< 5fps */
	FPS10   = (1 << 1), /**< 10fps */
	FPS15   = (1 << 2), /**< 15fps */
	FPS20   = (1 << 3), /**< 20fps */
	FPS25   = (1 << 4), /**< 25fps */
	FPS30   = (1 << 5), /**< 30fps */
} sensor_fps_t;

typedef enum
{
	EVENT_LCD_ROTATE_MBCMD = 0x18,
	EVENT_LCD_ROTATE_MBRSP = 0x19,

	EVENT_LCD_RESIZE_MBCMD = 0x1a,
	EVENT_LCD_RESIZE_MBRSP = 0x1b,

	EVENT_LCD_DEC_SW_MBCMD = 0x1c,
	EVENT_LCD_DEC_SW_MBRSP = 0x1d,
} media_mailbox_event_t;

typedef struct {
	media_camera_type_t camera_type;
	media_ppi_t ppi;
	sensor_fps_t fps;
} media_camera_device_t;

/**
 * @brief define frame buffer
 * @{
 */
typedef struct
{
	pixel_format_t fmt;
	uint8_t mix : 1;
	uint8_t *frame;
	uint16_t width;
	uint16_t height;
	uint32_t length;
	uint32_t size;
	uint32_t sequence;
} frame_buffer_t;

/**
 * @brief define camera type
 * @{
 */
typedef enum
{
	UNKNOW_CAMERA,
	DVP_CAMERA,    /**< dvp camera */
	UVC_CAMERA,    /**< uvc camera */
} camera_type_t;

/**
 * @brief define struct for debug
 * @{
 */
typedef struct
{
	uint16_t isr_jpeg;    /**< jpeg frame count */
	uint16_t isr_decoder; /**< jpeg decode frame count */
	uint16_t err_dec;     /**< jpeg decode error count */
	uint16_t isr_lcd;     /**< lcd display count */
	uint16_t fps_lcd;     /**< lcd display fps */
	uint16_t fps_wifi;    /**< wifi transfer fps */
	uint16_t wifi_read;   /**< wifi transfer read cnt */
	uint16_t psram_busy;  /**< sram to psram busy cnt */
	uint16_t transfer_timer_us; /**< transfer a frame time */
} media_debug_t;

/**
 * @brief get camera width
 * @{
 */
static inline uint16_t ppi_to_pixel_x(media_ppi_t ppi)
{
	return ppi >> 16;
}

/**
 * @brief get camera height
 * @{
 */
static inline uint16_t ppi_to_pixel_y(media_ppi_t ppi)
{
	return ppi & 0xFFFF;
}

static inline uint16_t ppi_to_pixel_x_block(media_ppi_t ppi)
{
	return (ppi >> 16) / 8;
}

static inline uint16_t ppi_to_pixel_y_block(media_ppi_t ppi)
{
	return (ppi & 0xFFFF) / 8;
}

/**
 * @brief get camera support ppi compare with user set
 * @{
 */
static inline media_ppi_cap_t pixel_ppi_to_cap(media_ppi_t ppi)
{
	media_ppi_cap_t cap = PPI_CAP_UNKNOW;

	switch (ppi)
	{
		case PPI_320X240:
			cap = PPI_CAP_320X240;
			break;

		case PPI_320X480:
			cap = PPI_CAP_320X480;
			break;

		case PPI_480X272:
			cap = PPI_CAP_480X272;
			break;

		case PPI_480X320:
			cap = PPI_CAP_480X320;
			break;

		case PPI_480X480:
			cap = PPI_CAP_480X480;
			break;

		case PPI_640X480:
			cap = PPI_CAP_640X480;
			break;

		case PPI_480X800:
			cap = PPI_CAP_480X800;
			break;

		case PPI_800X480:
			cap = PPI_CAP_800X480;
			break;
		case PPI_864X480:
			cap = PPI_CAP_864X480;
			break;

		case PPI_800X600:
			cap = PPI_CAP_800X600;
			break;

		case PPI_1024X600:
			cap = PPI_CAP_1024X600;
			break;

		case PPI_1280X720:
			cap = PPI_CAP_1280X720;
			break;

		case PPI_1600X1200:
			cap = PPI_CAP_1600X1200;
			break;

		case PPI_DEFAULT:
		default:
			break;
	}

	return cap;
}

/**
 * @brief get yuv422 image size
 * @{
 */
static inline uint32_t get_ppi_size(media_ppi_t ppi)
{
	return (ppi >> 16) * (ppi & 0xFFFF) * 2;
}

static inline media_ppi_t get_string_to_ppi(char *string)
{
	uint32_t value = PPI_DEFAULT;

	if (os_strcmp(string, "1280X720") == 0)
	{
		value = PPI_1280X720;
	}

	if (os_strcmp(string, "1024X600") == 0)
	{
		value = PPI_1024X600;
	}

	if (os_strcmp(string, "640X480") == 0)
	{
		value = PPI_640X480;
	}

	if (os_strcmp(string, "480X320") == 0)
	{
		value = PPI_480X320;
	}

	if (os_strcmp(string, "480X272") == 0)
	{
		value = PPI_480X272;
	}

	if (os_strcmp(string, "320X480") == 0)
	{
		value = PPI_320X480;
	}

	if (os_strcmp(string, "320X240") == 0)
	{
		value = PPI_320X240;
	}

	if (os_strcmp(string, "480X800") == 0)
	{
		value = PPI_480X800;
	}

	if (os_strcmp(string, "800X480") == 0)
	{
		value = PPI_800X480;
	}

	if (os_strcmp(string, "480X854") == 0)
	{
		value = PPI_480X854;
	}

	if (os_strcmp(string, "800X600") == 0)
	{
		value = PPI_800X600;
	}

	if (os_strcmp(string, "864X480") == 0)
	{
		value = PPI_864X480;
	}

	if (os_strcmp(string, "480X480") == 0)
	{
		value = PPI_480X480;
	}
	return value;
}

static inline sensor_fps_t get_string_to_fps(char *string)
{
	sensor_fps_t value = FPS0;

	if (os_strcmp(string, "5") == 0)
	{
		value = FPS5;
	}

	if (os_strcmp(string, "10") == 0)
	{
		value = FPS10;
	}

	if (os_strcmp(string, "15") == 0)
	{
		value = FPS15;
	}

	if (os_strcmp(string, "20") == 0)
	{
		value = FPS20;
	}

	if (os_strcmp(string, "25") == 0)
	{
		value = FPS25;
	}

	if (os_strcmp(string, "30") == 0)
	{
		value = FPS30;
	}

	return value;
}

/*
 * @}
 */

#ifdef __cplusplus
}
#endif
