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
#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_ilib/ndp120_regs.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp_driver.h>
#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/ndp120_spi_regs.h>
#include <syntiant-dsp-firmware/ndp120_dsp_mailbox.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>

#include "syntiant_ndp120_config.h"

/* ndp120 reg state for config funcs */
struct syntiant_ndp120_config_state_s {
    unsigned int dnn_input;
    unsigned int input_clock_rate;
    unsigned int core_clock_rate;
    uint32_t fw_state_addr;
    uint32_t fw_enable;
    uint32_t fw_max_adjustment_gain;
    uint32_t fw_nom_speech_quiet;
    uint32_t fw_match_per_frame;
    uint32_t smplctl;
    uint32_t smplmark;
    uint32_t smplsts;
    uint32_t tank;
    uint32_t i2sctl;
    uint32_t pdmctl;
    uint32_t clkctl0;
    uint32_t clkctl1;
    uint32_t clkctl2;
    uint32_t freqctl;
    uint32_t fifo_thresh[5];
    uint8_t ctl;
};

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
static int
config_read_state(struct syntiant_ndp_device_s *ndp,
                                  struct syntiant_ndp120_config_state_s *st)
{
    int s;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t addr;

    if (ndp120->mcu_fw_state_addr) {
        addr = ndp120->mcu_fw_state_addr
            + (uint32_t) offsetof(struct ndp120_fw_state_s, enable);
        s = syntiant_ndp120_read(ndp, 1, addr, &st->fw_enable);
        if (s)
            goto out;

        addr = ndp120->mcu_fw_state_addr
            + (uint32_t) offsetof(struct ndp120_fw_state_s,
                                  mb_state.enable_match_for_every_frame);
        s = syntiant_ndp120_read(ndp, 1, addr, &st->fw_match_per_frame);
        if (s)
            goto out;
    } else {
        st->fw_enable = 0;
        st->fw_match_per_frame = 0;
    }

    s = syntiant_ndp120_read(ndp, 0, NDP120_SPI_CTL, &st->ctl);
    if (s)
        goto out;

 out:
    return s;
}

/* RW API for interrupt clearance parameter
 * in_config: NULL when device configuration is written
 * out_config: NULL when device configuration is read
 */
static int
config_int_clear(struct syntiant_ndp_device_s *ndp,
                      const syntiant_ndp120_config_misc_t *in_config,
                      syntiant_ndp120_config_misc_t *out_config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct ndp120_auto_clear auto_clear;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t addr;

    if (ndp120->mcu_fw_state_addr) {
        memset(&auto_clear, 0, sizeof(auto_clear));
        addr = ndp120->mcu_fw_state_addr
            + (uint32_t) offsetof(struct ndp120_fw_state_s, auto_clear);
        if (in_config &&
                in_config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_INT_CLEAR) {
            auto_clear.num_frames = (uint16_t) in_config->frame_count;
            auto_clear.match_clear = (uint16_t) in_config->clear_match;
            /* auto_clear.int_count is set to 0 already */
            s = syntiant_ndp120_write_block(ndp, 1, addr, &auto_clear,
                sizeof(auto_clear));
        } else if (out_config) {
            s = syntiant_ndp120_read_block(ndp, 1, addr, &auto_clear,
                sizeof(auto_clear));
            if (s) goto out;
            out_config->frame_count = auto_clear.num_frames;
            out_config->clear_match = auto_clear.match_clear;
            out_config->int_clear_count = auto_clear.int_count;
        } else {
            s = SYNTIANT_NDP_ERROR_CONFIG;
        }
    } else {
        s = SYNTIANT_NDP_ERROR_UNINIT;
    }
out:
    return s;
}

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
static int
config_set_input(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_misc_t *config,
                                 struct syntiant_ndp120_config_state_s *st)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned int i;
    uint32_t dsp_fw_state_adx;
    ndp120_dsp_data_flow_setup_t flow_setup;

    (void)st;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_INPUT) {

        dsp_fw_state_adx = syntiant_ndp120_get_dsp_fw_pointer(ndp);
        if (!dsp_fw_state_adx) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto error;
        }

        s = syntiant_ndp120_dsp_flow_setup_get_no_sync(ndp, &flow_setup);
        if(s) goto error;

        /* fixit */

        for(i = 0; i < ARRAY_LEN(flow_setup.src_pcm_audio); i++) {
            ndp120_dsp_data_flow_rule_t *rule = &flow_setup.src_pcm_audio[i];
            if (NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) {
                rule->src_param = (uint8_t)config->input;
            }

        }

        /* apply flow */
        s = syntiant_ndp120_dsp_flow_setup_apply_no_sync(ndp, &flow_setup);
        if (s) goto error;

        /*restartrt DSP to reset buffers */
        syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_RESTART, NULL);
        if (s) goto error;
    }

