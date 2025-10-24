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
#include <driver/media_types.h>
#include <driver/hal/hal_jpeg_types.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief dvp sensor id type
 * @{
 */
typedef enum
{
	ID_UNKNOW = 0,
	ID_PAS6329,
	ID_OV7670,
	ID_PAS6375,
	ID_GC0328C,
	ID_BF2013,
	ID_GC0308C,
	ID_HM1055,
	ID_GC2145,
	ID_OV2640,
	ID_GC0308,
	ID_TVP5150
} sensor_id_t;

/**
 * @brief dvp_camera config
 * @{
 */
typedef struct
{
	media_ppi_t ppi;  /**< sensor image resolution */
	media_camera_type_t type;  /**< sensor output mode yuv/jpeg */
	int (*fb_jpeg_init) (media_ppi_t max_ppi); /**< init jpeg psram buffer */
	int (*fb_jpeg_deinit) (void); /**< deinit jpeg psram buffer */
	int (*fb_display_init) (media_ppi_t max_ppi); /**< init yuv psram buffer */
	int (*fb_display_deinit) (void); /**< deinit yuv psram buffer */
	void (*fb_jpeg_complete) (frame_buffer_t* buffer);  /**< dvp notify upper layer a complete frame ready */
	void (*fb_display_complete) (frame_buffer_t* buffer);
	frame_buffer_t* (*fb_jpeg_malloc)(void); /**< malloc jpeg psram buffer */
	void (*fb_jpeg_free)(frame_buffer_t* buffer);
	void (*fb_display_free)(frame_buffer_t* buffer);
	frame_buffer_t* (*fb_display_malloc)(void); /**< malloc yuv psram buffer */
	int (*fb_h264_init) (media_ppi_t max_ppi); /**< init h264 psram buffer */
	int (*fb_h264_deinit) (void); /**< deinit h264 psram buffer */
	void (*fb_h264_complete) (frame_buffer_t* buffer);  /**< dvp notify upper layer a complete frame ready */
	frame_buffer_t* (*fb_h264_malloc)(void); /**< malloc h264 psram buffer */
	void (*fb_h264_free)(frame_buffer_t* buffer);
	void (*drop_frame)(uint8_t index);  /**< jpeg module drop dvp source frame according to index */
} dvp_camera_config_t;


/**
 * @brief dvp sensor configure
 * @{
 */
typedef struct
{
	char *name;  /**< sensor name */
	media_ppi_t def_ppi;  /**< sensor default resolution */
	sensor_fps_t def_fps;  /**< sensor default fps */
	jpeg_clk_t  clk;  /**< sensor work clk in config fps and ppi */
	pixel_format_t fmt; /**< sensor input data format */
	jpeg_sync_level_t vsync; /**< sensor vsync active level  */
	jpeg_sync_level_t hsync; /**< sensor hsync active level  */
	uint16_t id;  /**< sensor type, sensor_id_t */
	uint16_t address;  /**< sensor write register address by i2c */
	uint16_t fps_cap;  /**< sensor support fps */
	uint16_t ppi_cap;  /**< sensor support resoultions */
	bool (*detect)(void);  /**< auto detect used dvp sensor */
	int (*init)(void);  /**< init dvp sensor */
	int (*set_ppi)(media_ppi_t ppi);  /**< set resolution of sensor */
	int (*set_fps)(sensor_fps_t fps);  /**< set fps of sensor */
	int (*power_down)(void);  /**< power down or reset of sensor */
	int (*dump_register)(media_ppi_t ppi);  /**< dump sensor register */
	void (*read_register)(bool enable);  /**< read sensor register when write*/
} dvp_sensor_config_t;

#define GC_QVGA_USE_SUBSAMPLE          1

/**
 * @brief current used camera config
 * @{
 */
typedef struct
{
	media_ppi_t ppi;     /**< sensor image resolution */
	sensor_fps_t fps;    /**< sensor fps */
	char *name;          /**< sensor name */
	uint16_t id;         /**< sensor type, sensor_id_t */
	uint16_t fps_cap;    /**< sensor support fps */
	uint16_t ppi_cap;    /**< sensor support resoultions */
	uint32_t pixel_size; /**< sensor output image width */
	media_camera_type_t type;     /**< dvp camera work type */
} dvp_camera_device_t;


/*
 * @}
 */

#ifdef __cplusplus
}
#endif
