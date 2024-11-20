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
#ifndef SYNTIANT_PACKAGE_H
#define SYNTIANT_PACKAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_packager/syntiant_package_consts.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>

/* the following will prevent some OS targets from redefining offsetof */
#ifdef __GNUC__
#undef offsetof
#define offsetof __builtin_offsetof
#endif

/**
 * @brief General TLV structure
 */
typedef struct tlv_ {
    uint32_t tag;
    uint32_t length;
    int8_t val[1];    /* infinitely long array */
} tlv_t;

/**
 * @brief calculate CRC32 on bytes without using any library
 *
 * @param[in] bytes array of bytes
 * @param len len of array
 * @return uint32_t CRC32 value
 */
uint32_t crc32_no_lib(uint8_t *bytes, size_t len);

/**
 * @brief get Intial CRC32 value
 *
 * @return uint32_t intermediate CRC32 value
 */
uint32_t crc32_no_lib_init(void);

/**
 * @brief update the CRC32 using given bytes
 *
 * @param crc initial or current CRC32 value
 * @param bytes array of bytes
 * @param len length of array
 * @return uint32_t intermediate CRC32 value
 */
uint32_t crc32_no_lib_update(uint32_t crc, uint8_t *bytes, size_t len);

/**
 * @brief finalize the CRC32 value
 *
 * @param crc intermediate CRC32 value
 * @return uint32_t final CRC32 value
 */
uint32_t crc32_no_lib_finalize(uint32_t crc);

#if defined(WINDOWS_KERNEL) || defined(_MSC_VER) || defined(__MINGW32__)
#pragma pack(push)
#pragma pack(1)

#if defined(SYNTIANT_PACK)
#undef SYNTIANT_PACK
#define SYNTIANT_PACK
#endif

#endif
/**
 * @brief fc parameters v2
 *
 * @param num_features totall number of input features
 * @param num_static_features total number of static features
 * @param dnn_frame_size number of features to feed NN before it stars
 * @param num_nodes num of num nodes per layer
 * @param scale_factor scale factor for each layer
 * @param num_biases num of biases for each layer
 * @param input_clipping_threshold  input features <= this value would be made zero.
 * @param dnn_run_threshold  if any input feature >= this threshold, NN would run.
 * @param quantization_scheme linear or squared quantization for each layer.
 * @param out_shift applies 2^-x on DNN summation, multiply scale_factor 2^x
*/
typedef struct syntiant_fc_params_v2_t_{
    uint16_t num_features;
    uint16_t num_static_features;
    uint16_t dnn_frame_size;
    uint8_t num_nodes[4];
    uint8_t scale_factor[4];
    uint8_t num_biases[4];
    uint8_t input_clipping_threshold;
    uint8_t dnn_run_threshold;
    uint8_t quantization_scheme[4];
    uint8_t out_shift[4];
} syntiant_fc_params_v2_t;

SYNTIANT_CASSERT(sizeof(syntiant_fc_params_v2_t) == FC_CONFIG_PARAMS_V2_SIZE,
                 "Unexpected size for syntiant_fc_params_v2_t")
/**
 * @brief fc parameters v3
 *
 * @param num_features totall number of input features
 * @param num_static_features total number of static features
 * @param dnn_frame_size number of features to feed NN before it stars
 * @param num_nodes num of num nodes per layer
 * @param scale_factor scale factor for each layer
 * @param num_biases num of biases for each layer
 * @param input_clipping_threshold  input features <= this value would be made zero
 * @param dnn_run_threshold  if any input feature >= this threshold, NN would run
 * @param quantization_scheme linear or squared quantization for each layer
 * @param out_shift applies 2^-x on DNN summation, multiply scale_factor 2^x
*/
typedef struct syntiant_fc_params_v3_t_{
    uint8_t dnn_run_threshold;
    uint8_t input_clipping_threshold;
    uint16_t num_features;
    uint16_t num_static_features;
    uint16_t dnn_frame_size;
    uint8_t num_nodes[4];
    uint8_t scale_factor[4];
    uint8_t num_biases[4];
    uint8_t quantization_scheme[4];
    uint8_t out_shift[4];
} syntiant_fc_params_v3_t;
SYNTIANT_CASSERT(sizeof(syntiant_fc_params_v3_t) == FC_CONFIG_PARAMS_V3_SIZE,
                 "Unexpected size for syntiant_fc_params_v3_t")

/**
 * @brief union for storing fc parameters v2 and v3
 *
 * @param fc_params_v2 fc params v2
 * @param fc_params_v3 fc params v3
 */
typedef union syntiant_fc_params_t_{
    syntiant_fc_params_v2_t fc_params_v2;
    syntiant_fc_params_v3_t fc_params_v3;
} syntiant_fc_params_t;


/**
 * @brief metadata for fc params
 *
 * @param params_to_read reserved
 * @param num_features number of features
 * @param num_static_features total number of static features
 * @param dnn_frame_size number of features to fee NN before it starts
 * @param num_nodes num of num nodes per layer
 * @param num_biases num of biases for each layer
*/
typedef struct syntiant_fc_params_metadata_t_{
    uint32_t params_to_read;
    uint16_t num_features;
    uint16_t num_static_features;
    uint16_t dnn_frame_size;
    uint16_t rsvd;
    uint8_t num_nodes[4];
    uint8_t num_biases[4];
} syntiant_fc_params_metadata_t;
SYNTIANT_CASSERT(sizeof(syntiant_fc_params_metadata_t)
                 == FC_PARAMS_METADATA_SIZE,
                 "Unexpected size for syntiant_fc_params_metadata_t")

/**
 * @brief general information about number of states and classes and recently parsed timeout
 * @param num_states total number of states
 * @param num_classes total number of classes
 * @param timeout timeout for the current state
 * @param cur_state current state
 * @param cur_class current class
 */
typedef struct syntiant_ph_params_metadata_v1_t_{
    uint32_t num_states;
    uint32_t num_classes;
    uint32_t timeout;
    uint32_t cur_state;
    uint32_t cur_class;
} syntiant_ph_params_metadata_v1_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_params_metadata_v1_t)
                 == PH_PARAMS_METADATA_v1_SIZE,
                 "Unexpected size for syntiant_ph_params_metadata_v1_t")

