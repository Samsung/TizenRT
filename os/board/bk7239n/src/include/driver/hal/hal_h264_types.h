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

#include <driver/dma_types.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define QCIF_WIDTH              176
// #define QCIF_HEIGHT             144
// #define QCIF_LINES             	(QCIF_HEIGHT / 16) - 1

// #define CIF_WIDTH               352
// #define CIF_HEIGHT              288
// #define CIF_LINES               (CIF_HEIGHT / 16) - 1

// #define LCD_ST7282_WIDTH		480
// #define LCD_ST7282_HEIGHT		272
// #define LCD_ST7282_LINES		(LCD_ST7282_HEIGHT / 16) - 1

// #define VGA_WIDTH               640
// #define VGA_HEIGHT              480
// #define VGA_LINES               (VGA_HEIGHT / 16) - 1

// #define FCIF_WIDTH              704
// #define FCIF_HEIGHT             576
// #define FCIF_LINES              (FCIF_HEIGHT / 16) - 1

// #define HDTV_720P_WIDTH         1280
// #define HDTV_720P_HEIGHT        720
// #define HDTV_720P_LINES         (HDTV_720P_HEIGHT / 16) - 1

// #define HDTV_1080P_WIDTH        1920
// #define HDTV_1080P_HEIGHT       1080
// #define HDTV_1080P_LINES        (HDTV_1080P_HEIGHT / 16) - 1

#define PBITS_FACT1             16
#define PBITS_FACT2             22
#define PBITS_FACT3             27
#define PBITS_FACT4             28
#define PBITS_FACT5             29

typedef uint8_t h264_unit_t;
typedef void (*h264_isr_t)(h264_unit_t id, void *param);

// typedef enum {
//     QCIF = 0,
//     CIF,
// 	,
//     VGA,
//     FCIF,
//     HDTV_720P,
//     HDTV_1080P,
//     H264_RESOLUTION_MAX
// } h264_resolution_group;

typedef struct {
    uint32_t num_pframe;
    uint32_t skip_mode;

    uint32_t qp;
    uint32_t cqp_off;
    uint32_t tqp;

    uint32_t iframe_igain;
    uint32_t iframe_pgain;
    uint32_t iframe_dgain;
    uint32_t iframe_min_qp;
    uint32_t iframe_max_qp;

    uint32_t pframe_igain;
    uint32_t pframe_pgain;
    uint32_t pframe_dgain;
    uint32_t pframe_min_qp;
    uint32_t pframe_max_qp;

    uint32_t profile_idc;
    uint32_t level_idc;
    uint32_t filter_mode;
    uint32_t imb_bits;
    uint32_t pmb_bits;
    uint32_t nal_align_mode;
    uint32_t cons_ipred_mode;
    uint32_t reset_i_state_mode;

    uint32_t vui_time_scale_L;
    uint32_t vui_time_scale_H;
    uint32_t vui_num_u_tick_L;
    uint32_t vui_num_u_tick_H;
    uint32_t bitrate_scale;
    uint32_t cpb_size_scale;
    uint32_t bitrate_value_less1;
    uint32_t cpb_size_value_less1;

    uint32_t init_cpb_removal_delay;
    uint32_t init_enc_buffer_level_L;
    uint32_t init_enc_buffer_level_H;
    uint32_t bits_per_frame_int_L;
    uint32_t bits_per_frame_int_H;
    uint32_t bits_per_frame_rem_L;
    uint32_t bits_per_frame_rem_H;
    uint32_t cpb_size_L;
    uint32_t cpb_size_H;
} h264_config_t;

typedef struct
{
	uint8_t init_qp;
	uint8_t i_min_qp;
	uint8_t i_max_qp;
	uint8_t p_min_qp;
	uint8_t p_max_qp;
} h264_qp_t;

typedef struct
{
    uint8_t *rx_buf;
    uint8_t dma_channel;
    uint32_t rx_buf_len;
    uint32_t transfer_len;
    dma_isr_t dma_rx_finish_handler;
    dma_isr_t dma_rx_half_handler;
} h264_dma_config_t;

#ifdef __cplusplus
}
#endif