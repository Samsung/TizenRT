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
#ifndef SYNTIANT_PACKAGE_CONSTS_H
#define SYNTIANT_PACKAGE_CONSTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "syntiant_package_tags.h"

/**
 * @brief sizes defined for pkg parser's parsing granularity
 *
 * TAG_SIZE size of tag which is 4 bytes in A0 and B0
 * LENGTH_SIZE size of length which is 4 bytes in A0 and B0
 * VALID_SIZE size of validity of each section in config region
 * CRC_SIZE size of crc which is 4 bytes in A0 and B0
 * PARSER_SCRATCH_SIZE maximum size of scratch for parser to store data before
 * processing
 * VERSION_MAX_SIZE maximum possible size of version
 * LABELS_MAX_SIZE maximum possible size of labels
 * FW_MAX_SIZE maximum possible size of firmware
 */
enum syntiant_package_constants_e{
    TAG_SIZE = 4,                 /*size of tag in TLV*/
    LENGTH_SIZE = 4,              /*size of length in TLV*/
    CRC_SIZE = 4,                 /*size of CRC*/
    INPUT_CLK_RATE_SIZE = 4,      /*size of input clk rate*/
    PARSER_SCRATCH_SIZE = 2048,    /*maximum size of scratch to store parser's data*/
    VERSION_MAX_SIZE = 128,       /*maximum possible size of version*/
    LABELS_MAX_SIZE = 2048,       /*maximum possible size of labels*/
    FW_MAX_SIZE = 256*1024,       /*maximum possible size of firmware*/
    NN_NDP10X_MAX_SIZE = 0x48000,
    NN_MAX_SIZE = 0xA0000 + 0x40000, /* 640KB params + 256KB aliased data */
    NN_MAX_SIZE_115_A0 = 0x5F880 + 0x10000, /* 382KB params+ 64KB aliased data*/
    BOARD_PARAMS_V1_SIZE = 72,
    BOARD_PARAMS_V2_SIZE = 72,
    BOARD_PARAMS_V3_SIZE = 84,
    BOARD_PARAMS_V4_SIZE = 92,
    FC_CONFIG_PARAMS_V2_SIZE = 28,
    FC_CONFIG_PARAMS_V3_SIZE = 28,
    FC_PARAMS_METADATA_SIZE = 20,
    PH_PARAMS_SIZE = 24,
    PH_PARAMS_SIZE_V2 = 28,
    PH_PARAMS_METADATA_v1_SIZE = 20,
    PH_PARAMS_METADATA_v2_SIZE = 32,
    PH_PARAMS_METADATA_v3_SIZE = 44,
    HW_PARAMS_V2_SIZE = 192,
    PH_COLLECTION_PARAMS_SIZE = 80,
    NN_METADATA_V1_SIZE = 80,
    DSP_FLOW_COLLECTION_V1_SIZE = 8,
    DSP_FLOW_COLLECTION_V2_SIZE = 12,
    MCU_ORCHESTRATOR_V1_SIZE = 16,
    NN_PARAMS_ENCRYPTED_METADATA_V1_SIZE = 32,
    NN_SOFTMAX_SIZE = 4,
    MCU_ORCHESTRATOR_V3_SIZE = 20,
    NN_LABELS_MAX_LENGTH_V2 = 724,
    DSP_FW_VERSION_MAX_LENGTH_V2 = 32,
    MCU_FW_VERSION_MAX_LENGTH_V2 = 32,
    PACKAGE_VERSION_MAX_LENGTH_V2 = 48,
    PBI_VERSION_MAX_LENGTH = 16
};

/**
 * @brief valid bits for config sections
 *
 * SYNTIANT_CONFIG_LABELS_VALID if labels in config area valid
 * SYNTIANT_CONFIG_FW_VERSION_VALID if fw version in config area is valid
 * SYNTIANT_CONFIG_NN_VERSION_VALID if params version in config area is valid
 * SYNTIANT_CONFIG_PKG_VERSION_VALID if pkg version in config area is valid
 */