error:
    return s;
}

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
static int
config_update_state(struct syntiant_ndp_device_s *ndp,
                                    struct syntiant_ndp120_config_state_s *st0,
                                    struct syntiant_ndp120_config_state_s *st)
{
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t addr;
    int s = SYNTIANT_NDP_ERROR_NONE;
    int do_reset;

    do_reset = st0->smplctl != st->smplctl || st0->smplmark != st->smplmark;

    do_reset = 1;

    /* TODO update pdm ctl, dnn ctl etc. */
    if (do_reset) {

        if (st0->fw_enable != st->fw_enable) {
            addr = ndp120->mcu_fw_state_addr
                + (uint32_t) offsetof(struct ndp120_fw_state_s, enable);
            s = syntiant_ndp120_write(ndp, 1, addr, st->fw_enable);
            if (s)
                goto out;

        }

        if (st0->fw_match_per_frame != st->fw_match_per_frame) {
            SYNTIANT_PRINTF("setting match-per-frame: %s\n", st->fw_match_per_frame ? "on" : "off");
            if (st->fw_match_per_frame) {
                /* Before turning on MPF interrupt, we need to retrieve DSP's
                   PCM ring buffer pointers and save them to ilib */
                s = syntiant_ndp120_init_ring_buffer_pointers_no_sync(ndp, 0);
                if (s) goto out;
            }
            addr = ndp120->mcu_fw_state_addr + (uint32_t) offsetof(struct ndp120_fw_state_s, mb_state.enable_match_for_every_frame);
            s = syntiant_ndp120_write(ndp, 1, addr, st->fw_match_per_frame);
            if (s)
                goto out;
        }
    }

    if (do_reset) {
        s = syntiant_ndp120_write(ndp, 0, NDP120_SPI_INTSTS,
            NDP120_SPI_INTSTS_MATCH_INT(1) | NDP120_SPI_INTSTS_DNN_INT(1) |
            NDP120_SPI_INTSTS_AE_INT(1) | NDP120_SPI_INTSTS_WM_INT(1));
        if (s) goto out;
    }

out:
    return s;
}

static int
config_set_fifo_threshold(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t dsp_fw_state_adx;
    uint32_t addr;
    uint32_t sample_threshold = 0;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    dsp_fw_state_adx = syntiant_ndp120_get_dsp_fw_pointer(ndp);
    if (!dsp_fw_state_adx) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    if (config->fifo_threshold % 16 == 0) {
        sample_threshold = 4;
    } else if (config->fifo_threshold % 8 == 0) {
        sample_threshold = 2;
    } else if (config->fifo_threshold % 4 == 0) {
        sample_threshold = 1;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }
    s = syntiant_ndp120_write_block(ndp, 1,
        NDP120_DSP_CONFIG_FIFOSAMPLETHRESHOLD(config->fifo_number),
        &sample_threshold, sizeof(sample_threshold));
    if(s) {
        goto error;
    }

    config->hw_threshold = sample_threshold;
    addr = ndp120->dsp_fw_state_addr + (uint32_t)offsetof(ndp120_dsp_fw_base_t,
            config.fifo_threshold_bytes);
    addr += (uint32_t)
            sizeof(((ndp120_dsp_config_t *)0)->fifo_threshold_bytes[0]) *
            config->fifo_number;

    s = ndp_mcu_write(addr, config->fifo_threshold);
    if (s) goto error;

    /*restartrt DSP to reset buffers */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_RESTART, NULL);
    if (s) goto error;

    error:
        return s;
}

