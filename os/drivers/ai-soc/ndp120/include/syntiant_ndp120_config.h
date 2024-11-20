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

#ifndef SYNTIANT_NDP120_CONFIG_H
#define SYNTIANT_NDP120_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_ndp_driver.h>

enum ndp_device_type_e {
    SYNTIANT_NDP115A0,
    SYNTIANT_NDP120B0,
    SYNTIANT_NDP200A0,
    SYNTIANT_UNKNOWN_DEV
};

int syntiant_ndp120_do_mailbox_req_no_sync(
    struct syntiant_ndp_device_s *ndp, uint8_t req, uint32_t *resp);

int syntiant_ndp120_config_pdm_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_pdm_t *config);

int syntiant_ndp120_config_i2s_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2s_t *config);

int syntiant_ndp120_config_mic_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_mic_t *config);

int syntiant_ndp120_config_decimation_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_decimation_t *config);

int syntiant_ndp120_config_gain_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_gain_t *config);

int syntiant_ndp120_config_farrow_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_farrow_t *config);

int syntiant_ndp120_config_bwcoef_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_bwcoef_t *config);

int syntiant_ndp120_get_main_clk_freq_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq);

int syntiant_ndp120_config_nn_metadata_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_nn_metadata_t * config);

int syntiant_ndp120_config_mcu_orchestrator_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_mcu_orch_t * config);

int syntiant_ndp120_dsp_flow_setup_apply_no_sync(
    struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup);

int syntiant_ndp120_dsp_flow_setup_get_no_sync(
    struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup);

int syntiant_ndp120_posterior_config_no_sync(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_posterior_config_s *config);

int syntiant_ndp120_config_misc_no_sync(struct syntiant_ndp_device_s *ndp,
            syntiant_ndp120_config_misc_t *config);

int syntiant_ndp120_init_ring_buffer_pointers_no_sync(
    struct syntiant_ndp_device_s *ndp, int reset);

int syntiant_ndp120_config_notify_on_sample_ready_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t enable);

int syntiant_ndp120_get_pdm_enable_status(struct syntiant_ndp_device_s *ndp,
                                          uint32_t *pdm_enable);

uint32_t ndp_get_scratch_address(struct syntiant_ndp_device_s *ndp);

int syntiant_ndp120_scratch_get_valid(struct syntiant_ndp_device_s *ndp, uint32_t *valid);
int syntiant_ndp120_scratch_get_valid_skip_crc(struct syntiant_ndp_device_s *ndp, uint32_t *valid);
int syntiant_ndp120_scratch_set_valid(struct syntiant_ndp_device_s *ndp, uint32_t valid);

/**
 * @brief NDP120 MPF function
 *
 * Used to turn on or off the match per frame interrupt
 *
 * @param ndp NDP state object
 * @param mpf indicates 0 for off and 1 for on
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_mpf(struct syntiant_ndp_device_s *ndp, int mpf);

/*TODO Remove all these macros */
#define SCRATCH_VARIABLE_ADX(x) (uint32_t)(ndp_get_scratch_address(ndp) + \
    (uint32_t) offsetof(syntiant_ndp120_scratch_t, x))

#define SCRATCH_VALID_ADX (SCRATCH_VARIABLE_ADX(valid))
#define SCRATCH_CHECKSUM_ADX (SCRATCH_VARIABLE_ADX(checksum))
#define SCRATCH_PDM_SAMPLE_RATE_ADX(x) (uint32_t) (SCRATCH_VARIABLE_ADX(pdm_sample_rate) + x * sizeof(uint32_t))
#define SCRATCH_PDM_RATE_ADX(x) (uint32_t) (SCRATCH_VARIABLE_ADX(pdm_rate) + x * sizeof(uint32_t))
#define SCRATCH_PDM_CLK_MODE_ADX(x) (uint32_t) (SCRATCH_VARIABLE_ADX(pdm_clk_mode) + x * sizeof(uint32_t))
#define SCRATCH_PDM_MODE_ADX(x) (uint32_t) (SCRATCH_VARIABLE_ADX(pdm_mode) + x * sizeof(uint32_t))
#define SCRATCH_PDM_MAIN_CLK_AT_LAST_CONFIG_ADX (SCRATCH_VARIABLE_ADX(pdm_main_clock_at_last_config))
#define SCRATCH_EXT_CLK_FREQ_ADX (SCRATCH_VARIABLE_ADX(ext_clk_freq))
#define SCRATCH_PLL_CLK_FREQ_ADX (SCRATCH_VARIABLE_ADX(pll_clk_freq))
#define SCRATCH_FLL_CLK_FREQ_ADX (SCRATCH_VARIABLE_ADX(fll_clk_freq))
#define SCRATCH_PLL_PRESET (SCRATCH_VARIABLE_ADX(pll_preset))
#define SCRATCH_FLL_PRESET (SCRATCH_VARIABLE_ADX(fll_preset))
#define SCRATCH_CRC_REGION_BEGIN_ADX SCRATCH_VALID_ADX
#define SCRATCH_CRC_REGION_LENGTH (sizeof(syntiant_ndp120_scratch_t) - (uint32_t) offsetof(syntiant_ndp120_scratch_t, valid))

#ifdef __cplusplus
}
#endif

#endif