enum syntiant_config_validity_constants_e {

    /* from package loading */
    SYNTIANT_CONFIG_LABELS_VALID                = 0x00000001U,
    SYNTIANT_CONFIG_FW_VERSION_VALID            = 0x00000002U,
    SYNTIANT_CONFIG_NN_VERSION_VALID            = 0x00000004U,
    SYNTIANT_CONFIG_PKG_VERSION_VALID           = 0x00000008U,
    SYNTIANT_CONFIG_DSP_FW_VERSION_VALID        = 0x00000010U,
    SYNTIANT_CONFIG_PBI_VERSION_VALID           = 0x00000020U,

    /* from config (?), maybe packages*/

    SYNTIANT_CONFIG_INPUT_CLK_VALID             = 0x00000020U,

    /* pdm modes have two channels, thus the bits are spread out */
    SYNTIANT_CONFIG_PDM_SAMPLE_RATE_VALID       = 0x00000001U,
    SYNTIANT_CONFIG_PDM_RATE_VALID              = 0x00000004U,
    SYNTIANT_CONFIG_PDM_CLK_MODE_VALID          = 0x00000010U,
    SYNTIANT_CONFIG_PDM_MODE_VALID              = 0x00000040U,
    SYNTIANT_CONFIG_PDM_MAX_SPL_VALID           = 0x00000100U,
    /* end spread */

    SYNTIANT_CONFIG_PDM_MAIN_CLK_AT_LAST_CONFIG = 0x00000400U,
    SYNTIANT_CONFIG_EXT_CLK_FREQ_VALID          = 0x00000800U,
    SYNTIANT_CONFIG_MAIN_CLK_SRC_VALID          = 0x00010000U,
    SYNTIANT_CONFIG_PLL_CLK_FREQ_VALID          = 0x00020000U,
    SYNTIANT_CONFIG_FLL_CLK_FREQ_VALID          = 0x00040000U,
    SYNTIANT_CONFIG_PLL_PRESET_VALID            = 0x00080000U,
    SYNTIANT_CONFIG_FLL_PRESET_VALID            = 0x00100000U,

    /* don't forget to update if you change the above */
    SYNTIANT_CONFIG_VALID_MASK                  = 0xFFFFFU
};

/**
 * @brief Package Parsing Errors
 *
 */