/**
 * @brief general information about number of states and classes and recently parsed timeout
 * @param num_states total number of states
 * @param num_classes total number of classes
 * @param ph_type posterior handler frame processor function type
 * @param timeout timeout for the current state
 * @param cur_state current state
 * @param cur_class current class
 */
typedef struct syntiant_ph_params_metadata_v2_t_{
    uint32_t num_states;
    uint32_t num_classes;
    uint32_t ph_type;
    uint32_t timeout;
    uint32_t timeout_action;
    uint32_t timeout_action_arg;
    uint32_t cur_state;
    uint32_t cur_class;
} syntiant_ph_params_metadata_v2_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_params_metadata_v2_t)
                  == PH_PARAMS_METADATA_v2_SIZE,
                 "Unexpected size for syntiant_ph_params_metadata_v2_t")

/**
 * @brief general information about number of states and classes and recently parsed timeout
 * @param num_states total number of states
 * @param num_classes total number of classes
 * @param ph_type posterior handler frame processor function type
 * @param timeout timeout for the current state
 * @param cur_state current state
 * @param cur_class current class
 * @param adaptive_frames
 * @param adaptive_denominator
 * @param adaptive_max_updates
 */
typedef struct syntiant_ph_params_metadata_v3_t_{
    uint32_t adaptive_frames;
    uint32_t adaptive_denominator;
    uint32_t adaptive_max_updates;
    uint32_t num_states;
    uint32_t num_classes;
    uint32_t ph_type;
    uint32_t timeout;
    uint32_t timeout_action;
    uint32_t timeout_action_arg;
    uint32_t cur_state;
    uint32_t cur_class;
} syntiant_ph_params_metadata_v3_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_params_metadata_v3_t)
                  == PH_PARAMS_METADATA_v3_SIZE,
                 "Unexpected size for syntiant_ph_params_metadata_v3_t")

/**
 * @brief union for ph params metadata
 *
 * @param v1 ph params metadata v1
 * @param v2 ph params metadata v2
 * @param v3 ph params metadata v3
 */

typedef union syntiant_ph_params_metadata_t_{
    syntiant_ph_params_metadata_v1_t v1;
    syntiant_ph_params_metadata_v2_t v2;
    syntiant_ph_params_metadata_v3_t v3;
} syntiant_ph_params_metadata_t;


/**
 * @brief posterior params for class
 *
 * @param phwin window
 * @param phth threshold
 * @param phbackoff backoff
 * @param phaction action
 * @param pharg argument
 * @param phqueuesize smoothing queue size
 */
typedef struct syntiant_ph_params_t_{
    uint32_t phwin;
    uint32_t phth;
    uint32_t phbackoff;
    uint32_t phaction;
    uint32_t pharg;
    uint32_t phqueuesize;
} syntiant_ph_params_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_params_t)
                 == PH_PARAMS_SIZE,
                 "Unexpected size for syntiant_ph_params_t")
/**
 * @brief posterior params for class
 *
 * @param phwin window
 * @param phth threshold
 * @param phbackoff backoff
 * @param phaction action
 * @param pharg argument
 * @param phqueuesize smoothing queue size
 * @param adaptive_threshold_on
 */
typedef struct syntiant_ph_param_t_{
    uint32_t phwin;
    uint32_t phth;
    uint32_t phbackoff;
    uint32_t phaction;
    uint32_t pharg;
    uint32_t phqueuesize;
    uint32_t adaptive_threshold_on;
} syntiant_ph_params_v2_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_params_v2_t)
                 == PH_PARAMS_SIZE_V2,
                 "Unexpected size for syntiant_ph_params_v2_t")

/* TAG_NN_PH_PARAMETERS_V7 (TLV 77 0x4D) */
typedef struct {
    struct {
        uint32_t num_ph;
    } collection;
    struct {
        struct {
            uint32_t algorithm_type;
            uint32_t version;
            uint32_t nn_idx;
            uint32_t num_states;
            uint32_t num_classes;
        } param;
        struct {
            uint32_t adaptive_frames;
            uint32_t adaptive_denominator;
            uint32_t adaptive_max_updates;
        } adaptive;
    } ph_;
    struct {
        struct {
            uint32_t timeout;
            uint32_t timeout_action;
        } param;
        union {
            struct {
                uint32_t timeout_action_arg;
            } scp;
            struct {
                uint32_t timeout_action_match_arg;
                uint32_t timeout_action_move_arg;
            } mcp;
        } u;
    } state_;
    struct {
        struct {
            uint32_t window;
            uint32_t threshold;
        } param;
        union {
            struct {
                uint32_t backoff;
                uint32_t action;
                uint32_t action_arg;
                uint32_t smoothing_queue_size;
                uint32_t adaptive_threshold;
            } scp;
            struct {
                uint32_t action;
                uint32_t action_match_arg;
                uint32_t action_move_arg;
                uint32_t smoothing_queue_size;
            } mcp;
        } u;
    } class_;
    struct {
        unsigned int cur_ph;
        unsigned int cur_state;
        unsigned int cur_class;
        unsigned int parsing;
        unsigned int parsed;
    } parser;
} syntiant_ph_collection_params_v7_t;

enum pbi_version_chip_type_t_ {
    PBI_CHIP_TYPE_NDP10X_B0 = 1,
    PBI_CHIP_TYPE_NDP115_A0 = 2,
    PBI_CHIP_TYPE_NDP120_B0 = 3,
    PBI_CHIP_TYPE_NDP200_A0 = 4
};

enum nn_ph_alorithm_type_t_ {
    ALGORITHM_TYPE_SCP = 0,
    ALGORITHM_TYPE_MCP = 1
};

enum nn_ph_action_scp {
    SCP_ACTION_MATCH = 0,
    SCP_ACTION_STATE = 1,
    SCP_ACTION_STAY = 2
};
enum nn_ph_action_mcp {
    MCP_ACTION_STAY = 0,
    MCP_ACTION_MOVE = 1,
    MCP_ACTION_MATCH_AND_MOVE = 2
};

/**
 * @brief Posterior handler collection parameters used for parsing.
 */
