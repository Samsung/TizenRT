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
#ifndef SYNTIANT_NDP200_H
#define SYNTIANT_NDP200_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_ndp120.h>

int syntiant_ndp200_config_clk(struct syntiant_ndp_device_s *ndp,
                         enum syntiant_ndp_init_mode_e init_mode,
                         uint8_t clock_option,  int use_xtal,
                         uint32_t clock_option_issued, uint32_t i2c_clk_rate);

int syntiant_ndp200_config_watch_setup(struct syntiant_ndp_device_s *ndp,
                         int input_type,
                         ndp200_dsp_fw_image_sensor_config_t *img_sensor_config,
                         syntiant_ndp120_config_sensor_t *sensor_config);


int syntiant_ndp200_config_record_setup(struct syntiant_ndp_device_s *ndp,
                         ndp200_dsp_fw_image_sensor_config_t *img_sensor_config,
                         syntiant_ndp120_config_sensor_t *sensor_config);



int syntiant_ndp200_config_watch_record_setup(struct syntiant_ndp_device_s *ndp,
                         ndp200_dsp_fw_image_sensor_config_t *img_sensor_config);


int syntiant_ndp200_enable_continous_mode(struct syntiant_ndp_device_s *ndp);

int syntiant_ndp200_disable_continous_mode(struct syntiant_ndp_device_s *ndp);


int syntiant_ndp200_get_sample_size(struct syntiant_ndp_device_s *ndp,
    unsigned int *len);

int syntiant_ndp200_dsp_get_put_capture_args(
    struct syntiant_ndp_device_s *ndp,
    ndp200_dsp_fw_image_sensor_config_t *capture_args, uint8_t write);

int syntiant_ndp200_write_softmax_config(struct syntiant_ndp_device_s *ndp,
                                     syntiant_softmax_t *softmax);

/* Align these enums with the NDP120 presets */
enum syntiant_ndp200_clock_frequency_type {
   SYNTIANT_NDP200_CLOCK_MODE_FLL_0P9V_32768_24P000MHZ = 0,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_0P9V_32768_21P504MHZ = 2,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_1V_32768_49P152MHZ = 5,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_1V_32768_55P296MHZ = 6,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_1P1V_32768_55P296MHZ = 8,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_1P1V_32768_76P8MHZ = 9,
   SYNTIANT_NDP200_CLOCK_MODE_PLL_1P1V_32768_98P304MHZ = 10,
   SYNTIANT_NDP200_CLOCK_MODE_NULL = -1
};
#ifdef __cplusplus
}
#endif

#endif /* SYNTIANT_NDP200_H  */