enum syntiant_package_parsing_errors_e {
    SYNTIANT_PACKAGE_ERROR_NONE = 0,
    SYNTIANT_PACKAGE_ERROR_HEADER = -1,
    SYNTIANT_PACKAGE_ERROR_FIRMWARE_VERSION_STRING_V1 = -2,
    SYNTIANT_PACKAGE_ERROR_PAD = -3,
    SYNTIANT_PACKAGE_ERROR_CHECKSUM = -4,
    SYNTIANT_PACKAGE_ERROR_NN_VERSION_STRING = -5,
    SYNTIANT_PACKAGE_ERROR_NDP10X_NN_CONFIG_V1 = -6,
    SYNTIANT_PACKAGE_ERROR_NDP10X_NN_PARAMETERS_V1 = -7,
    SYNTIANT_PACKAGE_ERROR_FIRMWARE = -8,
    SYNTIANT_PACKAGE_ERROR_NDP10X_HW_CONFIG_V1 = -9,
    SYNTIANT_PACKAGE_ERROR_NN_LABELS_V1 = -10,
    SYNTIANT_PACKAGE_ERROR_NDP10X_HW_CONFIG_V2 = -11,
    SYNTIANT_PACKAGE_ERROR_NN_PARAMETERS_V2 = -12,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V1 = -13,
    SYNTIANT_PACKAGE_ERROR_NDP10X_B0_ENCRYPTED_FIRMWARE = -14,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V2 = -15,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V3 = -16,
    SYNTIANT_PACKAGE_ERROR_FIRMWARE_VERSION_STRING_V2 = -17,
    SYNTIANT_PACKAGE_ERROR_NN_VERSION_STRING_V2 = -18,
    SYNTIANT_PACKAGE_ERROR_PACKAGE_VERSION_STRING = -19,
    SYNTIANT_PACKAGE_ERROR_NDP10X_B0_NN_CONFIG_V1 = -20,
    SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V1 = -21,
    SYNTIANT_PACKAGE_ERROR_NDP10X_NN_PARAMETERS_V3 = -22,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V4 = -23,
    SYNTIANT_PACKAGE_ERROR_NN_LABELS_V2 = -24,
    SYNTIANT_PACKAGE_ERROR_NDP10X_B0_NN_CONFIG_V2 = -25,
    SYNTIANT_PACKAGE_ERROR_NDP10X_B0_NN_CONFIG_V3 = -26,
    SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V2 = -27,
    SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V3 = -28,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V5 = -29,
    SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V1 = -30,
    SYNTIANT_PACKAGE_ERROR_NN_METADATA_V1 = -31,
    SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V4 = -32,
    SYNTIANT_PACKAGE_ERROR_DSP_FLOW_COLLECTION_V1 = -33,
    SYNTIANT_PACKAGE_ERROR_MCU_ORCHESTRATOR_V1 = -34,
    SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V6 = -35,
    SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7 = -36,
    SYNTIANT_PACKAGE_ERROR_PBI_VERSION = -37,
    SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1 = -38,
    SYNTIANT_PACKAGE_ERROR_NDP120_B0_DSP_FLOW_COLLECTION_TAG = -39,
    SYNTIANT_PACKAGE_ERROR_SENSOR_CONFIG_V1 = -40,
    SYNTIANT_PACKAGE_ERROR_PIN_MUX_CONFIG_V1 = -41,
    SYNTIANT_PACKAGE_ERROR_PDM_CFG_V2 = -42,
    SYNTIANT_PACKAGE_ERROR_PH_EXCLUDED = -43,
    SYNTIANT_PACKAGE_ERROR_SENTINEL_END = -44, /*Not used, marks end reg errs*/
    SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR = -999,
    SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV = -1000
};

#define SYNTIANT_PACKAGE_ERROR_NAMES                                           \
    {   "none",                                                                \
        "header",                                                              \
        "firmware_version_string_v1",                                          \
        "pad",                                                                 \
        "checksum",                                                            \
        "nn_version_string",                                                   \
        "ndp10x_nn_config_v1",                                                 \
        "ndp10x_nn_parameters_v1",                                             \
        "firmware",                                                            \
        "ndp10x_hw_config_v1",                                                 \
        "nn_labels_v1",                                                        \
        "ndp10x_hw_config_v2",                                                 \
        "nn_parameters_v2",                                                    \
        "nn_ph_parameters_v1",                                                 \
        "ndp10x_b0_encrypted_firmware",                                        \
        "nn_ph_parameters_v2",                                                 \
        "nn_ph_parameters_v3",                                                 \
        "firmware_version_string_v2",                                          \
        "nn_version_string_v2",                                                \
        "package_version_string",                                              \
        "ndp10x_b0_nn_config_v1",                                              \
        "board_calibration_params_v1",                                         \
        "ndp10x_nn_parameters_v3",                                             \
        "nn_ph_parameters_v4",                                                 \
        "nn_labels_v2",                                                        \
        "ndp10x_b0_nn_config_v2",                                              \
        "ndp10x_b0_nn_config_v3",                                              \
        "board_calibration_params_v2",                                         \
        "board_calibration_params_v3",                                         \
        "nn_ph_parameters_v5",                                                 \
        "nn_ph_collection_v1",                                                 \
        "nn_metadata_v1",                                                      \
        "board_calibration_params_v4",                                         \
        "dsp_flow_collection_v1",                                              \
        "mcu_orchestrator_v1",                                                 \
        "nn_ph_parameters_v6",                                                 \
        "nn_ph_collection_v7",                                                 \
        "pbi_version",                                                         \
        "pdm_cfg_params_v1",                                                   \
        "dsp_flow_collection",                                                 \
        "sensor_config_v1",                                                    \
        "pin_mux_config_v1",                                                   \
        "pdm_cfg_v2",                                                          \
        "posterior_handling excluded in mcu firmware",                         \
        "sentinel_end",                                                        \
        "incrementally_parsing_error",                                         \
        "unknown_tlv"                                                          \
    }