typedef struct syntiant_ph_collection_params_t_{
    struct {
        uint32_t num_ph; /* add gree space to the struct for the future */
    } collection;
    struct {
        uint32_t num_classes;
        uint32_t num_states;
        uint32_t ph_type;
    } ph;
    struct {
        uint32_t timeout;
        uint32_t timeout_action;
        uint32_t timeout_action_arg0;
        uint32_t timeout_action_arg1;
    } state;
    struct {
        uint32_t window;
        uint32_t threshold;
        uint32_t backoff;
        uint32_t queuesize;
        uint32_t action;
        uint32_t action_arg0;
        uint32_t action_arg1;
    } class_;
    struct {
        uint32_t cur_ph;
        uint32_t cur_state;
        uint32_t cur_class;
        uint32_t parsing;
        uint32_t parsed;
    } parser;
} syntiant_ph_collection_params_t;
SYNTIANT_CASSERT(sizeof(syntiant_ph_collection_params_t)
                  == PH_COLLECTION_PARAMS_SIZE,
                 "Unexpected size for syntiant_ph_collection_params_t")

/**
 * @brief Neural network metadata parameters used for parsing.
 */
typedef struct syntiant_nn_metadata_v1_t_{
    struct {
        uint8_t nn_num;
        uint8_t dnn_mode;
        uint16_t padding;
    } initial_meta;
    struct {
        uint32_t num_layers;
        uint32_t is_nn_cached;
        uint32_t nn_input_isa_idx;
        uint32_t nn_output_isa_idx;
        uint32_t nn_input_layer_type;
    } base_meta;
    struct {
        uint32_t x;
        uint32_t y;
        uint32_t z;
    } inp_size;
    struct {
        uint32_t input_coord;
        uint32_t output_coord;
    } coords;
    struct {
        uint32_t input_base_coord_max;
        uint32_t output_base_coord_max;
        uint32_t input_base_coord_add;
        uint16_t input_offset_add;
        uint16_t input_offset_max;
        uint16_t output_base_coord_add;
        uint16_t output_base_coord_stride;
    } cache_params;
    struct {
        uint32_t cur_nn;
        uint32_t cur_layer;
        uint32_t parsing;
        uint32_t parsed;
    } parser;
} syntiant_nn_metadata_v1_t;
SYNTIANT_CASSERT(sizeof(syntiant_nn_metadata_v1_t)
                  == NN_METADATA_V1_SIZE,
                 "Unexpected size for syntiant_nn_metadata_v1_t")

enum {
    METADATA_MAX_NUM_LAYERS = 32,
    METADATA_MAX_NUM_INPUTS = 4,
    METADATA_MAX_NUM_OUTPUTS = 4
};

/**
 * @brief Neural network metadata parameters used for parsing.
 */
typedef struct syntiant_nn_metadata_v2_t_ {
    struct {
        uint8_t nn_num;
        uint8_t dnn_mode;
        uint16_t padding;
    } initial_meta;
    struct {
        uint8_t num_layers;
        uint8_t num_inputs;
        uint8_t num_outputs;
        uint8_t is_cached;
    } base_meta;

    uint8_t layers[METADATA_MAX_NUM_LAYERS];
    uint8_t inputs[METADATA_MAX_NUM_INPUTS];
    uint8_t outputs[METADATA_MAX_NUM_OUTPUTS];
    struct {
        uint32_t input_base_coord_max;
        uint32_t output_base_coord_max;
        uint32_t input_base_coord_add;
        uint16_t input_offset_add;
        uint16_t input_offset_max;
        uint16_t output_base_coord_add;
        uint16_t output_base_coord_stride;
    } cache_params;

    struct {
        uint32_t cur_subgraph;
        uint32_t cur_layer;
        uint32_t parsing;
        uint32_t parsed;
    } parser;
} syntiant_nn_metadata_v2_t;

typedef union syntiant_nn_metadata_t_{
    syntiant_nn_metadata_v1_t v1;
    syntiant_nn_metadata_v2_t v2;
} syntiant_nn_metadata_t;

typedef struct {
    uint16_t algo_id;
    uint8_t algo_config_index;
    uint8_t pad;
} syntiant_algo_attach_v1_t;

typedef union syntiant_algo_attach_t_ {
    syntiant_algo_attach_v1_t v1;
} syntiant_algo_attach_t;

/**
 * @brief A series of DSP flow rules with their set id.
 */
typedef struct syntiant_dsp_flow_collection_v1_t_{
    uint8_t set_id;
    uint8_t src_type;
    uint8_t src_param;
    uint8_t dst_type;
    uint8_t dst_param;
    int8_t  algo_config_index;
    uint8_t pad[2];
} syntiant_dsp_flow_collection_v1_t;
SYNTIANT_CASSERT(sizeof(syntiant_dsp_flow_collection_v1_t)
                  == DSP_FLOW_COLLECTION_V1_SIZE,
                 "Unexpected size for syntiant_dsp_flow_collection_v1_t")

typedef struct syntiant_dsp_flow_collection_v2_t_{
    uint8_t set_id;
    uint8_t src_type;
    uint8_t dst_type;
    int8_t  algo_config_index;
    uint32_t src_param;
    uint32_t dst_param;
} syntiant_dsp_flow_collection_v2_t;
SYNTIANT_CASSERT(sizeof(syntiant_dsp_flow_collection_v2_t)
                  == DSP_FLOW_COLLECTION_V2_SIZE,
                 "Unexpected size for syntiant_dsp_flow_collection_v2_t")

typedef union syntiant_dsp_flow_collection_t_{
    syntiant_dsp_flow_collection_v1_t v1;
    syntiant_dsp_flow_collection_v2_t v2;
} syntiant_dsp_flow_collection_t;

/* This define should match with the NDP120_DSP_ALGO_CONFIG_LEN_BYTES
 * in syntiant-dsp-firmware/ndp120_dsp_algo_interface.h
 * We can not include that interface here as it breaks the linux driver
 * compilation
 */
#define NDP120_ALGO_CONFIG_AREA_LEN (64)

typedef struct {
    uint32_t index;
    uint8_t config[NDP120_ALGO_CONFIG_AREA_LEN];
} syntiant_dsp_fe_config_data_t ;


