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
#include <driver/psram_types.h>


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
	ID_TVP5150,
	ID_SC101
} sensor_id_t;

/**
 * @brief dvp_camera config
 * @{
 */
typedef struct
{
	media_camera_device_t *device;
	int (*fb_init) (fb_type_t type); /**< init frame buffer */
	int (*fb_deinit) (fb_type_t type); /**< deinit frame buffer */
	void (*fb_clear) (fb_type_t type); /**< clear frame buffer */
	void (*fb_complete) (frame_buffer_t* buffer);  /**< dvp notify upper layer a complete frame ready */
	frame_buffer_t* (*fb_malloc)(fb_type_t type, uint32_t size); /**< malloc jpeg psram buffer */
	void (*fb_free)(frame_buffer_t* buffer);
} dvp_camera_config_t;


/**
 * @brief dvp sensor configure
 * @{
 */
typedef struct
{
	char *name;  /**< sensor name */
	media_ppi_t def_ppi;  /**< sensor default resolution */
	frame_fps_t def_fps;  /**< sensor default fps */
	mclk_freq_t  clk;  /**< sensor work clk in config fps and ppi */
	pixel_format_t fmt; /**< sensor input data format */
	sync_level_t vsync; /**< sensor vsync active level  */
	sync_level_t hsync; /**< sensor hsync active level  */
	uint16_t id;  /**< sensor type, sensor_id_t */
	uint16_t address;  /**< sensor write register address by i2c */
	uint16_t fps_cap;  /**< sensor support fps */
	uint16_t ppi_cap;  /**< sensor support resoultions */
	bool (*detect)(void);  /**< auto detect used dvp sensor */
	int (*init)(void);  /**< init dvp sensor */
	int (*set_ppi)(media_ppi_t ppi);  /**< set resolution of sensor */
	int (*set_fps)(frame_fps_t fps);  /**< set fps of sensor */
	int (*power_down)(void);  /**< power down or reset of sensor */
	int (*dump_register)(media_ppi_t ppi);  /**< dump sensor register */
	void (*read_register)(bool enable);  /**< read sensor register when write*/
} dvp_sensor_config_t;

#define GC_QVGA_USE_SUBSAMPLE          1

/*
 * @}
 */

#ifdef __cplusplus
}
#endif
