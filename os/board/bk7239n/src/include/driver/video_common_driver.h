// Copyright 2023-2024 Beken
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

#include <common/bk_include.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_video_camera_packet_list_init(mem_location_t locate, uint16_t MaxPacketSize, uint8_t max_packet_cnt, uint8_t cnt, uint32_t idx_uvc);

bk_err_t bk_video_camera_packet_list_deinit(void);

bk_err_t bk_video_camera_packet_list_free(void);

camera_packet_t *bk_video_camera_packet_malloc(void);

void bk_video_camera_packet_free(camera_packet_t *camera_packet);

camera_packet_t *bk_video_camera_packet_pop(void);

void bk_video_camera_packet_push(camera_packet_t *camera_packet);

camera_packet_t *bk_video_camera_packet_dual_malloc(void);

camera_packet_t *bk_video_camera_packet_dual_pop(void);

void bk_video_camera_packet_dual_push(camera_packet_t *camera_packet);

void bk_video_camera_packet_dual_free(camera_packet_t *camera_packet);

bk_err_t bk_video_camera_packet_list_dual_free(void);

bk_err_t bk_video_power_on(uint8_t gpio, uint8_t activ_level);

bk_err_t bk_video_power_off(uint8_t gpio, uint8_t activ_level);

bk_err_t bk_video_set_mclk(mclk_freq_t mclk);

bk_err_t bk_video_dvp_mclk_enable(yuv_mode_t mode);

bk_err_t bk_video_dvp_mclk_disable(void);

bk_err_t bk_video_encode_start(yuv_mode_t mode);

bk_err_t bk_video_encode_stop(yuv_mode_t mode);

bk_err_t bk_video_gpio_init(dvp_gpio_mode_t mode);

bk_err_t bk_video_gpio_deinit(dvp_gpio_mode_t mode);

bk_err_t bk_video_yuv_encode_init(media_camera_device_t *device);

bk_err_t bk_video_yuv_encode_deinit(void);

bk_err_t bk_video_yuv_encode_start(frame_buffer_t *src_frame, frame_buffer_t *dst_frame);

uint8_t *dvp_camera_yuv_base_addr_init(frame_resl_t resolution, yuv_mode_t mode);

uint32_t bk_video_identify_h264_nal_fmt(uint8_t *buf, uint32_t size);

bk_err_t bk_video_compression_ratio_config(compress_ratio_t *config);

#ifdef __cplusplus
}
#endif