#define NDP120_ALGO_CONFIG_V2_AREA_LEN (256)
typedef union {
    struct {
        uint32_t signature;
        uint32_t frame_step_ms;
        uint32_t power_offset;
        uint32_t preemphasis_expoonent;
        uint16_t num_filter_banks;
        int8_t algo_config_index;
        uint8_t secondary_validation;
        uint16_t melbin_boundary[1];
    } param;
    struct {
        uint8_t config[NDP120_ALGO_CONFIG_V2_AREA_LEN];
    } data;
} syntiant_algo_config_t;

#define NUM_FILTER_BANKS_COUNT_MAX  \
    ( ( NDP120_ALGO_CONFIG_V2_AREA_LEN - \
        offsetof(syntiant_algo_config_t, param.melbin_boundary) \
      ) / sizeof(uint16_t) )

typedef struct {
    uint8_t src_type;
    uint8_t src_param;
    uint8_t src_output_slot;
    uint8_t dst_input_slot;
} syntiant_algo_multi_config_params_t;

typedef struct {
    uint32_t signature;
    uint8_t no_of_inputs;
    uint8_t exec_type;
    uint16_t reserved;
    syntiant_algo_multi_config_params_t data[NDP120_DSP_ALGO_CONFIG_MAX_COUNT];
} syntiant_multi_input_algo_config_t;

enum {
    ALGO_PARSE_HEADER = 1,
    ALGO_PARSE_ARRAY
};

typedef struct {
    uint32_t index;
    uint32_t signature;
    uint32_t frame_size_ms;
    uint32_t frame_step_ms;
    uint32_t power_offset;
    uint32_t preemphasis_expoonent;
    uint16_t num_filter_banks;
    uint16_t melbin_boundary[NUM_FILTER_BANKS_COUNT_MAX]; /* odd aligned on u32, even not */
    struct {
        unsigned parsing;
        unsigned parsed;
        unsigned index;
        unsigned next_index;
        unsigned count;
    } parser;
} syntiant_dsp_fe_config_v2_data_t;

typedef struct {
    uint32_t index;
    uint32_t signature;
    uint32_t frame_size_ms;
    uint32_t frame_step_ms;
    uint32_t power_offset;
    uint32_t preemphasis_expoonent;
    uint16_t num_filter_banks;
    uint16_t secondary_validation;
    uint16_t melbin_boundary[NUM_FILTER_BANKS_COUNT_MAX];
    struct {
        unsigned parsing;
        unsigned parsed;
        unsigned index;
        unsigned next_index;
        unsigned count;
    } parser;
} syntiant_dsp_fe_config_v3_data_t;

/**
 * @brief A series of MCU orchestrator nodes.
 */
typedef struct syntiant_mcu_orch_v1_t_{
    uint8_t id;
    uint8_t flow_id;
    uint8_t status;
    uint8_t type;
    uint8_t action;
    uint8_t num_inputs;
    uint8_t num_outputs;
    uint8_t input_edges[4];
    uint8_t next_ids[4];
    uint8_t pad;
} syntiant_mcu_orch_v1_t;
SYNTIANT_CASSERT(sizeof(syntiant_mcu_orch_v1_t)
                  == MCU_ORCHESTRATOR_V1_SIZE,
                 "Unexpected size for syntiant_mcu_orch_v1_t")


/**
 * @brief A series of MCU orchestrator nodes.
 */
typedef struct syntiant_mcu_orch_v3_t_{
    uint8_t node_id;
    uint8_t dsp_flow_id;
    uint8_t status;/*on or off */
    uint8_t type;/*logic state */
    uint8_t action;
    uint8_t num_inputs;
    uint8_t num_outputs;
    uint8_t preroll;/*for nn node, in units of frame count (max 3)*/
    uint16_t pad;
    uint16_t orch_timeout;/* In units of frame count, 0 means no timeout.Both logic node and nn node can have runtime values*/
    uint8_t input_edges[4];/*ordering of input edges is*/
                           /*important here,which will determine the sequencing of events for NN nodes attached to the same logic node.*/
    uint8_t next_ids[4];
} syntiant_mcu_orch_v3_t;
SYNTIANT_CASSERT(sizeof(syntiant_mcu_orch_v3_t)
                  == MCU_ORCHESTRATOR_V3_SIZE,
                 "Unexpected size for syntiant_mcu_orch_v3_t")

typedef union syntiant_mcu_orch_t_{
    syntiant_mcu_orch_v1_t v1;
    syntiant_mcu_orch_v3_t v3;
} syntiant_mcu_orch_t;

typedef struct syntiant_object_decoder_ph_params_v1_t_ {
  uint32_t input_shape[3];
  uint32_t receptive_field[2];
  uint32_t receptive_field_offset[2];
  uint32_t outputs_per_region;
  uint32_t num_attributes;
} syntiant_object_decoder_ph_params_v1_t;

#define SYNTIANT_PDM_CHANNEL_COUNT  2U
#define SYNTIANT_NDP_PH_EXCLUDED 1

/* tlv 85 (0x55) */
struct ndp120_pdm_params_v1 {
    uint8_t  aud_id;
    uint8_t  mode;
    uint8_t  clk_mode;
    uint8_t  pad1;
    uint32_t clk_rate;
    uint32_t sample_rate;
    uint32_t frphasestep;
    uint32_t dcstaticremval;
    uint8_t  dcremovalmode;
    uint8_t  bypass_farrow;
    uint16_t pad2;
    struct pdm_params_channel_v1 {
        uint8_t  decimation_inshift;
        uint8_t  decimation_outshift;
        uint16_t pad;
        uint8_t  numdelay;
        uint8_t  agcshiftdir;
        uint8_t  agcshiftcnt;
        uint8_t  zcgainchange;
        uint32_t agcfinegrainmul;
    } channel[SYNTIANT_PDM_CHANNEL_COUNT];
};

