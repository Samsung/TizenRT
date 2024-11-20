/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/


#ifndef NDP120_DSP_FW_STATE_H_
#define NDP120_DSP_FW_STATE_H_
#include <syntiant-dsp-firmware/ndp120_dsp_sample.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

#include <syntiant-dsp-firmware/ndp120_dsp_flow.h>
#include <syntiant-dsp-firmware/ndp120_dsp_dnn.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fifo.h>
#include <syntiant-dsp-firmware/ndp120_dsp_memory.h>
#include <syntiant-dsp-firmware/ndp120_dsp_algo_interface.h>
#include <syntiant-dsp-firmware/ndp120_dsp_audio_sync.h>
#include <syntiant-dsp-firmware/ndp_serial.h>
#include <syntiant-dsp-firmware/ndp120_serial.h>
#include <syntiant-dsp-firmware/ndp200_dsp_image_sensor.h>

/* debug enums */
enum {
    NDP120_DEBUG_MESSAGES = 0x1,
    NDP120_DEBUG_DNN_CYCLE_COUNT = 0x2
};

enum {
    NDP120_DSP_ALGO_CONFIG_DEFAULT_SIZE = NDP120_DSP_ALGO_CONFIG_LEN_BYTES,
    NDP120_DSP_ALGO_CONFIG_TOTAL_SIZE   = 1024
};
/*
    Field `preset_id` is for use solely by the host to leave
    a breadcrumb WRT what "preset" config is in place.
    It has no operational use in the DSP code.
 */

typedef struct {
    ndp120_dsp_data_flow_rule_t src_pcm_audio[NDP120_PCM_DATA_FLOW_RULE_MAX];
    ndp120_dsp_data_flow_rule_t src_function[NDP120_FUNC_DATA_FLOW_RULE_MAX];
    ndp120_dsp_data_flow_rule_t src_nn[NDP120_NN_DATA_FLOW_RULE_MAX];
    ndp120_dsp_data_flow_rule_t src_mcu[NDP120_MCU_DATA_FLOW_RULE_MAX];
    ndp120_dsp_data_flow_rule_t src_sensor[NDP120_SENSOR_DATA_FLOW_RULE_MAX];
} ndp120_dsp_data_flow_setup_t;

typedef struct {
    uint32_t frame_cnt;         /* processed frames             */
    uint32_t dnn_int_cnt;       /* dnn interrupts               */
    uint32_t dnn_err_cnt;       /* dnn error counts             */
    uint32_t h2d_mb_cnt;        /* host   -> DSP mb             */
    uint32_t d2m_mb_cnt;        /* DSP    -> MCU mb             */
    uint32_t m2d_mb_cnt;        /* MCU    -> DSP mb             */
    uint32_t watermark_cnt;     /* DSP WM -> host               */
    uint32_t fifo_overflow_cnt; /* FW detected fifo overflow    */
    uint32_t mem_alloc_err_cnt; /* mem alloc error count */
    uint32_t func_debug_cnt; /* Func debug cnt */
    uint32_t pcm_debug_cnt; /* pcm front end */
    uint32_t dnn_run_err_cnt; /* dnn_run encountering error */
    uint32_t nn_cycle_cnt[NDP120_DNN_MAX_NUM_NETWORKS];  /* cycle count per NN */
    uint32_t nn_run_cnt[NDP120_DNN_MAX_NUM_NETWORKS];  /* DNN run count per NN */
} ndp120_dsp_counters_t;

typedef struct {
    int32_t algo_id;            /* -1 means "none" */
    int32_t algo_config_index;  /* -1 means "none" */
    int32_t algo_init_status;
} ndp120_dsp_algo_t;

typedef struct {
    /* size of each sample, in bytes */
    uint32_t aud_samp_size_bytes;
    uint32_t func_samp_size_bytes;
    uint32_t sensor_samp_size_bytes;

    /* # of samples that can be stored */
    uint32_t aud_samp_cap;
    uint32_t func_samp_cap;
    uint32_t sensor_samp_cap;

    uint32_t fifo_threshold_bytes[FIFO_CNT];
    uint32_t notify_on_sample_ready;

    uint32_t sample_rate;

} ndp120_dsp_config_t;

typedef struct {
    uint32_t fifo_start_adx[FIFO_CNT];
    uint32_t fifo_end_adx[FIFO_CNT];
    uint32_t fifo_latest_frame_ptr[FIFO_CNT];
} ndp120_dsp_fifo_info_t ;