/* Write or Read the softmax scale factor of specified NN */
static int config_softmax(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config, int write)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t scale_factor;
    uint32_t addr = ndp->d.ndp120.mcu_fw_smax_smoother_addr;
    if (!addr) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
    addr += (unsigned int) sizeof(struct ndp120_result_softmax_smoother_s) * config->nn_num +
        (unsigned int) offsetof(struct ndp120_result_softmax_smoother_s,
                scale_factor);
    if (write) {
        DEBUG_PRINTF("writing softmax scale factor %u to 0x%x for nn:%d\n",
                config->quantized_sf, addr, config->nn_num);
        s = syntiant_ndp120_write(ndp, 1, addr, (uint32_t) config->quantized_sf);
    } else {
        s = syntiant_ndp120_read(ndp, 1, addr, &scale_factor);
        if (s) {
            goto error;
        }
        config->quantized_sf = (int16_t)scale_factor;
    }
error:
    return s;
}

/* change vad mode of a vad mic sensor */
static int config_vad_mode(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx, payload_in_MSB = 0;

    DEBUG_PRINTF("setting vad mode:%d\n", config->vad_mode);
    if (config->vad_mode <=
        SYNTIANT_NDP120_CONFIG_VALUE_VAD_MIC_MODE_VAD_AADD2) {
        adx = NDP120_DSP_MB_H2D_ADDR;
        payload_in_MSB = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB,
                                                   config->vad_mode);
        s = syntiant_ndp120_write_block(ndp, 1, adx, &payload_in_MSB,
                                        sizeof(uint32_t));
        if (s) goto error;
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_EXT_VAD_CONTROL, NULL);
        if (s) goto error;

    } else {
        s = SYNTIANT_NDP_ERROR_ARG;
    }

error:
    return s;
}

static int config_secondary_validation_type(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config, int write)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t adx = 0;
    uint8_t sec_val_type[4];

    if (!ndp120->mcu_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = (uint32_t) (ndp120->mcu_fw_state_addr + (uint32_t)
            offsetof(struct ndp120_fw_state_s, sec_validation_type));

    s = ndp_mcu_read_block(adx, sec_val_type, sizeof(uint32_t));
    if (s) goto error;

    if (write) {
        sec_val_type[config->nn_num] = (uint8_t)config->sec_val_type;
        s = ndp_mcu_write_block(adx, sec_val_type, sizeof(uint32_t));
    } else {
        config->sec_val_type = (uint32_t)sec_val_type[config->nn_num];
    }

error:
    return s;
}

