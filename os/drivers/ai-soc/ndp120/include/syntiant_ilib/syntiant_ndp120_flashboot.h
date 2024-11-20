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
#ifndef SYNTIANT_NDP120_DEBUG_H
#define SYNTIANT_NDP120_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

enum __dsp_config_array_dimensions__ {
    NDP120_AUD_BLOCKS = 3,
    NDP120_PDM_CHANNELS = 2,
    NDP120_PDM_BLOCKS = 2
};

struct ndp_aud {
    uint32_t inf;
    uint32_t sample_rate;
    uint32_t pdm_rate;
    uint32_t clk_mode;
    uint32_t mode;
};

struct ndp_farrow {
    uint32_t bypass;
    uint32_t phase_step;
};

struct ndp_misc_cfg {
    uint32_t mic;
    uint32_t numdelay[NDP120_PDM_CHANNELS];
    uint32_t agcshiftdir[NDP120_PDM_CHANNELS];
    uint32_t agcshiftcnt[NDP120_PDM_CHANNELS];
    uint32_t agcfinegrainmul[NDP120_PDM_CHANNELS];
    uint32_t zcgainchange[NDP120_PDM_CHANNELS];
};

struct ndp_pdm_cfg {
    uint32_t mic;
    uint32_t dec;
    uint32_t dcremovalmode[NDP120_PDM_CHANNELS];
    uint32_t inshift[NDP120_PDM_CHANNELS];
    uint32_t outshift[NDP120_PDM_CHANNELS];
};

struct ndp_bwcoef_cfg {
    uint32_t a1[NDP120_PDM_CHANNELS];
    uint32_t a2[NDP120_PDM_CHANNELS];
    uint32_t b0[NDP120_PDM_CHANNELS];
    uint32_t b1[NDP120_PDM_CHANNELS];
    uint32_t b2[NDP120_PDM_CHANNELS];
};

struct ndp_dsp_config {
    uint32_t signature;
    uint32_t main_clk;
    uint8_t inf_set[NDP120_AUD_BLOCKS];
    uint8_t rsvd;
    struct ndp_aud aud[NDP120_AUD_BLOCKS];
    struct ndp_farrow farrow[NDP120_AUD_BLOCKS];
    struct ndp_misc_cfg gain[NDP120_AUD_BLOCKS];
    struct ndp_bwcoef_cfg bwcoef[NDP120_AUD_BLOCKS];
    struct ndp_pdm_cfg pdmcfg_b[NDP120_PDM_BLOCKS];
};

struct ndp_scratch_dev {
    struct {
        uint32_t fw_ver_len;
        uint32_t dsp_fw_ver_len;
        uint32_t pkg_ver_len;
        uint32_t labels_len;
        uint32_t pbi_ver_len;
    } len;
    struct {
        char fw_ver[NDP120_MCU_FW_VER_MAX_LEN];
        char dsp_ver[NDP120_MCU_DSP_FW_VER_MAX_LEN];
        char pkg_ver[NDP120_MCU_PKG_VER_MAX_LEN];
        char labels[NDP120_MCU_LABELS_MAX_LEN];
        char pbi_ver[NDP120_MCU_PBI_VER_MAX_LEN];
    } str;
    struct ndp_dsp_config dsp_cfg;
};
/* flashboot: need the following for getting scratch dev config */

#define NDP120_MCU_SCRATCH_DEV_SIG      (0x600DDEED)

/* flashboot: end */

#ifdef __cplusplus
}
#endif
#endif /* SYNTIANT_NDP120_DEBUG_H */