typedef struct {

    /* samples in buf */
    uint32_t aud_samp_cnt;

    /* aud_annotation_cnt is same as aud_samp_cnt */
    uint32_t func_samp_cnt;

    uint32_t sensor_samp_cnt;

    /* how many times we've reset the sample buffers */
    uint32_t sample_reset_flag;

    /* ptr to buffers */
    uint32_t aud_samp_buf_ptr;
    uint32_t aud_annotation_buf_ptr;
    uint32_t func_samp_buf_ptr;
    uint32_t sensor_samp_buf_ptr;

    /* prods/cons for each type */
    uint32_t aud_samp_buf_prod;
    uint32_t aud_samp_buf_cons;

    uint32_t aud_annotation_buf_prod;
    uint32_t aud_annotation_buf_cons;

    uint32_t func_samp_buf_prod;
    uint32_t func_samp_buf_cons;

    uint32_t sensor_samp_buf_prod;
    uint32_t sensor_samp_buf_cons;

} ndp120_dsp_buffers_t;

/*
 *
 * BUFFER MACROS
 *
 */

/* chunk # --> adx */
#define _NDP120_DSP_X_CHUNK_TO_ADX(fw_state_ptr, x, chunk_num) ((void*)((fw_state_ptr)->base.buffers.x ## _buf_ptr + chunk_num * (fw_state_ptr)->base.config.x ## _size_bytes))
#define NDP120_DSP_AUD_SAMP_CHUNK_TO_ADX(fw_state_ptr, chunk_num) _NDP120_DSP_X_CHUNK_TO_ADX(fw_state_ptr, aud_samp, chunk_num)
#define NDP120_DSP_FUNC_SAMP_CHUNK_TO_ADX(fw_state_ptr, chunk_num) _NDP120_DSP_X_CHUNK_TO_ADX(fw_state_ptr, func_samp, chunk_num)
#define NDP120_DSP_AUD_ANNOTATION_CHUNK_TO_ADX(fw_state_ptr, chunk_num) ((void*)((fw_state_ptr)->base.buffers.aud_annotation_buf_ptr + chunk_num * sizeof(ndp120_dsp_audio_sample_annotation_t)))


/* adx --> chunk # */
#define _NDP120_DSP_X_ADX_TO_CHUNK(fw_state_ptr, x, adx) (((uint32_t)adx - (fw_state_ptr)->base.buffers.x ## _buf_ptr) / (fw_state_ptr)->base.config.x ## _size_bytes)
#define NDP120_DSP_AUD_SAMP_ADX_TO_CHUNK(fw_state_ptr, adx) _NDP120_DSP_X_ADX_TO_CHUNK(fw_state_ptr, aud_samp, adx)
#define NDP120_DSP_FUNC_SAMP_ADX_TO_CHUNK(fw_state_ptr, adx) _NDP120_DSP_X_ADX_TO_CHUNK(fw_state_ptr, func_samp, adx)
#define NDP120_DSP_AUD_ANNOTATION_ADX_TO_CHUNK(fw_state_ptr, adx) (((uint32_t)adx - (fw_state_ptr)->base.buffers.aud_annotation_buf_ptr) / sizeof(ndp120_dsp_audio_sample_annotation_t))

/* chunk size */
#define _NDP120_DSP_X_SIZE_BYTES(fw_state_ptr, x) ((fw_state_ptr)->base.config.x ## _size_bytes)
#define NDP120_DSP_AUD_SAMP_SIZE_BYTES(fw_state_ptr) _NDP120_DSP_X_SIZE_BYTES(fw_state_ptr, aud_samp)
#define NDP120_DSP_FUNC_SAMP_SIZE_BYTES(fw_state_ptr) _NDP120_DSP_X_SIZE_BYTES(fw_state_ptr, func_samp)
#define NDP120_DSP_AUD_ANNOTATION_SIZE_BYTES(fw_state_ptr) sizeof(ndp120_dsp_audio_sample_annotation_t)

/* get prod or cons */
#define _NDP120_DSP_X_Y(fw_state_ptr, x, y) ((void*)(fw_state_ptr)->base.buffers.x ## _buf_ ## y)
#define NDP120_DSP_AUD_SAMP_CONS(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, aud_samp, cons)
#define NDP120_DSP_AUD_SAMP_PROD(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, aud_samp, prod)
#define NDP120_DSP_FUNC_SAMP_CONS(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, func_samp, cons)
#define NDP120_DSP_FUNC_SAMP_PROD(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, func_samp, prod)
#define NDP120_DSP_AUD_ANNOTATION_CONS(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, aud_annotation, cons)
#define NDP120_DSP_AUD_ANNOTATION_PROD(fw_state_ptr) _NDP120_DSP_X_Y(fw_state_ptr, aud_annotation, prod)

#define NDP_HOST_EXTRACTION_LOCK_SIG 0xACCEED88U

enum {
  NDP120_DSP_FW_ENABLE_SOFTMAX  = 1
};

