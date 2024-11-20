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


#ifndef NDP120_PH_H
#define NDP120_PH_H

#include "syntiant-firmware/ndp120_result.h"
#include "syntiant-firmware/ndp120_firmware.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NDP120_PHACTION_MCP_ACTION_MASK 0x000000ff
#define NDP120_PHACTION_MCP_ACTION_MASK_SHIFT 0
#define NDP120_PHACTION_MCP_ACTION_EXTRACT(x) \
    ((x & NDP120_PHACTION_MCP_ACTION_MASK) >> NDP120_PHACTION_MCP_ACTION_MASK_SHIFT)

#define NDP120_PHACTION_MCP_MATCH_ARG_MASK 0x0000ff00
#define NDP120_PHACTION_MCP_MATCH_ARG_MASK_SHIFT 8
#define NDP120_PHACTION_MCP_MATCH_ARG_EXTRACT(x) \
    ((x & NDP120_PHACTION_MCP_MATCH_ARG_MASK) >> NDP120_PHACTION_MCP_MATCH_ARG_MASK_SHIFT)

#define NDP120_PHACTION_MCP_MOVE_ARG_MASK 0x00ff0000
#define NDP120_PHACTION_MCP_MOVE_ARG_MASK_SHIFT 16
#define NDP120_PHACTION_MCP_MOVE_ARG_EXTRACT(x) \
    ((x & NDP120_PHACTION_MCP_MOVE_ARG_MASK) >> NDP120_PHACTION_MCP_MOVE_ARG_MASK_SHIFT)

enum ndp120_ph_action_scp_e {
    NDP120_PH_ACTION_SCP_STATE_M = 0x7f,
    NDP120_PH_ACTION_SCP_STAY = NDP120_PH_ACTION_SCP_STATE_M,
    NDP120_PH_ACTION_SCP_MATCH_M = 0x80
};

enum ndp120_ph_action_mcp_e {
    NDP120_PH_ACTION_MCP_MATCH = 0,
    NDP120_PH_ACTION_MCP_MOVE = 1,
    NDP120_PH_ACTION_MCP_STAY = 2
};

enum ndp120_ph_action_scp_unencoded {
    NDP120_PH_ACTION_SCP_STATE_MATCH = 0,
    NDP120_PH_ACTION_SCP_STATE_STATE = 1,
    NDP120_PH_ACTION_SCP_STATE_STAY = 2
};

#define PH_ACTION_MASK 0x000000ff
#define PH_ACTION_ARG0_MASK 0x0000ff00
#define PH_ACTION_ARG0_SHIFT 8
#define PH_ACTION_ARG1_MASK 0x00ff0000
#define PH_ACTION_ARG1_SHIFT 16

/**
 * @brief parameters for each class
 *
 */
struct ndp120_ph_class_params_s {
    uint32_t window;
    uint32_t threshold;
    uint32_t backoff;
    uint32_t action;
    uint32_t smoothing_queue_size;
    uint32_t adaptive_threshold;
};
#define PH_PARAM_DWORD_SIZE     (sizeof(struct ndp120_ph_class_params_s)/sizeof(uint32_t))

/**
 * @brief parameters for each state
 *
 */
struct ndp120_ph_state_params_s {
    uint32_t timeout;
    uint32_t timeout_action;
    uint32_t class_params_offset;
};

/**
 * @brief All parameters used by posterior handler
 *
 */
struct ndp120_ph_params_s {
    uint32_t num_classes;
    uint32_t num_states;
    /* the frame processing function to be used */
    uint32_t ph_type;
    uint32_t adaptive_frames;       /* interval between updates 0 -> adapt off */
    uint32_t adaptive_denominator;  /* denominator of update */
    uint32_t adaptive_max_updates;  /* maximum updates */
#ifdef X86_64
    uint32_t *params_memory;        /* pointer to address of params_memory*/
#else
    uint32_t params_memory;         /* pointer to address of params_memory*/
#endif
};

/* configure posterior class specified by ph/state/class idx */

enum syntiant_ndp120_tiny_posterior_params_limits {
    NSP120_POSTERIOR_THRESHOLD_MAX_VALUE = 0xFFFF,
    NDP120_POSTERIOR_SMOOTHER_MAX_QUEUE_SIZE =
        NDP120_RESULT_SOFTMAX_SMOOTHER_MAX_QUEUE_SIZE
};

