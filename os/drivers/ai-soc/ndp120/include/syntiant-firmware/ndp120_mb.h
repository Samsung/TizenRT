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

#ifndef NDP120_MB_H
#define NDP120_MB_H

#include "syntiant-firmware/ndp120_result.h"

/**
 * @brief defines the states of MCU to HOST mailbox exchange
 *
 */
enum m2h_state_e { M2H_STATE_IDLE = 0, M2H_STATE_MATCH = 1 };

/**
 * @brief defines the states of HOST to MCU mailbox exchange
 *
 */
enum h2m_state_e {
    H2M_STATE_IDLE = 0,
    H2M_STATE_EXTOP = 1,
    H2M_STATE_DATA_OUT = 2,
    H2M_STATE_LOAD = 3
};

/**
 * @brief define constants to extract information from mailbox.
 *
 */
enum ndp_mb_protcol_e {
    NDP_MB_HOST_TO_MCU_OWNER = 0x80,
    NDP_MB_HOST_TO_MCU_M = 0x7F,
    NDP_MB_HOST_TO_MCU_S = 0,
    NDP_MB_MCU_TO_HOST_OWNER = 0x80,
    NDP_MB_MCU_TO_HOST_M = 0x7F,
    NDP_MB_MCU_TO_HOST_S = 0
};


#define NDP_MB_HOST_TO_MCU_INSERT(m, r) \
    ((((m) & ~NDP_MB_HOST_TO_MCU_M) \
      ^ NDP_MB_HOST_TO_MCU_OWNER) \
     | ((r) << NDP_MB_HOST_TO_MCU_S))

#define NDP_MB_MCU_TO_HOST_INSERT(m, r) \
    ((((m) & ~NDP_MB_MCU_TO_HOST_M) \
      ^ NDP_MB_MCU_TO_HOST_OWNER) \
     | ((r) << NDP_MB_MCU_TO_HOST_S))


/* Host -> MCU */
enum ndp_mbin_req_e {
    NDP_MBIN_REQUEST_NOP      = 0x0,
    NDP_MBIN_REQUEST_CONT     = 0x1,
    NDP_MBIN_REQUEST_PREPARE  = 0x2,
    NDP_MBIN_REQUEST_EXTENDED_CMDS = 0x3,
    NDP_MBIN_REQUEST_DATA     = 0x4,
    NDP_MBIN_REQUEST_SECURE_CMDS = 0x5,
    NDP_MBIN_REQUEST_TILIB_CMDS = 0x6,
    NDP_MBIN_REQUEST_PH_HEAP_ALLOCATION = 0x7,
    NDP_MBIN_REQUEST_MIADDR   = 0x8,
    NDP_MBIN_REQUEST_LOAD     = 0x9,

    NDP_MBIN_REQUEST_IGNORE_START = 0xa,
    NDP_MBIN_REQUEST_IGNORE_END   = 0xf,
    NDP_MBIN_H2DSP_REQUEST    = (1<<6)
};

enum ndp_mbin_extended_req_e {
    NDP120_MBIN_H2M_EXT__START__            = 0x30,
    NDP_MBIN_REQUEST_RESET_INFERENCE_STATE  = NDP120_MBIN_H2M_EXT__START__,
    NDP_MBIN_REQUEST_SET_AUTO_CLEAR_INT     = 0x31,
    NDP_MBIN_REQUEST_GET_PH_ENABLED_STATE   = 0x32,
    NDP_MBIN_REQUEST_WFI_CONTROL_ON         = 0x33,
    NDP_MBIN_REQUEST_WFI_CONTROL_OFF        = 0x34,
    NDP_MBIN_REQUEST_POSTERIOR_HANDLER      = 0x35,
    NDP_MBIN_REQUEST_GET_MEM_INFO           = 0x36,
    NDP120_MBIN_H2M_EXT__END__              = 0x3F
};

/* Following MB extender commands are keyed off NDP_MBIN_REQUEST_SECURE_CMDS and
 * use open ram for communicating with the MCU
 */