#define SYNTIANT_PACKAGE_ERROR_NAME(e)                                         \
    ( ( (e) <= SYNTIANT_PACKAGE_ERROR_NONE &&                                  \
        (e) > SYNTIANT_PACKAGE_ERROR_SENTINEL_END                              \
      ) ? syntiant_package_error_names[-(e)] :                                 \
      ( (e) <= SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR &&                 \
        (e) >= SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV                              \
      ) ? syntiant_package_error_names[                                        \
            -(e)                                                               \
            +SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR                      \
            -SYNTIANT_PACKAGE_ERROR_SENTINEL_END                               \
          ] : "*unknown*"                                                      \
    )

extern char *syntiant_package_error_names[];

extern const char* syntiant_package_error_name(int e);


/**
 * @brief Package parsing mode when reading a chunk of package
 *
 */
enum syntiant_pkg_mode_e {
    PACKAGE_MODE_DONE = 0x0,
    PACKAGE_MODE_TAG_START = 0x1,
    PACKAGE_MODE_TAG_CONT = 0x2,
    PACKAGE_MODE_LENGTH_START = 0x3,
    PACKAGE_MODE_LENGTH_CONT = 0x4,
    PACKAGE_MODE_VALUE_START = 0x5,
    PACKAGE_MODE_VALUE_CONT = 0x6,
    PACKAGE_MODE_VALID_PARTIAL_VALUE = 0x7,
    PACKAGE_MODE_NO_PARTIAL_VALUE = 0x8
};

/**
 * @brief Posterior action enum
 *
 */
enum syntiant_packager_ph_action_e {
    SYNTIANT_PH_ACTION_MATCH = 0,
    SYNTIANT_PH_ACTION_STATE = 1,
    SYNTIANT_PH_ACTION_STAY = 2
};


/**
 * @brief DNN inputs
 *
 */
enum syntiant_package_dnn_input_e {
    SYNTIANT_PACKAGE_DNN_INPUT_NONE = 0,
    SYNTIANT_PACKAGE_DNN_INPUT_PDM0 = 1,
    SYNTIANT_PACKAGE_DNN_INPUT_PDM1 = 2,
    SYNTIANT_PACKAGE_DNN_INPUT_PDM_SUM = 3,
    SYNTIANT_PACKAGE_DNN_INPUT_I2S_LEFT = 4,
    SYNTIANT_PACKAGE_DNN_INPUT_I2S_RIGHT = 5,
    SYNTIANT_PACKAGE_DNN_INPUT_I2S_SUM = 6,
    SYNTIANT_PACKAGE_DNN_INPUT_I2S_MONO = 7,
    SYNTIANT_PACKAGE_DNN_INPUT_I2S_DIRECT = 8,
    SYNTIANT_PACKAGE_DNN_INPUT_SPI = 9,
    SYNTIANT_PACKAGE_DNN_INPUT_SPI_DIRECT = 10
};

/**
 * @brief Posterior handler collection parser states
 *
 */
enum syntiant_phc_parser_state_e {
    PHC_PARSE_COLLECTION_PARAMS = 1,
    PHC_PARSE_PH_PARAMS = 2,
    PHC_PARSE_STATE_PARAMS = 3,
    PHC_PARSE_CLASS_PARAMS = 4
};

