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


#ifndef NDP120_RESULTS_H
#define NDP120_RESULTS_H

#include <syntiant-dsp-firmware/ndp120_dsp_dnn.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ndp120_main_state_s;

enum ndp_results_constants_e {
    NDP120_RESULT_SOFTMAX_SMOOTHER_MAX_QUEUE_SIZE = 12
};

/**
 * @brief Cache ISA based params needed to read results of NN
 *
 */
struct ndp120_nn_output_cache_s {
    /* output activation type */
    uint16_t activation_type;
    /* Number of values present in the output layer */
    uint16_t num_entries;
    /* data bank coordinate of the activations */
    uint32_t output_coord;
};

/**
 * @brief intializes the result object
 *
 * @param result Result Object
 */
void ndp120_result_init(struct ndp120_dnn_result_s *result);

/**
 * @brief reads the activations from hardware registers
 *
 * @param result Result object
 */
void ndp120_result_read_activations(struct ndp120_dnn_result_s *result);

/**
 * @brief Represents the Softmax Smoother
 *
 */
struct ndp120_result_softmax_smoother_s {
    uint32_t num_classes;
    uint32_t scale_factor;
#ifdef X86_64
    uint32_t *queue_sizes;        /* pointer to queue_sizes[] */
    uint32_t *queue_curr_ptr;     /* pointer to queue_curr_ptr[] */
    uint32_t **queues;            /* pointer to pointer to queues[][] */
#else
    uint32_t queue_sizes;         /* pointer to queue_sizes[] */
    uint32_t queue_curr_ptr;      /* pointer to queue_curr_ptr[] */
    uint32_t queues;              /* pointer to pointer to queues[][] */
#endif
};

/**
 *  @Brief Represent results data corresponding to a match
 */
struct syntiant_ndp120_secure_match_data {
    uint32_t tank_ptr;          /**< tank pointer at match */
    uint32_t summary;           /**< match summary */
    uint32_t algo_match_summary; /**< Algo match summary */
    uint32_t match_frame_count; /**< Record count of match frame */
    uint32_t match_producer;    /**< producer pointer for match */
    uint16_t total_nn;          /**< total number of NNs on the device */
    uint8_t nn_id;              /**< NN id for the match event */
    uint8_t sec_val_status;     /**< indicates secondary validation status */
    uint16_t num_classes;       /**< number of classes (# entries in output layer) of the network */
    uint16_t activation_type;   /**< activation type */
    union {
        uint8_t u8[NDP120_DNN_RESULT_MAX_NUM_CLASSES];
        int8_t s8[NDP120_DNN_RESULT_MAX_NUM_CLASSES];
        int16_t s16[NDP120_DNN_RESULT_MAX_NUM_CLASSES];
    } raw_strengths;
};

/**
 * @brief intializes the softmax smoother
 *
 * @param smoother Smoother object
 */
void ndp120_result_softmax_smoother_init(
    struct ndp120_result_softmax_smoother_s *smoother);

/**
 * @brief Set the params for softmax smoother
 *
 * @param smoother Smoother object
 * @param num_classes Num of classes for which smoothing needs to be done
 * @param queue_sizes Queue sizes for each class.
 */
void ndp120_result_softmax_smoother_set(
    struct ndp120_result_softmax_smoother_s *smoother, uint32_t num_classes,
    uint32_t *queue_sizes);

/**
 * @brief smoothes the softmax probabilties.
 *
 * @param smoother Smoother object
 * @param result result object
 */
void ndp_result_softmax_smoother_do_smoothing(
    struct ndp120_result_softmax_smoother_s *smoother,
    struct ndp120_dnn_result_s *result);

/**
 * @brief  Maintain a cache of NN ISA params that are needed to extract
 * and process activation of a NN properly.
 *
 * @param fw_main_state FW main state object
 */
void
nn_output_cache_init(struct ndp120_main_state_s *fw_main_state);

/**
 * @brief Stores the summary and tank pointer of the match
 *
 * @param fw_main_state FW main state object
 * @param nn indicates neural network id
 * @param summary indicates the summary of the match
 * @param tankptr indicates the sample tank pointer
 *
 */
void ndp120_record_match(struct ndp120_main_state_s *fw_main_state,
        uint8_t nn, uint32_t summary, uint32_t tankptr);

#ifdef __cplusplus
}
#endif
#endif