enum ndp_mbin_sec_req_e {
    NDP_MBIN_REQUEST_SECURE_MAIN_CLK = 0x501,
    NDP_MBIN_REQUEST_SECURE_START = 0x502,
    NDP_MBIN_REQUEST_SECURE_STOP = 0x503,
    NDP_MBIN_REQUEST_SECURE_GET_INFO = 0x505,
    NDP_MBIN_REQUEST_SECURE_GET_NN_GRAPH = 0x506,
    NDP_MBIN_REQUEST_SECURE_GET_DEBUG = 0x507,
    NDP_MBIN_REQUEST_SECURE_GET_DSP_FLOW = 0x508,
    NDP_MBIN_REQUEST_SECURE_GET_AUDIO_CHUNK_SIZE = 0x509,
    NDP_MBIN_REQUEST_SECURE_INPUT_CONFIG = 0x50a,
    NDP_MBIN_REQUEST_SECURE_SPI_DIRECT = 0x50c,
    NDP_MBIN_REQUEST_SECURE_SWITCH_FLOW_SET = 0x50d,
    NDP_MBIN_REQUEST_SECURE_GET_SENSOR_SAMPLE_SIZE = 0x50e,
    NDP_MBIN_REQUEST_SECURE_CONFIG_MISC_INTERRUPTS = 0x50f,
    NDP_MBIN_REQUEST_SECURE_POSTERIOR_CONFIG = 0x510,
    NDP_MBIN_REQUEST_SECURE_ALGO_CONFIG_AREA = 0x511,
    NDP_MBIN_REQUEST_SECURE_INIT_EXTRACTION = 0x512,
    NDP_MBIN_REQUEST_SECURE_INIT_RING_BUF_PTRS = 0x513,
    NDP_MBIN_REQUEST_SECURE_POSTERIOR_HANDLER = 0x514
};

/* Following MB extender commands are keyed off NDP_MBIN_REQUEST_TILIB_CMDS and
 * use open ram for communicating with the MCU
 */
enum ndp_mbin_tilib_req_e {
    NDP_MBIN_REQUEST_TILIB_LOAD_DSP = 0x600,
    NDP_MBIN_REQUEST_TILIB_LOAD_NN = 0x601,
    NDP_MBIN_REQUEST_TILIB_MAIN_CLK_CONFIG = 0x602,
    NDP_MBIN_REQUEST_TILIB_PDM_CLK_START = 0x603,
    NDP_MBIN_REQUEST_TILIB_PDM_CLK_STOP = 0x604,
    NDP_MBIN_REQUEST_TILIB_GET_INFO = 0x606,
    NDP_MBIN_REQUEST_TILIB_GET_NN_GRAPH = 0x607,
    NDP_MBIN_REQUEST_TILIB_GET_DEBUG = 0x608,
    NDP_MBIN_REQUEST_TILIB_GET_DSP_FLOW = 0x609,
    NDP_MBIN_REQUEST_TILIB_GET_AUDIO_CHUNK_SIZE = 0x60a,
    NDP_MBIN_REQUEST_TILIB_INPUT_CONFIG = 0x60b,
    NDP_MBIN_REQUEST_TILIB_SPI_DIRECT = 0x60c,
    NDP_MBIN_REQUEST_TILIB_SWITCH_FLOW_SET = 0x60d,
    NDP_MBIN_REQUEST_TILIB_SERIAL_TRANSFER = 0x610,
    NDP_MBIN_REQUEST_TILIB_CONFIG_PDM_PARAMS = 0x611,
    NDP_MBIN_REQUEST_TILIB_CONFIG_INTERRUPTS = 0x612,
    NDP_MBIN_REQUEST_TILIB_WFI_CONTROL = 0x613,
    NDP_MBIN_REQUEST_TILIB_POSTERIOR_CONFIG = 0x614,
    NDP_MBIN_REQUEST_TILIB_ALGO_CONFIG_AREA = 0x615,
    NDP_MBIN_REQUEST_TILIB_GET_SENSOR_SAMPLE_SIZE = 0x616
};

/* mbin_respone */
/* response from MCU -> Host */
enum ndp_mb_response_e {
    NDP_MBIN_RESPONSE_SUCCESS   = 0x0,
    NDP_MBIN_RESPONSE_CONT      = 0x1,
    NDP_MBIN_RESPONSE_ERROR     = (1<<5),
    NDP_MBIN_RESPONSE_H2DSP_RESP = (1<<6)
};

#define NDP_MBIN_RESPONSE_PH_EXCLUDED 0x1