/**
 * @brief Neural network metadata parser states
 *
 */
enum syntiant_neural_network_metadata_parser_state_e {
    NNM_PARSE_HEADER = 1,
    NNM_PARSE_BASE_META = 2,
    NNM_PARSE_INP_SIZE = 3,
    NNM_PARSE_COORD = 4,
    NNM_PARSE_CACHE_INST = 5,
    NNM_PARSE_INDICES_LAYER = 6,
    NNM_PARSE_INDICES_INPUT = 7,
    NNM_PARSE_INDICES_OUTPUT = 8,
    NNM_PARSE_PADDING = 9
};

/**
 * @brief sensor config parser states
 *
 */
enum syntiant_sensor_config_parser_state_e {
    SENC_PARSE_BASE_CONFIG = 1,
    SENC_PARSE_IFC_TRANSACTION = 2,
    SENC_PARSE_DONE = 3
};

/* NDP120_PCM_DATA_FLOW_RULE_MAX = 16
 * NDP120_FUNC_DATA_FLOW_RULE_MAX = 32
 * NDP120_NN_DATA_FLOW_RULE_MAX = 32
 * NDP120_MCU_DATA_FLOW_RULE_MAX = 8
 */
#define NN_FLOW_RULES_TOTAL (16 + 32 + 32 + 8)
#define NN_FLOW_METADATA_SIZE (DSP_FLOW_COLLECTION_V1_SIZE * NN_FLOW_RULES_TOTAL)

/**
 * @brief TLV input consTANTS for pdm, farrow, gain, decimation, i2s
 *
 */
/* tlv 85 (0x55) constants */
enum ndp120_tlv_pdm_params_v1_constants_e {
    NDP120_TLV_PDM_V1_AUD0                             = 0,
    NDP120_TLV_PDM_V1_AUD1                             = 1,
    NDP120_TLV_PDM_V1_AUD_MODE_OFF                     = 0,
    NDP120_TLV_PDM_V1_AUD_MODE_LEFT                    = 1,
    NDP120_TLV_PDM_V1_AUD_MODE_RIGHT                   = 2,
    NDP120_TLV_PDM_V1_AUD_MODE_STEREO                  = 3,
    NDP120_TLV_PDM_V1_AUD_CLK_MODE_EXTERNAL            = 0,
    NDP120_TLV_PDM_V1_AUD_CLK_MODE_INTERNAL            = 1,
    NDP120_TLV_PDM_V1_AUD_CLK_MODE_DUAL_INTERNAL       = 2,
    NDP120_TLV_PDM_V1_AUD_CLK_RATE_LOW                 = 768000,
    NDP120_TLV_PDM_V1_AUD_CLK_RATE_HIGH                = 3072000,
    NDP120_TLV_PDM_V1_AUD_SAMPLE_RATE_LOW              = 16000,
    NDP120_TLV_PDM_V1_AUD_SAMPLE_RATE_HIGH             = 48000,
    NDP120_TLV_PDM_V1_AUD_NUM_DELAY_MAX                = 63,
    NDP120_TLV_PDM_V1_FARROW_BYPASS_DISABLE            = 0,
    NDP120_TLV_PDM_V1_FARROW_BYPASS_ENABLE             = 1,
    NDP120_TLV_PDM_V1_FARROW_PHASE_STEP_HIGH           = 0x08000000,
    NDP120_TLV_PDM_V1_DECIMATION_INSHIFT_HIGH          = 13,
    NDP120_TLV_PDM_V1_DECIMATION_OUTSHIFT_HIGH         = 7,
    NDP120_TLV_PDM_V1_REMOVAL_MODE_OFF                 = 0,
    NDP120_TLV_PDM_V1_REMOVAL_MODE_STATIC              = 1,
    NDP120_TLV_PDM_V1_REMOVAL_MODE_ON                  = 2,
    NDP120_TLV_PDM_V1_GAIN_SHIFT_LEFT                  = 0,
    NDP120_TLV_PDM_V1_GAIN_SHIFT_RIGHT                 = 1,
    NDP120_TLV_PDM_V1_GAIN_SHIFT_CNT_MAX               = 7,
    NDP120_TLV_PDM_V1_GAIN_CHANGE_FALSE                = 0,
    NDP120_TLV_PDM_V1_GAIN_CHANGE_TRUE                 = 1,
    NDP120_TLV_PDM_V1_GAIN_FINEGRAIN_MUL_HIGH          = 0x3FFF
};