static int config_secondary_validation_fields(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config, int write)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx = 0;
    uint8_t size = 0;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    struct ndp120_val_type_one_shot_s one_shot;
    struct ndp120_val_type_pattern_s patterned;

    if (!ndp120->mcu_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }
    switch (config->sec_val_type) {
        case NDP120_SEC_VAL_TYPE_ONE_SHOT:
            size = sizeof(struct ndp120_val_type_one_shot_s);
            adx = (uint32_t)(ndp120->mcu_fw_state_addr +
                  (uint32_t)offsetof(struct ndp120_fw_state_s, one_shot));
            adx += (uint32_t)(size * config->nn_num);

            s = ndp_mcu_read_block(adx, (uint32_t *)&one_shot, size);
            if (s) goto error;

            if (write) {
                one_shot.threshold = config->threshold;
                one_shot.ratio_threshold = config->ratio_threshold;
                one_shot.num_frames = config->num_frames;
                one_shot.notification_type = config->notification_type;
                s = ndp_mcu_write_block(adx, (uint32_t *)&one_shot, size);
                if (s) goto error;
            } else {
                config->threshold = one_shot.threshold;
                config->ratio_threshold = one_shot.ratio_threshold;
                config->num_frames = one_shot.num_frames;
                config->notification_type = one_shot.notification_type;
            }
            break;

        case NDP120_SEC_VAL_TYPE_PATTERN:
            size = sizeof(struct ndp120_val_type_pattern_s);
            adx = (uint32_t)(ndp120->mcu_fw_state_addr +
                  (uint32_t)offsetof(struct ndp120_fw_state_s, patterned));
            adx += (uint32_t)(size * config->nn_num);

            s = ndp_mcu_read_block(adx, (uint32_t *)&patterned, size);
            if (s) goto error;

            if (write) {
                patterned.num_occurrences = config->num_occurrences;
                patterned.frame_range_low = config->frame_range_low;
                patterned.frame_range_high = config->frame_range_high;
                patterned.notification_type = config->notification_type;
                s = ndp_mcu_write_block(adx, (uint32_t *)&patterned, size);
                if (s) goto error;
            } else {
                config->num_occurrences = patterned.num_occurrences;
                config->frame_range_low = patterned.frame_range_low;
                config->frame_range_high = patterned.frame_range_high;
                config->notification_type = patterned.notification_type;
            }
            break;

        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
    }

    if (s) {
        SYNTIANT_PRINTF("%s error (s=%u)\n", write ? "write" : "read", s);
        goto error;
    }

error:
    return s;
}

