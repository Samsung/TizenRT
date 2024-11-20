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


#ifndef NDP120_DSP_DNN_H_
#define NDP120_DSP_DNN_H_

#ifndef __KERNEL__
#include <stdint.h>
#endif

#ifdef NDP200
#define NDP120_DNN_RESULT_MAX_NUM_ACTIVATIONS 600 
/* max number of final layer activations per neural network. */
#else
#define NDP120_DNN_RESULT_MAX_NUM_ACTIVATIONS 4
/* max number of final layer activations per neural network. */
#endif

/**
 * @brief Constants used by DSP & MCU firmwares.
 * They should be multiple of 4.
 * */
enum ndp120_dnn_constants_e {
    NDP120_DNN_MAX_NUM_LAYERS = 32, /* max number of layers per neural network */
    NDP120_DNN_MAX_NUM_NETWORKS = 2, /* max number of neural networks. */
    NDP120_DNN_RESULT_MAX_NUM_CLASSES = 2,
    /* max number of classes per neural network */
    NDP120_DNN_MAX_NUM_INPUTS = 4, /* max number of inputs per neural network */
    NDP120_DNN_MAX_NUM_OUTPUTS = 4
    /* max number of outputs per neural network */
};

/**
 * @brief Stores cache state for all layers in a neural network.
 *
 */
struct ndp120_dnn_cache_state_s {
    uint32_t cur_input_coords[NDP120_DNN_MAX_NUM_LAYERS];
    uint32_t cur_output_coords[NDP120_DNN_MAX_NUM_LAYERS];
    uint32_t cur_input_offsets[NDP120_DNN_MAX_NUM_LAYERS];
};

typedef struct ndp120_dnn_cache_state_s ndp120_dnn_cache_state_t;

/**
 * @brief Represents shape of tensor.
 * @note This struct is shared with packager & ilib.
 */
struct ndp120_dnn_tensor_shape_s {
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

typedef struct ndp120_dnn_tensor_shape_s ndp120_dnn_tensor_shape_t;

/**
 * @brief Stores cache instructions for a layer.
 * @note This struct is shared with packager  & ilib
 *
 */
struct ndp120_dnn_layer_cache_inst_s {
    uint32_t input_base_coord_max;
    uint32_t output_base_coord_max;
    uint32_t input_base_coord_add;
    uint16_t input_offset_add;
    uint16_t input_offset_max;
    uint16_t output_base_coord_add;
    uint16_t output_base_coord_stride;
};

typedef struct ndp120_dnn_layer_cache_inst_s ndp120_dnn_layer_cache_inst_t;


/**
 * @brief Represents version 1 metadata for a neural network.
 * @note This struct is shared with packager & ilib.
 */
struct ndp120_dnn_network_metadata_v1_s {
    uint32_t layers_per_nn;
    uint32_t is_nn_cached;
    uint32_t input_layer_isa_idx;
    uint32_t output_layer_isa_idx;
    uint32_t input_layer_type;
    ndp120_dnn_tensor_shape_t input_layer_size;
    uint32_t input_coords[NDP120_DNN_MAX_NUM_LAYERS];
    uint32_t output_coords[NDP120_DNN_MAX_NUM_LAYERS];
    ndp120_dnn_layer_cache_inst_t cache_instructions[NDP120_DNN_MAX_NUM_LAYERS];
};

typedef struct ndp120_dnn_network_metadata_v1_s ndp120_dnn_network_metadata_v1_t;

struct ndp120_dnn_network_metadata_v2_s {
    uint8_t num_layers;
    uint8_t num_inputs;
    uint8_t num_outputs;
    uint8_t is_cached;
    uint8_t layers[NDP120_DNN_MAX_NUM_LAYERS];
    uint8_t inputs[NDP120_DNN_MAX_NUM_INPUTS];
    uint8_t outputs[NDP120_DNN_MAX_NUM_OUTPUTS];
    ndp120_dnn_layer_cache_inst_t cache_instructions[NDP120_DNN_MAX_NUM_LAYERS];
};

typedef struct ndp120_dnn_network_metadata_v2_s
    ndp120_dnn_network_metadata_v2_t;

union ndp120_dnn_network_metadata_u {
    ndp120_dnn_network_metadata_v1_t v1;
    ndp120_dnn_network_metadata_v2_t v2;
};

typedef union ndp120_dnn_network_metadata_u ndp120_dnn_network_metadata_t;

enum ndp120_dnn_metadata_version_e {
    NDP120_DNN_METADATA_VERSION_V1 = 0,
    NDP120_DNN_METADATA_VERSION_V2 = 1
};


/**
 * @brief Represents metadata for all neural networks.
 * @note This struct is shared with ilib.
 *
 */
struct ndp120_dnn_metadata_s {
    uint8_t version;
    uint8_t nn_cnt;
    uint8_t dnn_mode;
    uint8_t padding;
    ndp120_dnn_network_metadata_t nn_metadata[NDP120_DNN_MAX_NUM_NETWORKS];
};

typedef struct ndp120_dnn_metadata_s ndp120_dnn_metadata_t;

/**
 * @brief Represents the state of a neural network.
 *
 */
struct ndp120_dnn_network_state_s {
   /* NN info */
    uint32_t input_size;
    uint32_t nn_meta; /* pointer type is struct ndp120_dnn_metadata_s * */

    /* cached conv working data */
    ndp120_dnn_cache_state_t cache_metadata;

    /* working data */
    uint32_t input_offset;
    uint32_t write_offset;
};

typedef struct ndp120_dnn_network_state_s ndp120_dnn_network_state_t;

/**
 * @brief Stores all the results collected from hardware and firmware
 *
 */
struct ndp120_dnn_result_s {
    uint32_t num_classes;
    uint32_t max_index;
    uint32_t winner_one_hot;
    uint32_t summary;
    uint32_t tankptr;
    uint32_t raw_strengths_ptr;
    uint32_t softmax_strengths_ptr;
};

union ndp120_result_strengths_u {
    uint8_t *u8;
    int8_t *s8;
    int16_t *s16;
};

typedef struct ndp120_dnn_result_s ndp120_dnn_result_t;
typedef union ndp120_result_strengths_u ndp120_raw_strengths_t;

#endif