/* tlv 86 (0x56) constants */
enum ndp120_tlv_i2s_in_constants_e {
    NDP120_TLV_I2S_IN_AUD0                          = 0,
    NDP120_TLV_I2S_IN_AUD1                          = 1,
    NDP120_TLV_I2S_IN_I2S_EDGE_POS                  = 0,
    NDP120_TLV_I2S_IN_I2S_EDGE_NEG                  = 1,
    NDP120_TLV_I2S_IN_I2S_MODE_STANDARD             = 0,
    NDP120_TLV_I2S_IN_I2S_MODE_BURST_DNN            = 1,
    NDP120_TLV_I2S_IN_I2S_MODE_TDM                  = 2,
    NDP120_TLV_I2S_IN_I2S_MODE_PDM                  = 3,
    NDP120_TLV_IS2_IN_SAMPLE_RATE_LOW               = 16000,
    NDP120_TLV_IS2_IN_SAMPLE_RATE_HIGH              = 48000,
    NDP120_TLV_I2S_IN_FRAMESIZE_LOW                 = 8,
    NDP120_TLV_I2S_IN_FRAMESIZE_HIGH                = 32,
    NDP120_TLV_I2S_IN_SAMPLESIZE_LOW                = 8,
    NDP120_TLV_I2S_IN_SAMPLESIZE_HIGH               = 32,
    NDP120_TLV_I2S_IN_MSB_INDEX_LOW                 = 8,
    NDP120_TLV_I2S_IN_MSB_INDEX_HIGH                = 32,
    NDP120_TLV_I2S_IN_I2S_DELAYED_SENS_NORMAL       = 0,
    NDP120_TLV_I2S_IN_I2S_DELAYED_SENS_DELAYED      = 1,
    NDP120_TLV_I2S_IN_I2S_PACKED_STEREO             = 0,
    NDP120_TLV_I2S_IN_I2S_PACKED_MONO               = 1,
    NDP120_TLV_I2S_IN_I2S_ENABLE_FALSE              = 0,
    NDP120_TLV_I2S_IN_I2S_ENABLE_TRUE               = 1,
    NDP120_TLV_I2S_IN_FARROW_BYPASS_DISABLE         = 0,
    NDP120_TLV_I2S_IN_FARROW_BYPASS_ENABLE          = 1,
    NDP120_TLV_I2S_IN_FARROW_PHASE_STEP_HIGH        = 0x08000000,
    NDP120_TLV_I2S_IN_AUD_NUM_DELAY_MAX             = 63,
    NDP120_TLV_I2S_IN_GAIN_SHIFT_LEFT               = 0,
    NDP120_TLV_I2S_IN_GAIN_SHIFT_RIGHT              = 1,
    NDP120_TLV_I2S_IN_GAIN_SHIFT_CNT_MAX            = 7,
    NDP120_TLV_I2S_IN_GAIN_CHANGE_FALSE             = 0,
    NDP120_TLV_I2S_IN_GAIN_CHANGE_TRUE              = 1,
    NDP120_TLV_I2S_IN_GAIN_FINEGRAIN_MUL_HIGH       = 0x3FFF
};