int
syntiant_ndp120_config_misc_no_sync(struct syntiant_ndp_device_s *ndp,
                               syntiant_ndp120_config_misc_t *config)
{
    struct syntiant_ndp120_config_state_s st, st0;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    ndp120_dsp_config_t fw_config;
    struct ndp120_misc_posterior post_settings;
    ndp120_dsp_fifo_info_t fifo_info;
    uint32_t adx;

    int s = SYNTIANT_NDP_ERROR_NONE;

    if (!ndp120->mcu_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    if (config->set & (uint32_t) ~SYNTIANT_NDP120_CONFIG_SET_MISC_ALL_M) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t, config);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &fw_config, sizeof(fw_config));
    if (s) {
	    SYNTIANT_PRINTF("error in reading DSP FW config block\n");
	    goto error;
    }
    ndp120->audio_sample_rate = fw_config.sample_rate;
    ndp120->audio_frame_step = fw_config.aud_samp_size_bytes/PCM_AUDIO_SAMPLE_WIDTH_BYTES;
    ndp120->audio_sample_size_bytes = fw_config.aud_samp_size_bytes;

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t, fifo_info);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &fifo_info, sizeof(fifo_info));
    if (s) {
	    SYNTIANT_PRINTF("error reading fifo info\n");
 	    goto error;
    }
    ndp120->audio_frame_step = fw_config.fifo_threshold_bytes[4]/PCM_AUDIO_SAMPLE_WIDTH_BYTES; /* FIXME, this is only SPI */
    /*ndp120->audio_sample_size_bytes = dsp_fw_state.fifo_threshold_bytes[0]; */
    if (ndp->device_type >= 0x40 && ndp->device_type <= 0x43) {
        ndp120->image_frame_size =
            fw_config.fifo_threshold_bytes[4];
        config->image_buffer_used =
            (uint32_t)(fifo_info.fifo_end_adx[4] - fifo_info.fifo_start_adx[4]);
    }

    config->audio_buffer_used = (uint32_t)(fifo_info.fifo_end_adx[4] -
            fifo_info.fifo_start_adx[4]);

    if (!config->set && !config->get) {
        config->input = ndp120->dnn_input;
        config->audio_frame_size = ndp120->audio_frame_size;
        if (ndp120->audio_sample_size_bytes) {
            config->audio_sample_size_bytes
                = (uint32_t) (ndp120->audio_sample_size_bytes
                + sizeof(ndp120_dsp_audio_sample_annotation_t));
        }
        config->audio_frame_step = ndp120->audio_frame_step;
        config->dnn_frame_size = ndp120->dnn_frame_size;
        config->image_frame_size = ndp120->image_frame_size;
        goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_FIFO_THRESHOLD) {
        s = config_set_fifo_threshold(ndp, config);
        if (s) {
            SYNTIANT_PRINTF("error in setting fifo threshold\n");
            goto error;
        }
    }

    if (config->get & SYNTIANT_NDP120_CONFIG_SET_MISC_FIFO_THRESHOLD) {
        s = syntiant_ndp_read(ndp, 1,
            NDP120_DSP_CONFIG_FIFOSAMPLETHRESHOLD(config->fifo_number),
            &(config->hw_threshold));
        if (s) {
            DEBUG_PRINTF("fifo_threshold: error reading threshold value\n");
            goto error;
        }
        adx = ndp120->dsp_fw_state_addr +
                (uint32_t)offsetof(ndp120_dsp_fw_base_t,
                    config.fifo_threshold_bytes);
        adx += (uint32_t)
                sizeof(((ndp120_dsp_config_t *)0)->fifo_threshold_bytes[0]) *
                    config->fifo_number;

        s = ndp_mcu_read(adx, &config->fifo_threshold);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_SOFTMAX) {
        s = config_softmax(ndp, config, 1);
        if (s) goto error;
    }

    if (config->get & SYNTIANT_NDP120_CONFIG_SET_MISC_SOFTMAX) {
        s = config_softmax(ndp, config, 0);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_VAD_MODE) {
        s = config_vad_mode(ndp, config);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_TYPE) {
        s = config_secondary_validation_type(ndp, config, 1);
        if (s) goto error;
    }

    if (config->get & SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_TYPE) {
        s = config_secondary_validation_type(ndp, config, 0);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_CONFIG) {
        s = config_secondary_validation_fields(ndp, config, 1);
        if (s) goto error;
    }

    if (config->get & SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_CONFIG) {
        s = config_secondary_validation_fields(ndp, config, 0);
        if (s) goto error;
    }

    s = config_set_input(ndp, config, &st);
    if (s) {
	    SYNTIANT_PRINTF("error in setting input\n");
	    goto error;
    }

    s = config_read_state(ndp, &st);
    if (s) {
	    SYNTIANT_PRINTF("error in reading state\n");
	    goto error;
    }

    st.fw_state_addr = ndp120->mcu_fw_state_addr;
    st.input_clock_rate = ndp120->input_clock_rate;

    st0 = st;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_MATCH_PER_FRAME_ON) {
        st.fw_match_per_frame = config->match_per_frame_on;
    }

    adx = ndp120->mcu_fw_state_addr + (uint32_t)offsetof(struct ndp120_fw_state_s, post_settings);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &post_settings, sizeof(post_settings));
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_SPL_THRESH) {
        post_settings.spl_thresh = config->spl_thresh;
        DEBUG_PRINTF("updating spl threshold to %d\n", post_settings.spl_thresh);
        s = syntiant_ndp120_write_block(ndp, 1, adx, &post_settings, sizeof(post_settings));
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_SAMPLE_RATE) {
        adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t, config);
        fw_config.sample_rate = config->sample_rate;
        s = syntiant_ndp120_write_block(ndp, 1, adx, &fw_config, sizeof(fw_config));
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_MISC_INT_CLEAR) {
        s = config_int_clear(ndp, config, NULL);
        if (s) goto error;
    }
    if (config->get & SYNTIANT_NDP120_CONFIG_SET_MISC_INT_CLEAR) {
        s = config_int_clear(ndp, NULL, config);
        if (s) goto error;
    }

    config->sample_rate = ndp120->audio_sample_rate;
    config->spl_thresh = (uint8_t)post_settings.spl_thresh;

    config->match_per_frame_on = st0.fw_match_per_frame;

    s = config_update_state(ndp, &st0, &st);
    if (s) {
	      SYNTIANT_PRINTF("error in updating state\n");
	      goto error;
    }

    ndp120->input_clock_rate = st.input_clock_rate;

 error:
     return s;
}

