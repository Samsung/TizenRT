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




#ifndef NDP200_DSP_IMAGE_SENSOR_H_
#define NDP200_DSP_IMAGE_SENSOR_H_

typedef struct {
    uint32_t frame_rate;
    uint16_t set_exposure;
    uint16_t num_frames;
    uint16_t horz_size;
    uint16_t vert_size;
    uint16_t crop_horz_size;
    uint16_t crop_vert_size;
    uint16_t crop_center_horz_size;
    uint16_t crop_center_vert_size;
    uint8_t set_gain;
    uint8_t min_gain;
    uint8_t max_gain;
    uint8_t flip;
    uint8_t mirror;
    uint8_t auto_exposure;
    uint8_t motion_enable;
    uint8_t pixel_target;
} ndp200_dsp_fw_image_sensor_config_t;

#endif
