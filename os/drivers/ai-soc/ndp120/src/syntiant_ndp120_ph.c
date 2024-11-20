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
#ifndef __LINUX_KERNEL__
#include <stdint.h>
#else
#include <linux/types.h>
#endif

#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/ndp120_regs.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp120_ph.h>
#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant-firmware/ndp120_ph.h>
#include <syntiant_ilib/syntiant_ndp120_mailbox.h>

#include <syntiant-firmware/ph/scp_ph.h>
#include <syntiant-firmware/ph/mcp_ph.h>

#include "syntiant_ndp120_config.h"

#define  NDP120_FW_STATE_DISABLE_POSTERIOR 0x4
#define  NDP120_FW_STATE_DISABLE_SMAX_SMOOTHER 0x8

enum syntiant_ndp120_posterior_constants_e {
    POSTERIOR_ENABLE_BITS =
        NDP120_FW_STATE_ENABLE_SMAX_SMOOTHER |
        NDP120_FW_STATE_ENABLE_POSTERIOR
};

static uint32_t ph_data_size_array[MAX_PH_ALGOS] =
    {sizeof(struct scp_ph_data_s), 0};
static uint32_t ph_data_init[MAX_NNETWORKS] = {0};

static int syntiant_ndp120_posterior_config_encode_action(
    unsigned int ph_type, unsigned int atype,
    unsigned int arg0, unsigned int arg1, uint32_t *action)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (ph_type == 0){
        switch (atype) {
        case SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_MATCH:
            if (63 < arg0) {
                s = SYNTIANT_NDP_ERROR_ARG;
                goto error;
            }
            *action = NDP120_PH_ACTION_SCP_MATCH_M | arg0;
            break;
        case SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STATE:
            *action = arg0;
            break;
        case SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STAY:
            *action = NDP120_PH_ACTION_SCP_STAY;
            break;
        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    } else {
        /* Corrected to match the decode function */
        *action = atype & PH_ACTION_MASK;
        *action |= (arg0 << PH_ACTION_ARG0_SHIFT) & PH_ACTION_ARG0_MASK;
        *action |= (arg1 << PH_ACTION_ARG1_SHIFT) & PH_ACTION_ARG1_MASK;
    }
#if SYNTIANT_NDP120_DEBUG_PH
    DEBUG_PRINTF(
        "+++ ENCODE(ph_type=%u): atype=%x arg0=%x arg1=%x -> action=%#x\n",
        ph_type, atype, arg0, arg1, (unsigned)*action);
#endif
error:
    return s;
}

static void syntiant_ndp120_posterior_config_decode_action(
    uint32_t action, unsigned int ph_type, unsigned int *atype,
    unsigned int *arg0, unsigned int *arg1)
{
    if (ph_type == 0) {
        if (action & NDP120_PH_ACTION_SCP_MATCH_M) {
            *atype = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_MATCH;
            *arg0 = action & ~((unsigned int) NDP120_PH_ACTION_SCP_MATCH_M);
        } else if (action == NDP120_PH_ACTION_SCP_STAY) {
            *atype = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STAY;
        } else {
            *atype = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STATE;
            *arg0 = action;
        }
    } else {
        *atype = action & PH_ACTION_MASK;
        *arg0 = (action & PH_ACTION_ARG0_MASK) >> PH_ACTION_ARG0_SHIFT;
        *arg1 = (action & PH_ACTION_ARG1_MASK) >> PH_ACTION_ARG1_SHIFT;
    }
#if SYNTIANT_NDP120_DEBUG_PH
    DEBUG_PRINTF(
        "+++ DECODE(ph_type=%u): atype=%x arg0=%x arg1=%x <- action=%#x\n",
        ph_type, *atype, *arg0, *arg1, (unsigned)action);
#endif
}

