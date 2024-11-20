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
#ifndef SYNTIANT_NDP120_PH_H
#define SYNTIANT_NDP120_PH_H

#include <syntiant_ilib/syntiant_ndp_driver.h>

typedef struct syntiant_ndp120_posterior_config_s {
    unsigned int set;         /**< configuration variable set flags */
    unsigned int states;      /**< number of states */
    unsigned int classes;     /**< number of classes */
    unsigned int ph_type;     /**< posterior handler type used */
    unsigned int smoothing_queue_size;
    /**< number of frames to smooth probabilities over */
    unsigned int state;       /**< state number to configure */
    unsigned int timeout;     /**< state timeout in DNN frames */
    unsigned int timeout_action_type; /**< timeout action type (MATCH|STATE) */
    unsigned int timeout_action_arg0; /* first action argument */
    unsigned int timeout_action_arg1; /* second action argument */
    unsigned int class_index;       /**< class_index number to configure */
    unsigned int threshold;   /**< class active threshold level 0-65535 */
    unsigned int window;      /**< class active window in DNN frames 0-255 */
    unsigned int backoff;     /**< match backoff timer in DNN frames 0-255 */
    unsigned int action_type; /**< match action type */
    unsigned int action_arg0; /* first action argument */
    unsigned int action_arg1; /* second action argument */
    unsigned int ph_num; /**< number of activat phs */
    unsigned int ph_idx; /***< The index of posterior handler */
    unsigned int adaptive_threshold; /**< class scp adaptive threshold */
    unsigned int adaptive_frames; /**< ph adaptive frames */
    unsigned int adaptive_denominator; /**< ph adaptive denominator */
    unsigned int adaptive_max_updates; /**< ph adaptive max updates */
    int enable;
} syntiant_ndp120_posterior_config_t;


/**
 * @brief NDP120 posterior configurations.
 *
 * Set or Get mcu firmware posterior configurations. 'set' variable in the structure 'config' 
 * indicates the bit mask of the configurations that are to be updated.
 *
 * @param ndp NDP state object
 * @param config represents the posterior configurations.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_posterior_config(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_posterior_config_s *config);

#endif