/* tlv 86 (0x56) */
struct ndp120_i2s_in_params {
    uint8_t  aud_id;
    uint8_t  edge;
    uint8_t  mode;
    uint8_t  pad1;
    uint32_t clk_rate; /* sample_rate */
    uint8_t  framesize;
    uint8_t  samplesize;
    uint8_t  msb_index;
    uint8_t  delayed_flop_sensitivity;
    uint8_t  packed;
    uint8_t  leftchenable;
    uint8_t  rightchenable;
    uint8_t  bypass_farrow;
    uint32_t frphasestep;
    struct i2s_in_channel {
        uint8_t  numdelay;
        uint8_t  agcshiftdir;
        uint8_t  agcshiftcnt;
        uint8_t  zcgainchange;
        uint32_t agcfinegrainmul;
    } channel[SYNTIANT_PDM_CHANNEL_COUNT];
};

/* tlv 87 (0x57) */
struct ndp120_i2s_out_params {
    uint32_t clk_rate; /* sample_rate */
    uint8_t  edge;
    uint8_t  mode;
    uint8_t  framesize;
    uint8_t  samplesize;
    uint8_t  msb_index;
    uint8_t  delayed_flop_sensitivity;
    uint8_t  packed;
    uint8_t  leftchenable;
    uint8_t  rightchenable;
    uint8_t  aud2_out_mode;
    uint8_t  aud2clkoutneeded;
    uint8_t  audio_out_fs_ext_enable;
    uint8_t  aud2_src_type;
    uint8_t  aud2_src_param;
    uint8_t  bypass_farrow;
    uint8_t  pad;
    uint32_t frphasestep;
    struct i2s_out_channel {
        uint8_t  numdelay;
        uint8_t  agcshiftdir;
        uint8_t  agcshiftcnt;
        uint8_t  zcgainchange;
        uint32_t agcfinegrainmul;
    } channel[SYNTIANT_PDM_CHANNEL_COUNT];
};

/* tlv 88 (0x58) */
struct ndp120_dsp_sync_params {
    uint8_t  sync_mode;
    uint8_t  sync_ref;
    uint8_t  sync_adj;
    uint8_t  pad;
    int32_t  sync_offset;
    int32_t  sync_chan_delay_ticks;
};

/* tlv 96 (0x60) */
struct ndp120_pdm_params_v2 {
    uint8_t  aud_id;
    uint8_t  mode;
    uint8_t  clk_mode;
    uint8_t  bypass_farrow;
    uint32_t clk_rate;
    uint32_t sample_rate;
    uint32_t frphasestep;
    struct pdm_params_channel_v2 {
        uint8_t  decimation_inshift;
        uint8_t  decimation_outshift;
        uint8_t  dcremovalmode;
        uint8_t  numdelay;
        uint8_t  agcshiftdir;
        uint8_t  agcshiftcnt;
        uint8_t  zcgainchange;
        uint8_t  pad1;
        uint32_t agcfinegrainmul;
        uint32_t dcstaticremval;
        uint32_t a1;
        uint32_t a2;
        uint32_t b0;
        uint32_t b1;
        uint32_t b2;
    } channel[SYNTIANT_PDM_CHANNEL_COUNT];
};

typedef union {
    struct ndp120_pdm_params_v1     tlv85;
    struct ndp120_i2s_in_params     tlv86;
    struct ndp120_i2s_out_params    tlv87;
    struct ndp120_dsp_sync_params   tlv88;
    struct ndp120_pdm_params_v2     tlv96;
} syntiant_pdm_config_t;

typedef struct syntiant_pin_mux_config_t_ {
    uint8_t  aud1_pin_sel;
    uint8_t  mspi_pin_sel;
    uint16_t pad;
} syntiant_pin_mux_config_t;

typedef struct syntiant_sensor_ifc_transaction_t_ { /* per transaction */
    uint8_t num_ifc_bytes;    /* number of bytes to read/write  E.g. 1~12 */
    uint8_t ifc_transaction_type; /* 0: write, 1: read */
    uint8_t ifc_start_addr;       /* starting address of burst read/write */
    uint8_t delay;        /* add delay after this transaction: true or false*/
    uint8_t ifc_bytes[SYNTIANT_PACKAGE_NUM_IFC_BYTES_MAX]; /* ifc data bytes */
} syntiant_sensor_ifc_transaction_t;

typedef struct syntiant_sensor_gpio_int_t_ { /* per gpio */
    uint8_t gpio_role;  /* role of gpio pin */
    uint8_t intrpt;     /* gpio pin number for interrupt. E.g. 23 */
} syntiant_sensor_gpio_int_t;

typedef struct syntiant_sensor_config_v1_t_ {
    struct {
        uint8_t  sensor_num;  /* sensor number from 0~3, unique per TLV */
        uint8_t  sensor_id;   /* sensor identifier identifying the specific
                                 type of sensor. */
        uint16_t pad1;
        uint32_t num_gpio_used; /* number of gpio used for this sensor. 1~2 */

        /* role of gpio pin, one per gpio */
        syntiant_sensor_gpio_int_t
                 sensor_gpio[SYNTIANT_PACKAGE_NUM_GPIO_PINS_MAX];

        uint8_t  ifc;      /* interface to sensor. E.g. i2c, spi0 */
        uint8_t  ifc_addr; /* interface address. E.g. i2c address 0x61 */
        uint16_t pad2;
        uint32_t ifc_clk_rate; /* interface clock rate. E.g. 400 Khz for i2c */

        uint32_t num_axes;  /* number of axes collected.  0~15 */
        uint16_t axis_tank; /* capture axis in tank. 1: on, 0: off.
                               One bit per axis, up to 15 bits */
        uint16_t axis_input; /* add axis to input vector. 1: on, 0: off.
                                One bit per axis, up to 15 bits */
        uint32_t num_param; /* number of sensor parameters 0~16 */
        uint8_t  param_value[SYNTIANT_PACKAGE_NUM_PARAMETERS_MAX];

        uint32_t num_ifc_transactions; /* number of burst read or write ifc
                                          transactions (0~16) */
    } base_sensor_config;
    syntiant_sensor_ifc_transaction_t
                 sensor_trans[SYNTIANT_PACKAGE_IFC_TRANSACTIONS_MAX];
    struct {
        uint32_t cur_transaction;
        uint32_t parsing;
        uint32_t parsed;
    } parser;
} syntiant_sensor_config_v1_t;

typedef union {
    syntiant_sensor_config_v1_t v1;
} syntiant_sensor_config_t;