static int syntiant_ndp120_posterior_init(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_posterior_config_s *config,
    const struct ndp120_ph_params_s *pparams)
{
    struct ndp120_ph_state_params_s sparams;
    struct ndp120_ph_class_params_s cparams;
    uint32_t class_base_for_state_offset;
    uint32_t ph_idx_addr;
    uint32_t ph_params_addr = 0, params_base_addr = 0, params_base_addr2 = 0;
    uint32_t ph_data_addr = 0, ph_data_size = 0;
    uint32_t saddr, caddr;
    uint32_t ph_idx = 0;
    unsigned int i, j;
    int s;
    uint32_t offset = 0x0;

    /* write ph index to the scratch area instead of main fw state */
    ph_idx_addr = ndp_get_scratch_address(ndp) + NDP120_ILIB_SCRATCH_LENGTH - 4;
    ph_idx = config->ph_idx;
    s = syntiant_ndp120_write(ndp, 1, ph_idx_addr, ph_idx);
    if (s) goto error;

    ph_data_addr = (uint32_t)ndp->d.ndp120.mcu_fw_ph_data_coll_addr;
    ph_data_addr += config->ph_type * (uint32_t)sizeof(uint32_t);

    s = syntiant_ndp120_read(ndp, 1, ph_data_addr, &ph_params_addr);
    if (s) goto error;

    ph_data_size = ph_data_size_array[config->ph_type];

    if (!ph_params_addr) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }

    ph_params_addr += config->ph_idx * ph_data_size;

    offset = offsetof(struct ndp120_ph_params_s, num_classes);
    s = syntiant_ndp120_write_block(
        ndp, 1, ph_params_addr + offset, &pparams->num_classes,
        offsetof(struct ndp120_ph_params_s, adaptive_frames) -
            offsetof(struct ndp120_ph_params_s, num_classes));
    if (s) goto error;

    /* send ph heap allocation mailbox event to mcu */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_PH_HEAP_ALLOCATION, NULL);
    if (s) goto error;

    sparams.timeout = 0;
    sparams.timeout_action = 0;

    cparams.window = 0;
    cparams.threshold = 65535;
    cparams.backoff = 0;
    cparams.smoothing_queue_size = 1;

    offset = offsetof(struct ndp120_ph_params_s, params_memory);
    params_base_addr = ph_params_addr + offset;

    /* get the pseudo-heap allocated address (params_base_addr2) */
    s = syntiant_ndp120_read_block(ndp, 1, params_base_addr, &params_base_addr2, sizeof(uint32_t));
    if (s) goto error;

    class_base_for_state_offset
        = pparams->num_states
        * ((unsigned int) sizeof(struct ndp120_ph_state_params_s))
        / ((unsigned int) sizeof(uint32_t));

    for (i = 0; i < pparams->num_states; i++) {

        class_base_for_state_offset
            += i * (pparams->num_classes * ((unsigned int) sizeof(cparams))
                    / ((unsigned int) sizeof(uint32_t)));
        sparams.class_params_offset = class_base_for_state_offset;
        saddr = params_base_addr2 + i * ((unsigned int) sizeof(sparams));

        s = syntiant_ndp120_write_block(
            ndp, 1, saddr, &sparams, sizeof(sparams));
        if (s) goto error;

        for (j = 0; j < pparams->num_classes; j++) {
            cparams.action = NDP120_PH_ACTION_SCP_MATCH_M | j;
            caddr = params_base_addr2 + class_base_for_state_offset
                * ((unsigned int) sizeof(uint32_t))
                + j * ((unsigned int) sizeof(cparams));
            s = syntiant_ndp120_write_block(
                ndp, 1, caddr, &cparams, sizeof(cparams));
            if (s) goto error;
        }
    }

    offset = offsetof(struct ndp120_ph_params_s, num_classes);
    s = syntiant_ndp120_write_block(
        ndp, 1, ph_params_addr + offset, &pparams->num_classes,
        offsetof(struct ndp120_ph_params_s, adaptive_frames) -
            offsetof(struct ndp120_ph_params_s, num_classes));

    ph_data_init[config->ph_idx] = 1;
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_posterior_config_no_sync(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_posterior_config_s *config)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t fw_state_addr = ndp120->mcu_fw_state_addr;
    uint32_t ph_params_addr = 0, ph_data_addr = 0, ph_data_size = 0;
    uint32_t state_addr, state_addr2, state_addr2_orig, params_addr;
    uint32_t offset = 0x0, dsp_offset = 0, dsp_flag = 0xFFFFFFFF;
    uint32_t dsp_dev_flag;
    uint32_t fw_state_enable;
    uint32_t ph_num, ph_type;
    struct ndp120_ph_params_s pparams;
    struct ndp120_ph_state_params_s sparams;
    struct ndp120_ph_class_params_s cparams;
    int s;

    if (config->set
        & ((unsigned int)~SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ALL_M)) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if ((config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE)
        && (config->set
            & ((unsigned int)~SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE))) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if ((config->set & (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES
                        | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES
                        | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE))
        && (config->set
            & ~((unsigned int)
                (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES
                 | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES
                 | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE)))) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if ((config->set & (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT
                        | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION))
        && (config->set
            & ~((unsigned int)
                (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT
                 | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION)))) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    offset = offsetof(struct ndp120_fw_state_s, enable);
    s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset, &fw_state_enable);
    if (s) goto error;


    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE) {
        if (config->enable & NDP120_FW_STATE_ENABLE_SMAX_SMOOTHER) {
            fw_state_enable |= NDP120_FW_STATE_ENABLE_SMAX_SMOOTHER;
            dsp_flag = NDP120_DSP_FW_ENABLE_SOFTMAX;
        } else if (config->enable & NDP120_FW_STATE_DISABLE_SMAX_SMOOTHER) {
            fw_state_enable &= ~((unsigned int)NDP120_FW_STATE_ENABLE_SMAX_SMOOTHER);
            dsp_flag = 0;
        }

        if (config->enable & NDP120_FW_STATE_ENABLE_POSTERIOR) {
            fw_state_enable |= NDP120_FW_STATE_ENABLE_POSTERIOR;
        } else if (config->enable & NDP120_FW_STATE_DISABLE_POSTERIOR) {
            fw_state_enable &= ~((unsigned int)NDP120_FW_STATE_ENABLE_POSTERIOR);
        }

        if (!config->enable) {
            fw_state_enable &= ~((unsigned int)POSTERIOR_ENABLE_BITS);
        }

        s = syntiant_ndp120_write(ndp, 1, fw_state_addr + offset, fw_state_enable);
        if (s) goto error;

        if (dsp_flag != 0xFFFFFFFF) {
            if (!ndp120->dsp_fw_state_addr) {
                s = SYNTIANT_NDP_ERROR_UNINIT;
                goto error;
            }
            /* update softmax enable field in DSP ops flag*/
            dsp_offset = offsetof(ndp120_dsp_fw_base_t, dsp_ops_flag);
            s = syntiant_ndp120_read(ndp, 1, ndp120->dsp_fw_state_addr +
                dsp_offset, &dsp_dev_flag);
            if (s) goto error;
            dsp_dev_flag = (dsp_dev_flag & (uint32_t) ~NDP120_DSP_FW_ENABLE_SOFTMAX) |
                dsp_flag;
            s = syntiant_ndp120_write(ndp, 1, ndp120->dsp_fw_state_addr +
                dsp_offset, dsp_dev_flag);
            if (s) goto error;
        }

        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_NOP, NULL);
        if (s) goto error;

        goto got_enable;
    }

    if (config->enable &&
          (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_NUM_PH)) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    offset = offsetof(struct ndp120_fw_state_s, ph_num);
    s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset, &ph_num);
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_NUM_PH) {
        ph_num = config->ph_num;
        s = syntiant_ndp120_write(ndp, 1, fw_state_addr + offset, ph_num);

        /* do not check --> fallthru */
        goto error;
    }

    s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset, &ph_num);
    if (s) goto error;

    config->ph_num = ph_num;

    if (config->ph_idx >= ph_num){
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE) {
        offset = offsetof(struct ndp120_fw_state_s, ph_types);
        s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset + (uint32_t)sizeof(uint32_t) * config->ph_idx, &ph_type);
        if (s) goto error;

        ph_type = config->ph_type;
        s = syntiant_ndp120_write(ndp, 1, fw_state_addr + offset + (uint32_t)sizeof(uint32_t) * config->ph_idx, ph_type);
        if (s) goto error;

        s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset + (uint32_t)sizeof(uint32_t) * config->ph_idx, &ph_type);
        if (s) goto error;
    }

    if (ph_data_init[config->ph_idx]) {
        offset = offsetof(struct ndp120_fw_state_s, ph_types);
        s = syntiant_ndp120_read(ndp, 1, fw_state_addr + offset + (uint32_t)sizeof(uint32_t) * config->ph_idx, &ph_type);
        if (s) goto error;
    } else {
        ph_type = config->ph_type;
    }

    ph_data_addr = (uint32_t)ndp->d.ndp120.mcu_fw_ph_data_coll_addr;
    ph_data_addr += ph_type * (uint32_t)sizeof(uint32_t);

    s = syntiant_ndp120_read(ndp, 1, ph_data_addr, &ph_params_addr);
    if (s) goto error;

    if (!ph_params_addr) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }

    ph_data_size = ph_data_size_array[ph_type];
    ph_params_addr += config->ph_idx * ph_data_size;

    offset = offsetof(struct ndp120_ph_params_s, num_classes);
    s = syntiant_ndp120_read_block(ndp, 1, ph_params_addr + offset, &pparams.num_classes,
        offsetof(struct ndp120_ph_params_s, adaptive_frames) -
            offsetof(struct ndp120_ph_params_s, num_classes));
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES) {
        pparams.num_states = config->states;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES) {
        pparams.num_classes = config->classes;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE) {
        pparams.ph_type = config->ph_type;
    }

    if (config->set
        & (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES
            | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES
            | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE)) {
        s = syntiant_ndp120_posterior_init(ndp, config, &pparams);
        if (s) goto error;

        s = syntiant_ndp120_read_block(
            ndp, 1, ph_params_addr + offset, &pparams.num_classes,
            offsetof(struct ndp120_ph_params_s, adaptive_frames) -
                offsetof(struct ndp120_ph_params_s, num_classes));
        if (s) goto error;

        config->states = pparams.num_states;
        config->classes = pparams.num_classes;
        config->ph_type = pparams.ph_type;

        goto got_structure;
    }

    offset = offsetof(struct ndp120_ph_params_s, adaptive_frames);
    s = syntiant_ndp120_read_block(
        ndp, 1, ph_params_addr + offset, &pparams.adaptive_frames,
        offsetof(struct ndp120_ph_params_s, params_memory) -
            offsetof(struct ndp120_ph_params_s, adaptive_frames));
    if (s) goto error;

    if (config->set& SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_FRAMES) {
        pparams.adaptive_frames = config->adaptive_frames;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_DENOMIATOR) {
        pparams.adaptive_denominator = config->adaptive_denominator;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_MAX_UPDATES) {
        pparams.adaptive_max_updates = config->adaptive_max_updates;
    }

    if (config->set
        & (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_FRAMES
            | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_DENOMIATOR
            | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_MAX_UPDATES)) {
        s = syntiant_ndp120_write_block(
            ndp, 1, ph_params_addr + offset, &pparams.adaptive_frames,
            offsetof(struct ndp120_ph_params_s, params_memory) -
                offsetof(struct ndp120_ph_params_s, adaptive_frames));
        if (s) goto error;

        s = syntiant_ndp120_read_block(
            ndp, 1, ph_params_addr + offset, &pparams.adaptive_frames,
            offsetof(struct ndp120_ph_params_s, params_memory) -
                offsetof(struct ndp120_ph_params_s, adaptive_frames));
        if (s) goto error;
    }

    config->adaptive_frames = pparams.adaptive_frames;
    config->adaptive_denominator = pparams.adaptive_denominator;
    config->adaptive_max_updates = pparams.adaptive_max_updates;

    if (!config->set && !pparams.num_states) {
        goto got_structure;
    }

    if (pparams.num_states <= config->state) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    offset = offsetof(struct ndp120_ph_params_s, params_memory);
    state_addr = ph_params_addr + offset;

    /*important fix: note we are using pseudo-heap, and params_memory now is a pointer*/
    s = syntiant_ndp120_read_block(ndp, 1, state_addr, &state_addr2, sizeof(uint32_t));
    state_addr2_orig = state_addr2;
    state_addr2 += config->state * ((unsigned int)sizeof(sparams));

    s = syntiant_ndp120_read_block(ndp, 1, state_addr2, &sparams, sizeof(sparams));
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT) {
        sparams.timeout = config->timeout;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION) {
        s = syntiant_ndp120_posterior_config_encode_action(
            config->ph_type, config->timeout_action_type,
            config->timeout_action_arg0, config->timeout_action_arg1,
            &sparams.timeout_action);
        if (s) goto error;
    }

    if (config->set &
        (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT |
         SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION)) {
        s = syntiant_ndp120_write_block(ndp, 1, state_addr2, &sparams, sizeof(sparams));
        if (s) goto error;

        s = syntiant_ndp120_read_block(ndp, 1, state_addr2, &sparams, sizeof(sparams));
        if (s) goto error;
    }

    config->timeout = sparams.timeout;

    syntiant_ndp120_posterior_config_decode_action(
        sparams.timeout_action, pparams.ph_type, &config->timeout_action_type,
        &config->timeout_action_arg0, &config->timeout_action_arg1);

    if (!(config->set
          & ~(unsigned int)
          (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT
           | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION))
        && !pparams.num_classes) {
        goto got_structure;
    }

    if (pparams.num_classes <= config->class_index) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    state_addr2 = state_addr2_orig;
    params_addr = state_addr2
        + sparams.class_params_offset * ((unsigned int)sizeof(uint32_t))
        + config->class_index * ((unsigned int)sizeof(cparams));
    s = syntiant_ndp120_read_block(
        ndp, 1, params_addr, &cparams, sizeof(cparams));
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_THRESHOLD) {
        cparams.threshold = config->threshold;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_WINDOW) {
        cparams.window = config->window;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_BACKOFF) {
        cparams.backoff = config->backoff;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_SM_QUEUE_SIZE) {
        if (NDP120_RESULT_SOFTMAX_SMOOTHER_MAX_QUEUE_SIZE
            < config->smoothing_queue_size) {
            SYNTIANT_PRINTF("Invalid argument: smoothing %d exceeds maximum %d\n",
                config->smoothing_queue_size,
                NDP120_RESULT_SOFTMAX_SMOOTHER_MAX_QUEUE_SIZE);
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
        cparams.smoothing_queue_size = config->smoothing_queue_size;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_THRESHOLD) {
        cparams.adaptive_threshold = config->adaptive_threshold;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ACTION) {
        s = syntiant_ndp120_posterior_config_encode_action(
            pparams.ph_type, config->action_type, config->action_arg0,
            config->action_arg1, &cparams.action);
        if (s) goto error;
    }

    if (config->set &
            (SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_THRESHOLD
                | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_WINDOW
                | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_BACKOFF
                | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ACTION
                | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_SM_QUEUE_SIZE
                | SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_THRESHOLD)) {
        s = syntiant_ndp120_write_block(
            ndp, 1, params_addr, &cparams, sizeof(cparams));
        if (s) goto error;

        s = syntiant_ndp120_read_block(
            ndp, 1, params_addr, &cparams, sizeof(cparams));
        if (s) goto error;
    }

    config->threshold = cparams.threshold;
    config->window = cparams.window;
    config->backoff = cparams.backoff;
    config->smoothing_queue_size = cparams.smoothing_queue_size;
    config->adaptive_threshold = cparams.adaptive_threshold;

    syntiant_ndp120_posterior_config_decode_action(
        cparams.action, pparams.ph_type, &config->action_type,
        &config->action_arg0, &config->action_arg1);

got_structure:
    config->states = pparams.num_states;
    config->classes = pparams.num_classes;
    config->ph_type = pparams.ph_type;

got_enable:
    config->enable =
        (fw_state_enable & POSTERIOR_ENABLE_BITS) == POSTERIOR_ENABLE_BITS;

error:
    return s;
}