/* mbout */
/* request from MCU -> Host */
enum ndp_mbout_req_e {
    NDP_MBOUT_REQ_SUCCESS   = 0x0,
    NDP_MBOUT_REQ_CONT      = 0x1,
    NDP_MBOUT_REQ_RUNNING   = 0x10,
    NDP_MBOUT_REQ_BOOTING   = 0x11,
    NDP_MBOUT_REQ_LOAD_DONE = 0x12,
    NDP_MBOUT_REQ_RUNNING_TEST = 0x13,
    NDP_MBOUT_OTHER_LOAD_DONE = 0x14,
    NDP_MBOUT_REQ_HEARTBEAT = 0x17,
    NDP_MBOUT_REQ_SECURE_EXTRACT = 0x18,
    NDP_MBOUT_REQ_ERROR     = (1<<5),
    NDP_MBOUT_REQ_MATCH_NETWORK_MASK = 0x40
};

enum ndp_mbout_response_e {
    NDP_MBOUT_RESPONSE_DSP2H_RESP = (1<<6)
};

/* error codes (6-bit) */
enum ndp_mb_error_e {
    NDP_MB_ERROR_NONE = 0x0,
    NDP_MB_ERROR_UNEXPECTED = 0x1,
    NDP_MB_ERROR_PACKAGE_MAGIC_TLV = 0x2,
    NDP_MB_ERROR_PACKAGE_FW_SIZE = 0x3,
    NDP_MB_ERROR_PACKAGE_INTEGRITY = 0x4,
    NDP_MB_ERROR_PACKAGE_MISSING_FW = 0x5,
    NDP_MB_ERROR_PACKAGE_FORMAT = 0x6,
    NDP_MB_ERROR_AUTH = 0x7,
    NDP_MB_ERROR_AES = 0x8,
    NDP_MB_ERROR_I2C = 0xa,
    NDP_MB_ERROR_PACKAGE_NN_METADATA = 0xb,
    NDP_MB_ERROR_FLL_NOT_LOCKED = 0xc,
    NDP_MB_ERROR_INVALID_PIN_MUX_CONFIG = 0xd,
    NDP_MB_ERROR_DONT_SEND_RESPONSE = 0xe,
    NDP_MB_ERROR_M2D_TIMEOUT = 0xf,
    NDP_MB_ERROR_HW_SERIAL_FLASH = 0x10,
    NDP_MB_ERROR_HW_MCU_EXCEPTION = 0x11,
    NDP_MB_ERROR_UNALLOC_MEM = 0x12,
    NDP_MB_ERROR_DSP_CONFIG = 0x13,
    NDP_MB_ERROR_INVALID_CONFIG = 0x14,
    NDP_MB_ERROR_INVALID_TLV_LEN = 0x15,
    NDP_MB_ERROR_INVALID_CHIP_TYPE = 0x16,
    NDP_MB_ERROR_INVALID_PDM_CONFIG = 0x17,
    NDP_MB_ERROR_INVALID_PH_ACTION = 0x18,
    NDP_MB_ERROR_INVALID_PH_SQ_SZ = 0x19,
    NDP_MB_ERROR_INVALID_PH = 0x1a,
    NDP_MB_ERROR_INVALID_ALGO_CONFIG = 0x1b,
    NDP_MB_ERROR_INVALID_PBI = 0x1c,
    NDP_MB_ERROR_INVALID_SENSOR_CONFIG = 0x1d,
    NDP_MB_ERROR_TIMEOUT = 0x1e,
    NDP_MB_ERROR_INVALID_CLOCK = 0x1f,
    NDP_MB_ERROR_ARGS = 0x20,
    NDP_MB_ERROR_NO_MEMORY = 0x21
};

#define NDP_CONFIG_ERROR_INVALID_CFG_ID  (0xDEAD0001U)
#define NDP_CONFIG_ERROR_INSUFF_MEMORY   (0xDEAD0002U)
#define NDP_CONFIG_ERROR_INVALID_SIZE    (0xDEAD0003U)
#define NDP_CONFIG_ERROR_CONFIG_OTHER    (0xDEAD0004U)

#define NDP_MB_ERROR_INSERT(e) \
    ((NDP_MBIN_RESPONSE_ERROR) | (e))

#define NDP120_SPI_MATCH_MATCH_MASK 0x40
enum ndp_mb_match_offsets_e {
    NDP120_MB_MATCH_SUMMARY_O = 0x00,
    NDP120_MB_MATCH_BINARY_O = 0x04,
    NDP120_MB_MATCH_STRENGTH_O = 0x0c
};