typedef struct syntiant_softmax_t_ {
    uint16_t nn_num;
    int16_t quantized_sf;
} syntiant_softmax_t;
SYNTIANT_CASSERT(sizeof(syntiant_softmax_t)
            == NN_SOFTMAX_SIZE,
            "Unexpected size for syntiant_softmax_t")

/**
 * @brief hw parameters
 *
 * @param sample_frequency_hz sample frequency in hz
 * @param frame_size frame size in num of samples
 * @param frame_step frame step in num of samples
 * @param num_filter_banks num of filter banks
 * @param num_activations_usednum of activations used for final layer
 * @param mel_bins_length num of melbins
 * @param mel_bin FFT bins
 */
typedef struct syntiant_hw_params_v2_t_{
   uint32_t sample_frequency_hz;
   uint32_t frame_size;
   uint32_t frame_step;
   uint32_t num_filter_banks;
   uint32_t num_activations_used;
   uint32_t mel_bins_length;
   uint32_t mel_bins[40+2]; /* this 42 will be replaced with mel_bins_length */
} syntiant_hw_params_v2_t;
SYNTIANT_CASSERT(sizeof(syntiant_hw_params_v2_t)
                 == HW_PARAMS_V2_SIZE,
                 "Unexpected size for syntiant_hw_params_v2_t")

/**
 * @brief board parameters version 1
 *
 * @param dnn_input DNN input source
 * @param input_clock_rate input clock rate
 * @param pdm_clock_rate PDM clock rate
 * @param pdm_dc_offset DC offset for DN inputs
 * @param pdm_clock_ndp PDM clock source
 * @param power_offset
 * @param preemphasis_exponent
 * @param cal_pdm_in_shift
 * @param cal_pdm_out_shift
 * @param power_scale_exponent
 * @param agc
 * @param equalizer
 */
typedef struct syntiant_board_params_v1_t_{
    uint8_t dnn_input;
    uint32_t input_clock_rate;
    uint32_t pdm_clock_rate;
    int32_t pdm_dc_offset[2];
    uint8_t pdm_clock_ndp;
    uint8_t power_offset;
    uint32_t preemphasis_exponent;
    uint8_t cal_pdm_in_shift[2];
    uint8_t cal_pdm_out_shift[2];
    uint32_t power_scale_exponent;
    uint8_t agc;
    int8_t equalizer[40];
} SYNTIANT_PACK syntiant_board_params_v1_t;
SYNTIANT_CASSERT(sizeof(syntiant_board_params_v1_t)
                 == BOARD_PARAMS_V1_SIZE,
                 "Unexpected size for syntiant_board_params_v1_t")

/**
 * @brief board parameters version 2
 *
 * @param input_clock_rate input clock rate
 * @param pdm_clock_rate PDM clock rate
 * @param preemphasis_exponent
 * @param power_scale_exponent
 * @param pdm_dc_offset DC offset for DN inputs
 * @param cal_pdm_in_shift
 * @param cal_pdm_out_shift
 * @param dnn_input DNN input source
 * @param pdm_clock_ndp PDM clock source
 * @param power_offset
 * @ param agc
 * @param equalizer
 */
typedef struct syntiant_board_params_v2_t_{
    uint8_t agc;
    uint8_t dnn_input;
    uint8_t pdm_clock_ndp;
    uint8_t power_offset;
    uint8_t cal_pdm_in_shift[2];
    uint8_t cal_pdm_out_shift[2];
    uint32_t input_clock_rate;
    uint32_t pdm_clock_rate;
    int32_t pdm_dc_offset[2];
    uint32_t preemphasis_exponent;
    uint32_t power_scale_exponent;
    int8_t equalizer[40];
} syntiant_board_params_v2_t;
SYNTIANT_CASSERT(sizeof(syntiant_board_params_v2_t)
                 == BOARD_PARAMS_V2_SIZE,
                 "Unexpected size for syntiant_board_params_v2_t")
/**
 * @brief board parameters version 3
 *
 * @param input_clock_rate input clock rate
 * @param pdm_clock_rate PDM clock rate
 * @param preemphasis_exponent
 * @param power_scale_exponent
 * @param pdm_dc_offset DC offset for DN inputs
 * @param cal_pdm_in_shift
 * @param cal_pdm_out_shift
 * @param dnn_input DNN input source
 * @param pdm_clock_ndp PDM clock source
 * @param power_offset
 * @ param agc
 * @param equalizer
 * @param agc max adjustment
 * @param agc reference quiet level
 */
typedef struct syntiant_board_params_v3_t_{
    uint8_t agc;
    uint8_t dnn_input;
    uint8_t pdm_clock_ndp;
    uint8_t power_offset;
    uint8_t cal_pdm_in_shift[2];
    uint8_t cal_pdm_out_shift[2];
    uint32_t input_clock_rate;
    uint32_t pdm_clock_rate;
    int32_t pdm_dc_offset[2];
    uint32_t preemphasis_exponent;
    uint32_t power_scale_exponent;
    int8_t equalizer[40];
    uint32_t agc_ref_lvl;
    uint32_t agc_max_adj[2];
} syntiant_board_params_v3_t;
SYNTIANT_CASSERT(sizeof(syntiant_board_params_v3_t)
                 == BOARD_PARAMS_V3_SIZE,
                 "Unexpected size for syntiant_board_params_v3_t")
/**
 * @brief board parameters version 4
 *
 * @param input_clock_rate input clock rate
 * @param pdm_clock_rate PDM clock rate
 * @param preemphasis_exponent
 * @param power_scale_exponent
 * @param pdm_dc_offset DC offset for DN inputs
 * @param cal_pdm_in_shift
 * @param cal_pdm_out_shift
 * @param dnn_input DNN input source
 * @param pdm_clock_ndp PDM clock source
 * @param power_offset
 * @ param agc
 * @param equalizer
 * @param agc max adjustment
 * @param agc reference quiet level
 * #noise_threshold noise threshold level
 * @noise_threshold_win noise threshold windows
 */