struct syntiant_ndp120_secure_posterior_config_s {
    unsigned set;
    struct {
        uint32_t num_nn;
    } collection_;
    struct {
        uint32_t nn_idx;
        struct ndp120_ph_params_s ph;
    } ph_;
    struct {
        uint32_t idx;
        struct ndp120_ph_state_params_s st;
    } st_;
    struct {
        uint32_t idx;
        struct ndp120_ph_class_params_s cl;
    } cl_;
};


/**
 * @brief Posterior Handler state
 *
 */
struct ndp120_ph_state_s {
    uint32_t current_class;
    uint32_t class_counts[NDP120_DNN_RESULT_MAX_NUM_CLASSES];
    uint32_t adaptive_class_count;
    uint32_t current_state;
    uint32_t backoff_counter;
    uint32_t timeout;
    uint32_t timeout_action;
    uint32_t frame_counter;
    uint32_t winner;
    uint32_t winner_prob;
    uint32_t threshold;
    uint32_t match;
    uint32_t window;
    uint32_t tankptr;
    uint32_t class_tankptr;
    uint32_t adaptive_frames_remaining;
    uint32_t adaptive_updates_remaining;
#ifdef X86_64
    uint32_t *params_addr;
#else
    uint32_t params_addr;
#endif
};

struct posterior_template_s {
    void (*init)(uint32_t nn_num);
    void (*process_frame)(uint32_t nn_num, struct ndp120_dnn_result_s *result,
            uint32_t tankptr);
    void* (*get_handle)(uint32_t nn_num);
    void (*reset_state)(uint32_t nn_num);
};
extern struct posterior_template_s gPh[MAX_PH_ALGOS];

enum ph_type_e {
    NDP120_PH_SCP = 0,
    /*add user defined algorithm type (ph type) here*/
    NDP120_PH_CUSTOM = 1
};

#ifndef X86_64
/**
 * @brief intializes the posterior handler params. It sets the memory to zero.
 *
 * @param params posterior handler params
 */
void ndp_ph_params_init(struct ndp120_ph_params_s *params);

/**
 * @brief intializes the posterior handler params, such as pseudo heap memory allocation.
 *
 * @param nn_num: neural network id
 */
struct ndp120_ph_params_s * ndp_ph_get_params(uint32_t nn_num);

/**
 * @brief intializes the posterior handler params, such as pseudo heap memory allocation.
 *
 * @param nn_num: neural network id
 */
void ndp_ph_init(uint32_t nn_num);

/**
 * @brief initializes the Softmax smoother from posterior handler params.
 *
 * @param ph_params posterior handler params
 * @param smoother Smoother object
 */
void ndp120_ph_init_result_smoother(struct ndp120_ph_params_s *ph_params,
    struct ndp120_result_softmax_smoother_s *smoother);

/**
 * @brief process a new frame result and picks a winner
 *
 * @param nn_num: neural network num
 *        result:
 *        tankptr:
 */
void ndp_ph_process_frame(uint8_t nn_num, struct ndp120_dnn_result_s *result, uint32_t tankptr);

/**
 * @brief get handle of a posterior
 *
 * @param nn_num: network num
 */
void* ndp_ph_get_handle(uint8_t nn_num);
#endif

void ndp_ph_init_result_smoother(void);

struct ndp120_ph_state_params_s *
ndp120_ph_state_params_get(struct ndp120_ph_params_s *ph_params, uint32_t state_num);

typedef struct syntiant_ndp120_ph_params_t_{
    uint32_t phwin;
    uint32_t phth;
    uint32_t phbackoff;
    uint32_t phaction;
    uint32_t pharg;
    uint32_t phqueuesize;
} syntiant_ndp120_ph_params_t;

typedef struct syntiant_ph_params_state_t_{
    uint32_t num_classes;
    uint32_t timeout;
    syntiant_ndp120_ph_params_t ph_class[1];
} syntiant_ph_params_state_t;

typedef struct syntiant_ndp120_ph_params_metadeta_t_{
    uint32_t num_states;
    syntiant_ph_params_state_t state[1];
} syntiant_ndp120_ph_params_metadeta_t;

struct ndp120_ph_class_params_s *
ndp_ph_class_params_get(struct ndp120_ph_params_s *ph_params,
    uint32_t state_num, uint32_t class_num);

int
ndp_valid_nn_done_message(void);

int
ndp_ph_encode_action(uint32_t ph_type, uint32_t atype, uint32_t arg0,
    uint32_t arg1, uint32_t *action);

#ifdef __cplusplus
}
#endif

#endif