int syntiant_ndp120_posterior_config(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_posterior_config_s *config)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_posterior_config\n");
        goto error;
    }

    s = syntiant_ndp120_posterior_config_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

static int syntiant_ndp120_custom_posterior_handler_no_sync(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_posterior_handler_s *cfg)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t length =  (uint32_t)sizeof(*cfg) + cfg->length;
    void *payload = cfg + 1;

    addr += (uint32_t)sizeof(uint32_t);
    /* write total length */
    s = syntiant_ndp120_write_block(
        ndp, SYNTIANT_NDP120_MCU, addr, &length, sizeof(length));
    if (s) goto error;

    addr += (uint32_t)sizeof(length);
    /* write *cfg struct */
    s = syntiant_ndp120_write_block(
        ndp, SYNTIANT_NDP120_MCU, addr, cfg, sizeof(*cfg));
    if (s) goto error;

    if (cfg->op != NDP120_PH_OP_READ ||
        cfg->op != NDP120_PH_OP_ATTACH_NN) {
        addr += (uint32_t)sizeof(*cfg);
        /* write payload */
        s = syntiant_ndp120_write_block(
            ndp, SYNTIANT_NDP120_MCU, addr, payload, cfg->length);
        if (s) goto error;
    }

    /* send command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP_MBIN_REQUEST_POSTERIOR_HANDLER, NULL);
    if (s) goto error;

    memset(payload, 0, cfg->length);
    addr = NDP120_MCU_OPEN_RAM_RESULTS;
    /* read payload */
    s = syntiant_ndp120_read_block(
        ndp, SYNTIANT_NDP120_MCU, addr, payload, cfg->length);
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_get_posterior_type(struct syntiant_ndp_device_s *ndp,
                                       uint32_t nn_id, uint32_t* ph_id)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t fw_state_addr = ndp120->mcu_fw_state_addr;
    uint32_t offset;

    if (!fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto out;
    }
    offset = offsetof(struct ndp120_fw_state_s, ph_types);
    s = syntiant_ndp120_read(ndp, 1,
        fw_state_addr + offset + (uint32_t)sizeof(uint32_t) * nn_id,
        ph_id);
    if (s) goto out;
    DEBUG_PRINTF("posterior type: %d for network:%d\n", *ph_id, nn_id);
out:
    return s;
}

int syntiant_ndp120_custom_posterior_handler(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_posterior_handler_s *cfg)
{
    int s;

    if (cfg->id >= MAX_PH_ALGOS ||
        cfg->op > NDP120_PH_OP_ATTACH_NN ||
        cfg->length > SYNTIANT_POSTERIOR_HANDLER_DATA_MAXLEN ||
        cfg->nn_id >= MAX_NNETWORKS) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_posterior_config\n");
        goto error;
    }

    s = syntiant_ndp120_custom_posterior_handler_no_sync(ndp, cfg);
    if (s) {
        goto error;
    }

    s = (ndp->iif.unsync)(ndp->iif.d);
    if (s) {
        goto error;
    }

error:
    return s;
}
