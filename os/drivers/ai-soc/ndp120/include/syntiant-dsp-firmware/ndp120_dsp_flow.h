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

#ifndef NDP120_DSP_FLOW_H_
#define NDP120_DSP_FLOW_H_

#ifndef __KERNEL__
#include <stdint.h>
#endif
#include <syntiant-dsp-firmware/ndp120_dsp_sample.h>

/* pass in a src_type */
#define NDP120_DSP_DATA_FLOW_SRC_TYPE_STR(x) \
    (x) == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO ? "PCM" : \
    (x) == NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION ? "FUNC" : \
    (x) == NDP120_DSP_DATA_FLOW_SRC_TYPE_NN ? "NN" : \
    (x) == NDP120_DSP_DATA_FLOW_SRC_TYPE_SENSOR ? "SENSOR" : \
    (x) == NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU ? "MCU" : "UNKNOWN"

/* pass in a _rule_ */
#define NDP120_DSP_DATA_FLOW_RULE_DST_STR(x) \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_NONE ? "<INVALID>" : \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION ? "FUNC" : \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_NN ? "NN" : \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT ? \
    ((x).dst_param ==  NDP120_DSP_DATA_FLOW_DST_SUBTYPE_SENSOR ? "HOST_EXT_SENSOR" : \
    ((x).dst_param ==  NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO ? "HOST_EXT_AUDIO" : "HOST_EXT_FEATURE")) : \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_I2S ? "I2S" : \
    (x).dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_MCU ? "MCU" : "UNKNOWN"

/* src_type enums for flow tuples */
typedef enum {
    /* If you update this, update the _STR
       macro above */
    NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
    NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION,
    NDP120_DSP_DATA_FLOW_SRC_TYPE_NN,
    NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU,
    NDP120_DSP_DATA_FLOW_SRC_TYPE_SENSOR,
    NDP120_DSP_DATA_FLOW_SRC_TYPE_COUNT
} syntiant_ndp120_flow_src_type_t;

/* src_param enums for flow tuples */
typedef enum {
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT    = 1,
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_RIGHT   = 2,
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO  =
        (NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT |
        NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_RIGHT),
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_LEFT    = 4,
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_RIGHT   = 8,
    NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_STEREO  =
        (NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_LEFT |
        NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_RIGHT),
    NDP120_DSP_DATA_FLOW_SRC_PARAM_4CHANNEL     =
        (NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO |
        NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_STEREO),
    NDP120_DSP_DATA_FLOW_SRC_PARAM_SPI          = 16
} syntiant_ndp120_flow_src_param_t;

#define NDP120_PCM_DATA_FLOW_RULE_MAX 16
#define NDP120_FUNC_DATA_FLOW_RULE_MAX 32
#define NDP120_NN_DATA_FLOW_RULE_MAX 32
#define NDP120_MCU_DATA_FLOW_RULE_MAX 8
#define NDP120_SENSOR_DATA_FLOW_RULE_MAX 4
#define NDP120_FLOW_SET_IDS_MAX 4

/* dst_type enums for flow tuples */
typedef enum {
    /* NONE mostly exists so that the SRC
       and DST enums line up, numerically */
    NDP120_DSP_DATA_FLOW_DST_TYPE_NONE,
    NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION,
    NDP120_DSP_DATA_FLOW_DST_TYPE_NN,
    NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT,
    NDP120_DSP_DATA_FLOW_DST_TYPE_I2S,
    NDP120_DSP_DATA_FLOW_DST_TYPE_MCU
    /* If you update this, update the _STR
       macro above */
} syntiant_ndp120_flow_dst_type_t;

/* dst_type sub_type enums for flow tuples */
typedef enum {
    NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO = NDP120_DSP_SAMPLE_TYPE_PCM_AUDIO,
    NDP120_DSP_DATA_FLOW_DST_SUBTYPE_FEATURE = NDP120_DSP_SAMPLE_TYPE_FUNCTION,
    NDP120_DSP_DATA_FLOW_DST_SUBTYPE_SENSOR = NDP120_DSP_SAMPLE_TYPE_SENSOR
} syntiant_ndp120_flow_dst_subtype_t;

/* dst_param enums for flow tuples */
typedef enum {
    NDP120_DSP_DATA_FLOW_DST_PARAM_UNSPECIFIED = 0
} syntiant_ndp120_flow_dst_param_t;

/* enums for DSP front end functions */
typedef enum {
    NDP120_DSP_DATA_FLOW_FUNCTION_10X_FEATURE_EXTRACTOR = 0,
    NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF   = 12,
    NDP120_DSP_DATA_FLOW_FUNCTION_NOAEC_FF  = 13,
    NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF_49 = 49
} syntiant_ndp120_flow_func_t;

#define NDP120_DSP_FLOW_RULE_IS_VALID(x) ((x).dst_type != NDP120_DSP_DATA_FLOW_DST_TYPE_NONE)
#define NDP120_DSP_FLOW_RULE_INVALIDATE(x) ((x).dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_NONE)

typedef enum {
    SYNTIANT_NDP120_DSP_FLOWSET_DISABLE_ALL = 255
} syntiant_ndp120_flowset_ops_t;

typedef enum {
    SYNTIANT_NDP120_DSP_FLOW_MULTI_INPUT = 0x01,
    SYNTIANT_NDP120_DSP_FLOW_DELAY_DST_EXEC = 0x02,
    SYNTIANT_NDP120_DSP_FLOW_MISC_INPUT_SIZE = 0x04
} syntiant_ndp120_algo_exec_t;

typedef struct {
    uint32_t src_param;
    /**< source of a flow tuple */
    uint32_t dst_param;
    /**< destination of a flow tuple */
    uint8_t  dst_type;
    /**< destination type (@syntiant_ndp120_flow_dst_type_t) of a flow tuple */
    int8_t   algo_config_index; /* -1 means "none" */
    /**< Index of a function's config/scratch area, if any */
    uint8_t  set_id;
    /**< Flowset id of this flow tuple */
    uint8_t  algo_exec_property;
    /**< Specifies algo exec property */
} ndp120_dsp_data_flow_rule_t;

/** @brief DSP Error codes
 */
enum {
    NDP120_DSP_FW_NO_ERROR = 0,
    NDP120_DSP_FW_WARNING_ARG,
    NDP120_DSP_FW_INVALID_ARG,
    NDP120_DSP_FW_NO_MEM,
    NDP120_DSP_ALGO_PROCESS_ERROR
};

#endif