/* tlv 87 (0x57) constants */
enum ndp120_tlv_i2s_out_constants_e {
    NDP120_TLV_I2S_OUT_AUD2                         = 2,
    NDP120_TLV_IS2_OUT_SAMPLE_RATE_LOW              = 16000,
    NDP120_TLV_IS2_OUT_SAMPLE_RATE_HIGH             = 48000,
    NDP120_TLV_IS2_OUT_I2S_EDGE_POS                 = 0,
    NDP120_TLV_IS2_OUT_I2S_EDGE_NEG                 = 1,
    NDP120_TLV_IS2_OUT_I2S_MODE_STANDARD            = 0,
    NDP120_TLV_IS2_OUT_I2S_MODE_BURST_DNN           = 1,
    NDP120_TLV_IS2_OUT_I2S_MODE_TDM                 = 2,
    NDP120_TLV_IS2_OUT_I2S_MODE_PDM                 = 3,
    NDP120_TLV_I2S_OUT_FRAMESIZE_LOW                = 8,
    NDP120_TLV_I2S_OUT_FRAMESIZE_HIGH               = 32,
    NDP120_TLV_I2S_OUT_SAMPLESIZE_LOW               = 8,
    NDP120_TLV_I2S_OUT_SAMPLESIZE_HIGH              = 32,
    NDP120_TLV_I2S_OUT_MSB_INDEX_LOW                = 8,
    NDP120_TLV_I2S_OUT_MSB_INDEX_HIGH               = 32,
    NDP120_TLV_IS2_OUT_I2S_DELAYED_SENS_NORMAL      = 0,
    NDP120_TLV_IS2_OUT_I2S_DELAYED_SENS_DELAYED     = 1,
    NDP120_TLV_IS2_OUT_I2S_PACKED_STEREO            = 0,
    NDP120_TLV_IS2_OUT_I2S_PACKED_MONO              = 1,
    NDP120_TLV_IS2_OUT_I2S_ENABLE_FALSE             = 0,
    NDP120_TLV_IS2_OUT_I2S_ENABLE_TRUE              = 1,
    NDP120_TLV_IS2_OUT_I2S_AUD2_OUT_MODE_CONTROLLER = 0,
    NDP120_TLV_IS2_OUT_I2S_AUD2_OUT_MODE_TARGET     = 1,
    NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_TYPE_PCM        = 0,
    NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_TYPE_FUNCTION   = 1,
    NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_PARAM_LOW       = 1,
    NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_PARAM_HIGH      = 12,
    NDP120_TLV_IS2_OUT_FARROW_BYPASS_DISABLE        = 0,
    NDP120_TLV_IS2_OUT_FARROW_BYPASS_ENABLE         = 1,
    NDP120_TLV_I2S_OUT_FARROW_PHASE_STEP_HIGH       = 0x08000000,
    NDP120_TLV_I2S_OUT_AUD_NUM_DELAY_MAX            = 63,
    NDP120_TLV_IS2_OUT_GAIN_SHIFT_LEFT              = 0,
    NDP120_TLV_IS2_OUT_GAIN_SHIFT_RIGHT             = 1,
    NDP120_TLV_I2S_OUT_GAIN_SHIFT_CNT_MAX           = 7,
    NDP120_TLV_IS2_OUT_GAIN_CHANGE_FALSE            = 0,
    NDP120_TLV_IS2_OUT_GAIN_CHANGE_TRUE             = 1,
    NDP120_TLV_I2S_OUT_GAIN_FINEGRAIN_MUL_HIGH      = 0x3FFF
};

/* tlv 88 (0x58) constants */
enum ndp12_tlv_dsp_sync_constants_e {
    NDP120_TLV_DSP_SYNC_AUD0                        = 0,
    NDP120_TLV_DSP_SYNC_AUD1                        = 1,
    NDP120_TLV_DSP_SYNC_AUD2                        = 2,
    NDP120_TLV_DSP_SYNC_DSP_SYNC_MODE_OFF           = 0,
    NDP120_TLV_DSP_SYNC_DSP_SYNC_MODE_ALIGN         = 1,
    NDP120_TLV_DSP_SYNC_DSP_SYNC_MODE_FULL          = 2
};