/**
 * @brief Mailbox state object
 *
 */

struct ndp120_mb_state_s {
    /* start ordering match with ndp10x*/
    uint32_t enable_match_for_every_frame;

    uint32_t m2h_state;
    uint32_t m2h_req;
    uint32_t m2h_rsp_success;
    uint32_t m2h_rsp_unknown;
    uint32_t m2h_match_skipped;

    uint32_t h2m_state;
    uint32_t h2m_req_nop;
    uint32_t h2m_req_extop;
    uint32_t h2m_req_data;
    uint32_t h2m_req_cont;

    uint32_t h2m_unexpected_nop;
    uint32_t h2m_unexpected_extop;
    uint32_t h2m_unexpected_cont;
    uint32_t h2m_unexpected_data;
    uint32_t h2m_req_unknown;
    uint32_t h2m_extop_unknown;

    uint32_t h2m_data;
    uint32_t h2m_data_count;
    uint32_t previous_mbox;
    /* end match with ndp10x*/

    uint8_t h2m_req_load;
    uint8_t h2m_unexpected_load;

    uint8_t h2m_req_prepare;
    uint8_t h2m_unexpected_prepare;

    uint32_t previous_mbox_out;
    uint32_t previous_mbox_in;

    uint8_t h2m_req_heap_allocation;
    uint8_t h2m_unexpected_heap_allocation;

    uint8_t h2m_req_secure_start;
    uint8_t h2m_req_secure_stop;
};

/**
 * @brief Send host to MCU request response.
 * enum ndp_mb_response_e
 *
 * @param bstate : firmware mailbox state object
 * @param response : response defined in ndp_mb_response_e
 */
void
ndp_send_h2m_mbin_response(
    struct ndp120_mb_state_s* bstate, uint8_t response);

/**
 * @brief Sends a M2H request to host
 *
 * @param bstate : firmware mailbox state object
 * @param request Request to be sent to Host
 */
void
ndp_send_m2h_mbout_request(
    struct ndp120_mb_state_s* bstate, uint8_t request);

/**
 * @brief Send a MATCH request to host based on result
 *
 * @param mb_state   firmware mailbox state object
 * @param result result calculated by posterior handler.
 * @param network_id neural network id
 * @param network_output output data from neural network graph
 */
void
ndp_mb_send_match(
        struct ndp120_main_state_s* fw_main_state,
        struct ndp120_dnn_result_s *result,
        uint8_t network_id,
        uint8_t network_output);

/**
 * @brief Send a error request to host
 *
 * @param error_code specifies type of the error
 */
void
ndp_mb_send_error(uint8_t error_code);

/* algo config area */
struct syntiant_algo_area_s {
    uint8_t algo_id;
    uint8_t op;
    union {
        uint8_t size_;    /* create */
        uint8_t offset;   /* write, read */
    } u;
    uint8_t length;  /* payload length */
    /* payload follows at end of struct */
};

enum syntiant_algo_area_op_e {
    NDP120_ALGO_OP_CREATE = 1,
    NDP120_ALGO_OP_WRITE = 2,
    NDP120_ALGO_OP_READ = 3
};

enum syntiant_algo_area_payload_length_limits_e {
    NDP120_ALGO_AREA_PAYLOAD_MIN_LEN = 32,
    NDP120_ALGO_AREA_PAYLOAD_MAX_LEN = 256
};

#define NDP120_ALGO_AREA_ALIGNMENT  32U  /* power of 2 */

/* arbitrary posterior handler */

struct syntiant_posterior_handler_s {
    uint8_t id;
    uint8_t op;
    uint8_t nn_id;    /* valid only for op == NDP120_ALGO_OP_ATTACH_NN */
    uint8_t pad;
    uint32_t length;  /* payload length */
    /* payload follows this struct */
};

enum syntiant_posterior_handler_constants_e {
    SYNTIANT_POSTERIOR_HANDLER_DATA_MAXLEN = 1024
};

enum syntiant_posterior_handler_op_e {
    NDP120_PH_OP_CREATE = 1,
    NDP120_PH_OP_WRITE = 2,
    NDP120_PH_OP_READ = 3,
    NDP120_PH_OP_ATTACH_NN = 4
};


#endif