typedef struct syntiant_board_params_v4_t_{
    uint8_t agc;
    uint8_t dnn_input;
    uint8_t pdm_clock_ndp;
    uint8_t power_offset;
    uint8_t cal_pdm_in_shift[2];
    uint8_t cal_pdm_out_shift[2];
    uint32_t input_clock_rate;
    uint32_t pdm_clock_rate;
    int32_t pdm_dc_offset[2];
    uint32_t preemphasis_exponent;
    uint32_t power_scale_exponent;
    int8_t equalizer[40];
    uint32_t agc_ref_lvl;
    uint32_t agc_max_adj[2];
    int32_t noise_threshold;
    uint32_t noise_thresh_win;
} syntiant_board_params_v4_t;
SYNTIANT_CASSERT(sizeof(syntiant_board_params_v4_t)
                 == BOARD_PARAMS_V4_SIZE,
                 "Unexpected size for syntiant_board_params_v4_t")

/**
 * @brief union for storing board params
 *
 * @param board_params_v1 board params v1
 * @param board_params_v2 board params v2
 */

typedef union syntiant_board_params_t_{
    syntiant_board_params_v1_t board_params_v1;
    syntiant_board_params_v2_t board_params_v2;
    syntiant_board_params_v3_t board_params_v3;
    syntiant_board_params_v4_t board_params_v4;
} syntiant_board_params_t;

/**
 * @brief union for storing ph params
 *
 * @param ph_params posterior params v1
 * @param ph_params_v2 posterior params v2
 */
typedef union syntiant_ph_param_sp_t_ {
    syntiant_ph_params_t v1;
    syntiant_ph_params_v2_t v2;
} syntiant_ph_param_sp_t;

/**
 * @brief struct defining the PBI version information in the PBI Versino tag.
 *
 * @param chip_type The NDP chip this FW is designed for
 * @param major     The major release number of this PBI version
 * @param minor     The minor release number of this PBI version
 * @param patch     The patch release number of this PBI version
 */
typedef struct syntiant_pbi_version_params_t_ {
    uint32_t    chip_type; /* pbi_version_chip_type_t_ */
    uint32_t    major;
    uint32_t    minor;
    uint32_t    patch;
} syntiant_pbi_version_params_t;

typedef struct dsp_mcu_secondary_validation {
    uint8_t validation_type;
    uint8_t notification_type;
    uint8_t  network_id;
    uint8_t pad;
    uint16_t num_frames;
    uint8_t pad1[2];
    uint32_t threshold;
    uint32_t ratio_threshold;
} syntiant_dsp_mcu_secondary_validation_t;


typedef struct mcu_secondary_validation {
    uint8_t validation_type;
    uint8_t notification_type;
    uint8_t network_id;
    uint8_t num_occurrences;
    uint16_t frame_range_low;
    uint16_t frame_range_high;
} syntiant_mcu_secondary_validation_t;

typedef struct syntiant_dsp_algo_params_s {
    struct {
        uint16_t algo_config_idx;
        uint16_t payload_size;
    } base;
    uint8_t payload[NDP120_ALGO_CONFIG_V2_AREA_LEN];
} syntiant_dsp_algo_params_t;

/**
 * @brief union for storing C data structures read from chunks
 *
 * @param ph_params posterior params
 * @param syntiant_fc_params_t fc params
 * @param hw_params hardware config
 * @param board_params board parameters
 * @param fw_version version of fw
 * @param params_version version of DNN params
 * @param labels class labels
 * @param fw firmware
 * @param packed_params packed params
 */
typedef union syntiant_pkg_parser_data_t_{
    syntiant_pbi_version_params_t pkg_pbi_version_params;
    syntiant_ph_param_sp_t ph_params;
    syntiant_ph_collection_params_t phc_params;
    syntiant_ph_collection_params_v7_t phc_v7;
    syntiant_fc_params_t fc_params;
    syntiant_hw_params_v2_t hw_params;
    syntiant_board_params_t board_params;
    syntiant_nn_metadata_t nn_metadata;
    syntiant_algo_attach_t algo_attach;
    syntiant_dsp_flow_collection_t dsp_flow_collection;
    syntiant_dsp_fe_config_data_t dsp_fe_config;
    syntiant_dsp_fe_config_v2_data_t dsp_fe_config_v2;
    syntiant_dsp_fe_config_v3_data_t dsp_fe_config_v3;
    syntiant_mcu_orch_t mcu_orchestrator;
    syntiant_pdm_config_t pdm_cfg;
    syntiant_pin_mux_config_t pin_mux_cfg;
    syntiant_sensor_config_t sensor_cfg;
    syntiant_softmax_t softmax;
    uint8_t fw_version[VERSION_MAX_SIZE];
    uint8_t dsp_fw_version[VERSION_MAX_SIZE];
    uint8_t params_version[VERSION_MAX_SIZE];
    uint8_t pkg_version[VERSION_MAX_SIZE];
    uint8_t pkger_version[VERSION_MAX_SIZE];
    uint8_t labels[PARSER_SCRATCH_SIZE];
    uint8_t fw[PARSER_SCRATCH_SIZE];
    uint8_t packed_params[PARSER_SCRATCH_SIZE];
    struct {
        uint32_t params_config;
        uint32_t data_config;
    } dnn_power;
    struct {
      uint16_t num_frames;
      uint16_t clear_on_match;
    } auto_clear;
    syntiant_dsp_mcu_secondary_validation_t dsp_mcu_sec_val;
    syntiant_mcu_secondary_validation_t mcu_sec_val;
    ndp120_dsp_fw_mic_settings_t mic_settings;
    syntiant_dsp_algo_params_t algo_params;
} syntiant_pkg_parser_data_t;

/**
 * @brief union for storing C data structures to help keeping track of velues
 * partially read
 * @param ph_metadata metadata for posterior params
 * @param fc_metadata metadata for fc params
 * @param fw_metadata metadata for firmware that tracks num of chunks on which
 * fw coming
 * @param labels_metadata metadata for firmware that tracks num of chunks on which
 * fw coming
 */
typedef union syntiant_pkg_parser_metadata_t_{
    syntiant_ph_params_metadata_t ph_metadata;
    syntiant_fc_params_metadata_t fc_metadata;
    uint32_t fw_metadata;
    uint32_t labels_metadata;
    uint32_t scratch_metadata[NN_FLOW_METADATA_SIZE];
} syntiant_pkg_parser_metadata_t;