/* tlv 96 (0x60) constants */
enum ndp120_tlv_pdm_params_v2_constants_e {
    NDP120_TLV_PDM_V2_AUD0                             = 0,
    NDP120_TLV_PDM_V2_AUD1                             = 1,
    NDP120_TLV_PDM_V2_AUD_MODE_OFF                     = 0,
    NDP120_TLV_PDM_V2_AUD_MODE_LEFT                    = 1,
    NDP120_TLV_PDM_V2_AUD_MODE_RIGHT                   = 2,
    NDP120_TLV_PDM_V2_AUD_MODE_STEREO                  = 3,
    NDP120_TLV_PDM_V2_AUD_CLK_MODE_EXTERNAL            = 0,
    NDP120_TLV_PDM_V2_AUD_CLK_MODE_INTERNAL            = 1,
    NDP120_TLV_PDM_V2_AUD_CLK_MODE_DUAL_INTERNAL       = 2,
    NDP120_TLV_PDM_V2_AUD_CLK_RATE_LOW                 = 768000,
    NDP120_TLV_PDM_V2_AUD_CLK_RATE_HIGH                = 3072000,
    NDP120_TLV_PDM_V2_AUD_SAMPLE_RATE_LOW              = 16000,
    NDP120_TLV_PDM_V2_AUD_SAMPLE_RATE_HIGH             = 48000,
    NDP120_TLV_PDM_V2_AUD_NUM_DELAY_MAX                = 63,
    NDP120_TLV_PDM_V2_FARROW_BYPASS_DISABLE            = 0,
    NDP120_TLV_PDM_V2_FARROW_BYPASS_ENABLE             = 1,
    NDP120_TLV_PDM_V2_FARROW_PHASE_STEP_HIGH           = 0x08000000,
    NDP120_TLV_PDM_V2_DECIMATION_INSHIFT_HIGH          = 13,
    NDP120_TLV_PDM_V2_DECIMATION_OUTSHIFT_HIGH         = 7,
    NDP120_TLV_PDM_V2_REMOVAL_MODE_OFF                 = 0,
    NDP120_TLV_PDM_V2_REMOVAL_MODE_STATIC              = 1,
    NDP120_TLV_PDM_V2_REMOVAL_MODE_ON                  = 2,
    NDP120_TLV_PDM_V2_GAIN_SHIFT_LEFT                  = 0,
    NDP120_TLV_PDM_V2_GAIN_SHIFT_RIGHT                 = 1,
    NDP120_TLV_PDM_V2_GAIN_SHIFT_CNT_MAX               = 7,
    NDP120_TLV_PDM_V2_GAIN_CHANGE_FALSE                = 0,
    NDP120_TLV_PDM_V2_GAIN_CHANGE_TRUE                 = 1,
    NDP120_TLV_PDM_V2_GAIN_FINEGRAIN_MUL_HIGH          = 0x3FFF,
    NDP120_TLV_PDM_V2_BW_COEFF_HIGH                    = 0x00FFFFFF
};

/**
 * @brief Sensor config enums
 *
 */
enum syntiant_package_sensor_config_constants_e {
    SYNTIANT_PACKAGE_NUM_GPIO_PINS_MAX          = 2,
    SYNTIANT_PACKAGE_NUM_SENSORS_MAX            = 4,
    SYNTIANT_PACKAGE_NUM_IFC_BYTES_MAX          = 12,
    SYNTIANT_PACKAGE_NUM_PARAMETERS_MAX         = 16,
    SYNTIANT_PACKAGE_IFC_TRANSACTIONS_MAX       = 16
};

#ifdef __cplusplus
}
#endif

#endif
