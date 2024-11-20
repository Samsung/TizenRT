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


#ifndef _NDP120_ORCHESTRATOR_H
#define _NDP120_ORCHESTRATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __KERNEL__
#include <stdint.h>
#ifdef X86_64
#include <stdio.h>
#include <stdlib.h>
#endif
#endif

#define MAX_NDP120_NNGRAPH_NODES 16
#define MAX_GATE_INPUT  4
#define MAX_GATE_OUTPUT 4

enum {
    NDP_NONE = 0,
	NDP_AND = 1,
	NDP_OR = 2,
    NDP_XOR = 3
};

enum {
	NDP_LOGIC_GATE = 0,
	NDP_NNETWORK = 1
};

/** NDP120 orchestration graph node
 * num_inputs = 0 for first set of NN nodes in the graph
 * num_outputs = 0 for terminal node of the graph
 */
typedef struct ndp120_nno_node {
    uint8_t next_ids[MAX_GATE_OUTPUT];  /**< next node id */
    uint8_t input[MAX_GATE_INPUT];      /**< ids of input nodes */
    uint8_t type;                       /**< type: network, logic */
    uint8_t action;                     /**< action type */
    uint8_t flowset_id;                 /**< DSP flow group id for this node */
    uint8_t num_inputs;                 /**< number of input edges */
    uint8_t num_outputs;                /**< number of output edges */
    uint8_t id;                         /**< node id */
    uint8_t output;                     /**< output logic */
    uint8_t status;                     /**< status: on/off */
    uint16_t timeout;                   /**< timeout for a NN node */
    uint16_t timer;                     /**< timer for a NN node */
    uint8_t preroll;                    /**< for nn node, in units of frame count (max 3)*/
    uint8_t src_buffer;
    uint8_t saved_output;               /**< saving output until time out of next NN */
    uint8_t match_summary;              /**< match summary */
} ndp120_nno_node_t;

/* NDP120 NN orchestration graph */
struct ndp120_nn_graph {
    uint8_t num_nodes;                  /**< number of nodes */
    uint8_t flowmap;                    /**< flowmap of all the NNs */
    uint8_t nn_output;                  /**< NN output of the graph */
    uint8_t enable_cascade;             /**< Enable/disable Cascaded networks  */
    uint16_t save_tank_ptr;
    uint16_t data;
    ndp120_nno_node_t nn_graph[MAX_NDP120_NNGRAPH_NODES]; /**< all the nodes */
};

/** NN graph orchestrator
 * @param nn, NN node number
 * @param match, match data
 * @param timeout, for this NN node
 * returns network output of this graph, -1 for error condition.
 */
uint8_t ndp_orchestrate(uint8_t nn, uint8_t match);

/** Get active set of NN nodes in the graph
 */
void ndp_trace_graph(uint8_t, uint32_t *output);

#ifdef __cplusplus
} /* extern "C" { */
#endif
#endif /* _NDP120_ORCHESTRATOR_H */