int
syntiant_ndp120_config_misc(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_misc_t *config)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
	      SYNTIANT_PRINTF("error in config_misc:%d\n", s);
	      return s;
    }

    s = syntiant_ndp120_config_misc_no_sync(ndp, config);
    if(s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_mpf(struct syntiant_ndp_device_s *ndp,
    int mpf)
{
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t addr;
    int s = SYNTIANT_NDP_ERROR_NONE;

    addr = ndp120->mcu_fw_state_addr + (uint32_t) offsetof(
                struct ndp120_fw_state_s, mb_state.enable_match_for_every_frame);
    s = syntiant_ndp120_write(ndp, 1, addr, (uint32_t) mpf);
    SYNTIANT_PRINTF("setting match-per-frame: %s\n",
                   mpf ? "on" : "off");

   return s;
}

int
syntiant_ndp120_get_sensor_status(struct syntiant_ndp_device_s *ndp,
       syntiant_ndp120_sensor_status_t *sensor_status)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned char sensor_type;
    uint32_t addr_sensor_state = 0;
    uint32_t addr_sensor = 0;
    uint32_t control, sensor_id;
    struct ndp_fw_sensor_configuration_s sensors[NDP_FW_SENSOR_MAX];
    ndp120_sensor_state_t sensor_state[NDP_FW_SENSOR_MAX];

    sensor_id = sensor_status->sensor_num;
    if (sensor_id >= NDP_FW_SENSOR_MAX) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }

    addr_sensor = ndp->d.ndp120.dsp_fw_state_addr +
                  (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                  serial_sensor_mgmt.sensor_config);
    s = ndp->driver->read_block(ndp, 1, addr_sensor, sensors, sizeof(sensors));
    if (s)
        goto out;

    /* check sensor id to see if it's initialized */
    control = sensors[sensor_id].control;
    sensor_type = (control & NDP_FW_SENSOR_CONTROL_ID_MASK)
                   >> NDP_FW_SENSOR_CONTROL_ID_SHIFT;
    if (sensor_type == NDP_FW_SENSOR_ID_NONE) {
        SYNTIANT_PRINTF("error in syntiant_ndp120_get_sensor_status: "
                        "no sensor added at sensor num=%u\n",
                        (unsigned int)sensor_id);
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }

    addr_sensor_state = ndp->d.ndp120.dsp_fw_state_addr +
                   (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                   serial_sensor_mgmt.sensor_state);
    s = ndp->driver->read_block(ndp, 1, addr_sensor_state, sensor_state,
                                sizeof(sensor_state));
    if (s) goto out;

    if (sensor_status->flag & SYNTIANT_NDP120_SENSOR_STATUS_INT_COUNT ||
        sensor_status->flag & SYNTIANT_NDP120_SENSOR_STATUS_ALL_M) {
        sensor_status->int_count = sensor_state[sensor_id].int_count;
    }

    if (sensor_status->flag & SYNTIANT_NDP120_SENSOR_STATUS_ALL_M) {
        if (sensor_status->sensor_type == SYNTIANT_CONFIG_SENSOR_ID_T5838) {
            sensor_status->u.sensor_t5838 = sensor_state[sensor_id].u.
                                            sensor_t5838;
        }
    }
out:
    return s;
}
