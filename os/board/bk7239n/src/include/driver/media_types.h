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
#include <string.h>
#include <driver/video_common_types.h>
#include <driver/psram_types.h>
#include <driver/hal/hal_h264_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief define all image resolution, unit pixel
 * @{
 */

#define PIXEL_170   (170)
#define PIXEL_240   (240)
#define PIXEL_272   (272)
#define PIXEL_288   (288)
#define PIXEL_320   (320)
#define PIXEL_360   (360)
#define PIXEL_400   (400)
#define PIXEL_412   (412)
#define PIXEL_454   (454)
#define PIXEL_480   (480)
#define PIXEL_576   (576)
#define PIXEL_600   (600)
#define PIXEL_640   (640)
#define PIXEL_720   (720)
#define PIXEL_800   (800)
#define PIXEL_854   (854)
#define PIXEL_864   (864)
#define PIXEL_960   (960)
#define PIXEL_1024 (1024)
#define PIXEL_1080 (1080)
#define PIXEL_1200 (1200)
#define PIXEL_1280 (1280)
#define PIXEL_1296 (1296)
#define PIXEL_1600 (1600)
#define PIXEL_1920 (1920)
#define PIXEL_2304 (2304)
#define PIXEL_4320 (4320)
#define PIXEL_7680 (7680)

/**
 * @brief define camera type
 * @{
 */
typedef enum
{
	UNKNOW_CAMERA,
	DVP_CAMERA,    /**< dvp camera */
	UVC_CAMERA,    /**< uvc camera */
	NET_CAMERA,    /**< net camera */
} camera_type_t;

typedef enum {
	MASTER_TURN_OFF,
	MASTER_TURNING_OFF,
	MASTER_TURNING_ON,
	MASTER_TURN_ON,
} media_state_t;



typedef enum {
    ROTATE_NONE = 0, /**< no rotate */
    ROTATE_90, /**< Image rotaged 90 degress*/
    ROTATE_180, /**< No support yet, reserved for the future*/
    ROTATE_270, /**< Image rotaged 270 degress*/
} media_rotate_t;


typedef enum{
	NONE_ROTATE,
	SW_ROTATE,
	HW_ROTATE,
}media_rotate_mode_t;



typedef enum
{
	NONE_DECODE = 0,
	SOFTWARE_DECODING_MAJOR,
	SOFTWARE_DECODING_MINOR,  //change SOFTWARE_DECODING_MAJOR  and //change SOFTWARE_DECODING_MINOR
	HARDWARE_DECODING,
	JPEGDEC_HW_MODE = HARDWARE_DECODING,
	JPEGDEC_SW_MODE
} media_decode_mode_t;

typedef enum
{
	JPEGDEC_BY_LINE = 0,
	JPEGDEC_BY_FRAME,
} media_decode_type_t;


typedef enum
{
	PPI_DEFAULT     = 0,
	PPI_170X320     = (PIXEL_170 << 16) | PIXEL_320,
	PPI_320X240     = (PIXEL_320 << 16) | PIXEL_240,
	PPI_320X480     = (PIXEL_320 << 16) | PIXEL_480,
	PPI_360X480     = (PIXEL_360 << 16) | PIXEL_480,
	PPI_400X400     = (PIXEL_400 << 16) | PIXEL_400,
	PPI_412X412     = (PIXEL_412 << 16) | PIXEL_412,
	PPI_454X454     = (PIXEL_454 << 16) | PIXEL_454,
	PPI_480X272     = (PIXEL_480 << 16) | PIXEL_272,
	PPI_480X320     = (PIXEL_480 << 16) | PIXEL_320,
	PPI_480X480     = (PIXEL_480 << 16) | PIXEL_480,
	PPI_640X480     = (PIXEL_640 << 16) | PIXEL_480,
	PPI_480X800     = (PIXEL_480 << 16) | PIXEL_800,
	PPI_480X854     = (PIXEL_480 << 16) | PIXEL_854,
	PPI_480X864     = (PIXEL_480 << 16) | PIXEL_864,
	PPI_720X288     = (PIXEL_720 << 16) | PIXEL_288,
	PPI_720X576     = (PIXEL_720 << 16) | PIXEL_576,
	PPI_720X1280    = (PIXEL_720 << 16) | PIXEL_1280,
	PPI_854X480     = (PIXEL_854 << 16) | PIXEL_480,
	PPI_800X480     = (PIXEL_800 << 16) | PIXEL_480,
	PPI_864X480     = (PIXEL_864 << 16) | PIXEL_480,
	PPI_960X480     = (PIXEL_960 << 16) | PIXEL_480,
	PPI_800X600     = (PIXEL_800 << 16) | PIXEL_600,
	PPI_1024X600    = (PIXEL_1024 << 16) | PIXEL_600,
	PPI_1280X720    = (PIXEL_1280 << 16) | PIXEL_720,
	PPI_1600X1200   = (PIXEL_1600 << 16) | PIXEL_1200,
	PPI_1920X1080   = (PIXEL_1920 << 16) | PIXEL_1080,
	PPI_2304X1296   = (PIXEL_2304 << 16) | PIXEL_1296,
	PPI_7680X4320   = (PIXEL_7680 << 16) | PIXEL_4320,
} media_ppi_t;

typedef enum
{
	PPI_CAP_UNKNOW      = 0,
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
	PPI_CAP_170X320     = (1 << 16), /**< 170 * 320 */
} media_ppi_cap_t;

typedef enum {
	PIXEL_FMT_UNKNOW,         /**< unknow image format */
	PIXEL_FMT_JPEG,           /**< image foramt jpeg */
	PIXEL_FMT_H264,
	PIXEL_FMT_H265,
	PIXEL_FMT_YUV444,
	PIXEL_FMT_YUYV,  /**< lcd/jpeg_decode support */
	PIXEL_FMT_VYUY,  /**< jpeg_decode support */
	PIXEL_FMT_UYVY,
	PIXEL_FMT_YYUV,   /**< jpeg_decode support */
	PIXEL_FMT_VUYY,   /**< jpeg_decode support */
	PIXEL_FMT_UVYY,
	PIXEL_FMT_YUV422,
	PIXEL_FMT_I420,
	PIXEL_FMT_YV12,
	PIEXL_FMT_YUV420P,
	PIXEL_FMT_NV12,
	PIXEL_FMT_NV21,
	PIXEL_FMT_YUV420SP,
	PIXEL_FMT_YUV420,
	PIXEL_FMT_RGB444,
	PIXEL_FMT_RGB555,
	PIXEL_FMT_RGB565,     /**< input data format is rgb565(big endian), high pixel is bit[31-16], low pixel is bit[15-0] (PIXEL BIG ENDIAN)*/
	PIXEL_FMT_RGB565_LE,  /**< input data format is rgb565(big endian), high pixel is bit[15-0], low pixel is bit[31-16] (PIXEL little ENDIAN)*/
	PIXEL_FMT_BGR565,
	PIXEL_FMT_RGB666,
	PIXEL_FMT_RGB888,
	PIXEL_FMT_BGR888,
	PIXEL_FMT_ARGB8888,
	PIXEL_FMT_GRAY,
	PIXEL_FMT_RAW,
	PIXEL_FMT_PNG,
} pixel_format_t;

typedef enum
{
	FPS0    = 0,        /**< 0fps */
	FPS5    = (1 << 0), /**< 5fps */
	FPS10   = (1 << 1), /**< 10fps */
	FPS15   = (1 << 2), /**< 15fps */
	FPS20   = (1 << 3), /**< 20fps */
	FPS25   = (1 << 4), /**< 25fps */
	FPS30   = (1 << 5), /**< 30fps */
} frame_fps_t;

typedef enum
{
	EVENT_LCD_ROTATE_MBCMD = 0x18,
	EVENT_LCD_ROTATE_MBRSP = 0x19,

	EVENT_LCD_RESIZE_MBCMD = 0x1a,
	EVENT_LCD_RESIZE_MBRSP = 0x1b,

	EVENT_LCD_DEC_SW_MBCMD = 0x1c,
	EVENT_LCD_DEC_SW_MBRSP = 0x1d,

	EVENT_LCD_DEC_SW_OPEN_MBCMD = 0x1e,
	EVENT_LCD_DEC_SW_OPEN_MBRSP = 0x1f,
} media_mailbox_event_t;

typedef enum
{
	TRS_STATE_DISABLED,
	TRS_STATE_ENABLED,
} media_trs_state_t;

typedef enum
{
	STORAGE_STATE_DISABLED,
	STORAGE_STATE_ENABLED,
} media_storage_state_t;

typedef enum
{
	LCD_STATE_DISABLED,
	LCD_STATE_ENABLED,
	LCD_STATE_DISPLAY,
} media_lcd_state_t;

typedef enum
{
	CAMERA_STATE_DISABLED,
	CAMERA_STATE_ENABLED,
} media_camera_state_t;

typedef enum
{
	AUDIO_STATE_DISABLED,
	AUDIO_STATE_ENABLED,
} media_audio_state_t;

typedef enum {
	CAMERA_MEM_IN_PSRAM = 0,
	CAMERA_MEM_IN_SRAM,
} mem_location_t;

typedef enum
{
	CAM_STREAM_READY = 0,
	CAM_STREAM_IDLE,
	CAM_STREAM_BUSY,
	CAM_STREAM_RESLING,
	CAM_STREAM_ERROR,
} cam_stream_state_t;

typedef struct
{
    uint32_t uvc_dev_id;
	uint8_t *data_buffer;
	uint8_t  num_packets;
	mem_location_t locate;
	uint32_t data_buffer_size;
//  cam_stream_state_t packet_state;
//  cam_stream_state_t *state;
	uint8_t packet_state;
	uint8_t *state;
	uint16_t *num_byte;
	uint16_t *actual_num_byte;
} camera_packet_t;

typedef struct
{
	media_audio_state_t   aud_state;
	media_camera_state_t  cam_state;
	media_lcd_state_t     lcd_state;
	media_storage_state_t stor_state;
	media_trs_state_t     trs_state;
} media_modules_state_t;

typedef struct
{
	uint16_t width;
	uint16_t height;
} frame_resl_t;

typedef struct
{
	frame_resl_t resolution;
	frame_fps_t  fps;
} frame_info_t;

typedef struct
{
	yuv_mode_t     mode;
	pixel_format_t fmt;
	frame_info_t   info;
}media_uvc_device_t;

typedef struct {
	camera_type_t  type;
	yuv_mode_t     mode;
	pixel_format_t fmt;
	frame_info_t   info;

	uint32_t num_uvc_dev;
    uint16_t dualstream;
	yuv_mode_t     d_mode;
	pixel_format_t d_fmt;
	frame_info_t   d_info;
    media_uvc_device_t uvc_device[2];
} media_camera_device_t;


typedef struct frame_buffer_t frame_buffer_t;

typedef struct {
	void (*free)(frame_buffer_t *frame_buffer);
} frame_buffer_callback_t;


/**
 * @brief define frame buffer
 * @{
 */
struct frame_buffer_t
{
	camera_type_t  type;
	pixel_format_t fmt;
	uint8_t mix : 1;
	uint8_t err_state : 1;
	uint8_t crc;
	uint8_t *frame;
	uint8_t *base_addr;
	uint32_t timestamp;
	uint16_t width;
	uint16_t height;
	uint32_t length;
	uint32_t size;
	uint32_t sequence;
	uint32_t h264_type;
	const frame_buffer_callback_t *cb;
};

typedef enum
{
	IMAGE_UNKNOW = 0,
	IMAGE_YUV    = (1 << 0),
	IMAGE_RGB    = (1 << 1),
	IMAGE_MJPEG  = (1 << 2),
	IMAGE_H264   = (1 << 3),
} image_format_t;

/**
 * @brief define camera config
 * @{
 */
typedef struct
{
	uint8_t type;
	uint8_t  fps;
	uint16_t image_format;
	uint16_t pixel_format;
	uint16_t width;
	uint16_t height;
} camera_config_t;


typedef struct
{
	void              (*init)  (mem_location_t locate, uint16_t MaxPacketSize, uint8_t max_packet_cnt, uint8_t cnt, uint32_t idx_uvc);
	void              (*deinit)(void);
	camera_packet_t * (*malloc)(void);
	void              (*push)  (camera_packet_t *packet);
	camera_packet_t * (*pop)   (void);
	void              (*free)  (camera_packet_t *packet);
	camera_packet_t * (*d_malloc)(void);
	void              (*d_push)  (camera_packet_t *packet);
	camera_packet_t * (*d_pop)   (void);
	void              (*d_free)  (camera_packet_t *packet);
} camera_packet_control_t;

typedef struct
{
	int             (*init)  (fb_type_t type);
	int             (*deinit)(fb_type_t type);
	void             (*clear)(fb_type_t type);
	frame_buffer_t * (*malloc)(fb_type_t type, uint32_t size);
	void             (*push)  (frame_buffer_t *fb);
	frame_buffer_t * (*pop)   (void);
	void             (*free)  (frame_buffer_t *fb);
} frame_buffer_control_t;


typedef struct {
	uint32_t device_ppi;			/**< lcd open by ppi */
	char * device_name; 			/**< lcd open by lcd Driver IC name */
} lcd_open_t;


typedef struct {
	media_rotate_mode_t mode;
	media_rotate_t angle;
	pixel_format_t fmt;
    uint8_t scale_en;
} rot_open_t;

typedef struct {
	uint32_t src_ppi;			/**< scale src width and height */
	uint32_t dst_ppi;			/**< dst ppi */
} lcd_scale_t;

typedef struct {
	yuv_mode_t mode;
	h264_qp_t qp;
	uint8_t  enable;
	uint16_t jpeg_up;
	uint16_t jpeg_low;
	uint16_t imb_bits;
	uint16_t pmb_bits;
} compress_ratio_t;

/**
 * @brief define bt audio config
 * @{
 */

typedef struct
{
    int32_t src_rate;
    int32_t src_ch;
    int32_t src_bits;
    int32_t dest_rate;
    int32_t dest_ch;
    int32_t dest_bits;
    int32_t complexity;
    int32_t down_ch_idx;
} bt_audio_resample_init_req_t;


typedef struct
{
    uint8 *in_addr;
    uint32_t *in_bytes_ptr;
    uint8_t *out_addr;
    uint32_t *out_bytes_ptr;
} bt_audio_resample_req_t;

typedef struct
{
    void *handle;
    uint8_t type;
    uint8_t *in_addr;
    uint32_t *out_len_ptr;
} bt_audio_encode_req_t;


/**
 * @brief define struct for debug
 * @{
 */
typedef struct
{
	uint16_t isr_jpeg;    /**< jpeg frame count */
	uint16_t isr_h264;    /**< h264 frame count */
	uint16_t isr_decoder; /**< jpeg decode frame count */
	uint16_t err_dec;     /**< jpeg decode error count */
	uint16_t isr_lcd;     /**< lcd display count */
	uint16_t fps_lcd;     /**< lcd display fps */
	uint16_t lvgl_draw;   /**< lvgl draw frame count */
	uint16_t isr_rotate;  /**< lcd rotate frame count */
	uint16_t err_rot;     /**< lcd rotate error count */
	uint16_t fps_wifi;    /**< wifi transfer fps */
	uint16_t psram_busy;  /**< sram to psram busy cnt */
	uint32_t jpeg_length; /**< jpeg frame length */
	uint32_t h264_length; /**< h264 frame length */
	uint32_t uvc_error;   /**< error number frame of uvc */
    uint32_t uvc_h264_error;
	uint32_t jpeg_kbps;   /**< jpeg data bits stream rate */
	uint32_t h264_kbps;   /**< h264 data bits stream rate */
	uint32_t wifi_kbps;   /**< wifi transfer data bits stream rate */
	uint32_t meantimes;
	uint16_t retry_times;
	uint16_t count;
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
		case PPI_170X320:
			cap = PPI_CAP_170X320;
			break;

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

	if (strcmp(string, "7680X4320") == 0)
	{
		value = PPI_7680X4320;
	}

	if (strcmp(string, "2304X1296") == 0)
	{
		value = PPI_2304X1296;
	}

	if (strcmp(string, "1920X1080") == 0)
	{
		value = PPI_1920X1080;
	}

	if (strcmp(string, "1280X720") == 0)
	{
		value = PPI_1280X720;
	}

	if (strcmp(string, "720X1280") == 0)
	{
		value = PPI_720X1280;
	}

	if (strcmp(string, "1024X600") == 0)
	{
		value = PPI_1024X600;
	}

	if (strcmp(string, "640X480") == 0)
	{
		value = PPI_640X480;
	}

	if (strcmp(string, "480X320") == 0)
	{
		value = PPI_480X320;
	}

	if (strcmp(string, "480X272") == 0)
	{
		value = PPI_480X272;
	}

	if (strcmp(string, "320X480") == 0)
	{
		value = PPI_320X480;
	}

	if (strcmp(string, "320X240") == 0)
	{
		value = PPI_320X240;
	}

	if (strcmp(string, "480X800") == 0)
	{
		value = PPI_480X800;
	}

	if (strcmp(string, "800X480") == 0)
	{
		value = PPI_800X480;
	}

	if (strcmp(string, "480X854") == 0)
	{
		value = PPI_480X854;
	}
	if (strcmp(string, "480X864") == 0)
	{
		value = PPI_480X864;
	}
	if (strcmp(string, "800X600") == 0)
	{
		value = PPI_800X600;
	}

	if (strcmp(string, "864X480") == 0)
	{
		value = PPI_864X480;
	}

	if (strcmp(string, "480X480") == 0)
	{
		value = PPI_480X480;
	}

	if (strcmp(string, "400X400") == 0)
	{
		value = PPI_400X400;
	}

	if (strcmp(string, "412X412") == 0)
	{
		value = PPI_412X412;
	}

	if (strcmp(string, "170X320") == 0)
	{
		value = PPI_170X320;
	}
	if (strcmp(string, "960X480") == 0)
	{
		value = PPI_960X480;
	}

	return value;
}

static inline frame_fps_t get_string_to_fps(char *string)
{
	frame_fps_t value = FPS0;

	if (strcmp(string, "5") == 0)
	{
		value = FPS5;
	}

	if (strcmp(string, "10") == 0)
	{
		value = FPS10;
	}

	if (strcmp(string, "15") == 0)
	{
		value = FPS15;
	}

	if (strcmp(string, "20") == 0)
	{
		value = FPS20;
	}

	if (strcmp(string, "25") == 0)
	{
		value = FPS25;
	}

	if (strcmp(string, "30") == 0)
	{
		value = FPS30;
	}

	return value;
}

static inline uint8_t get_actual_value_of_fps(uint8_t rate, frame_fps_t fps)
{
	if (rate == 30 && fps == FPS30)
	{
		return rate;
	}

	if (rate == 25 && fps == FPS25)
	{
		return rate;
	}

	if (rate == 20 && fps == FPS20)
	{
		return rate;
	}

	if (rate == 15 && fps == FPS15)
	{
		return rate;
	}

	if (rate == 10 && fps == FPS10)
	{
		return rate;
	}

	if (rate == 5 && fps == FPS5)
	{
		return rate;
	}

	return 0xFF;
}

/**
 * @brief init sei nalu delf define, the length equal to 96 bytes
 * @{
 */

static inline void h264_encode_sei_init(uint8_t *sei)
{
	sei[0] = 0x00;
	sei[1] = 0x00;
	sei[2] = 0x00;
	sei[3] = 0x01;
	sei[4] = 0x06;
	sei[5] = 0x05;
	sei[6] = 0x59;//96 - 7 = 89 (16 bytes uuid + 73 bytes pyload)

	for (int i = 0; i < 16; i++)
	{
		// add uuid
		sei[7 + i] = 0xAF;
	}

	// offset = 7 + 16 = 23

	sei[95] = 0x80; // rbsp trailing bits
}

#define DEFAULT_VIDEO_TRAS_CONFIG() {          \
		.send = NULL,          \
		.prepare = NULL,                \
		.get_tx_buf = NULL,                  \
		.get_tx_size = NULL,                  \
		.fmt = PIXEL_FMT_UNKNOW,                             \
	}

#define DEFAULT_CAMERA_CONFIG() {        \
		.type = DVP_CAMERA,     \
		.mode = JPEG_MODE,               \
		.fmt = PIXEL_FMT_JPEG,           \
		.info.resolution.width = 640,    \
		.info.resolution.height = 480,   \
		.info.fps = FPS25,         \
		.num_uvc_dev = 1,                \
		.dualstream = 0,                 \
		.d_mode = H264_MODE,             \
		.d_fmt = PIXEL_FMT_JPEG,         \
        .d_info.resolution.width = 1920, \
        .d_info.resolution.height = 1080,\
        .d_info.fps = FPS25,             \
}


/*
 * @}
 */

#ifdef __cplusplus
}
#endif
