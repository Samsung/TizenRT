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
/*
 * ILib-internal NDP120 driver-specific definitions
 */
#ifndef SYNTIANT_NDP120_DRIVER_H
#define SYNTIANT_NDP120_DRIVER_H

#include <syntiant_packager/syntiant_package_consts.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant-dsp-firmware/ndp120_dsp_mailbox.h>
#include <syntiant_packager/syntiant_package.h>

enum syntiant_ndp120_device_mailbox_directions_e {
    SYNTIANT_NDP120_DEVICE_MAILBOX_HOST_TO_MCU = 0x0,
    SYNTIANT_NDP120_DEVICE_MAILBOX_MCU_TO_HOST = 0x1,
    SYNTIANT_NDP120_DEVICE_MAILBOX_DIRECTIONS = 0x2
};

typedef enum syntiant_ndp120_bootloader_mode_e {
    SYNTIANT_NDP120_BOOTLOADER_MODE_START = 0,
    SYNTIANT_NDP120_BOOTLOADER_MODE_IN_PROGRESS = 1,
    SYNTIANT_NDP120_BOOTLOADER_MODE_COMPLETE = 2
} syntiant_ndp120_bootloader_mode_t ;

typedef struct {
    uint32_t adx;
    uint32_t bytes_rem_segment;
    uint32_t hdr_idx;
    uint32_t segment_count;
    uint8_t  hdr[TAG_SIZE + LENGTH_SIZE];
} syntiant_ndp120_multisegment_state_t;

typedef struct {
    uint32_t window_lower;
    uint32_t window_upper;
    uint32_t window_idx;
    /* chunk remainder since MCU transfers
       must be multiples of 4 bytes */
    uint32_t remainder_len;
    uint8_t remainder[4];

    syntiant_ndp120_bootloader_mode_t mode;
} syntiant_ndp120_bootloader_state_t;

typedef struct {
    uint8_t mbin_seq;
    uint8_t mbin_resp_seq;
    uint8_t mbout_seq;
    uint8_t mbin_resync;

    int watermarkint_state;
    ndp120_dsp_mailbox_msg_t watermarkint;

    int mbin_sync_count;
    int mbin_data_count;

    uint32_t mbin_data;
    uint32_t watermarkint_data;

    uint8_t mbin_resp;
    uint8_t mbout;
    uint8_t watermarkint_resync;
    uint8_t mbin_state;

} syntiant_ndp120_mb_state_s;

/**
 * @brief NDP120 device-specific interface library internal state object
 */
typedef struct syntiant_ndp120_device_s {
    uint32_t mcu_fw_pointers_addr; /**< 0 means MCU is not running */

    uint32_t mcu_fw_state_addr;
    uint32_t mcu_fw_ph_data_coll_addr;
    uint32_t mcu_fw_smax_smoother_addr;
    uint32_t mcu_fw_orchestrator_graph_addr;
    uint32_t mcu_fw_dbg_state_addr;

    uint32_t dsp_fw_state_addr;
    uint32_t dsp_pcm_audio_sample_last_ptr;
    uint32_t dsp_pcm_audio_annotation_last_ptr;
    uint32_t dsp_function_sample_last_ptr;
    uint32_t dsp_sensor_sample_last_ptr;
    uint32_t dsp_sampletank_mem_type;
    uint32_t extract_percentage;

    unsigned int dnn_input;
    unsigned int input_clock_rate;
    unsigned int core_clock_rate;
    unsigned int audio_frame_size;
    unsigned int audio_frame_step;
    unsigned int audio_sample_size_bytes;
    unsigned int audio_sample_rate;
    unsigned int dnn_frame_size;
    unsigned int image_frame_size;

    uint32_t classes[MAX_NNETWORKS];
    uint32_t fwver_len;
    uint32_t dspfwver_len;
    uint32_t paramver_len;
    uint32_t labels_len;
    uint32_t pkgver_len;
    uint32_t matches;
    uint32_t tankptr_last;
    uint32_t tankptr_match;
    uint32_t algo_match_summary;
    uint32_t num_networks;
    uint32_t last_network_id;
    uint32_t max_feature_len;

    uint32_t match_producer[MAX_NNETWORKS];
    uint32_t match_consumer[MAX_NNETWORKS];
    syntiant_ndp120_mb_state_s mb_state;
    unsigned int chip_is_locked;
    int secure_boot_required;
    unsigned int nn_params_loaded;
    unsigned int nn_metadata_loaded;
    unsigned int match_per_frame_on;
    syntiant_ndp120_bootloader_state_t bl_state;
    syntiant_ndp120_multisegment_state_t ms_state;
    uint8_t init_mode;
    uint8_t sec_val_status;
    uint8_t mcu_fw_loaded;
    uint8_t dsp_fw_loaded;
    unsigned int match_frame_count;
    unsigned int boot_flag;
    unsigned int boot_from_flash;
    unsigned int pdm_block_enabled_while_sensor_set;
    unsigned int core_clock_freq;
} syntiant_ndp120_device_t;