/* Host facing data struct */
typedef struct ndp120_dsp_fw_base_s {
    uint32_t magic;

    /* Data flow */
    uint32_t data_flow_current_set_id;
    ndp120_dsp_data_flow_setup_t data_flow;

    /* Memory config */
    ndp120_dsp_config_t config;
    ndp120_dsp_buffers_t buffers;
    ndp120_dsp_fifo_info_t fifo_info;

    /* DNN */
    ndp120_dnn_metadata_t metadata;

    struct {
        uint8_t dnn_last_network_completed;
        /* spl */
        uint8_t decibels_measured;
        /* type of heap memory used for sample buffer- DSP or DNN */
        uint8_t sample_buffer_heap_type;
        /* state of the device */
        uint8_t device_state;
    } dsp_state;

    /* Addresses of algo config areas  */
    uint32_t algo_config[NDP120_DSP_ALGO_CONFIG_MAX_COUNT];
    /* Size of each algo config areas */
    uint32_t algo_config_size[NDP120_DSP_ALGO_CONFIG_MAX_COUNT];

    /* largest winstep for fifos */
    uint32_t max_winstep_us;

    /* DNN power configuration area */
    uint32_t dnn_params_power_cfg;
    uint32_t dnn_data_power_cfg;

    /* Debug */
    ndp120_dsp_counters_t counters;
    volatile uint8_t debug_buf[128];
    uint32_t debug_flag;

    /* System clock rate */
    /* Written by ILIB so the DSP can accurately use
       timers */
    uint32_t clock_rate;

    /* How often to send a heartbeat to the MCU.
       Use 0 to disable */
    uint32_t heartbeat_interval_ms;

    /* MCU <--> DSP extended mailbox */
    uint32_t d2m_shared_area[4];

    /* flag to enable/disable various data path functions in DSP firmware */
    uint32_t dsp_ops_flag;

    /* clock divisor value */
    uint32_t mcu_clk_div;

    uint32_t fw_wake_count;

} ndp120_dsp_fw_base_t;

typedef struct {
    uint8_t src_type;
    uint8_t src_param;
    uint8_t _dummy0;
    uint8_t _dummy1;
} ndp120_dsp_fw_aud2_config_t;

typedef struct {
    uint8_t enable_clk_div;
    uint8_t _dummy0;
    uint8_t _dummy1;
    uint8_t _dummy2;
} ndp120_dsp_fw_pwr_mgmt_t;

typedef struct {
    /* Serial */
    ndp_fw_serial_t serial;

    /* Sensor setup */
    ndp_fw_sensor_configuration_t sensor_config[NDP_FW_SENSOR_MAX];

    /* GPIO setup */
    uint8_t gpio_role[NDP120_FW_GPIO_MAX];

    /* sensor state */
    ndp120_sensor_state_t sensor_state[NDP_FW_SENSOR_MAX];
} ndp120_dsp_fw_serial_sensor_mgmt_t;

/* circular buffer data struct for
 * secondary validation */
typedef struct {
#ifdef X86_64
    uint32_t *entry_ptr;
#else
    uint32_t entry_ptr;
#endif
    uint32_t head;
    uint32_t size;
    uint32_t init;
    uint32_t curr_frame_cnt;
} circular_buffer_t;

typedef struct {
    int8_t sensitivity[FIFO_MIC_CNT];
    uint8_t delay[FIFO_MIC_CNT];
} ndp120_dsp_fw_mic_settings_t;

struct ndp120_dsp_audio_params_s {
    uint8_t estimated_noise_level;
    uint8_t db_spl;
    uint8_t misc[2];
};

typedef struct {
    ndp120_dsp_fw_base_t base;
    /* AUD2 */
    ndp120_dsp_fw_aud2_config_t aud2_config;

    /* PWR MGMT */
    ndp120_dsp_fw_pwr_mgmt_t pwr_mgmt;

    /* SYNC config */
    ndp120_dsp_audio_sync_config_t audio_sync_config;

    /* Algorithms and their config */
    ndp120_dsp_algo_t algos[NDP120_DSP_ALGO_MAX_COUNT];

    /* Serial devices */
    ndp120_dsp_fw_serial_sensor_mgmt_t serial_sensor_mgmt;

    /* Image sensor */
    ndp200_dsp_fw_image_sensor_config_t image_sensor_config;


    /* Any struct that needs to be shared with host must appear before
     * DNN state, which contains a pointer and the size of a pointer
     * causes havoc to host side if the host is running in 64bit mode
     */
    /* DNN */
    ndp120_dnn_network_state_t dnn_state[NDP120_DNN_MAX_NUM_NETWORKS];

    /* memories */
    ndp120_dsp_memory_state_t mem_state;

    circular_buffer_t cb[NDP120_DSP_ALGO_CONFIG_MAX_COUNT];

    /* algo config index of each nn */
    int8_t algo_config_index[NDP120_DNN_MAX_NUM_NETWORKS];

    uint8_t padding[2];

    uint32_t device_id;

    ndp120_dsp_fw_mic_settings_t mic_settings;

    uint32_t magic_end;
} ndp120_dsp_fw_state_t;

/* this is an enum so it will be picked up by clang2py */
/* also, it has to be less than 2**31 -1 for ISO C     */

enum {
    NDP120_DSP_FW_STATE_MAGIC = 0xFACADE,
    NDP120_DSP_FW_STATE_MAGIC_END = 0xEDACAF };

#endif