#define SYNTIANT_NDP_ENFORCE_FW_COMPATIBILITY_CHECK   (1)

/*
 * @brief package parser state
 * @param data structs to store values
 * @param metadata struct to store metadata information for incrementally parsing
 * @param expected bytes to read for tag, length, (partial) values
 * @param header_val header magic value
 * @param calc_crc last calculated crc
 * @param partially_read_length amount partially read from the total value
 * @param magic_header_found of header is found
 * @param mode parsing mode (starting/continuing to read tag, length, or value)
 * @param value_mode value parsing mode (partial value is ready or not)
 * @param ptr current pointer in a chunk
 * @param open_ram_begin pointers to begining of a chunk
 * @param open_ram_end pointer to the end of a chunk
 * @param tag last seen tag
 * @param length last seen length
 * @param exp_crc expected crc
 * @param is_current_fw_package if fw_package is observed
 * @param is_current_params_package if params package is observed
 * @param is_current_dsp_fw_package if dsp fw pacakge is observed
 * @param package_pbi_version PBI version values
 */
typedef struct syntiant_pkg_parser_state_t_ {
    syntiant_pkg_parser_data_t data;
    syntiant_pkg_parser_metadata_t metadata;
    uint32_t expected_bytes;
    uint32_t header_val;
    uint32_t calc_crc;
    uint32_t partially_read_length;
    int magic_header_found;
    int mode;
    int value_mode;
    uint8_t* ptr;
    uint8_t* open_ram_begin;
    uint8_t* open_ram_end;
    uint8_t tag[TAG_SIZE];
    uint8_t length[LENGTH_SIZE];
    uint8_t exp_crc[CRC_SIZE];
    uint8_t is_multisegment;
    uint8_t is_current_fw_package;
    uint8_t is_current_params_package;
    uint8_t is_current_dsp_fw_package;
    syntiant_pbi_version_params_t package_pbi_version;
    uint8_t ph_excluded;
    uint8_t fw_compatibility_check;
    uint8_t rsvd[2];
} syntiant_pkg_parser_state_t;

#if defined(WINDOWS_KERNEL) || defined(_MSC_VER) || defined(__MINGW32__)
#pragma pack(pop)
#endif

/**
 * @brief intialize and prepare syntiant_ndp_pkg_parser_state_t to start parsing
 * a raw package
 * param[in] pstate parser state
 * param[in] config_address address of config
 * param[in] open_ram_begin first address of open ram
 * param[in] open_ram_end last address of open ram
 */
extern void syntiant_pkg_parser_init(syntiant_pkg_parser_state_t *pstate);

/**
 * @brief reset ptr in syntiant_ndp_pkg_parser_state_t to start processing a
 * new chunk
 * param[in] pstate parser state
 */
extern void syntiant_pkg_parser_reset(syntiant_pkg_parser_state_t *pstate);

/**
 * @brief pkg header information
 *
 * @param[in] tlv tag, length, and value as an array of bytes
 * @return true or false if tlv is Magic Number TLV or not
 */
extern int syntiant_pkg_check_header_tlv(uint8_t *tlv);


/**
 * @read specific amount of data from chunk, and update CRC
 *
 * @param[in] pstate package parser status
 * @param[in] dest destination to store data or point to data
 * @param[in] max_length maximum length of data to read
 * @param[in] fake if skip the data or parse it
 * @param[in] calc_crc if crc should be updated or not
 * @param[in] copy copy data from open ram to dest or set dest to point to a place in openram
 * @return amount of data that is read
 */
extern unsigned int syntiant_pkg_read_chunk_data(syntiant_pkg_parser_state_t
                                                 *pstate, uint8_t* dest,
                                                 uint32_t max_length,
                                                 int fake, int calc_crc,
                                                 int copy);

/**
 * @read tag
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_pkg_read_tag(syntiant_pkg_parser_state_t *pstate);

/**
 * @read length
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_pkg_read_length(syntiant_pkg_parser_state_t *pstate);

/**
 * @read value
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_pkg_read_value(syntiant_pkg_parser_state_t *pstate,
                                   int collect_log);

/**
 * @parse part of chunk
 *
 * @param[in] pstate parser status
 * @param[in] collect_log 1 means to print log, and 0 means no need to print log
 * @return error code
 */
extern int syntiant_pkg_parse_chunk(syntiant_pkg_parser_state_t *pstate,
                                   int collect_log);

/**
 * @prepare chunk for processing
 *
 * @param[in] pstate parser status
 * @param[in] chunk pointer to the begining of a chunk
 * @param[in] length chunk length
 * @param[in] copy copy the chunk to another memory before process or not
 * @return error code
 */
extern void syntiant_pkg_preprocess_chunk(syntiant_pkg_parser_state_t *pstate,
                                          uint8_t* chunk, int length,
                                          int copy);

/**
 * @parse sentinel value
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_pkg_parse_checksum_value(syntiant_pkg_parser_state_t
                                             *pstate);


/**
 * @parse posterior handling parameters value
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_pkg_parse_posterior_params(syntiant_pkg_parser_state_t
                                               *pstate, int collect_log);


/**
 * @parse fc parameters
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_pkg_parse_fc_config_params(syntiant_pkg_parser_state_t
                                               *pstate, int collect_log);

/**
 * @parse values which are stored in parser state such as version, etc.
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_pkg_parse_stored_params(syntiant_pkg_parser_state_t
                                            *pstate, int collect_log);

/**
 * @prints values which are stored in parser state such as version, etc.
 *
 * @param[in] pstate parser status
 * @return error code
 */
extern int syntiant_pkg_print_stored_params(syntiant_pkg_parser_state_t
                                            *pstate);

/**
 * @parse partially stored parameters such as fw and packed params
 *
 * @param[in] pstate parser status
 * @param[in] collect_log if print log or not
 * @return error code
 */
extern int syntiant_pkg_parse_partially_stored_params
(syntiant_pkg_parser_state_t *pstate, int collect_log);

/**
 * @parse header value
 *
 * @param[in] pstate parser status
 * @return error code
 */

extern int syntiant_pkg_parse_header_value(syntiant_pkg_parser_state_t
                                           *pstate);

#ifdef __cplusplus
}
#endif

#endif