extern struct syntiant_ndp_driver_s syntiant_ndp120_driver;

/**
 * @brief config layout as it is stored in scratch
 */
typedef struct {
    uint32_t checksum; /* must be first */
    uint32_t valid;    /* must be second */

    uint32_t sram_ema;
    uint32_t input_clk;

    uint32_t ext_clk_freq;
    uint32_t pll_clk_freq;
    uint32_t fll_clk_freq;
    uint32_t pll_preset;
    uint32_t fll_preset;
    uint32_t pdm_main_clock_at_last_config;

    uint32_t pdm_sample_rate[2];
    uint32_t pdm_rate[2];
    uint32_t pdm_clk_mode[2];
    uint32_t pdm_mode[2];

    uint32_t label_size;
    uint8_t labels[NN_LABELS_MAX_LENGTH_V2];
    uint32_t fw_version_size;
    uint8_t fw_version[MCU_FW_VERSION_MAX_LENGTH_V2];
    uint32_t pkg_version_size;
    uint8_t pkg_version[PACKAGE_VERSION_MAX_LENGTH_V2];
    uint32_t dsp_fw_version_size;
    uint8_t dsp_fw_version[DSP_FW_VERSION_MAX_LENGTH_V2];
    uint32_t pbi_version_size;
    uint8_t pbi_version[PBI_VERSION_MAX_LENGTH];
} syntiant_ndp120_scratch_t;

SYNTIANT_CASSERT(
    sizeof(syntiant_ndp120_scratch_t) < NDP120_ILIB_SCRATCH_LENGTH,
    "syntiant_ndp120_scratch_t has grown too large")


/**
 * @parse store version information into the scratch area
 *
 * @param[in] ndp   NDP state object
 * @param[in] tag   TLV tag identifying type of version info
 * @param[in] ver   pointer to the version info to be stored
 * @param[in] len   length of version info
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_store_version
(struct syntiant_ndp_device_s *ndp, uint32_t tag, uint8_t *ver, uint32_t len);

/**
 * @parse partially stored parameters such as fw and packed params
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_partially_stored_params
(syntiant_pkg_parser_state_t *pstate, int collect_log);


/**
 * @parse neural network metadata value
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_read_nn_metadata(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse neural network metadata v2 value
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_read_nn_metadata_v2(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse DSP FE config V1 and V2
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_read_dsp_fe_config_v3(
    syntiant_pkg_parser_state_t *pstate);

/**
 * @parse DSP flow rule collections
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int
syntiant_ndp120_pkg_parse_dsp_flow_collection(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse algo attach
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int
syntiant_ndp120_pkg_parse_algo_attach(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse mcu orchestrator nodes params
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_mcu_orchestrator(syntiant_pkg_parser_state_t
                                               *pstate);

/**
 * @parse mcu orchestrator nodes params
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_mcu_orchestrator_v3(syntiant_pkg_parser_state_t
                                               *pstate);

/**
 * @parse pdm configuration params
 * @parse i2s in configuration params
 * @parse i2s out configuration params
 * @parse dsp sync configuration params
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_pdm_cfg_v1(syntiant_pkg_parser_state_t *pstate);
extern int syntiant_ndp120_pkg_parse_pdm_cfg_v2(syntiant_pkg_parser_state_t *pstate);
extern int
syntiant_ndp120_pkg_parse_pin_mux_cfg(syntiant_pkg_parser_state_t *pstate);
extern int
syntiant_ndp120_pkg_parse_i2s_in_cfg(syntiant_pkg_parser_state_t *pstate);
extern int
syntiant_ndp120_pkg_parse_i2s_out_cfg(syntiant_pkg_parser_state_t *pstate);
extern int
syntiant_ndp120_pkg_parse_dsp_sync_cfg(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse dsp sensor configuration params
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int
syntiant_ndp120_pkg_parse_sensor_cfg(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse mcu softmax params
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_softmax(syntiant_pkg_parser_state_t *pstate);

/**
 * @parse dnn power configurations
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_dnn_pwr_config(
    syntiant_pkg_parser_state_t *pstate);

/**
 * @parse interrupt auto clear configurations
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_ndp120_pkg_parse_auto_clear_interrupt(
    syntiant_pkg_parser_state_t *pstate);

extern int syntiant_ndp120_pkg_parse_read_dsp_mcu_secondary_validation_v1(
    syntiant_pkg_parser_state_t *pstate);

extern int syntiant_ndp120_pkg_parse_read_mcu_secondary_validation_v1(
    syntiant_pkg_parser_state_t *pstate);

extern int syntiant_ndp120_pkg_parse_read_mic_settings(
    syntiant_pkg_parser_state_t *pstate);

extern int syntiant_ndp120_pkg_parse_read_algo_params_v1(
    syntiant_pkg_parser_state_t *pstate);

#ifdef __cplusplus
}
#endif

#endif
