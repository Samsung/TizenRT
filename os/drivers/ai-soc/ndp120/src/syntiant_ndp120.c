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

#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_ilib/syntiant_ndp_ilib_version.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_ilib/syntiant_ndp_driver.h>
#include <syntiant_ilib/syntiant_ndp_error.h>

#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>
#include <syntiant-dsp-firmware/ndp120_dsp_sample.h>
#include <syntiant-dsp-firmware/ndp120_dsp_mailbox.h>
#include <syntiant-dsp-firmware/ndp120_dsp_flow.h>
#include <syntiant-dsp-firmware/ndp120_dsp_memory.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp115.h>
#include <syntiant_ilib/ndp120_spi_regs.h>
#include <syntiant_ilib/ndp120_regs.h>
#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp120_mailbox.h>
#include <syntiant_ilib/syntiant_ndp120_ph.h>

#include <syntiant_packager/syntiant_package_consts.h>
#include <syntiant_packager/syntiant_package.h>

#include <syntiant-firmware/ndp120_ph.h>
#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant-firmware/ndp120_result.h>
#include <syntiant-firmware/ndp120_mb.h>

#include "syntiant_ndp120_config.h"
#include "syntiant_ndp_config.h"

#if SYNTIANT_NDP120_DEBUG_POLL
#ifndef __KERNEL__
#include <unistd.h>
#include <sys/time.h>
#endif

/* Convenience macros for operations on timevals.
NOTE: `timercmp' does not work for >= or <=.  */
#ifndef timerisset
# define timerisset(tvp)    ((tvp)->tv_sec || (tvp)->tv_usec)
#endif
#ifndef timerclear
# define timerclear(tvp)    ((tvp)->tv_sec = (tvp)->tv_usec = 0)
#endif
#ifndef timercmp
# define timercmp(a, b, CMP)                                \
  (((a)->tv_sec == (b)->tv_sec) ?                           \
   ((a)->tv_usec CMP (b)->tv_usec) :                        \
   ((a)->tv_sec CMP (b)->tv_sec))
#endif
#ifndef timeradd
# define timeradd(a, b, result)                             \
  do {                                                      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;           \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;        \
    while ((result)->tv_usec >= 1000000)                    \
      {                                                     \
        ++(result)->tv_sec;                                 \
        (result)->tv_usec -= 1000000;                       \
      }                                                     \
  } while (0)
#endif

#ifndef timersub
# define timersub(a, b, result)                             \
  do {                                                      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;           \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;        \
    if ((result)->tv_usec < 0) {                            \
      --(result)->tv_sec;                                   \
      (result)->tv_usec += 1000000;                         \
    }                                                       \
  } while (0)
#endif
#endif

#if SYNTIANT_NDP120_DEBUG
int DEBUG_PRINTF(const char *fmt, ...) {
    int ret;
    va_list args;
    va_start(args, fmt);
#ifdef __KERNEL__
    ret = vprintk(fmt, args);
#else
    ret = vfprintf(stderr, fmt, args);
#endif
    va_end(args);
    return ret;
}
#else
int DEBUG_PRINTF(const char * fmt, ...) { (void)fmt; return 0; }
#endif

#define minimum(a, b) ((a) < (b) ? (a) : (b))
#define UNALIGNED_SIZE(size, align)  ((size) & ((align) - 1))

#define DNN_PAYLOAD_MAX_SIZE   8
#define DNNBANK_MAX_PARAM_MASK 0XFFFFF
#define DNNBANK_MAX_DATA_MASK 0XFFFF

#define CUSTOM_MUTLI_INPUT_ALGO_CONFIG_SIGNATURE    0x04030201
#define CUSTOM_MUTLI_INPUT_ALGO_CONFIG_MAX_SIZE     32
#define CUSTOM_MUTLI_INPUT_ALGO_CONFIG_EXEC_TYPE    1

/* defined in DSP fw_state.h */
#define SYNTIANT_NDP120_DSP_DNN_CONFIGURED        (1)

struct ndp120_dsp_state {
    uint8_t last_network;
    uint8_t decibel_measured;
    uint8_t sampletank_mem_type;
    uint8_t device_state;
};

struct syntiant_sensor_cfg_state_t {
    uint8_t num_sensor_cfg;
    struct syntiant_ndp_device_s *ndp;
};

struct ring_buffer_pointers {
    uint32_t pcm_audio_sample_last_ptr;
    uint32_t function_sample_last_ptr;
    uint32_t pcm_audio_annotation_last_ptr;
    uint32_t sensor_sample_last_ptr;
} ring_buf_ptrs;

static struct syntiant_sensor_cfg_state_t syntiant_sensor_cfg_state = { 0 };
static syntiant_sensor_config_t sensor_cfg_saved[NDP_FW_SENSOR_MAX];
static int syntiant_ndp120_mailbox_trace = 0;
static uint32_t syntiant_ndp120_mailbox_req_number[2] = { 0, 0 };
static uint32_t syntiant_ndp120_mailbox_rsp_number[2] = { 0, 0 };
static uint32_t syntiant_ndp120_mailbox_unexpected[2] = { 0, 0 };
static uint32_t syntiant_ndp120_mailbox_error[2] = { 0, 0 };

static unsigned int syntiant_ndp120_device_types[] = {
    0x30, 0x31, 0x32, 0x33, /* QFN48  NDP120A0 Samples */
    0x34, 0x35, 0x36, 0x37, /* QFN48  NDP120B0         */
    0x38, 0x39, 0x3A, 0x3B, /* WLBA42 NDP120B0         */
    0x3C, /* Unlocked NDP120B0            */
    0x3D, /* Serial flash NDP120B0        */
    0x3E, /* Secure boot NDP120B0         */
    0x3F, /* Secure serial flash NDP120B0 */
    0x40, 0x41, 0x42, 0x43, /* NDP200A0                */
    0x0  /* len of list */
};

ndp120_debug_callback_ptr_t ndp120_debug_callback = NULL;

void syntiant_ndp120_set_debug_callback(ndp120_debug_callback_ptr_t p)
{
    ndp120_debug_callback = p;
}

/* typedefs */

/* pre-declare */

static int syntiant_ndp120_alloc_algo_config_area_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t algo_config_idx, uint32_t size);

/* helper functions */

static uint32_t ndp_umin(uint32_t a, uint32_t b)
{
    return minimum(a, b);
}

#if SYNTIANT_NDP120_DEBUG && SYNTIANT_NDP120_HEXDUMP
static void hexdump(unsigned addr, void *buf,  unsigned len)
{
    uint32_t *p = (uint32_t *)buf;
    unsigned i;
    for (i = 0; i < len; i += (unsigned)sizeof(*p), addr += (unsigned)sizeof(*p), p++) {
        if (i % 16 == 0) {
            printf("%#010x:", addr);
        }
        printf(" %08x", *p);
        if (i % 16 == 12) {
            printf("\n");
        }
    }
    if (i % 16) {
        printf("\n");
    }
}
#endif

enum {
    MB_STATE_NONE = 0,
    MB_STATE_SYNC = 1,
    MB_STATE_DATA = 2
};

struct mb_state_cache {
    uint8_t mbin_seq;
    uint8_t mbin_resp_seq;
    uint8_t mbout_seq;
    uint8_t mbin_resync;
    uint8_t mbin_resp;
    uint8_t mbin_state;
    uint8_t watermarkint_resync;
    uint8_t rsvd;
};

static int
mcu_fw_pointers(struct syntiant_ndp_device_s *ndp, int clear)
{
    int s = 0;
    struct ndp120_fw_state_pointers_s fwps;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (clear) {
        ndp120->mcu_fw_pointers_addr = ndp120->mcu_fw_state_addr
            = ndp120->mcu_fw_ph_data_coll_addr
            = ndp120->mcu_fw_smax_smoother_addr = 0;
        goto error;
    }

    s = ndp_mcu_read_block(ndp120->mcu_fw_pointers_addr, fwps.addresses, sizeof(fwps.addresses));
    if (s) goto error;

    ndp120->mcu_fw_state_addr
        = fwps.addresses[NDP120_FW_STATE_ADDRESS_INDEX_FW_STATE];
    ndp120->mcu_fw_ph_data_coll_addr
        = fwps.addresses[NDP120_FW_STATE_ADDRESS_INDEX_PH_DATA_COLL];
    ndp120->mcu_fw_smax_smoother_addr
        = fwps.addresses[NDP120_FW_STATE_ADDRESS_INDEX_SMAX_SMOOTHER];
    ndp120->mcu_fw_orchestrator_graph_addr
        = fwps.addresses[NDP120_FW_STATE_ADDRESS_INDEX_ORCHESTRATOR_PARAMS];
    ndp120->mcu_fw_dbg_state_addr
        = fwps.addresses[NDP120_FW_STATE_ADDRESS_INDEX_DBG_STATE];

    if (ndp120->mcu_fw_pointers_addr) {
        ndp120->mcu_fw_loaded = 1;
    }
    DEBUG_PRINTF("  miaddr: 0x%08X\n", ndp120->mcu_fw_pointers_addr);
    DEBUG_PRINTF("  fw_state: 0x%08X\n", ndp120->mcu_fw_state_addr);
    DEBUG_PRINTF("  ph_data_coll: 0x%08X\n", ndp120->mcu_fw_ph_data_coll_addr);
    DEBUG_PRINTF("  smoother: 0x%08X\n", ndp120->mcu_fw_smax_smoother_addr);
    DEBUG_PRINTF("  orch_graph: 0x%08X\n", ndp120->mcu_fw_orchestrator_graph_addr);
    DEBUG_PRINTF("  dbg_state: 0x%08X\n", ndp120->mcu_fw_dbg_state_addr);

error:
    return s;
}

static int clear_mcu_fw_pointers(struct syntiant_ndp_device_s *ndp)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    ndp120->mcu_fw_loaded = 0;
    return mcu_fw_pointers(ndp, 1);
}

static int get_mcu_fw_pointers(struct syntiant_ndp_device_s *ndp)
{
    return mcu_fw_pointers(ndp, 0);
}

static void clear_dsp_fw_pointers(struct syntiant_ndp_device_s *ndp)
{
    ndp->d.ndp120.dsp_fw_state_addr = 0;
}

uint32_t syntiant_ndp120_get_dsp_fw_pointer(struct syntiant_ndp_device_s *ndp)
{
    return ndp->d.ndp120.dsp_fw_state_addr;
}

uint32_t syntiant_ndp120_get_mcu_fw_pointer(struct syntiant_ndp_device_s *ndp)
{
    return ndp->d.ndp120.mcu_fw_state_addr;
}

uint32_t syntiant_ndp120_get_mcu_dbg_state_addr(struct syntiant_ndp_device_s *ndp)
{
    return ndp->d.ndp120.mcu_fw_dbg_state_addr;
}

int syntiant_ndp120_halt_mcu(struct syntiant_ndp_device_s *ndp)
{
    int s;
    uint8_t data;

    s = ndp_spi_read(NDP120_SPI_CTL, &data);
    if (s) goto error;
    data = NDP120_SPI_CTL_MCUHALT_INSERT(data, 1);
    s = ndp_spi_write(NDP120_SPI_CTL, data);
    if (s) goto error;

error:
    return s;
}

static int
syntiant_ndp120_pkg_boiler_plate(syntiant_pkg_parser_state_t *pstate)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0) {
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes) {
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = *(uint32_t *)pstate->length;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_softmax(syntiant_pkg_parser_state_t *pstate)
{
    unsigned int byte_read = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* sanity checks on the TLV */
    }

    memset(&pstate->data.softmax, 0, sizeof(pstate->data.softmax));
    byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&pstate->data.softmax, *(uint32_t *)pstate->length, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    return syntiant_ndp120_pkg_boiler_plate(pstate);
}


int
syntiant_ndp120_pkg_parse_auto_clear_interrupt(
    syntiant_pkg_parser_state_t *pstate)
{
    unsigned int byte_read = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* sanity checks on the TLV */
    }

    memset(&pstate->data.softmax, 0, sizeof(pstate->data.auto_clear));
    byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&pstate->data.auto_clear, *(uint32_t *)pstate->length, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    return syntiant_ndp120_pkg_boiler_plate(pstate);
}

int
syntiant_ndp120_pkg_parse_dnn_pwr_config(syntiant_pkg_parser_state_t *pstate)
{
    unsigned int byte_read = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* sanity checks on the TLV */
    }

    memset(&pstate->data.dnn_power, 0, sizeof(pstate->data.dnn_power));
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*) &pstate->data.dnn_power, *(uint32_t *) pstate->length, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    return syntiant_ndp120_pkg_boiler_plate(pstate);
}

int
syntiant_ndp120_pkg_parse_read_dsp_fe_config_v3(
    syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    syntiant_dsp_fe_config_v3_data_t *config = &pstate->data.dsp_fe_config_v3;
    unsigned bytes_wanted;

    config->parser.parsed = config->parser.parsing;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        memset(config, 0, sizeof(syntiant_dsp_fe_config_v3_data_t));
        config->parser.parsing = ALGO_PARSE_HEADER;
        config->parser.parsed = 0;

    } else if (config->parser.parsing == ALGO_PARSE_HEADER) {
        bytes_wanted
            = offsetof(syntiant_dsp_fe_config_v3_data_t, melbin_boundary);
        if (pstate->expected_bytes == 0) {
            pstate->expected_bytes = bytes_wanted;
        }
        pstate->partially_read_length
            += syntiant_pkg_read_chunk_data(pstate, (uint8_t *)&config->index,
                bytes_wanted, 0, 1, 1);
        if (pstate->expected_bytes == 0) {
            if (config->num_filter_banks >= NUM_FILTER_BANKS_COUNT_MAX) {
                SYNTIANT_PRINTF("ERROR (EXCEEDS max filter banks)\n");
                s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                goto error;
            }
            config->parser.parsing = ALGO_PARSE_ARRAY;
            config->parser.next_index = 0;
        }

    } else if (config->parser.parsing == ALGO_PARSE_ARRAY) {
        config->parser.index = config->parser.next_index;
        if (config->parser.index < (unsigned)config->num_filter_banks + 2) {
            unsigned banks_remaining = (unsigned)config->num_filter_banks + 2
                - config->parser.index;
            unsigned bank_slice_count = minimum(banks_remaining, 10);
            bytes_wanted = bank_slice_count
                * (unsigned)sizeof(*config->melbin_boundary);
            if (pstate->expected_bytes == 0) {
                pstate->expected_bytes = bytes_wanted;
            }
            pstate->partially_read_length
                += syntiant_pkg_read_chunk_data(pstate,
                    (uint8_t *)(config->melbin_boundary
                        + config->parser.index), bytes_wanted, 0, 1, 1);
            if (pstate->expected_bytes == 0) {
                config->parser.count = bank_slice_count;
                config->parser.next_index = config->parser.index
                    + bank_slice_count;
            }
        } else if (config->parser.index == (unsigned)
                   config->num_filter_banks + 2) {
            unsigned residual;
            if (config->num_filter_banks % 2 == 0) {
                /* melbin_boundary[] has 2 more alignment bytes */
                uint16_t align;
                bytes_wanted = sizeof(align);
                if (pstate->expected_bytes == 0)
                    pstate->expected_bytes = bytes_wanted;
                pstate->partially_read_length
                    += syntiant_pkg_read_chunk_data(
                        pstate, (uint8_t *)&align, bytes_wanted, 0, 1, 1);
            }
            residual = *(uint32_t *)pstate->length
                            - (unsigned)pstate->partially_read_length;
            if (residual) {
                SYNTIANT_PRINTF("ERROR (residual=%d)\n", residual);
                s =  SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                goto error;
            }
        } else {
            SYNTIANT_PRINTF("ERROR (overstep array)\n");
            s =  SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }

    } else {
        SYNTIANT_PRINTF("ERROR (unknown parsing %u)\n",
            config->parser.parsing);
        s =  SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
        goto error;
    }

    pstate->value_mode = (pstate->expected_bytes == 0)
        ? PACKAGE_MODE_VALID_PARTIAL_VALUE : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode
        = (pstate->partially_read_length < *(uint32_t *)pstate->length)
            ? PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_algo_attach(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.algo_attach.v1);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.algo_attach.v1, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_dsp_flow_collection(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    if (*(uint32_t *)pstate->tag == TAG_NDP120_B0_DSP_FLOW_COLLECTION_V1) {
        size = sizeof(pstate->data.dsp_flow_collection.v1);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->data.dsp_flow_collection.v1)),
            size, 0, 1, 1);
    } else if (*(uint32_t *)pstate->tag == TAG_NDP120_B0_DSP_FLOW_COLLECTION_V2) {
        size = sizeof(pstate->data.dsp_flow_collection.v2);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->data.dsp_flow_collection.v2)),
            size, 0, 1, 1);
    } else {
        s = SYNTIANT_PACKAGE_ERROR_NDP120_B0_DSP_FLOW_COLLECTION_TAG;
        goto error;
    }
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_mcu_orchestrator(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.mcu_orchestrator.v1);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->data.mcu_orchestrator.v1)),
            size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}


int
syntiant_ndp120_pkg_parse_mcu_orchestrator_v3(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.mcu_orchestrator.v3);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->data.mcu_orchestrator.v3)),
            size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_pdm_cfg_v1(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pdm_cfg.tlv85);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pdm_cfg.tlv85, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_pdm_cfg_v2(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pdm_cfg.tlv96);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pdm_cfg.tlv96, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}


int
syntiant_ndp120_pkg_parse_pin_mux_cfg(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pin_mux_cfg);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pin_mux_cfg, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_i2s_in_cfg(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pdm_cfg.tlv86);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pdm_cfg.tlv86, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_i2s_out_cfg(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pdm_cfg.tlv87);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pdm_cfg.tlv87, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_dsp_sync_cfg(syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.pdm_cfg.tlv88);
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.pdm_cfg.tlv88, size, 0, 1, 1);
    pstate->partially_read_length += byte_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes){
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }
error:
    return s;
}


int
syntiant_ndp120_pkg_parse_sensor_cfg(syntiant_pkg_parser_state_t *pstate)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;

    pstate->data.sensor_cfg.v1.parser.parsed =
        pstate->data.sensor_cfg.v1.parser.parsing;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        memset(&pstate->data.sensor_cfg.v1, 0,
                sizeof(pstate->data.sensor_cfg.v1));
        pstate->data.sensor_cfg.v1.parser.parsing = SENC_PARSE_BASE_CONFIG;
        pstate->data.sensor_cfg.v1.parser.parsed = 0;
    } else if (pstate->data.sensor_cfg.v1.parser.parsing ==
            SENC_PARSE_BASE_CONFIG){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.sensor_cfg.v1.base_sensor_config);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.sensor_cfg.v1.base_sensor_config)),
                sizeof(pstate->data.sensor_cfg.v1.base_sensor_config), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            if (pstate->data.sensor_cfg.v1.base_sensor_config.
                num_ifc_transactions) {
                pstate->data.sensor_cfg.v1.parser.parsing =
                        SENC_PARSE_IFC_TRANSACTION;
                pstate->data.sensor_cfg.v1.parser.cur_transaction = 0;
            } else {
                pstate->data.sensor_cfg.v1.parser.parsed = SENC_PARSE_DONE;

                /* save sensor related config data for next step */
                sensor_cfg_saved[syntiant_sensor_cfg_state.num_sensor_cfg].v1
                                = pstate->data.sensor_cfg.v1;
                syntiant_sensor_cfg_state.num_sensor_cfg++;
            }
        }
    } else if (pstate->data.sensor_cfg.v1.parser.parsing ==
            SENC_PARSE_IFC_TRANSACTION) {
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.sensor_cfg.v1.sensor_trans[0]);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.sensor_cfg.v1.sensor_trans[pstate->
                 data.sensor_cfg.v1.parser.cur_transaction])),
                 sizeof(pstate->data.sensor_cfg.v1.sensor_trans[0]), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->data.sensor_cfg.v1.parser.cur_transaction++;
            if (pstate->data.sensor_cfg.v1.parser.cur_transaction <
                pstate->data.sensor_cfg.v1.base_sensor_config.num_ifc_transactions) {
                pstate->data.sensor_cfg.v1.parser.parsing =
                    SENC_PARSE_IFC_TRANSACTION;
            } else {
                pstate->data.sensor_cfg.v1.parser.parsed = SENC_PARSE_DONE;

                /* save sensor related config data for next step */
                sensor_cfg_saved[syntiant_sensor_cfg_state.num_sensor_cfg].v1
                                = pstate->data.sensor_cfg.v1;
                syntiant_sensor_cfg_state.num_sensor_cfg++;
            }
        }
    } else {
        s =  SYNTIANT_PACKAGE_ERROR_SENSOR_CONFIG_V1;
        goto error;
    }

    pstate->value_mode =
        (pstate->expected_bytes == 0) ?
        PACKAGE_MODE_VALID_PARTIAL_VALUE : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t *)pstate->length) ?
                    PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_nn_metadata(syntiant_pkg_parser_state_t *pstate)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;

    pstate->data.nn_metadata.v1.parser.parsed =
        pstate->data.nn_metadata.v1.parser.parsing;

    if (pstate->data.nn_metadata.v1.parser.parsing ==
            NNM_PARSE_HEADER){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.nn_metadata.v1.initial_meta);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.nn_metadata.v1.initial_meta)),
                sizeof(pstate->data.nn_metadata.v1.initial_meta), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->data.nn_metadata.v1.parser.parsing = NNM_PARSE_BASE_META;

            /* Need to have a least one set of metadata */
            if (!pstate->data.nn_metadata.v1.initial_meta.nn_num){
                s =  SYNTIANT_PACKAGE_ERROR_NN_METADATA_V1;
                goto error;
            }
            pstate->data.nn_metadata.v1.parser.cur_nn = 0;
        }

    } else if (pstate->data.nn_metadata.v1.parser.parsing ==
            NNM_PARSE_BASE_META){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.nn_metadata.v1.base_meta);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.nn_metadata.v1.base_meta)),
                sizeof(pstate->data.nn_metadata.v1.base_meta), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->data.nn_metadata.v1.parser.cur_nn += 1;
            pstate->data.nn_metadata.v1.parser.cur_layer = 0;
            pstate->data.nn_metadata.v1.parser.parsing = NNM_PARSE_INP_SIZE;
        }
    } else if (pstate->data.nn_metadata.v1.parser.parsing ==
            NNM_PARSE_INP_SIZE){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.nn_metadata.v1.inp_size);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.nn_metadata.v1.inp_size)),
                sizeof(pstate->data.nn_metadata.v1.inp_size), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->data.nn_metadata.v1.parser.parsing = NNM_PARSE_COORD;
        }

    } else if (pstate->data.nn_metadata.v1.parser.parsing ==
            NNM_PARSE_COORD){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.nn_metadata.v1.coords);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.nn_metadata.v1.coords)),
                sizeof(pstate->data.nn_metadata.v1.coords), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->data.nn_metadata.v1.parser.cur_layer += 1;

            if (pstate->data.nn_metadata.v1.base_meta.is_nn_cached){
                pstate->data.nn_metadata.v1.parser.parsing =
                    NNM_PARSE_CACHE_INST;
            } else if (pstate->data.nn_metadata.v1.parser.cur_layer <
                    pstate->data.nn_metadata.v1.base_meta.num_layers){
                pstate->data.nn_metadata.v1.parser.parsing =
                    NNM_PARSE_COORD;
            } else {

                pstate->data.nn_metadata.v1.parser.parsing =
                    NNM_PARSE_BASE_META;
            }
        }

    } else if (pstate->data.nn_metadata.v1.parser.parsing ==
            NNM_PARSE_CACHE_INST){
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes =
                sizeof(pstate->data.nn_metadata.v1.cache_params);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.nn_metadata.v1.cache_params)),
                sizeof(pstate->data.nn_metadata.v1.cache_params), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            if (pstate->data.nn_metadata.v1.parser.cur_layer <
                    pstate->data.nn_metadata.v1.base_meta.num_layers){
                pstate->data.nn_metadata.v1.parser.parsing =
                    NNM_PARSE_COORD;
            } else {
                pstate->data.nn_metadata.v1.parser.parsing =
                    NNM_PARSE_BASE_META;
            }
        }

    } else if (pstate->mode == PACKAGE_MODE_VALUE_START){
        memset(&pstate->data.nn_metadata.v1, 0,
                sizeof(pstate->data.nn_metadata.v1));
        pstate->data.nn_metadata.v1.parser.parsing = NNM_PARSE_HEADER;
        pstate->data.nn_metadata.v1.parser.parsed = 0;

    } else {
        s =  SYNTIANT_PACKAGE_ERROR_NN_METADATA_V1;
        goto error;
    }

    pstate->value_mode =
        (pstate->expected_bytes == 0) ?
        PACKAGE_MODE_VALID_PARTIAL_VALUE : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t *)pstate->length) ?
                    PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_nn_metadata_v2(
    syntiant_pkg_parser_state_t *pstate)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    uint32_t i = 0;
    syntiant_nn_metadata_v2_t *meta = &pstate->data.nn_metadata.v2;

    meta->parser.parsed = meta->parser.parsing;

    if (meta->parser.parsing == NNM_PARSE_HEADER) {
        meta->parser.cur_subgraph = 0;
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes
                = sizeof(pstate->data.nn_metadata.v1.initial_meta);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t *)(&(pstate->data.nn_metadata.v1.initial_meta)),
            sizeof(pstate->data.nn_metadata.v1.initial_meta), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            meta->parser.parsing = NNM_PARSE_BASE_META;

            /* Need to have a least one set of metadata */
            if (!pstate->data.nn_metadata.v1.initial_meta.nn_num) {
                s = SYNTIANT_PACKAGE_ERROR_NN_METADATA_V1;
                goto error;
            }
        }
    } else if (meta->parser.parsing == NNM_PARSE_BASE_META) {
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = sizeof(meta->base_meta);
        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t *)(&(meta->base_meta)), sizeof(meta->base_meta), 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            meta->parser.cur_layer = 0;
            meta->parser.parsing = NNM_PARSE_INDICES_LAYER;
        }
    } else if (meta->parser.parsing == NNM_PARSE_INDICES_LAYER) {
        uint32_t size
            = (uint32_t)sizeof(meta->layers[0]) * meta->base_meta.num_layers;
        if (pstate->expected_bytes == 0) pstate->expected_bytes = size;
        byte_read = syntiant_pkg_read_chunk_data(
            pstate, (uint8_t *)(meta->layers), size, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0)
            meta->parser.parsing = NNM_PARSE_INDICES_INPUT;

    } else if (meta->parser.parsing == NNM_PARSE_INDICES_INPUT) {
        uint32_t size
            = (uint32_t)sizeof(meta->inputs[0]) * meta->base_meta.num_inputs;
        if (pstate->expected_bytes == 0) pstate->expected_bytes = size;
        byte_read = syntiant_pkg_read_chunk_data(
            pstate, (uint8_t *)(meta->inputs), size, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0)
            meta->parser.parsing = NNM_PARSE_INDICES_OUTPUT;

    } else if (meta->parser.parsing == NNM_PARSE_INDICES_OUTPUT) {
        uint32_t size
            = (uint32_t)sizeof(meta->outputs[0]) * meta->base_meta.num_outputs;
        if (pstate->expected_bytes == 0) pstate->expected_bytes = size;
        byte_read = syntiant_pkg_read_chunk_data(
            pstate, (uint8_t *)(meta->outputs), size, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0)
            meta->parser.parsing = NNM_PARSE_PADDING;
    } else if (meta->parser.parsing == NNM_PARSE_PADDING) {
        uint32_t size = (uint32_t)(meta->base_meta.num_outputs
            + meta->base_meta.num_inputs + meta->base_meta.num_layers);
        size = 4 * ((size + 3) / 4) - size;
        if (pstate->expected_bytes == 0) pstate->expected_bytes = size;
        for (i = 0; i < size; i++) {
            uint8_t pad;
            byte_read = syntiant_pkg_read_chunk_data(
                pstate, (uint8_t *)&pad, size, 0, 1, 1);
            pstate->partially_read_length += byte_read;
        }
        if (pstate->expected_bytes == 0)
            meta->parser.parsing = NNM_PARSE_CACHE_INST;
    } else if (meta->parser.parsing == NNM_PARSE_CACHE_INST) {
        if (meta->base_meta.is_cached) {
            if (pstate->expected_bytes == 0)
                pstate->expected_bytes = sizeof(meta->cache_params);
            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t *)(&(meta->cache_params)), sizeof(meta->cache_params),
                0, 1, 1);
            pstate->partially_read_length += byte_read;
            if (pstate->expected_bytes == 0) {
                meta->parser.cur_layer += 1;
                if (meta->parser.cur_layer < meta->base_meta.num_layers) {
                    meta->parser.parsing = NNM_PARSE_CACHE_INST;
                } else {
                    meta->parser.parsing = NNM_PARSE_BASE_META;
                }
            }
        } else {
            meta->parser.parsing = NNM_PARSE_BASE_META;
        }
        meta->parser.cur_subgraph += 1;
        /* Once cache params are parsed, we start parsing next subgraph so
         * increment the cur_subgraph index */
    } else if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        memset(meta, 0, sizeof(pstate->data.nn_metadata.v2));
        meta->parser.parsing = NNM_PARSE_HEADER;
        meta->parser.parsed = 0;
    } else {
        s = SYNTIANT_PACKAGE_ERROR_NN_METADATA_V1;
        goto error;
    }

    pstate->value_mode = (pstate->expected_bytes == 0)
        ? PACKAGE_MODE_VALID_PARTIAL_VALUE
        : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode = (pstate->partially_read_length < *(uint32_t *)pstate->length)
        ? PACKAGE_MODE_VALUE_CONT
        : PACKAGE_MODE_TAG_START;
error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_dsp_mcu_secondary_validation_v1(
            syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int bytes_read = 0;
    unsigned int size = 0;

    size = sizeof(pstate->data.dsp_mcu_sec_val);
    bytes_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&pstate->data.dsp_mcu_sec_val,
            size, 0, 1, 1);
    pstate->partially_read_length += bytes_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length) {
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0) {
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes) {
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_algo_params_v1(
        syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int bytes_read = 0;
    unsigned int size = 0;

    size = sizeof(pstate->data.algo_params.base);
    bytes_read = syntiant_pkg_read_chunk_data(
        pstate, (uint8_t *)&pstate->data.algo_params.base, size, 0, 1, 1);

    pstate->partially_read_length += bytes_read;

    bytes_read = syntiant_pkg_read_chunk_data(
        pstate, (uint8_t*)pstate->data.algo_params.payload,
        pstate->data.algo_params.base.payload_size, 0, 1, 1);
    pstate->partially_read_length += bytes_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length) {
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0) {
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes) {
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_mcu_secondary_validation_v1(
        syntiant_pkg_parser_state_t *pstate) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int bytes_read = 0;
    unsigned int size = 0;

    size = sizeof(pstate->data.mcu_sec_val);
    bytes_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&pstate->data.mcu_sec_val,
            size, 0, 1, 1);
    pstate->partially_read_length += bytes_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length) {
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0) {
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes) {
            /* Failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_read_mic_settings(syntiant_pkg_parser_state_t *pstate)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int bytes_read = 0;
    unsigned int size = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        /* sanity checks on the TLV */
    }

    size = sizeof(pstate->data.mic_settings);
    bytes_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)&pstate->data.mic_settings, size, 0, 1, 1);
    pstate->partially_read_length += bytes_read;

    if (pstate->partially_read_length == *(uint32_t*)pstate->length) {
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0) {
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    } else {
        if (pstate->expected_bytes) {
            /* failed to read the requested chunk, wait */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        } else {
            /* chunk was read successfully */
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->expected_bytes = size;
        }
    }

error:
    return s;
}

static int device_locked(struct syntiant_ndp_device_s *ndp, unsigned int *locked)
{
    uint32_t data;
    /* check to see if the chip is locked */
    int s = ndp_spi_read(NDP120_SPI_CTL, &data);
    if (s) goto error;
    *locked = NDP120_SPI_CTL_LOCKED_EXTRACT(data);
error:
    return s;
}

static void mailbox_reset_state(struct syntiant_ndp_device_s *ndp)
{
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;
    mb_state->mbin_resync = 1;
    mb_state->watermarkint_resync = 1;
}

static uint8_t get_mbin_seq(struct syntiant_ndp_device_s *ndp)
{
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;
    mb_state->mbin_seq ^= 0x80;
    return mb_state->mbin_seq;
}

int syntiant_ndp120_mbin_send(struct syntiant_ndp_device_s *ndp, uint8_t data)
{
    data &= 0x7F;
#if SYNTIANT_NDP120_DEBUG_MB
    if (data != NDP120_DSP_MB_H2D_WATERMARK) {
        syntiant_ndp120_mcu_mb_debug_print("--> mbin", data);
    }
#endif
    data |= get_mbin_seq(ndp);
    return ndp_spi_write(NDP120_SPI_MBIN, data);
}

int syntiant_ndp120_mbin_resp_get(struct syntiant_ndp_device_s *ndp, uint8_t *data)
{
    int s;
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;
    s = ndp_spi_read(NDP120_SPI_MBIN_RESP, data);
    if (s) goto error;

#if SYNTIANT_NDP120_DEBUG_MB
    syntiant_ndp120_mcu_mb_debug_print("<-- mbin_resp", *data);
#endif

    if ((*data & 0x80) != mb_state->mbin_resp_seq && !mb_state->mbin_resync) {
        DEBUG_PRINTF("mbin_resp sequence number is incorrect\n");
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }

    if (mb_state->mbin_resync) {
        mb_state->mbin_resync = 0;
        mb_state->mbin_resp_seq = *data & 0x80;
    }

    mb_state->mbin_resp_seq ^= 0x80;
    *data &= 0x7f;
error:
    return s;
}

#if SYNTIANT_NDP120_DEBUG_SIMULATOR
static int MAYBE_POLL(struct syntiant_ndp_device_s *ndp)
{
    static int first = 1;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t not;
    if (first) {
        DEBUG_PRINTF("remove me, this is only for sim development %s:%d\n", __FILE__, __LINE__);
        first = 0;
    }
    s = syntiant_ndp120_poll(ndp, &not, 1);
    return s;
}
#else
static int MAYBE_POLL(struct syntiant_ndp_device_s *ndp)
{
    (void) ndp;
    return 0;
}
#endif

static int check_dsp_fw_state_magic(struct syntiant_ndp_device_s *ndp) {
    int s;
    uint32_t magic, magic_end;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    s = ndp_mcu_read(ndp120->dsp_fw_state_addr, &magic);
    if (s) goto error;
    s = ndp_mcu_read(ndp120->dsp_fw_state_addr +
        (unsigned)offsetof(ndp120_dsp_fw_state_t, magic_end), &magic_end);
    if (s) goto error;

    DEBUG_PRINTF("offsets in fw state:\n");
    DEBUG_PRINTF("               base: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, base), sizeof(ndp120_dsp_fw_base_t));
    DEBUG_PRINTF("        aud2_config: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, aud2_config), sizeof(ndp120_dsp_fw_aud2_config_t));
    DEBUG_PRINTF("           pwr_mgmt: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, pwr_mgmt), sizeof(ndp120_dsp_fw_pwr_mgmt_t));
    DEBUG_PRINTF("  audio_sync_config: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, audio_sync_config), sizeof(ndp120_dsp_audio_sync_config_t));
    DEBUG_PRINTF("              algos: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, algos), sizeof(ndp120_dsp_algo_t));
    DEBUG_PRINTF(" serial_sensor_mgmt: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, serial_sensor_mgmt), sizeof(ndp120_dsp_fw_serial_sensor_mgmt_t));
    DEBUG_PRINTF("          dnn_state: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, dnn_state), sizeof(ndp120_dnn_network_state_t));
    DEBUG_PRINTF("          mem_state: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, mem_state), sizeof(ndp120_dsp_memory_state_t));
    DEBUG_PRINTF("          magic_end: %d size: %d\n",
        offsetof(ndp120_dsp_fw_state_t, magic_end), sizeof(uint32_t));

    if (magic != NDP120_DSP_FW_STATE_MAGIC ||
            magic_end != NDP120_DSP_FW_STATE_MAGIC_END) {
        s = SYNTIANT_NDP_ERROR_DSP_HDR_CRC;
        DEBUG_PRINTF("DPS fw state magic number mismatch\n");
        DEBUG_PRINTF("    magic is 0x%X, should be 0x%X\n",
            magic, NDP120_DSP_FW_STATE_MAGIC);
        DEBUG_PRINTF("magic_end is 0x%X, should be 0x%X\n",
            magic_end, NDP120_DSP_FW_STATE_MAGIC_END);
        goto error;
    }

error:
    return s;
}

static int
save_mb_state(struct syntiant_ndp_device_s *ndp)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    struct mb_state_cache cache = {0};

    cache.mbin_seq = ndp120->mb_state.mbin_seq;
    cache.mbin_resp_seq = ndp120->mb_state.mbin_resp_seq;
    cache.mbout_seq = ndp120->mb_state.mbout_seq;
    cache.mbin_resync = ndp120->mb_state.mbin_resync;
    cache.mbin_resp = ndp120->mb_state.mbin_resp;
    cache.mbin_state = ndp120->mb_state.mbin_state;
    cache.watermarkint_resync = ndp120->mb_state.watermarkint_resync;

    return syntiant_ndp120_write_block(ndp, 1,
            NDP120_MCU_OPEN_RAM_MATCH_RESULTS_END,
            (void*) &cache, sizeof(cache));
}

static int
restore_mb_state(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct mb_state_cache cache = {0};
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    s = syntiant_ndp120_read_block(ndp, 1,
            NDP120_MCU_OPEN_RAM_MATCH_RESULTS_END,
            (void *) &cache, sizeof(cache));
    if (s) goto error;
    ndp120->mb_state.mbin_seq = cache.mbin_seq;
    ndp120->mb_state.mbin_resp_seq = cache.mbin_resp_seq;
    ndp120->mb_state.mbout_seq = cache.mbout_seq;
    ndp120->mb_state.mbin_resync = cache.mbin_resync;
    ndp120->mb_state.mbin_resp = cache.mbin_resp;
    ndp120->mb_state.mbin_state = cache.mbin_state;
    ndp120->mb_state.watermarkint_resync = cache.watermarkint_resync;
error:
    return s;
}

int
syntiant_ndp120_do_mailbox_req_no_sync(struct syntiant_ndp_device_s *ndp,
    uint8_t req, uint32_t *resp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int is_mcu = 1;
    uint8_t actual_req = req;
    uint32_t payload = 0;
    uint32_t addr;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    switch (req) {
        case SYNTIANT_NDP120_MB_MCU_MIADDR:
            clear_mcu_fw_pointers(ndp);
            ndp120->mb_state.mbin_state = MB_STATE_DATA;
            ndp120->mb_state.mbin_data_count = 0;
            ndp120->mb_state.mbin_data = 0;
            break;
        case NDP120_DSP_MB_H2D_ADX_LOWER:
            clear_dsp_fw_pointers(ndp);
            ndp120->mb_state.watermarkint_state = MB_STATE_SYNC;
            ndp120->mb_state.watermarkint_data = 0;
            is_mcu = 0;
            break;
        case NDP120_DSP_MB_H2D_RESTART:
            ndp120->mb_state.watermarkint_data =
                NDP120_DSP_MB_GET_PAYLOAD(ndp120->mb_state.watermarkint);
            is_mcu = 0;
            break;
    }

    /* Special case for H2D or H2M */
    /* Put the actual request into NDP120_DSP_MB_H2D_ADDR, and send
       NDP120_DSP_MB_H2D to the mailbox */

    if (req == SYNTIANT_NDP120_MB_MCU_BOOT_FROM_FLASH) {
        DEBUG_PRINTF("~~> sending BOOT_FROM_FLASH to bootloader...\n");
    } else if (req >= NDP120_DSP_MB_H2D_EXT__START__ &&
               req <= NDP120_DSP_MB_H2D_EXT__END__) {
        uint32_t adx = NDP120_DSP_MB_H2D_ADDR;
        DEBUG_PRINTF("~~> sending MB H2D cmd %#x...\n", req);
        switch (req) {
            case NDP120_DSP_MB_H2D_EXT_SET_TANK_SIZE:
            case NDP120_DSP_MB_H2D_EXT_VAD_CONTROL:
            case NDP120_DSP_MB_H2D_EXT_ALGO_CONFIG_ALLOC:
            case NDP120_DSP_MB_H2D_EXT_RESET_ALGO_CONFIG:
            case NDP120_DSP_MB_H2D_EXT_MCU_CLK_DIV:
            case NDP120_DSP_MB_H2D_EXT_SET_DSP_DEBUG:
            case NDP120_DSP_MB_H2D_EXT_SENSOR_CONTROL:
            case NDP120_DSP_MB_H2D_EXT_AUTO_CLK_SCALING:
            case NDP120_DSP_MB_H2D_EXT_RW_POWER_PROFILE:
            case NDP120_DSP_MB_H2D_EXT_INIT_RING_BUF_PTR:
            case NDP120_DSP_MB_H2D_EXT_READ_DNN_STATE:
            case NDP120_DSP_MB_H2D_EXT_SET_DNN_RUN_DELAY:
            case NDP120_DSP_MB_H2D_EXT_SET_ID:
            case NDP120_DSP_MB_H2D_EXT_SET_BARGE_IN:
            case NDP120_DSP_MB_H2D_EXT_GET_MEM_INFO:
                s = ndp_mcu_read(adx, &payload);
                if (s) goto error;
                break;
            case NDP120_DSP_MB_H2D_REQUEST_NOP:
            case NDP120_DSP_MB_H2D_EXT_SENSOR_INIT:
            case NDP120_DSP_MB_H2D_EXT_GET_AUDIO_PARAMS:
            case NDP120_DSP_MB_H2D_EXT_MISC_INPUT_SIZE:
                is_mcu = 0;
                break;
            default:
                break;
        }
        payload = NDP120_H2D_SET_MESSAGE(payload, req);
        s = ndp_mcu_write(adx, payload);
        if (s) goto error;
        actual_req = NDP120_DSP_MB_H2D_EXT;
        DEBUG_PRINTF("Extended H2D mailbox command\n");
    } else if (req >= NDP120_MBIN_H2M_EXT__START__ &&
               req <= NDP120_MBIN_H2M_EXT__END__) {
        /* Writing EXT cmds in open RAM */
        uint32_t adr = NDP120_MCU_OPEN_RAM_BGN;
        DEBUG_PRINTF("~~> sending MB H2M cmd %#x...\n", req);
        s = ndp_mcu_write(adr, req);
        if (s) goto error;
        actual_req = NDP_MBIN_REQUEST_EXTENDED_CMDS;
        DEBUG_PRINTF("Extended H2M mailbox command\n");
    }

    s = syntiant_ndp120_mbin_send(ndp, actual_req);
    if (s) goto error;
    s = ndp->iif.mbwait(ndp->iif.d);
    if(s) {
        DEBUG_PRINTF("~~> do_mailbox_req_no_sync: mbwait failed %d\n", s);
        goto error;
    }
    s = MAYBE_POLL(ndp);
    if (s) goto error;

    if(resp != NULL) {
        *resp = is_mcu ? ndp120->mb_state.mbin_resp : ndp120->mb_state.watermarkint_data;
    }

    switch (req) {
        case SYNTIANT_NDP120_MB_MCU_MIADDR:
            ndp120->mcu_fw_pointers_addr = ndp120->mb_state.mbin_data;
            s = get_mcu_fw_pointers(ndp);
            if (s) goto error;
            break;

        case NDP120_DSP_MB_H2D_ADX_LOWER:
            ndp120->dsp_fw_state_addr &= 0xFFFF0000;
            ndp120->dsp_fw_state_addr |= ndp120->mb_state.watermarkint_data;
            DEBUG_PRINTF("LOWER: DSP FW PTR: 0x%x\n", ndp120->dsp_fw_state_addr);
            if (!ndp120->secure_boot_required && !ndp120->chip_is_locked) {
                s = check_dsp_fw_state_magic(ndp);
                if (s) goto error;
                /* don't check error -- DSP may not be loaded */
                (void)syntiant_ndp120_update_clock_rate_in_dsp(ndp);
            }
            break;

        case NDP120_DSP_MB_H2D_RESTART:
            s = syntiant_ndp120_init_ring_buffer_pointers_no_sync(ndp, 0);
            if (s) goto error;
            break;

        case NDP120_DSP_MB_H2D_EXT_SET_TANK_SIZE:
            if (ndp120->mb_state.watermarkint_data == NDP120_DSP_MB_D2H_NO_DNN_MEM) {
                s = SYNTIANT_NDP_ERROR_NOMEM;
                goto error;
            }
            break;

        case NDP120_DSP_MB_H2D_EXT_VAD_CONTROL:
            if (ndp120->mb_state.watermarkint_data == NDP120_DSP_MB_D2H_NO_VAD_MIC) {
                s = SYNTIANT_NDP_ERROR_UNSUP;
            }
            break;

        case NDP_MBIN_REQUEST_GET_PH_ENABLED_STATE:
            addr = (uint32_t) (ndp120->mcu_fw_state_addr +
                    (uint32_t) offsetof(struct ndp120_fw_state_s, mb_state.h2m_data));
            s = syntiant_ndp120_read_block(ndp, 1, addr, &ndp->pstate.ph_excluded, sizeof(uint32_t));
            break;
    }

    if (ndp120->nn_params_loaded && secure_boot) {
        s = save_mb_state(ndp);
    }
    if (s) goto error;
error:
    return s;
}

/*
 * Used by the CLI, mostly for debugging
 */
int
syntiant_ndp120_do_mailbox_req(struct syntiant_ndp_device_s *ndp,
    uint8_t req, uint32_t *resp)
{
    int s, s0;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_do_mailbox_req\n");
        goto error;
    }

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, req, resp);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

error:
    return s;
}

static int
syntiant_read_match_producer(struct syntiant_ndp_device_s *ndp)
{
    int s;
    uint32_t addr;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t net_id = ndp120->last_network_id;
    struct syntiant_ndp120_secure_match_data secure_match;

    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        /* read from Open RAM */
        s = syntiant_ndp120_read_block(ndp, 1, NDP120_MCU_OPEN_RAM_MATCH_RESULTS,
                                        &secure_match, sizeof(secure_match));
        if (s) return s;
        ndp120->match_producer[net_id] = secure_match.match_producer;
    } else {
        addr = (uint32_t) (ndp120->mcu_fw_state_addr +
                (uint32_t) offsetof(struct ndp120_fw_state_s, match_producer) +
                sizeof(net_id) * net_id);
        s = syntiant_ndp120_read(ndp, 1, addr, &ndp120->match_producer[net_id]);
    }
    return s;
}

static int
watermarkint_recv(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_mailbox_msg_t *data)
{
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;
    int s = SYNTIANT_NDP_ERROR_NONE;

    s = ndp->iif.mbwait(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in watermarkint_recv, %d\n", s);
        goto error;
    }
    s = MAYBE_POLL(ndp);
    if (s) goto error;
    *data = mb_state->watermarkint;

error:
    return s;
}

static int
watermarkint_get(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_mailbox_msg_t *data)
{
    int s;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (secure_boot) {
        s = ndp_mcu_read(NDP120_DSP_MB_D2H_ADDR, data);
    } else {
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_WATERMARKINT, data);
    }
    if (s) goto error;

error:
    return s;
}

/* MBOUT */
static int mbout_recv(struct syntiant_ndp_device_s *ndp, uint8_t *data)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;

    mb_state->mbout = 0;
    s = ndp->iif.mbwait(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in mbout_recv, %d\n", s);
        goto error;
    }
    s = MAYBE_POLL(ndp);
    if (s) goto error;
    *data = mb_state->mbout;

error:
    DEBUG_PRINTF("mbout_recv returning %d\n", s);
    return s;
}

static int mbout_send_resp(struct syntiant_ndp_device_s *ndp, uint8_t data)
{
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;

    data = (uint8_t)((data & 0x7f) | mb_state->mbout_seq);
#if SYNTIANT_NDP120_DEBUG_MB
    syntiant_ndp120_mcu_mb_debug_print("--> mbout_resp", data);
#endif
    return ndp_spi_write(NDP120_SPI_MBOUT_RESP, data);
}

static int mb_resp_is_data(uint8_t result)
{
    return (result & SYNTIANT_NDP120_MB_MCU_DATA_MASK) != 0;
}

static int mb_resp_is_error(uint8_t result, uint8_t * error)
{
    if (mb_resp_is_data(result)) return 0;

    if ((result & SYNTIANT_NDP120_MB_MCU_ERROR_MASK) == 0) return 0;

    if (error) {
        *error = result & (SYNTIANT_NDP120_MB_MCU_ERROR_MASK - 1);
    }
    return 1;
}

static int mbout_get(struct syntiant_ndp_device_s *ndp, uint8_t *data) {
    int s;
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;

    s = ndp_spi_read(NDP120_SPI_MBOUT, data);
    if (s) goto error;

#if SYNTIANT_NDP120_DEBUG_MB
    syntiant_ndp120_mcu_mb_debug_print("<-- mbout", *data);
#endif
    mb_state->mbout_seq = *data & 0x80;
    *data &= 0x7f;

error:
    return s;
}


static int
mbout_processor(struct syntiant_ndp_device_s *ndp, uint32_t *notify)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t data;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    *notify = 0;
    s = mbout_get(ndp, &data);
    if (s) goto error;

    ndp120->mb_state.mbout = data;

    switch (data) {
    case SYNTIANT_NDP120_MB_MCU_RUNNING:
        ndp120->mb_state.mbin_state = MB_STATE_SYNC;
        ndp120->mb_state.mbin_sync_count = 0;
        mailbox_reset_state(ndp);
        s = syntiant_ndp120_mbin_send(ndp, SYNTIANT_NDP120_MB_MCU_NOP);
        if (s) goto error;
        break;

    case SYNTIANT_NDP120_MB_MCU_RUNNING_TEST:
        ndp120->mb_state.mbin_state = MB_STATE_SYNC;
        ndp120->mb_state.mbin_sync_count = 0;
        mailbox_reset_state(ndp);
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT |
                    SYNTIANT_NDP_NOTIFICATION_INT_TEST_MODE;
        break;

    case SYNTIANT_NDP120_MB_MCU_NOP:
        s = mbout_send_resp(ndp, SYNTIANT_NDP120_MB_MCU_NOP);
        if (s) goto error;
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT;
        break;

    case SYNTIANT_NDP120_MB_MCU_BOOTING:
        DEBUG_PRINTF("Received BOOTING in mbout\n");
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT |
            SYNTIANT_NDP_NOTIFICATION_BOOTING;
        break;

    case SYNTIANT_NDP120_MB_MCU_CONT:
        DEBUG_PRINTF("Received CONT in mbout\n");
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT;
        break;

    case SYNTIANT_NDP120_MB_MCU_LOAD_DONE:
#if SYNTIANT_NDP120_DEBUG_MB
        syntiant_ndp120_mcu_mb_debug_print("received in mbout", data);
#endif
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT;
        break;

    case SYNTIANT_NDP120_MB_OTHER_LOAD_DONE:
#if SYNTIANT_NDP120_DEBUG
        syntiant_ndp120_mcu_mb_debug_print("received in mbout", data);
#endif
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT |
                  SYNTIANT_NDP_NOTIFICATION_OTHER_LOAD_DONE;
        break;

    case SYNTIANT_NDP120_MB_MCU_HEARTBEAT:
        break;

    case SYNTIANT_NDP120_MB_MCU_SECURE_EXTRACT:
        *notify = SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY;
        break;

    default:
        /* check for a match */
        if (data & SYNTIANT_NDP120_MB_MCU_NETWORK_MASK) {
            ndp120->last_network_id =
                (data & ~SYNTIANT_NDP120_MB_MCU_NETWORK_MASK) & 0xFF;
            s = syntiant_read_match_producer(ndp);
            if (s) goto error;
            s = mbout_send_resp(ndp, SYNTIANT_NDP120_MB_MCU_NOP);
            if (s) goto error;
            ndp120->matches++;
            *notify = SYNTIANT_NDP_NOTIFICATION_MATCH;
        } else {
            DEBUG_PRINTF("POLL: got unknown mbout: 0x%02X\n", data);
        }
        break;
    }

error:
    return s;
}

static int
dsp_mb_processor(struct syntiant_ndp_device_s *ndp, uint32_t *notify)
{
    int s = 0;
#if SYNTIANT_NDP120_DEBUG || SYNTIANT_NDP120_DEBUG_POLL
    uint32_t data;
    static int extract_count = 0;
#endif
    uint32_t payload;
    uint8_t message;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    s = watermarkint_get(ndp, &ndp120->mb_state.watermarkint);
    if (s) goto error;
    message = NDP120_DSP_MB_GET_MESSAGE(ndp120->mb_state.watermarkint);
    payload = NDP120_DSP_MB_GET_PAYLOAD(ndp120->mb_state.watermarkint);

#if SYNTIANT_NDP120_DEBUG_MB || SYNTIANT_NDP120_DEBUG_POLL
    if (message != NDP120_DSP_MB_D2H_DEBUG || !ndp120_debug_callback) {
        if (message != NDP120_DSP_MB_D2H_WATERMARK) {
            const char *name = syntiant_ndp120_dsp_mb_op_decoder(message);
            DEBUG_PRINTF("DSP sent: %s\n", name);
        }
    }
#endif

    switch (message) {
    case NDP120_DSP_MB_D2H_WATERMARK:
        if (payload != NDP120_D2H_INVALID_REQ) {
            *notify = SYNTIANT_NDP_NOTIFICATION_WATER_MARK;
#if SYNTIANT_NDP120_DEBUG || SYNTIANT_NDP120_DEBUG_POLL
            s = ndp_mcu_read(NDP120_DSP_CONFIG_BUFFILLLEVEL(4), &data);
            if (s) goto error;
            if (data) {
                DEBUG_PRINTF("FILL LEVEL: 0x%X, sending watermark ack\n", data);
            }

#endif
            s = syntiant_ndp120_mbin_send(ndp, NDP120_DSP_MB_H2D_WATERMARK);
            if (s) goto error;
        }
        break;

    case NDP120_DSP_MB_D2H_EXTRACT_READY:
        if (payload != NDP120_D2H_INVALID_REQ) {
            *notify |= SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY;
#if SYNTIANT_NDP120_DEBUG || SYNTIANT_NDP120_DEBUG_POLL
            s = ndp_mcu_read(NDP120_DSP_CONFIG_BUFFILLLEVEL(4), &data);
            if (s) goto error;
            DEBUG_PRINTF("FILL LEVEL: 0x%X\n", data);
            DEBUG_PRINTF("Sending extract ack\n");
            extract_count += 1;
            DEBUG_PRINTF("extract cnt: %d, payload: %d\n", extract_count, payload);
#endif
            /* writing a watermark because that's an accepted response if it was
             * a SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY the DSP would hang
             * the actual value is ignored */
            s = syntiant_ndp120_mbin_send(ndp, NDP120_DSP_MB_H2D_WATERMARK);
            if (s) goto error;
        }
        break;

    case NDP120_DSP_MB_D2H_RESTART:
        ndp120->mb_state.watermarkint_data = payload;
        switch (ndp120->mb_state.watermarkint_data) {
        case NDP120_DSP_MB_D2H_NO_DNN_MEM:
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN |
                        SYNTIANT_NDP_NOTIFICATION_DNN_MEM_ERROR |
                        SYNTIANT_NDP_NOTIFICATION_ERROR;
            break;
        case NDP120_DSP_MB_D2H_NO_DSP_MEM:
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN |
                        SYNTIANT_NDP_NOTIFICATION_DSP_MEM_ERROR |
                        SYNTIANT_NDP_NOTIFICATION_ERROR;
            break;
        case NDP120_DSP_MB_D2H_ALGO_ERROR_INIT:
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN |
                        SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR |
                        SYNTIANT_NDP_NOTIFICATION_ERROR;
            break;
        case NDP120_DSP_MB_D2H_ALGO_ERROR_PROCESS:
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN |
                        SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR |
                        SYNTIANT_NDP_NOTIFICATION_ERROR;
#if SYNTIANT_NDP120_DEBUG
            DEBUG_PRINTF("error:0x%x algo_area_index: %d\n", payload & 0xFF,
                (payload >> 8) & 0xFF);
#endif
            break;
        default:
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
            break;
        }
        break;

    case NDP120_DSP_MB_D2H_MODE_CHANGE:
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        break;

    case NDP120_DSP_MB_D2H_DEBUG:
        *notify = SYNTIANT_NDP_NOTIFICATION_DEBUG;
        break;

    case NDP120_DSP_MB_D2H_UNKNOWN_MIC_STATE:
        *notify = SYNTIANT_NDP_NOTIFICATION_UNKNOWN_MIC_STATE;
        break;

    case NDP120_DSP_MB_D2H_ALGO_ERROR_INIT:
        *notify = SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR |
            SYNTIANT_NDP_NOTIFICATION_ERROR;
#if SYNTIANT_NDP120_DEBUG && SYNTIANT_NDP120_DEBUG_POLL
        DEBUG_PRINTF("algo_config_index : %d\n", payload);
#endif
        break;

    case NDP120_DSP_MB_D2H_ALGO_ERROR_PROCESS:
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN |
                    SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR |
                    SYNTIANT_NDP_NOTIFICATION_ERROR;
#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("error:0x%x algo_area_index: %d\n", payload & 0xFF,
            (payload >> 8) & 0xFF);
#endif
        break;

    case NDP120_DSP_MB_D2H_ADX_LOWER:
        ndp120->mb_state.watermarkint_data = payload;
        DEBUG_PRINTF("LOWER: DSP FW PTR: 0x%x\n", ndp120->dsp_fw_state_addr);
        s = syntiant_ndp120_mbin_send(ndp, NDP120_DSP_MB_H2D_ADX_UPPER);
        if (s) goto error;
        break;

    case NDP120_DSP_MB_D2H_ADX_UPPER:
        ndp120->dsp_fw_state_addr &= 0xFFFF;
        ndp120->dsp_fw_state_addr |= payload << 16;
        DEBUG_PRINTF("UPPER: DSP FW PTR: 0x%x\n", ndp120->dsp_fw_state_addr);
        if (ndp120->dsp_fw_state_addr) {
            ndp120->dsp_fw_loaded = 1;
        }
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        break;

    case NDP120_DSP_MB_D2H_RUNNING:
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        DEBUG_PRINTF("got DSP RUNNING, mailbox notify\n");
        break;

    case NDP120_DSP_MB_D2H_EXTRACT_DELAY_ERROR:
        *notify |= SYNTIANT_NDP_NOTIFICATION_DELAY_ERROR |
            SYNTIANT_NDP_NOTIFICATION_ERROR;
        DEBUG_PRINTF("Extract delay error\n");
        break;

    case NDP120_DSP_MB_D2H_EXT:
        /* fall through */
        ndp120->mb_state.watermarkint_data = payload;
        *notify =  SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        switch (ndp120->mb_state.watermarkint_data) {
            case NDP120_DSP_MB_D2H_NO_DNN_MEM:
                DEBUG_PRINTF("got EXT, NDP120_DSP_MB_D2H_NO_DNN_MEM\n");
                *notify |= SYNTIANT_NDP_NOTIFICATION_DNN_MEM_ERROR |
                    SYNTIANT_NDP_NOTIFICATION_ERROR;
                break;

            case NDP120_DSP_FW_NO_MEM:
                DEBUG_PRINTF("got EXT, NDP120_DSP_FW_NO_MEM\n");
                *notify |= SYNTIANT_NDP_NOTIFICATION_DSP_MEM_ERROR |
                    SYNTIANT_NDP_NOTIFICATION_ERROR;
                break;

            case NDP120_DSP_FW_WARNING_ARG:
                DEBUG_PRINTF("got EXT, NDP120_DSP_FW_WARNING_ARG\n");
                break;

            case NDP120_DSP_FW_INVALID_ARG:
                DEBUG_PRINTF("got EXT, NDP120_DSP_FW_INVALID_ARG\n");
                *notify |= SYNTIANT_NDP_NOTIFICATION_INVALID_ARGS |
                    SYNTIANT_NDP_NOTIFICATION_ERROR;
                break;

            default:
                break;
        }
        DEBUG_PRINTF("got EXT, mailbox notify\n");
        break;

    default:
        DEBUG_PRINTF("Invalid DSP mailbox value: 0x%X\n", ndp120->mb_state.watermarkint);
        break;
    }

error:
    return s;
}

static int
mbin_processor(struct syntiant_ndp_device_s *ndp, uint32_t *notify)
{
    int s;
    uint8_t data;
    syntiant_ndp120_mb_state_s *mb_state = &ndp->d.ndp120.mb_state;

    s = syntiant_ndp120_mbin_resp_get(ndp, &data);
    if (s) goto error;

    mb_state->mbin_resp = data;

    /* post fw load sync code */
    if (mb_resp_is_error(data, NULL) && mb_state->mbin_state == MB_STATE_SYNC) {
        ++mb_state->mbin_sync_count;
        if (mb_state->mbin_sync_count < 3) {
            s = syntiant_ndp120_mbin_send(ndp, SYNTIANT_NDP120_MB_MCU_NOP);
            if (s) goto error;
        } else {
            mb_state->mbin_state = MB_STATE_NONE;
            *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        }
        goto error;
    }

    if (data == SYNTIANT_NDP120_MB_MCU_NOP) {
        if (mb_state->mbin_state == MB_STATE_SYNC) {
            mb_state->mbin_state = MB_STATE_NONE;
        }
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
    } else if (mb_resp_is_data(data)) {
        if (mb_state->mbin_state == MB_STATE_DATA) {
            int last, addr_shift, mb_shift;
            ++mb_state->mbin_data_count;
            if (mb_state->mbin_data_count < 6) {
                /* TODO plz use literrals in place of hard coded values */
                last = 0;
                addr_shift = 6;
                mb_shift = 26;
            } else {
                last = 1;
                addr_shift = 2;
                mb_shift = 30;
            }
            mb_state->mbin_data >>= addr_shift;
            mb_state->mbin_data |= (uint32_t)((data & 0x3F) << mb_shift);
            if (last) {
                mb_state->mbin_state = MB_STATE_NONE;
                *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
            } else {
                s = syntiant_ndp120_mbin_send(ndp, SYNTIANT_NDP120_MB_MCU_DATA);
                if (s) goto error;
            }
        }
    } else if(data == SYNTIANT_NDP120_MB_MCU_CONT) {
        *notify = SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN;
        goto error;
    }
error:
    return s;
}

static int
syntiant_ndp120_get_secured_status_no_sync(struct syntiant_ndp_device_s *ndp,
          int *is_secured)
{
    int s;
    uint32_t data;
    unsigned int secured, locked;

    s = ndp_spi_read(NDP120_SPI_ID0, &data);
    if (s) goto error;

    secured = NDP120_SPI_ID0_AUTH_FIRMWARE_EXTRACT(data);

    ndp->d.ndp120.boot_flag = NDP120_SPI_ID0_SERIAL_BOOT_EXTRACT(data) ?
        SYNTIANT_NDP120_SERIAL_BOOT : SYNTIANT_NDP120_HOST_BOOT;

    /* check to see if the chip is locked */
    s = ndp_spi_read(NDP120_SPI_CTL, &data);
    if (s) goto error;
    s = device_locked(ndp, &locked);
    if (s) goto error;

    *is_secured = locked || secured;

error:
    return s;
}


int
syntiant_ndp120_get_secured_status(struct syntiant_ndp_device_s *ndp,
          int *is_secured)
{
    int s, s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto out;

    s = syntiant_ndp120_get_secured_status_no_sync(ndp, is_secured);
#ifdef EMULATE_SECURE
    *is_secured = 1;
#endif
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

int syntiant_ndp120_clock_cfg_flashboot(struct syntiant_ndp_device_s *ndp,
    const struct syntiant_ndp120_secure_clk_config_data *cfg)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0 = 0;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_MAIN_CLK_CONFIG, sizeof(*cfg)};

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_clock_cfg_flashboot\n");
        return s;
    }

    /* save core clk */
    ndp->d.ndp120.core_clock_freq = cfg->core_freq;

    /* if not PLL */
    if (cfg->src != SYNTIANT_NDP120_MAIN_CLK_SRC_PLL) {
        cmd[1] = (uint32_t)offsetof(
            struct syntiant_ndp120_secure_clk_config_data, preset_value);
    }

    /* write cmd */
    s = ndp_mcu_write_block(addr, cmd, sizeof(cmd));
    if (s) goto error;
    addr += (uint32_t) sizeof(cmd);
    /* write payload */
    s = ndp_mcu_write_block(addr, cfg, sizeof(*cfg));
    if (s) goto error;

    /* alerts DSP to start/stop execution */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_FLASHBOOOT_CMD, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_clock_cfg(struct syntiant_ndp_device_s *ndp,
    const struct syntiant_ndp120_secure_clk_config_data *cfg)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0 = 0;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_SEC_MAIN_CLK_CONFIG, sizeof(*cfg)};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_clock_cfg\n");
        return s;
    }

    /* write secure cmd */
    s = ndp_mcu_write_block(addr, cmd, sizeof(cmd));
    if (s) goto error;
    addr += (uint32_t) sizeof(cmd);
    /* write payload */
    s = ndp_mcu_write_block(addr, cfg, sizeof(*cfg));
    if (s) goto error;

    /* alerts DSP to start/stop execution */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_dsp_restart(struct syntiant_ndp_device_s *ndp)
{
    int s, s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_dsp_restart\n");
        return s;
    }

    DEBUG_PRINTF("syntiant_ndp120_dsp_restart:\n");
    /* send MB cmd for DSP restart */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_RESTART,
        NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_pdm_clock_exe_mode_flashboot(
    struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp120_pdm_clk_exe_mode_e execution_mode)
{
    int s, s0;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[2] = {0};
    uint32_t data = 0;

    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_pdm_clock_exe_mode_flashboot\n");
        return s;
    }

    switch (execution_mode) {
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_CLEAN:
            cmd[0] = SYNTIANT_NDP120_PDM_CLK_START;
            cmd[1] = sizeof(data);
            break;
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_RESUME:
            cmd[0] = SYNTIANT_NDP120_PDM_CLK_START;
            cmd[1] = sizeof(data);
            data = 1;
            break;
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_PAUSE:
            cmd[0] = SYNTIANT_NDP120_PDM_CLK_STOP;
            memset(&ndp120->match_consumer, 0, sizeof(ndp120->match_consumer));
            break;
        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
    }

    s = syntiant_ndp120_write_block(ndp, 1, addr, cmd, sizeof(cmd));
    if (s) goto error;

    if (cmd[1]) {
        addr += (uint32_t) sizeof(cmd);
        s = syntiant_ndp120_write(ndp, 1, addr, data);
        if (s) goto error;
    }

    /* send MB cmd to MCU */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_FLASHBOOOT_CMD, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_pdm_clock_exe_mode(struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp120_pdm_clk_exe_mode_e execution_mode)
{
    int s, s0;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd_len[2] = {0};
    uint32_t data;

    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_pdm_clock_exe_mode\n");
        return s;
    }

    switch (execution_mode) {
        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_CLEAN:
            cmd_len[0] = SYNTIANT_NDP120_SEC_PDM_CLK_START;
            cmd_len[1] = sizeof(data);
            data = 0;
            break;
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_RESUME:
            cmd_len[0] = SYNTIANT_NDP120_SEC_PDM_CLK_START;
            cmd_len[1] = sizeof(data);
            data = 1;
            break;
        case SYNTIANT_NDP120_SEC_PDM_CLK_START_PAUSE:
            cmd_len[0] = SYNTIANT_NDP120_SEC_PDM_CLK_STOP;
            data = 0;
            memset(&ndp120->match_consumer, 0, sizeof(ndp120->match_consumer));
            break;
    }

    s = syntiant_ndp120_write_block(ndp, 1, addr, cmd_len, sizeof(cmd_len));
    if (s) goto error;

    if (cmd_len[1]) {
        addr += (uint32_t) sizeof(cmd_len);
        s = syntiant_ndp120_write(ndp, 1, addr, data);
        if (s) goto error;
    }

    /* send MB cmd to MCU */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_poll(struct syntiant_ndp_device_s *ndp,
    uint32_t *notifications, int clear)
{
#if SYNTIANT_NDP120_DEBUG_POLL
    static struct timeval last_debug_print;
    struct timeval tmp_time;
    int debug_print = 0;
#endif
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t notify = SYNTIANT_NDP_NOTIFICATION_NONE;
    uint8_t intsts;

    *notifications = 0;

    s = syntiant_ndp120_read(ndp, 0, NDP120_SPI_INTSTS, &intsts);
    if (s) {
        DEBUG_PRINTF("poll(read intsts) failed s=%d\n", s);
        goto error;
    }

#if SYNTIANT_NDP120_DEBUG_POLL
    {
        uint8_t intctl;
        syntiant_get_ms_time(&tmp_time);
        timersub(&tmp_time, &last_debug_print, &tmp_time);
        if ((tmp_time.tv_sec >= 2) || intsts) {
            syntiant_get_ms_time(&last_debug_print);
            debug_print = 1;
            s = syntiant_ndp120_read(ndp, 0, NDP120_SPI_INTCTL, &intctl);
            if (s) {
                DEBUG_PRINTF("poll(read intctl) failed s=%d\n", s);
                goto error;
            }
            DEBUG_PRINTF("poll(debug) intsts=%#x intctl=%#x\n", intsts, intctl);
        }
    }
    (void)debug_print;
#endif

    if (clear) {
        s = syntiant_ndp120_write(ndp, 0, NDP120_SPI_INTSTS, intsts);
        if (s) {
            DEBUG_PRINTF("poll(clear intsts) failed s=%d\n", s);
            goto error;
        }
    }

    /* H2M response interrupt */
    if (intsts & NDP120_SPI_INTSTS_MBIN_INT(1)) {
        s = mbin_processor(ndp, &notify);
        if (s) {
            DEBUG_PRINTF("poll(mbin processor) failed s=%d\n", s);
            goto error;
        }
        *notifications |= notify;
    }

    if (intsts & NDP120_SPI_INTSTS_DNN_INT(1)) {
        *notifications |= SYNTIANT_NDP_NOTIFICATION_DNN;
    }

    /* M2H request interrupt */
    /* Match comes via mailbox out event */
    if (intsts & NDP120_SPI_INTSTS_MBOUT_INT(1)) {
#if SYNTIANT_NDP120_DEBUG_POLL
        DEBUG_PRINTF("poll(debug) mbout interrupt\n");
#endif
        s = mbout_processor(ndp, &notify);
        if (s) {
            DEBUG_PRINTF("poll(mbout processor) failed s=%d\n", s);
            goto error;
        }

#if SYNTIANT_NDP120_DEBUG_POLL || SYNTIANT_NDP120_DEBUG_POLL_MATCH
        if (notify & SYNTIANT_NDP_NOTIFICATION_MATCH) {
            DEBUG_PRINTF("poll(debug) match message from mbout\n");
        }
#endif
        *notifications |= notify;
    }

    /* M2H Frequency domain completion (filter bank) interrupt */
    if (intsts & NDP120_SPI_INTSTS_FEATURE_INT(1)) {
        *notifications |= SYNTIANT_NDP_NOTIFICATION_FEATURE;
    }

    if (intsts & NDP120_SPI_INTSTS_AE_INT(1)) {
        *notifications |= SYNTIANT_NDP_NOTIFICATION_ERROR;
    }

    if (intsts & NDP120_SPI_INTSTS_RF_INT(1)) {
        *notifications |= SYNTIANT_NDP_NOTIFICATION_SPI_READ_FAILURE;
#if SYNTIANT_NDP120_DEBUG_POLL
        DEBUG_PRINTF("poll(): spi read failure\n");
#endif
        s = SYNTIANT_NDP_ERROR_SPI_READ_FAILURE;
        goto error;
    }

    if (intsts & NDP120_SPI_INTSTS_WM_INT(1)) {
        s = dsp_mb_processor(ndp, &notify);
        if (s) {
            DEBUG_PRINTF("poll(dsp mb processor) failed s=%d\n", s);
            goto error;
        }
#if SYNTIANT_NDP120_DEBUG_POLL
        DEBUG_PRINTF("poll(debug) updating DSP notifications with %#x\n", notify);
#endif
        *notifications |= notify;
    }
error:
    return s;
}

enum poll_wait_timeout {
    SYNTIANT_NDP_POLL_TIMEOUT_COUNT = 10000U,
    SYNTIANT_NDP_POLL_UDELAY_TIME   = 1000U
};

int syntiant_ndp120_poll_notification(
    struct syntiant_ndp_device_s *ndp, uint32_t notification_mask)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t notify = 0;
    unsigned counter = SYNTIANT_NDP_POLL_TIMEOUT_COUNT;

    while (counter--) {
        s = syntiant_ndp120_poll(ndp, &notify, 1);
        if (s) {
            goto done;
        }
        if (notify & notification_mask) {
            goto done;
        }
        if (ndp->iif.udelay) {
            ndp->iif.udelay(SYNTIANT_NDP_POLL_UDELAY_TIME);
        }
    }
    s = SYNTIANT_NDP_ERROR_TIMEOUT;

done:
    return s;
}

#ifdef SYNTIANT_NDP120_MAILBOX_TRACE
char *mailbox_dir[2] = { "h2m", "m2h" };

void
syntiant_ndp120_mb_request_trace(int m2h, uint8_t r)
{
    static char *reqs[] = SYNTIANT_NDP120_MB_MCU_REQUEST_DECODER;

    if (syntiant_ndp120_mailbox_trace) {
        if (1 < syntiant_ndp120_mailbox_trace
            || (r != SYNTIANT_NDP120_MB_MCU_REQUEST_CONT
                   && r != SYNTIANT_NDP120_MB_MCU_REQUEST_EXTOP
                   && (r & ~0x3) != SYNTIANT_NDP120_MB_MCU_REQUEST_DATA)) {
            SYNTIANT_PRINTF("%s req  %d: ", mailbox_dir[m2h],
                                syntiant_ndp120_mailbox_req_number[m2h]);
            if (r < sizeof(reqs) / sizeof(reqs[0])) {
                SYNTIANT_PRINTF("%s\n", reqs[r]);
            } else {
                SYNTIANT_PRINTF("ILLEGAL(%d)\n", r);
            }
        }
    }
    if (r < 8) {
        /*
         * only bump req count for 'primitive' (3-bit) requests
         * versus composite (extended/6-bit) requests
         */
        syntiant_ndp120_mailbox_req_number[m2h]++;
    }
}

void
syntiant_ndp120_mb_response_trace(int m2h, uint8_t r)
{
    static char *rsps[] = SYNTIANT_NDP120_MB_MCU_RESPONSE_DECODER;

    if (syntiant_ndp120_mailbox_trace) {
        if (1 < syntiant_ndp120_mailbox_trace
            || (r != SYNTIANT_NDP120_MB_MCU_RESPONSE_CONT
                   && r != SYNTIANT_NDP120_MB_MCU_RESPONSE_ERROR
                   && (r & ~0x3) != SYNTIANT_NDP120_MB_MCU_RESPONSE_DATA)) {
            SYNTIANT_PRINTF("%s rsp  %d: ", mailbox_dir[m2h],
                syntiant_ndp120_mailbox_rsp_number[m2h]);
            if (r < sizeof(rsps) / sizeof(rsps[0])) {
                SYNTIANT_PRINTF("%s\n", rsps[r]);
            } else {
                SYNTIANT_PRINTF("ILLEGAL(%d)\n", r);
            }
        }
    }
    syntiant_ndp120_mailbox_rsp_number[m2h]++;
}

void
syntiant_ndp120_mb_data_trace(int m2h, uint8_t *data, int bits)
{
    uint32_t d = 0;
    int i;

    if (syntiant_ndp120_mailbox_trace) {
        SYNTIANT_PRINTFi"%s data %d: ", mailbox_dir[m2h],
            syntiant_ndp120_mailbox_rsp_number[m2h]);
        if (bits <= sizeof(d) * 8) {
            memmove(&d, data, (bits + 7) / 8);
            SYNTIANT_PRINTF("0x%x", d);
        } else {
            for (i = 0; i < (bits + 7) / 8; i++) {
                SYNTIANT_PRINTF("0x%02x ", data[i]);
            }
        }
        SYNTIANT_PRINTF("\n");
    }
}

void
syntiant_ndp120_mb_unexpected_trace(int m2h, uint8_t r, uint8_t bits)
{
    int n;

    if (syntiant_ndp120_mailbox_trace) {
        n = m2h ? syntiant_ndp120_mailbox_req_number[m2h]
                : syntiant_ndp120_mailbox_rsp_number[m2h];
        SYNTIANT_PRINTF("%s unex %d: %d message unexpected, %d data bits remaining\n",
            mailbox_dir[m2h], n, r, bits);
    }
    syntiant_ndp120_mailbox_unexpected[m2h]++;
}

void
syntiant_ndp120_mb_error_trace(int m2h, uint8_t e)
{
    static char *errors[] = SYNTIANT_NDP120_MB_MCU_ERROR_DECODER;

    if (syntiant_ndp120_mailbox_trace) {
        SYNTIANT_PRINTF("%s err  %d: ", mailbox_dir[m2h],
            syntiant_ndp120_mailbox_rsp_number[m2h]);
        if (e < sizeof(errors) / sizeof(errors[0])) {
            SYNTIANT_PRINTF("%s\n", errors[e]);
        } else {
            SYNTIANT_PRINTF("ILLEGAL(%d)\n", e);
        }
    }
    syntiant_ndp120_mailbox_error[m2h]++;
}
#else
#define syntiant_ndp120_mb_request_trace(m2h, r) do { } while (0)
#define syntiant_ndp120_mb_response_trace(m2h, r) do { } while (0)
#define syntiant_ndp120_mb_data_trace(m2h, d, b) do { } while (0)
#define syntiant_ndp120_mb_unexpected_trace(m2h, r, bits) do { } while (0)
#define syntiant_ndp120_mb_error_trace(m2h, e) do { } while (0)
#endif


#if 0
static void
syntiant_ndp120_reset_mailbox_state(struct syntiant_ndp_device_s *ndp)
{

    /*
    int i;
    for (i = 0; i < SYNTIANT_NDP120_DEVICE_MAILBOX_DIRECTIONS; i++) {
        memset(&ndp->d.ndp120.mb[i], 0,
            sizeof(struct syntiant_ndp120_device_mb_state_s));
    }
    */
    ndp->d.ndp120.mbin = 0;
    ndp->d.ndp120.mbin_resp = 0;
    ndp->d.ndp120.fw_pointers_addr = 0;
    ndp->d.ndp120.fw_state_addr = 0;
}
#endif

/* ================== */
/* INTERNAL FUNCTIONS */
/* ================== */


static int
syntiant_ndp120_pkg_parse_nn_labels_v1_v3_tlv(struct syntiant_ndp_device_s *ndp)
{
    /* v2 was abandoned */
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t valid;
    uint32_t offset;
    uint32_t size;
    uint32_t daddr = ndp_get_scratch_address(ndp) +
        (uint32_t) offsetof(syntiant_ndp120_scratch_t, label_size);

    if(pstate->mode == PACKAGE_MODE_VALUE_START) {
        /*set valid bit and write length into config region*/
        s = syntiant_ndp120_write(ndp, 1, daddr, *(uint32_t*)pstate->length);
        if (s) goto error;

        ndp120->labels_len = *(uint32_t*)pstate->length;

    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE){
        offset = pstate->partially_read_length % PARSER_SCRATCH_SIZE ?
        pstate->partially_read_length -
            (pstate->partially_read_length % PARSER_SCRATCH_SIZE) :
            pstate->partially_read_length - PARSER_SCRATCH_SIZE;

        size = pstate->partially_read_length % PARSER_SCRATCH_SIZE ?
            (pstate->partially_read_length % PARSER_SCRATCH_SIZE) :
            PARSER_SCRATCH_SIZE;

        daddr = ndp_get_scratch_address(ndp) +
            ((uint32_t)offsetof(syntiant_ndp120_scratch_t,labels) + offset);
        s = syntiant_ndp120_write_block(ndp, 1, daddr, pstate->data.labels,
                size);
        if(s) goto error;
    }

    /* post-load actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        s = syntiant_ndp120_scratch_get_valid_skip_crc(ndp, &valid);
        if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;
        s = 0;
        valid = valid | SYNTIANT_CONFIG_LABELS_VALID;
        s = syntiant_ndp120_scratch_set_valid(ndp, valid);
        if (s) goto error;
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_php_collection_v7(struct syntiant_ndp_device_s *ndp)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_posterior_config_t ph_config;
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned int i;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-write actions */
        memset(&ph_config, 0, sizeof(ph_config));
        ph_config.set = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE;
        ph_config.enable = 0x0;
        s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
        /* fallthru */
        goto done;
    } else if (pstate->expected_bytes != 0) {
        goto done;
    }

    memset(&ph_config, 0, sizeof(ph_config));
    switch (pstate->data.phc_v7.parser.parsed) {
        case PHC_PARSE_COLLECTION_PARAMS:
            ph_config.set = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_NUM_PH;
            ph_config.ph_num = pstate->data.phc_v7.collection.num_ph;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;
            break;

        case PHC_PARSE_PH_PARAMS:
            ph_config.set =
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE;
            ph_config.states = pstate->data.phc_v7.ph_.param.num_states;
            ph_config.classes = pstate->data.phc_v7.ph_.param.num_classes;
            ph_config.ph_type = pstate->data.phc_v7.ph_.param.algorithm_type;
            ph_config.ph_idx = pstate->data.phc_v7.ph_.param.nn_idx;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;

            ph_config.set =
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_FRAMES |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_DENOMIATOR |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_MAX_UPDATES;
            ph_config.adaptive_frames =
                pstate->data.phc_v7.ph_.adaptive.adaptive_frames;
            ph_config.adaptive_denominator =
                pstate->data.phc_v7.ph_.adaptive.adaptive_denominator;
            ph_config.adaptive_max_updates =
                pstate->data.phc_v7.ph_.adaptive.adaptive_max_updates;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;
            break;

        case PHC_PARSE_STATE_PARAMS:
            ph_config.set =
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION;
            ph_config.timeout =
                pstate->data.phc_v7.state_.param.timeout;
            ph_config.timeout_action_type =
                pstate->data.phc_v7.state_.param.timeout_action;
            if (pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) {
                ph_config.timeout_action_arg0 =
                    pstate->data.phc_v7.state_.u.scp.timeout_action_arg;
            } else {
                ph_config.timeout_action_arg0 =
                    pstate->data.phc_v7.state_.u.mcp.timeout_action_match_arg;
                ph_config.timeout_action_arg1 =
                    pstate->data.phc_v7.state_.u.mcp.timeout_action_move_arg;
            }
            ph_config.ph_type = pstate->data.phc_v7.ph_.param.algorithm_type;
            ph_config.ph_idx = pstate->data.phc_v7.ph_.param.nn_idx;
            ph_config.state = pstate->data.phc_v7.parser.cur_state - 1;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;
            break;

        case PHC_PARSE_CLASS_PARAMS:
            ph_config.set =
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_THRESHOLD |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_WINDOW |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ACTION |
                SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_SM_QUEUE_SIZE;
            ph_config.threshold =
                pstate->data.phc_v7.class_.param.threshold;
            ph_config.window =
                pstate->data.phc_v7.class_.param.window;
            if (pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) {
                ph_config.set |=
                    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_BACKOFF |
                    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_THRESHOLD;
                ph_config.backoff =
                    pstate->data.phc_v7.class_.u.scp.backoff;
                ph_config.action_type =
                    pstate->data.phc_v7.class_.u.scp.action;
                ph_config.action_arg0 =
                    pstate->data.phc_v7.class_.u.scp.action_arg;
                ph_config.smoothing_queue_size =
                    pstate->data.phc_v7.class_.u.scp.smoothing_queue_size;
                ph_config.adaptive_threshold =
                    pstate->data.phc_v7.class_.u.scp.adaptive_threshold;
            } else {
                ph_config.action_type =
                    pstate->data.phc_v7.class_.u.mcp.action;
                ph_config.action_arg0 =
                    pstate->data.phc_v7.class_.u.mcp.action_match_arg;
                ph_config.action_arg1 =
                    pstate->data.phc_v7.class_.u.mcp.action_move_arg;
                ph_config.smoothing_queue_size =
                    pstate->data.phc_v7.class_.u.mcp.smoothing_queue_size;
            }
            ph_config.ph_type = pstate->data.phc_v7.ph_.param.algorithm_type;
            ph_config.ph_idx = pstate->data.phc_v7.ph_.param.nn_idx;
            ph_config.state = pstate->data.phc_v7.parser.cur_state - 1;
            ph_config.class_index = pstate->data.phc_v7.parser.cur_class - 1;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;
            break;
    }

    if (pstate->mode == PACKAGE_MODE_TAG_START) {
        /* posterior params load done */
        memset(&ph_config, 0, sizeof(ph_config));
        ph_config.set = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE;
        ph_config.enable = 0x3;
        s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
        if (s) goto done;

        /* set number of networks (assuming one ph per network) */
        memset(&ph_config, 0, sizeof(ph_config));
        s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
        if (s) goto done;
        ndp120->num_networks = ph_config.ph_num;

        /* set number of classes per network*/
        memset(ndp120->classes, 0, sizeof(ndp120->classes));
        for (i = 0; i < pstate->data.phc_v7.collection.num_ph; i++) {
            memset(&ph_config, 0, sizeof(ph_config));
            ph_config.ph_idx = i;
            s = syntiant_ndp120_posterior_config_no_sync(ndp, &ph_config);
            if (s) goto done;
            ndp120->classes[i] = ph_config.classes;
        }
    };

done:
    return s;
}

/**
 * @brief function to parse PBI version string
 * @param ndp device object
 */
static int
syntiant_ndp120_pkg_pbi_version_tlv (struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    const char *chip_name;
    enum pbi_version_chip_type_t_ chip_type = 0;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* nothing */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        pstate->package_pbi_version = pstate->data.pkg_pbi_version_params;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        chip_name = syntiant_ndp120_get_config_devtype(ndp->device_type,
            &chip_type);
        if (chip_type != PBI_CHIP_TYPE_NDP200_A0) {
            if (pstate->package_pbi_version.chip_type != chip_type) {
                s = SYNTIANT_NDP_ERROR_PACKAGE_PBI_VER;
                ERROR_PRINTF("Invalid PBI Version tlv for chip_type: %s\n",
                    chip_name);
            }
        }
    }

    return s;
}

static int
syntiant_ndp120_is_auth(struct syntiant_ndp_device_s *ndp, uint8_t *auth)
{
    uint32_t data;
    int s = syntiant_ndp120_read(ndp, SYNTIANT_NDP120_SPI, NDP120_SPI_ID0, &data);
    if (s) return s;
    *auth = NDP120_SPI_ID0_AUTH_FIRMWARE_EXTRACT(data);
    return s;
}

int
syntiant_ndp120_pkg_parse_store_version(struct syntiant_ndp_device_s *ndp,
    uint32_t tag, uint8_t *ver, uint32_t len)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t valid;
    uint32_t scratch_value_adx;
    uint32_t scratch_len_adx;

    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) {
        DEBUG_PRINTF("error from syntiant_ndp120_scratch_get_valid: %d\n", s);
        goto error;
    }
    s = 0;

    if (tag == TAG_FIRMWARE_VERSION_STRING_V1 ||
        tag == TAG_FIRMWARE_VERSION_STRING_V2) {
        valid |= SYNTIANT_CONFIG_FW_VERSION_VALID;
    } else if (tag == TAG_NN_VERSION_STRING_V1) {
        valid |= SYNTIANT_CONFIG_NN_VERSION_VALID;
    } else if (tag == TAG_DSP_FIRMWARE_VERSION_STRING_V1 ||
               tag == TAG_DSP_FIRMWARE_VERSION_STRING_V2) {
        valid |= SYNTIANT_CONFIG_DSP_FW_VERSION_VALID;
    } else if (tag == TAG_PACKAGE_VERSION_STRING ||
               tag == TAG_PACKAGE_VERSION_STRING_V2) {
        valid |= SYNTIANT_CONFIG_PKG_VERSION_VALID;
    } else if (tag == TAG_PACKAGE_BINARY_INTERFACE_VERSION) {
        valid |= SYNTIANT_CONFIG_PBI_VERSION_VALID;
    }

    switch (tag) {

    case TAG_FIRMWARE_VERSION_STRING_V1:
    case TAG_FIRMWARE_VERSION_STRING_V2:
        scratch_value_adx = SCRATCH_VARIABLE_ADX(fw_version);
        scratch_len_adx = SCRATCH_VARIABLE_ADX(fw_version_size);
        break;

    case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
        scratch_value_adx = SCRATCH_VARIABLE_ADX(dsp_fw_version);
        scratch_len_adx = SCRATCH_VARIABLE_ADX(dsp_fw_version_size);
        break;

    case TAG_PACKAGE_VERSION_STRING:
    case TAG_PACKAGE_VERSION_STRING_V2:
        scratch_value_adx = SCRATCH_VARIABLE_ADX(pkg_version);
        scratch_len_adx = SCRATCH_VARIABLE_ADX(pkg_version_size);
        break;

    case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
        scratch_value_adx = SCRATCH_VARIABLE_ADX(pbi_version);
        scratch_len_adx = SCRATCH_VARIABLE_ADX(pbi_version_size);
        break;

    default:
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    DEBUG_PRINTF("config: writing length: 0x%X to 0x%X\n", len, scratch_len_adx);
    s = syntiant_ndp120_write(ndp, 1, scratch_len_adx, len);
    if (s) goto error;

    DEBUG_PRINTF("config: writing value: '%*s' to 0x%X\n", -len, ver, scratch_value_adx);
    s = syntiant_ndp120_write_block(ndp, 1, scratch_value_adx, ver, len);
    if (s) goto error;

    s = syntiant_ndp120_scratch_set_valid(ndp, valid);
    if (s) goto error;
    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s) goto error;

error:
    return s;
}

/**
 * @brief function to parse version strings
 * @param ndp device object
 */
static int
syntiant_ndp120_pkg_parse_version_string_v1_tlv(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t *value_adx;
    uint32_t *len_adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;

    if (pstate->mode == PACKAGE_MODE_TAG_START &&
        pstate->partially_read_length == *(uint32_t*)pstate->length) {

        switch(*(uint32_t *)pstate->tag){

        case TAG_FIRMWARE_VERSION_STRING_V1:
        case TAG_FIRMWARE_VERSION_STRING_V2:
            value_adx = pstate->data.fw_version;
            len_adx = &ndp120->fwver_len;
            break;

        case TAG_NN_VERSION_STRING_V1:
            value_adx = pstate->data.params_version;
            len_adx = &ndp120->paramver_len;
            break;

        case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
        case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
            value_adx = pstate->data.dsp_fw_version;
            len_adx = &ndp120->dspfwver_len;
            break;

        case TAG_PACKAGE_VERSION_STRING:
        case TAG_PACKAGE_VERSION_STRING_V2:
            value_adx = pstate->data.pkg_version;
            len_adx = &ndp120->pkgver_len;
            break;

        default:
            s = SYNTIANT_NDP_ERROR_UNSUP;
            goto error;
        }

        *len_adx = *(uint32_t*)pstate->length;
        s = syntiant_ndp120_pkg_parse_store_version(ndp,
                *(uint32_t*)pstate->tag, value_adx, *len_adx);
    }

error:
    return s;
}


static int
syntiant_ndp120_pkg_parse_firmware_tlv(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    uint32_t offset, size;
    uint8_t data;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        uint32_t clkctl0;

        /* enable mbin and mbout interrupts */
        s = ndp_spi_read(NDP120_SPI_INTCTL, &data);
        if(s) goto error;
        data = NDP120_SPI_INTCTL_MBIN_INTEN_INSERT(data,1);
        data = NDP120_SPI_INTCTL_MBOUT_INTEN_INSERT(data,1);
        s = ndp_spi_write(NDP120_SPI_INTCTL, data);
        if(s) goto error;

        /* enable interrupt pin */
        s = ndp_spi_read(NDP120_SPI_CFG, &data);
        if(s) goto error;
        data = NDP120_SPI_CFG_INTEN_INSERT(data,1);
        s = ndp_spi_write(NDP120_SPI_CFG, data);
        if(s) goto error;

        /* halt the CPU */
        s = ndp_spi_read(NDP120_SPI_CTL, &data);
        if(s) goto error;
        data = NDP120_SPI_CTL_MCUHALT_INSERT(data,1);
        s = ndp_spi_write(NDP120_SPI_CTL, data);
        if(s) goto error;

        /* put MCU into reset state */
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL0, &clkctl0);
        if (s) goto error;
        clkctl0 = NDP120_CHIP_CONFIG_CLKCTL0_MCURSTB_MASK_INSERT(clkctl0, 0x0);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL0, clkctl0);
        if (s) goto error;

#ifndef __KERNEL__
        /* quench unhandled BOOTING mb message */
        s = mbout_recv(ndp, &data);
        if (s) goto error;
#endif
        /* disable bootrom */
        s = ndp_mcu_write(NDP120_SYSCTL_MEMCTRL, 0);
        if (s) goto error;

        /* clear interrupts */
        s = ndp_spi_write(NDP120_SPI_INTSTS, 0xff);
        if(s) goto error;
        memset(ndp120->match_producer, 0, sizeof(ndp120->match_producer));
        memset(ndp120->match_consumer, 0, sizeof(ndp120->match_consumer));
    }
    /* copy the firmware in BOOTRAM */
    else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        offset = (pstate->partially_read_length % PARSER_SCRATCH_SIZE) ?
            (pstate->partially_read_length -
                (pstate->partially_read_length % PARSER_SCRATCH_SIZE)) :
            (pstate->partially_read_length - PARSER_SCRATCH_SIZE);

        size = (pstate->partially_read_length % PARSER_SCRATCH_SIZE) ?
            (pstate->partially_read_length % PARSER_SCRATCH_SIZE) :
            PARSER_SCRATCH_SIZE;

        s = ndp_mcu_write_block(
            NDP120_BOOTRAM_REMAP + offset, pstate->data.fw, size);
        if (s) goto error;
    }

    /* post-load actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        uint32_t clkctl0;

        /* reset the MCU */
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL0, &clkctl0);
        if (s) goto error;
        clkctl0 = NDP120_CHIP_CONFIG_CLKCTL0_MCURSTB_MASK_INSERT(clkctl0, 0x1);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL0, clkctl0);
        if (s) goto error;

        /* unhalt the CPU */
        s = ndp_spi_read(NDP120_SPI_CTL, &data);
        if(s) goto error;
        data = (uint8_t)NDP120_SPI_CTL_MCUHALT_MASK_INSERT(data, 0);
        s = ndp_spi_write(NDP120_SPI_CTL, data);
        if(s) goto error;

#ifdef SKIP_FW_HANDSHAKE
        goto error;
#endif
        s = mbout_send_resp(ndp, SYNTIANT_NDP120_MB_MCU_ACK);
        if (s) goto error;
        s = mbout_recv(ndp, &data);
        DEBUG_PRINTF("got s:%d data: %d\n", s, data);
        if(s) goto error;

        switch (data) {
        case SYNTIANT_NDP120_MB_MCU_RUNNING:
            /* do nothing, intentionally */
            DEBUG_PRINTF("got running\n");
            ndp120->mcu_fw_loaded = 1;
            break;
        case SYNTIANT_NDP120_MB_MCU_RUNNING_TEST:
        case SYNTIANT_NDP120_MB_MCU_NOP:
            /* skip handshake */
            goto error;
        default:
            s = SYNTIANT_NDP_ERROR_FAIL;
            DEBUG_PRINTF("received unknown message 0x%X from MCU\n", data);
            goto error;
        }
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_MIADDR, NULL);
        if (s) goto error;

        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP_MBIN_REQUEST_GET_PH_ENABLED_STATE, NULL);
        if (s) goto error;
    }

error:
    return s;
}

static int
process_multisegment(struct syntiant_ndp_device_s *ndp)
{
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_ndp120_multisegment_state_t *ms_state = &ndp120->ms_state;
    uint32_t size;
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (pstate->is_multisegment) {
        uint8_t *cur = pstate->data.fw;

        size = (pstate->partially_read_length % PARSER_SCRATCH_SIZE)
            ? (pstate->partially_read_length % PARSER_SCRATCH_SIZE)
            : PARSER_SCRATCH_SIZE;

        while (size) {
#define DEBUG_MULTISEGMENT 0

            if (ms_state->bytes_rem_segment == 0) {
                /* new header */
                /* need to be careful, the parser chunk might
                    cut the header in half */

                if (ms_state->hdr_idx < sizeof(ms_state->hdr)) {
                    uint32_t bytes_to_read = MIN( (uint32_t)sizeof(ms_state->hdr) - ms_state->hdr_idx, size);
                    memcpy( ms_state->hdr + ms_state->hdr_idx, cur, bytes_to_read);
                    ms_state->hdr_idx += bytes_to_read;
                    size -= bytes_to_read;
                    cur += bytes_to_read;
                    /*
                    DEBUG_PRINTF("Added %u bytes to header, idx: %d, sizeof
                    header: "
                           "%lu",
                        bytes_to_read, ms_state->hdr_idx,
                    sizeof(ms_state->hdr)); printf("Header:\n");
                    printHex(ms_state->hdr, sizeof(ms_state->hdr));
                    */
                }

                if (ms_state->hdr_idx == sizeof(ms_state->hdr)) {
                    ms_state->adx = *(uint32_t *)ms_state->hdr;
                    ms_state->bytes_rem_segment = *(uint32_t *)(ms_state->hdr + sizeof(uint32_t));
                    ms_state->hdr_idx = 0;
#if DEBUG_MULTISEGMENT
                    DEBUG_PRINTF("Have full header. adx: 0x%X, size: %u\n",
                        ms_state->adx, ms_state->bytes_rem_segment);
#endif
                }
            } else {
                uint32_t bytes_to_write = MIN(size, ms_state->bytes_rem_segment);
                s = ndp_mcu_write_block(ms_state->adx, cur, bytes_to_write);
#if DEBUG_MULTISEGMENT
                if (ms_state->adx == 0x600800E0){
                    DEBUG_PRINTF("multiseg adx: %p\n", ms_state->adx);
                    hexdump(ms_state->adx, cur, bytes_to_write);
                }
#endif
                if (s)  {
                    DEBUG_PRINTF( "%s:%d ndp_mcu_write_block returned error %d\n", __FILE__, __LINE__, s);
                    goto error;
                }
                ms_state->adx += bytes_to_write;
                ms_state->bytes_rem_segment -= bytes_to_write;
                size -= bytes_to_write;
                cur += bytes_to_write;

#if DEBUG_MULTISEGMENT
                DEBUG_PRINTF("processing %u bytes, segment remaining: %u, chunk "
                       "remaining: %u",
                    bytes_to_write, ms_state->bytes_rem_segment, size);
#endif
            }
        }
    } else {
        s = SYNTIANT_NDP_ERROR_PACKAGE;
        goto error;
    }
error:
    return s;
}

int syntiant_ndp120_update_clock_rate_in_dsp(struct syntiant_ndp_device_s *ndp) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx, mcuclkdiv;
    uint32_t freq = 0, data;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    DEBUG_PRINTF("~~> update_clock_rate_in_dsp...\n");
    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        DEBUG_PRINTF("~~> update_clock_rate_in_dsp: dsp_fw_state_addr uninit (s=%u)\n", s);
        goto error;
    }
    s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, &freq);
    if (s) goto error;

    /* mcu clkdiv */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL0, &data);
    if (s) goto error;
    mcuclkdiv = NDP120_CHIP_CONFIG_CLKCTL0_MCUCLKDIV_EXTRACT(data);
    DEBUG_PRINTF("~~> update_clock_rate_in_dsp: mcuclkdiv=%u\n", mcuclkdiv);
    adx = ndp120->dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_state_t, base.clock_rate);
    if (freq/mcuclkdiv) {
        DEBUG_PRINTF("~~> update_clock_rate_in_dsp: change DSP core freq to %d\n",
            freq/mcuclkdiv);
        s = ndp_mcu_write(adx, freq/mcuclkdiv);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_set_heartbeat_interval_ms(struct syntiant_ndp_device_s *ndp,
                                              uint32_t interval_ms) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        SYNTIANT_PRINTF("dsp_fw_state_addr uninit (s=%u)\n", s);
        goto error;
    }
    adx = ndp120->dsp_fw_state_addr + (uint32_t)offsetof(ndp120_dsp_fw_state_t,
                                                base.heartbeat_interval_ms);
    s = ndp_mcu_write(adx, interval_ms);
    if (s) goto error;

    /* DSP restart */
    s = syntiant_ndp120_do_mailbox_req(ndp, NDP120_DSP_MB_H2D_RESTART, NULL);
    if (s) goto error;

error:
    return s;
}

static int
syntiant_ndp120_get_dsp_fw_running_state(struct syntiant_ndp_device_s *ndp)
{
    ndp120_dsp_mailbox_msg_t msg;
    uint8_t data;

    int s = watermarkint_recv(ndp, &msg);
    if(s) return s;

    data = NDP120_DSP_MB_GET_MESSAGE(msg);
    if (data != NDP120_DSP_MB_D2H_RUNNING) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("Did not receive 'running' from DSP, got:0x%x", data);
        return s;
    }

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_ADX_LOWER, NULL);
    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_firmware_tlv(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_multisegment_state_t *ms_state = &ndp->d.ndp120.ms_state;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;


    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* reset the DSP */
        uint8_t data_8 = 0;

        /* if the normal MCU fw isn't loaded,
           we need to halt the MCU so the
           bootloader doesn't respond */

        if (!ndp120->mcu_fw_state_addr) {
            s = syntiant_ndp120_halt_mcu(ndp);
            if (s) goto error;
            s = ndp_spi_write(NDP120_SPI_INTSTS, 0xff);
            if(s) goto error;
        }

        /* enable watermark interrupts */
        s = ndp_spi_read(NDP120_SPI_INTCTL, &data_8);
        if(s) goto error;
        data_8 = NDP120_SPI_INTCTL_WM_INTEN_INSERT(data_8,1);
        s = ndp_spi_write(NDP120_SPI_INTCTL, data_8);
        if(s) goto error;

        /* enable interrupt pin */
        s = ndp_spi_read(NDP120_SPI_CFG, &data_8);
        if(s) goto error;
        data_8 = NDP120_SPI_CFG_INTEN_INSERT(data_8, 1);
        s = ndp_spi_write(NDP120_SPI_CFG, data_8);
        if(s) goto error;

        if (!ndp120->mcu_fw_state_addr) {
            /* quench BOOTING mb message */
            s = mbout_recv(ndp, &data_8);
            if (s) return s;
        }

        memset(ms_state, 0, sizeof(syntiant_ndp120_multisegment_state_t));

    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        s = process_multisegment(ndp);
        if (s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        uint32_t data = 0;

        s = ndp_mcu_read(NDP120_CHIP_CONFIG_DSP_CFG, &data);
        if (s) goto error;
        data = NDP120_CHIP_CONFIG_DSP_CFG_RUN_STALL_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_CFG, data);
        if (s) goto error;

        ndp120->dsp_pcm_audio_sample_last_ptr = 0;
        ndp120->dsp_pcm_audio_annotation_last_ptr = 0;
        ndp120->dsp_function_sample_last_ptr = 0;

        DEBUG_PRINTF("waiting for RUNNING\n");
        s = syntiant_ndp120_get_dsp_fw_running_state(ndp);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_get_algo_config_area_sizes(struct syntiant_ndp_device_s *ndp,
              uint32_t *algo_config_sizes, uint8_t num_algo_config_areas)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t adx =
        ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, algo_config_size);

    if (num_algo_config_areas > NDP120_DSP_ALGO_CONFIG_MAX_COUNT) {
        num_algo_config_areas = NDP120_DSP_ALGO_CONFIG_MAX_COUNT;
    }
    return ndp_mcu_read_block(adx, algo_config_sizes,
                (uint32_t) sizeof(*algo_config_sizes) * num_algo_config_areas);
}

static int
ndp120_read_write_algo_config_no_sync(struct syntiant_ndp_device_s *ndp,
    unsigned int index, unsigned int offset, void *data, unsigned int size,
    int read)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    unsigned int max_index = NDP120_DSP_ALGO_CONFIG_MAX_COUNT;
    uint32_t adx, algo_addr = 0;

    if (index >= max_index) {
        s = SYNTIANT_NDP_ERROR_ARG;
        SYNTIANT_PRINTF("%u exceeds max index (s=%u)\n", index, s);
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        SYNTIANT_PRINTF("dsp_fw_state_addr uninit (s=%u)\n", s);
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr +
        (uint32_t)offsetof(ndp120_dsp_fw_base_t, algo_config) +
        index * (uint32_t)sizeof(uint32_t);
    s = ndp_mcu_read(adx, &algo_addr);
    if (s || !algo_addr) {
        s = SYNTIANT_NDP_ERROR_UNALLOC;
        goto error;
    }

    adx = algo_addr + offset;
    DEBUG_PRINTF("addr=%#x algo=%u index=%u offset=%u adx=%#x (size=%u)\n",
        ndp120->dsp_fw_state_addr,
        (unsigned) offsetof(ndp120_dsp_fw_base_t, algo_config),
        index, offset, adx, size);
    if (read) {
        s = ndp_mcu_read_block(adx, data, size);
    } else {
        s = ndp_mcu_write_block(adx, data, size);
    }
    if (s) {
        SYNTIANT_PRINTF("%s error (s=%u)\n", read ? "read" : "write", s);
        goto error;
    }

error:
    return s;
}

int
syntiant_ndp120_write_algo_config(struct syntiant_ndp_device_s *ndp,
    unsigned int index, void *data, unsigned int size)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = ndp120_read_write_algo_config_no_sync(ndp, index, 0, data, size, 0);
    if (s) goto error;

    /* DSP restart */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_RESTART,
                                               NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int
syntiant_ndp120_read_write_algo_bindings_no_sync(struct syntiant_ndp_device_s *ndp,
    void *data, unsigned int size, int read)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t adx;

    if (size != NDP120_DSP_ALGO_MAX_COUNT * sizeof(ndp120_dsp_algo_t)) {
        s = SYNTIANT_NDP_ERROR_ARG;
        SYNTIANT_PRINTF("dsp_fw_state_addr invalid arg (s=%u)\n", s);
        goto error;
    }
    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        SYNTIANT_PRINTF("dsp_fw_state_addr uninit (s=%u)\n", s);
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_state_t, algos);

    if (read) {
        s = ndp_mcu_read_block(adx, data, size);
    } else {
        s = ndp_mcu_write_block(adx, data, size);
    }
    if (s) {
        SYNTIANT_PRINTF("%s error (s=%u)\n", read ? "read" : "write", s);
        goto error;
    }

error:
    return s;
}

int syntiant_ndp120_read_write_algo_bindings(struct syntiant_ndp_device_s *ndp,
    void *data, unsigned int size, int read)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_read_write_algo_bindings_no_sync(ndp, data, size,
                                                         read);
    if (s) goto error;

    if (!read) {
        /* DSP restart */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_RESTART, NULL);
        if (s) goto error;
    }

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int
syntiant_ndp120_read_write_dnn_power_cfg_no_sync(struct syntiant_ndp_device_s
    *ndp, uint32_t *data, uint32_t size, int write)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t payload_in_openram[3] = {0};
    uint32_t adx_w = NDP120_DSP_OPEN_RAM_START;
    uint32_t adx_r = NDP120_DSP_OPEN_RAM_RESULTS;

    if (data == NULL || size != DNN_PAYLOAD_MAX_SIZE) {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        DEBUG_PRINTF("Error: Invalid args in syntiant_ndp120_read_write_dnn_power_cfg_no_sync\n");
        goto out;
    }

    if (write) {
        if (data[0] <= DNNBANK_MAX_PARAM_MASK && data[1] <= DNNBANK_MAX_DATA_MASK) {
            payload_in_openram[0] = size;
            payload_in_openram[1] = data[0];
            payload_in_openram[2] = data[1];
        } else {
            s = SYNTIANT_NDP_ERROR_CONFIG;
            DEBUG_PRINTF("Error: Invalid data in syntiant_ndp120_read_write_dnn_power_cfg_no_sync\n");
            goto out;
        }
    }

    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, adx_w,
            payload_in_openram, sizeof(payload_in_openram));
    if (s) {
        DEBUG_PRINTF("Error: Invalid write block in syntiant_ndp120_read_write_dnn_power_cfg_no_sync\n");
        goto out;
    }
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_EXT_RW_POWER_PROFILE, NULL);
    if (s) {
        DEBUG_PRINTF("Error: Invalid mailbox req in syntiant_ndp120_read_write_dnn_power_cfg_no_sync\n");
        goto out;
    }
    if (!write) {
        s = syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_MCU, adx_r,
            data, size);
        if (s) {
        DEBUG_PRINTF("Error: Invalid read block in syntiant_ndp120_read_write_dnn_power_cfg_no_sync\n");
        goto out;
        }
    }
out:
    return s;
}

int
syntiant_ndp120_read_write_dnn_power_cfg(struct syntiant_ndp_device_s *ndp,
        uint32_t *data, uint32_t size, int write)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_read_write_dnn_power_cfg_no_sync(ndp, data, size,
                                                         write);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_read_algo_config(struct syntiant_ndp_device_s *ndp,
    unsigned int index, void *data, unsigned int size)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = ndp120_read_write_algo_config_no_sync(ndp, index, 0, data, size, 1);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int ndp120_read_write_audio_sync_config(
    struct syntiant_ndp_device_s *ndp, ndp120_dsp_audio_sync_config_t *config,
    int read) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t)offsetof(ndp120_dsp_fw_state_t,
                                                         audio_sync_config);

    if (read) {
        s = ndp_mcu_read_block(adx, config, sizeof(*config));
    } else {
        s = ndp_mcu_write_block(adx, config, sizeof(*config));
    }
    if (s) goto error;

error:
    return s;
}

int
syntiant_ndp120_write_audio_sync_config(struct syntiant_ndp_device_s *ndp,
                                        ndp120_dsp_audio_sync_config_t *config) {
    int s = SYNTIANT_NDP_ERROR_NONE;

    s = ndp120_read_write_audio_sync_config(ndp, config, 0);
    if (s) goto error;

    /* DSP restart */
    s = syntiant_ndp120_do_mailbox_req(ndp, NDP120_DSP_MB_H2D_RESTART, NULL);
    if (s) goto error;

error:
    return s;
}

int
syntiant_ndp120_read_audio_sync_config(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_audio_sync_config_t *config)
{
    return ndp120_read_write_audio_sync_config(ndp, config, 1);
}

int
syntiant_ndp120_write_sample_config(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_config_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, config) +
            (uint32_t) offsetof(ndp120_dsp_config_t, aud_samp_size_bytes);
    s = ndp_mcu_write_block(adx, &config->aud_samp_size_bytes,
                            sizeof(config->aud_samp_size_bytes));
    if (s) goto error;

    adx = ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, config) +
            (uint32_t) offsetof(ndp120_dsp_config_t, func_samp_size_bytes);
    s = ndp_mcu_write_block(adx, &config->func_samp_size_bytes,
                            sizeof(config->func_samp_size_bytes));
    if (s) goto error;

    adx = ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, config) +
            (uint32_t) offsetof(ndp120_dsp_config_t, aud_samp_cap);
    s = ndp_mcu_write_block(adx, &config->aud_samp_cap,
                            sizeof(config->aud_samp_cap));
    if (s) goto error;

    adx = ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, config) +
            (uint32_t) offsetof(ndp120_dsp_config_t, func_samp_cap);
    s = ndp_mcu_write_block(adx, &config->func_samp_cap,
                            sizeof(config->func_samp_cap));
    if (s) goto error;

    /*restartrt DSP to re-allocate memory for sample tanks */
    s = syntiant_ndp120_do_mailbox_req(ndp, NDP120_DSP_MB_H2D_RESTART,
                                       NULL);
error:
    return s;
}

int
syntiant_ndp120_read_sample_config(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_config_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_base_t, config);
    s = ndp_mcu_read_block(adx, config, sizeof(*config));

error:
    return s;
}

int
syntiant_ndp120_read_dsp_tank_memory(struct syntiant_ndp_device_s *ndp,
                                     ndp120_dsp_memory_state_t *memory_state)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_state_t, mem_state);
    s = ndp_mcu_read_block(adx, memory_state, sizeof(*memory_state));

error:
    return s;
}

static int
syntiant_ndp120_read_dsp_state(struct syntiant_ndp_device_s *ndp,
                                struct ndp120_dsp_state *dsp_state)
{
    uint32_t adx;
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    adx = ndp120->dsp_fw_state_addr +
      (uint32_t) offsetof(ndp120_dsp_fw_base_t, dsp_state);

    if (!ndp120->dsp_fw_state_addr) {
        DEBUG_PRINTF("syntiant_ndp120_config_dsp_tank_memory: UNINIT\n");
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }
    s = syntiant_ndp120_read(ndp, 1, adx, dsp_state);
    if (s) {
        SYNTIANT_PRINTF("error in reading DSP FW config block\n");
        goto error;
    }

error:
    return s;
}

static int
read_health_check_counters(struct syntiant_ndp_device_s *ndp,
    uint32_t *dsp_fw_wake_count, uint32_t *mcu_fw_wake_count)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    adx = (uint32_t) ndp120->dsp_fw_state_addr +
        (uint32_t) offsetof(ndp120_dsp_fw_base_t, fw_wake_count);
    s = syntiant_ndp120_read_block(ndp, 1, adx, dsp_fw_wake_count,
        sizeof(*dsp_fw_wake_count));
    if (s) goto error;

    adx = (uint32_t) (ndp120->mcu_fw_state_addr + (uint32_t)
        offsetof(struct ndp120_fw_state_s, fw_wake_count));
    s = syntiant_ndp120_read_block(ndp, 1, adx, mcu_fw_wake_count,
        sizeof(*mcu_fw_wake_count));
error:
    return s;
}

int
syntiant_ndp120_check_fw(struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp_fw_state *state, uint32_t wait_period_us)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t dsp_fw_wake_count1, mcu_fw_wake_count1,
        dsp_fw_wake_count2, mcu_fw_wake_count2;
    *state = SYNTIANT_NDP_BOTH_FW_DEAD;

    s = read_health_check_counters(ndp, &dsp_fw_wake_count1,
        &mcu_fw_wake_count1);
    if (s) goto error;

    ndp->iif.udelay(wait_period_us);

    s = read_health_check_counters(ndp, &dsp_fw_wake_count2,
        &mcu_fw_wake_count2);
    if (s) goto error;

    if (dsp_fw_wake_count1 != dsp_fw_wake_count2 &&
        mcu_fw_wake_count1 != mcu_fw_wake_count2) {
        /* both mcu and dsp counters incremented */
        *state = SYNTIANT_NDP_BOTH_FW_ALIVE;
        goto error;
    }

    /* try one more time */
    ndp->iif.udelay(wait_period_us);

    s = read_health_check_counters(ndp, &dsp_fw_wake_count1,
        &mcu_fw_wake_count1);
    if (s) goto error;

    if (dsp_fw_wake_count1 != dsp_fw_wake_count2) {
        *state = SYNTIANT_NDP_DSP_FW_ALIVE;
    }
    if (mcu_fw_wake_count1 != mcu_fw_wake_count2) {
        *state |= SYNTIANT_NDP_MCU_FW_ALIVE;
    }

error:
    return s;
}

void
syntiant_ndp120_get_fw_load_status(struct syntiant_ndp_device_s *ndp,
                                    enum syntiant_ndp_load_state *state)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    *state = SYNTIANT_NDP_FW_NOT_LOADED;

    if (ndp120->mcu_fw_loaded) {
        *state |= SYNTIANT_NDP_MCU_FW_LOADED;
    }
    if (ndp120->dsp_fw_loaded) {
        *state |= SYNTIANT_NDP_DSP_FW_LOADED;
    }
    if (ndp120->nn_metadata_loaded && ndp120->nn_params_loaded) {
        *state |= SYNTIANT_NDP_NN_LOADED;
    }
}

int
syntiant_ndp120_get_audio_params(struct syntiant_ndp_device_s *ndp,
                                  struct ndp120_dsp_audio_params_s *aparams)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
               NDP120_DSP_MB_H2D_EXT_GET_AUDIO_PARAMS, NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, NDP120_DSP_OPEN_RAM_RESULTS,
          aparams, sizeof(*aparams));
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_dsp_get_info(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE, s0;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t addr_r = NDP120_DSP_OPEN_RAM_RESULTS;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_GET_INFO, NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, addr_r, &ndp120->extract_percentage,
        sizeof(ndp120->extract_percentage));
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_get_extract_sample_size(
    struct syntiant_ndp_device_s *ndp, int spi_speed, uint32_t sample_size,
    int channels, uint32_t *extract_size)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t time_per_frame_in_us;
    uint32_t num_frames_per_winstep, num_frames;
    uint32_t winstep_ms, sample_rate;
    uint32_t spi_speed_in_hz = (uint32_t)spi_speed / 1000000;
    uint32_t sample_width = PCM_AUDIO_SAMPLE_WIDTH_BYTES;
    uint32_t bits_per_byte = 8;
    uint32_t extract_delay;
    uint32_t extract_window_utilization = EXTRACT_WINDOW_UTILIZATION;
    syntiant_ndp120_config_pdm_t config_pdm;
    memset(&config_pdm, 0, sizeof(config_pdm));
    config_pdm.get = 1;
    s = syntiant_ndp120_config_pdm_no_sync(ndp, &config_pdm);
    if (s) {
        SYNTIANT_PRINTF("pdm: config pdm fail\n");
        goto done;
    }
    sample_rate = config_pdm.sample_rate;
    if (!ndp120->extract_percentage) {
        s = syntiant_ndp120_dsp_get_info(ndp);
        if (s) {
            goto done;
        }
    }
    extract_delay = ndp120->extract_percentage;

    time_per_frame_in_us = (sample_size * bits_per_byte / spi_speed_in_hz);
    winstep_ms = (sample_size * 1000 / sample_rate /  sample_width);
    num_frames_per_winstep = (winstep_ms * 1000 / time_per_frame_in_us);
    num_frames = num_frames_per_winstep * extract_delay / 100;
    /* setting num_frames based on extract window utilization */
    num_frames = num_frames *  extract_window_utilization / 100;
    if (channels >= TWO_CHANNELS) {
        sample_size += (uint32_t)sizeof(ndp120_dsp_audio_sample_annotation_t);
    }
    *extract_size = num_frames * sample_size;

done:
    return s;
}

int
syntiant_ndp120_get_memory_status(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_mcu_mem_state_t *mcu_dsp_mem_info)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP_MBIN_REQUEST_GET_MEM_INFO,NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, NDP120_MCU_OPEN_RAM_RESULTS,
        &mcu_dsp_mem_info->mcu_state, sizeof(mcu_dsp_mem_info->mcu_state));
    if (s) goto error;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_GET_MEM_INFO,NULL);
    if(s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, NDP120_DSP_OPEN_RAM_RESULTS,
        &mcu_dsp_mem_info->dsp_state, sizeof(mcu_dsp_mem_info->dsp_state));
    if(s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_config_dsp_tank_memory(struct syntiant_ndp_device_s *ndp,
                                       syntiant_ndp120_config_tank_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx, max_winstep_us;
    ndp120_dsp_config_t cfg = {0};
    ndp120_dsp_memory_state_t mem_state ={0};
    struct ndp120_dsp_state dsp_state = {0};
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};

    if (!config->set && !config->get) {
        DEBUG_PRINTF("syntiant_ndp120_config_dsp_tank_memory:not get or set,"
                     "returning\n");
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        DEBUG_PRINTF("syntiant_ndp120_config_dsp_tank_memory: UNINIT\n");
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }
    /* Read dsp tank memory to get specific memory statistics such as
       total memory, memory used by algorithms, and memory used by current
       sample tanks */
    s = syntiant_ndp120_read_dsp_tank_memory(ndp, &mem_state);
    if (s) goto error;

    DEBUG_PRINTF("dsp mem_heap_total = %d\ndsp mem_dnn_data_total = %d\n",
                  mem_state.mem_heap_total, mem_state.mem_dnn_data_total);
    DEBUG_PRINTF("dsp algo_mem_use_heap = %d\n"
                 "dsp algo_mem_use_dnn_data = %d\n",
                  mem_state.algo_mem_use_heap,
                  mem_state.algo_mem_use_dnn_data);
    DEBUG_PRINTF("dsp sample_mem_use_heap = %d\n"
                 "dsp sample_mem_use_dnn_data = %d\n",
                  mem_state.sample_mem_use_heap,
                  mem_state.sample_mem_use_dnn_data);

    /* For remaining memory: round aud_samp_cap down to integer */
    if (config->set) {
        adx = NDP120_DSP_MB_H2D_ADDR;
        payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
                config->sampletank_msec);
        s = syntiant_ndp120_write_block(ndp, 1, adx, payload_in_MSB,
                sizeof(payload_in_MSB));
        if (s) goto error;

        s = syntiant_ndp120_do_mailbox_req(ndp,
                NDP120_DSP_MB_H2D_EXT_SET_TANK_SIZE, NULL);
        if (s) goto error;
    }
    /* compute the sample tank size again based on what's been configured on the
     * device
     */
    adx = ndp120->dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_base_t, max_winstep_us);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &max_winstep_us,
                                   sizeof(max_winstep_us));
    if (s) goto error;

    adx = ndp120->dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_base_t, config);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &cfg, sizeof(cfg));
    if (s) {
        SYNTIANT_PRINTF("error in reading DSP FW config block\n");
        goto error;
    }

    s = syntiant_ndp120_read_dsp_state(ndp, &dsp_state);
    if (s) goto error;
    config->sampletank_msec = cfg.aud_samp_cap * max_winstep_us / 1000;
    config->sampletank_mem_type = dsp_state.sampletank_mem_type;
    ndp120->dsp_sampletank_mem_type =dsp_state.sampletank_mem_type;

error:
    return s;
}

static void
get_config_input(ndp120_dsp_data_flow_setup_t *driver_flow_setup,
    uint8_t *input_type)
{
    uint8_t idx;
    for (idx = 0; idx < ARRAY_LEN(driver_flow_setup->src_pcm_audio); idx++) {
        ndp120_dsp_data_flow_rule_t *rule = &driver_flow_setup->src_pcm_audio[idx];
        if (NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) {
            if (rule->src_param == NDP120_DSP_DATA_FLOW_SRC_PARAM_SPI) {
                *input_type = NDP120_DSP_DATA_FLOW_SRC_PARAM_SPI;
                break;
            }
        }
    }
}

int
syntiant_ndp120_config_flow_rules(struct syntiant_ndp_device_s *ndp,
                  enum syntiant_ndp120_input_config_mode_e input_type)
{
    int s, counter = 0;
    uint8_t idx = 0;
    uint8_t func_check = 0;
    uint8_t configure_input = SYNTIANT_NDP120_INPUT_CONFIG_PDM;
    ndp120_dsp_data_flow_setup_t driver_flow_setup = {0};
    syntiant_ndp120_config_pdm_t ndp120_pdm_config = {0};

    ndp120_pdm_config.get = 1;

    s = syntiant_ndp120_config_pdm_no_sync(ndp, &ndp120_pdm_config);
    if (s) {
        SYNTIANT_PRINTF("pdm: config pdm fail\n");
        goto out;
    }

    s = syntiant_ndp120_dsp_flow_setup_get_no_sync(ndp, &driver_flow_setup);
    if (s) goto out;

    get_config_input(&driver_flow_setup, &configure_input);
    if (configure_input != input_type) {
        for (idx = 0; idx < ARRAY_LEN(driver_flow_setup.src_pcm_audio); idx++) {
            ndp120_dsp_data_flow_rule_t *rule = &driver_flow_setup.src_pcm_audio[idx];
            if (NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) {
                if (input_type == SYNTIANT_NDP120_INPUT_CONFIG_SPI) {
                    func_check = (driver_flow_setup.src_pcm_audio[idx].dst_param ==
                                 NDP120_DSP_DATA_FLOW_FUNCTION_NOAEC_FF ||
                                 driver_flow_setup.src_pcm_audio[idx].dst_param ==
                                 NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF ||
                                 driver_flow_setup.src_pcm_audio[idx].dst_param ==
                                 NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF_49);
                    if (driver_flow_setup.src_pcm_audio[idx].dst_type ==
                        NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION && func_check) {
                        DEBUG_PRINTF("Watch via SPI is not supported for AFEs"
                                      "taking stereo input,please use the valid"
                                      "package(single channel) for SPI watch\n");
                        s = SYNTIANT_NDP_ERROR_PACKAGE;
                        goto out;
                    }
                    rule->src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_SPI;
                }
                if (input_type == SYNTIANT_NDP120_INPUT_CONFIG_PDM) {
                    if (ndp120_pdm_config.mode == NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO) {
                        if (driver_flow_setup.src_pcm_audio[idx].dst_type ==
                            NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT) {
                            counter++;
                            rule->src_param = (counter == 1) ? NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT :
                                NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_RIGHT;
                        }
                        else {
                            rule->src_param = driver_flow_setup.src_pcm_audio[idx].src_param;
                        }
                    }
                    else if(ndp120_pdm_config.mode == NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT ||
                        NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_RIGHT) {
                        rule->src_param = ndp120_pdm_config.mode;
                    }
                }
            }
        }
        s = syntiant_ndp120_dsp_flow_setup_apply_no_sync(ndp, &driver_flow_setup);
        if (s) goto out;

        s = syntiant_ndp120_dsp_restart(ndp);
        if (s) goto out;
    }

out:
    return s;
}

static int
syntiant_ndp120_pkg_parse_nn_softmax (struct syntiant_ndp_device_s *ndp) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_config_misc_t config_misc;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* nothing */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        memset(&config_misc, 0, sizeof(config_misc));
        config_misc.set |= SYNTIANT_NDP120_CONFIG_SET_MISC_SOFTMAX;
        config_misc.nn_num = pstate->data.softmax.nn_num;
        config_misc.quantized_sf = pstate->data.softmax.quantized_sf;
        s = syntiant_ndp120_config_misc_no_sync(ndp, &config_misc);
        if (s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_fe_config_v3(struct syntiant_ndp_device_s *ndp) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_dsp_fe_config_v3_data_t *ptr = &pstate->data.dsp_fe_config_v3;
    static syntiant_algo_config_t area;
    int incremental = 1;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        memset(&area, 0, sizeof(area));
        goto done;
    } else if (pstate->expected_bytes != 0) {
        goto done;
    }

    switch (ptr->parser.parsed) {
        case ALGO_PARSE_HEADER:
            area.param.signature = ptr->signature;
            area.param.frame_step_ms = ptr->frame_step_ms;
            area.param.power_offset = ptr->power_offset;
            area.param.preemphasis_expoonent = ptr->preemphasis_expoonent;
            area.param.num_filter_banks = ptr->num_filter_banks;
            area.param.algo_config_index = (int8_t) ptr->index;
            area.param.secondary_validation = (uint8_t)
                        ptr->secondary_validation;
            /* send an H2D EXT MB message for algo config area allocation */
            s = syntiant_ndp120_alloc_algo_config_area_no_sync(ndp, ptr->index,
                                      NDP120_DSP_ALGO_CONFIG_DEFAULT_SIZE);
            if (s) {
                if (s == SYNTIANT_NDP_WARN_CONFIG_EXISTS) {
                    DEBUG_PRINTF("Attempted to create an algo config area that exists\n");
                }
                goto done;
            }
            if (incremental) {
                s = ndp120_read_write_algo_config_no_sync(
                    ndp,
                    ptr->index,
                    offsetof(syntiant_algo_config_t, param.signature),
                    &area.param.signature,
                    offsetof(syntiant_algo_config_t, param.melbin_boundary) -
                        offsetof(syntiant_algo_config_t, param.signature),
                    0);
                if (s) goto done;
            }
            break;

        case ALGO_PARSE_ARRAY:
            if (ptr->parser.index < (unsigned)ptr->num_filter_banks + 2) {
                memcpy(
                    area.param.melbin_boundary + ptr->parser.index,
                    ptr->melbin_boundary + ptr->parser.index,
                    sizeof(*ptr->melbin_boundary) * ptr->parser.count);
                if (incremental) {
                    s = ndp120_read_write_algo_config_no_sync(
                        ndp,
                        ptr->index,
                        (unsigned)(
                            offsetof(syntiant_algo_config_t,
                                param.melbin_boundary) +
                            ptr->parser.index * sizeof(*ptr->melbin_boundary)),
                        area.param.melbin_boundary + ptr->parser.index,
                        (unsigned)sizeof(*ptr->melbin_boundary) *
                            ptr->parser.count,
                        0);
                    if (s) goto done;
                }
            } else if (ptr->parser.index ==
                        (unsigned)ptr->num_filter_banks + 2) {
                /* done */
            } else {
                SYNTIANT_PRINTF("ERROR melbin array overrun\n");
                s = SYNTIANT_NDP_ERROR_FAIL;
                goto done;
            }
            break;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        if (!incremental) {
            s = ndp120_read_write_algo_config_no_sync(
                ndp,
                ptr->index,
                offsetof(syntiant_algo_config_t, param.signature),
                &area.param,
                (unsigned)(offsetof(syntiant_algo_config_t, param.melbin_boundary) -
                    offsetof(syntiant_algo_config_t, param.signature) +
                    (area.param.num_filter_banks + 2u) *
                    sizeof(*area.param.melbin_boundary)),
                0);
            if (s) goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_dnn_power_config_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* nothing */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        s = syntiant_ndp120_read_write_dnn_power_cfg_no_sync(ndp,
            (uint32_t *) &pstate->data.dnn_power,
                sizeof(pstate->data.dnn_power), 1);
        if (s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_auto_clear_interrupt_config(
    struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_config_misc_t config;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* nothing */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        memset(&config, 0, sizeof(config));
        config.set |= SYNTIANT_NDP120_CONFIG_SET_MISC_INT_CLEAR;
        config.frame_count = pstate->data.auto_clear.num_frames;
        config.clear_match = pstate->data.auto_clear.clear_on_match;
        s = syntiant_ndp120_config_misc(ndp, &config);
        if (s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
    }

error:
    return s;
}
static int
syntiant_ndp120_pkg_parse_dsp_mcu_secondary_valildation_v1(
        struct syntiant_ndp_device_s *ndp) {
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_dsp_mcu_secondary_validation_t *parsed_node;
    struct ndp120_val_type_one_shot_s config = {0};
    uint8_t sec_val_type[NDP120_DNN_MAX_NUM_NETWORKS];
    uint32_t adr;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-read actions */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_node = &(pstate->data.dsp_mcu_sec_val);

        adr = (uint32_t) (ndp120->mcu_fw_state_addr + (uint32_t)
                offsetof(struct ndp120_fw_state_s, sec_validation_type));

        s = syntiant_ndp120_read_block(ndp, 1, adr,
            &sec_val_type, sizeof(uint32_t));
        if (s) goto error;

        sec_val_type[parsed_node->network_id] = parsed_node->validation_type;

        s = syntiant_ndp120_write_block(ndp, 1, adr,
            &sec_val_type, sizeof(uint32_t));
        if (s) goto error;

        memset(&config, 0, sizeof(config));
        config.threshold = parsed_node->threshold;
        config.ratio_threshold = parsed_node->ratio_threshold;
        config.num_frames = parsed_node->num_frames;
        config.notification_type = parsed_node->notification_type;

        adr = (uint32_t) (ndp120->mcu_fw_state_addr +
                (uint32_t) offsetof(struct ndp120_fw_state_s, one_shot));

        adr += (uint32_t) (sizeof(config) * (parsed_node->network_id));
        s = syntiant_ndp120_write_block(ndp, 1, adr, &config, sizeof(config));
        if (s) goto error;
#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nnetwork_id: %d\n", parsed_node->network_id);
        DEBUG_PRINTF("\tvalidation_type: %d\n", parsed_node->validation_type);
        DEBUG_PRINTF("\tnum_frames: %d\n", parsed_node->num_frames);
        DEBUG_PRINTF("\tthreshold: %d\n", parsed_node->threshold);
        DEBUG_PRINTF("\tratio_threshold: %d\n", parsed_node->ratio_threshold);
#endif
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_algo_params_v1(struct syntiant_ndp_device_s *ndp)
{

    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    static syntiant_algo_config_t area;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        memset(&area, 0, sizeof(area));
        goto done;
    } else if (pstate->expected_bytes != 0) {
        goto done;
    }

    memcpy(area.data.config, pstate->data.algo_params.payload,
        NDP120_ALGO_CONFIG_V2_AREA_LEN);

    /* send an H2D EXT MB message for algo config area allocation */
    s = syntiant_ndp120_alloc_algo_config_area_no_sync(ndp,
        pstate->data.algo_params.base.algo_config_idx,
        NDP120_DSP_ALGO_CONFIG_DEFAULT_SIZE);
    if (s) {
        if (s == SYNTIANT_NDP_WARN_CONFIG_EXISTS) {
            DEBUG_PRINTF(
                "Attempted to create an algo config area that exists\n");
        }
        goto done;
    }

    s = ndp120_read_write_algo_config_no_sync(ndp,
        pstate->data.algo_params.base.algo_config_idx, 0, area.data.config,
        NDP120_ALGO_CONFIG_V2_AREA_LEN, 0);
    if (s) goto done;

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_mcu_secondary_valildation_v1(
            struct syntiant_ndp_device_s *ndp) {
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_mcu_secondary_validation_t *parsed_node;
    struct ndp120_val_type_pattern_s config = {0};
    uint8_t sec_val_type[NDP120_DNN_MAX_NUM_NETWORKS];
    uint32_t adr;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-read actions */
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_node = &(pstate->data.mcu_sec_val);

        adr = (uint32_t) (ndp120->mcu_fw_state_addr + (uint32_t)
                offsetof(struct ndp120_fw_state_s, sec_validation_type));

        s = syntiant_ndp120_read_block(ndp, 1, adr,
            &sec_val_type, sizeof(uint32_t));
        if (s) goto error;

        sec_val_type[parsed_node->network_id] = parsed_node->validation_type;

        s = syntiant_ndp120_write_block(ndp, 1, adr,
            &sec_val_type, sizeof(uint32_t));
        if (s) goto error;

        memset(&config, 0, sizeof(config));
        config.num_occurrences = parsed_node->num_occurrences;
        config.frame_range_low = parsed_node->frame_range_low;
        config.frame_range_high = parsed_node->frame_range_high;
        config.notification_type = parsed_node->notification_type;

        adr = (uint32_t) (ndp120->mcu_fw_state_addr +
                (uint32_t) offsetof(struct ndp120_fw_state_s, patterned));
        adr += (uint32_t) (sizeof(config) * (parsed_node->network_id));
        s = syntiant_ndp120_write_block(ndp, 1, adr, &config, sizeof(config));
        if (s) goto error;
#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nnetwork_id: %d\n", parsed_node->network_id);
        DEBUG_PRINTF("\tvalidation_type: %d\n", parsed_node->validation_type);
        DEBUG_PRINTF("\nnum_occurrences: %d\n", parsed_node->num_occurrences);
        DEBUG_PRINTF("\tframe_range_low: %d\n", parsed_node->frame_range_low);
        DEBUG_PRINTF("\tframe_range_high: %d\n",
                        parsed_node->frame_range_high);
#endif
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_mic_settings(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_config_mic_t config = {
        0UL, SYNTIANT_NDP120_CONFIG_MIC_SETTINGS, 0, 0, 0, 0 };
    uint8_t i;

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    for (i = 0; i < FIFO_MIC_CNT; i++) {
        config.mic = i;
        config.sensitivity = pstate->data.mic_settings.sensitivity[i];
        config.delay = pstate->data.mic_settings.delay[i];
        s = syntiant_ndp120_config_mic_no_sync(ndp, &config);
        if (s) {
            DEBUG_PRINTF("parse_mic_settings: config mic[%u] failed %d\n", i, s);
            break;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_algo_attach_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        syntiant_algo_attach_v1_t *cfg = &pstate->data.algo_attach.v1;
        ndp120_dsp_algo_t algos[NDP120_DSP_ALGO_MAX_COUNT];
        unsigned i;

        s = syntiant_ndp120_read_write_algo_bindings_no_sync(
            ndp, algos, sizeof(algos), 1);
        if (s) {
            goto error;
        }
        for (i = 0; i < NDP120_DSP_ALGO_MAX_COUNT; i++) {
            if (algos[i].algo_id == cfg->algo_id) {
                algos[i].algo_config_index = cfg->algo_config_index;
                s = syntiant_ndp120_read_write_algo_bindings_no_sync(
                    ndp, algos, sizeof(algos), 0);
                if (s) {
                    goto error;
                }
            }
        }
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_flow_collection_v1(
    struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    ndp120_dsp_data_flow_rule_t rule = {0};
    syntiant_dsp_flow_collection_v1_t * parsed_rule;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    ndp120_dsp_data_flow_setup_t *setup =
        (ndp120_dsp_data_flow_setup_t *) pstate->metadata.scratch_metadata;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        s = syntiant_ndp120_dsp_flow_setup_reset(setup);
        if(s) goto error;
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_rule = &(pstate->data.dsp_flow_collection.v1);

#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nRule:\n");
        DEBUG_PRINTF("\tSet id: %d\n", parsed_rule->set_id);
        DEBUG_PRINTF("\tSrc type: %d\n", parsed_rule->src_type);
        DEBUG_PRINTF("\tSrc param: %d\n", parsed_rule->src_param);
        DEBUG_PRINTF("\tDest type: %d\n", parsed_rule->dst_type);
        DEBUG_PRINTF("\tDest param: %d\n", parsed_rule->dst_param);
        DEBUG_PRINTF("\tAlgo Config Index: %d\n", parsed_rule->algo_config_index);
#endif

        rule.set_id             = parsed_rule->set_id;
        rule.src_param          = parsed_rule->src_param;
        rule.dst_param          = parsed_rule->dst_param;
        rule.dst_type           = parsed_rule->dst_type;
        rule.algo_config_index  = parsed_rule->algo_config_index;
        s = syntiant_ndp120_dsp_flow_setup_add_rule(setup,
            &rule, parsed_rule->src_type);
        if(s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        s = syntiant_ndp120_dsp_flow_setup_apply_no_sync(ndp, setup);
        if(s) goto error;
    }

error:
    if (s) {
        DEBUG_PRINTF("Error: %d", s);
    }
    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_flow_collection_v2(
    struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    ndp120_dsp_data_flow_rule_t rule = {0};
    syntiant_dsp_flow_collection_v2_t * parsed_rule;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    ndp120_dsp_data_flow_setup_t *setup =
        (ndp120_dsp_data_flow_setup_t *) pstate->metadata.scratch_metadata;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        s = syntiant_ndp120_dsp_flow_setup_reset(setup);
        if(s) goto error;
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_rule = &(pstate->data.dsp_flow_collection.v2);

#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nRule:\n");
        DEBUG_PRINTF("\tSet id: %d\n", parsed_rule->set_id);
        DEBUG_PRINTF("\tSrc type: %d\n", parsed_rule->src_type);
        DEBUG_PRINTF("\tSrc param: %d\n", parsed_rule->src_param);
        DEBUG_PRINTF("\tDest type: %d\n", parsed_rule->dst_type);
        DEBUG_PRINTF("\tDest param: %d\n", parsed_rule->dst_param);
        DEBUG_PRINTF("\tAlgo Config Index: %d\n", parsed_rule->algo_config_index);
#endif

        rule.set_id             = parsed_rule->set_id;
        rule.src_param          = parsed_rule->src_param;
        rule.dst_param          = parsed_rule->dst_param;
        rule.dst_type           = parsed_rule->dst_type;
        rule.algo_config_index  = parsed_rule->algo_config_index;
        s = syntiant_ndp120_dsp_flow_setup_add_rule(setup,
            &rule, parsed_rule->src_type);
        if(s) goto error;
    }

    /* post-read actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        s = syntiant_ndp120_dsp_flow_setup_apply_no_sync(ndp, setup);
        if(s) goto error;
    }

error:
    if (s) {
        DEBUG_PRINTF("Error: %d", s);
    }
    return s;
}

static int maybe_send_prepare(struct syntiant_ndp_device_s *ndp) {
    int s = SYNTIANT_NDP_ERROR_NONE;

    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (ndp120->nn_metadata_loaded && ndp120->nn_params_loaded) {

        /* alerts DSP to init dnn state */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_EXT_NN_LOAD_COMPLETE, NULL);
        if (s) goto error;

        /* alerts MCU to init dnn state */
        s = syntiant_ndp120_do_mailbox_req_no_sync(
            ndp, SYNTIANT_NDP120_MB_MCU_PREPARE, NULL);
        if (s) goto error;
    }

error:
    return s;
}

int
syntiant_ndp120_pkg_parse_partially_stored_params(syntiant_pkg_parser_state_t *pstate,
    int collect_log){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        unsigned int templen = *(uint32_t*) pstate->length;
        unsigned int temptag = *(uint32_t*)pstate->tag;

        /*sanity check on fw size*/
        if (temptag == TAG_NDP120_B0_NN_PARAMETERS_V1) {
            if (templen >  NN_MAX_SIZE){
                SYNTIANT_PRINTF("Tag too large: %u > %u\n", templen,
                                NN_MAX_SIZE);
                s = SYNTIANT_PACKAGE_ERROR_FIRMWARE;
                goto error;
            }
        } else if (temptag == TAG_NDP115_A0_NN_PARAMETERS_V1) {
            if (templen >  NN_MAX_SIZE_115_A0) {
                SYNTIANT_PRINTF("Tag too large: %u > %u\n", templen,
                                NN_MAX_SIZE_115_A0);
                s = SYNTIANT_PACKAGE_ERROR_FIRMWARE;
                goto error;
            }
        } else if (templen > FW_MAX_SIZE){
            SYNTIANT_PRINTF("Tag too large: %u > %u\n", templen,
                            FW_MAX_SIZE);
            s = SYNTIANT_PACKAGE_ERROR_FIRMWARE;
            goto error;
        }
        pstate->metadata.fw_metadata =
        minimum(PARSER_SCRATCH_SIZE, templen);
        if (collect_log){
            SYNTIANT_PRINTF("\n--------------------------\n");
            SYNTIANT_PRINTF("\n - Firmware length: %u\n", templen);
        }
    }
    if (pstate->expected_bytes == 0)
        pstate->expected_bytes = pstate->metadata.fw_metadata;
    byte_read = syntiant_pkg_read_chunk_data(pstate,
        (uint8_t*)(&(pstate->data.fw)),
        pstate->metadata.fw_metadata, 0, 1, 1);
    pstate->partially_read_length += byte_read;
    if (pstate->partially_read_length == *(uint32_t*)pstate->length){
        pstate->mode = PACKAGE_MODE_TAG_START;
        pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
        if (pstate->expected_bytes != 0){
            s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            goto error;
        }
    }
    else{
        if (pstate->expected_bytes){
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        }
        else{
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            pstate->metadata.fw_metadata =
            minimum(PARSER_SCRATCH_SIZE, *(uint32_t*)pstate->length -
                pstate->partially_read_length);
            pstate->expected_bytes = pstate->metadata.fw_metadata;
        }
    }
error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse partially stored params\n");
    }
    return s;
}

static int
syntiant_ndp120_pkg_parse_nn_metadata(struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_nn_metadata_t config;
    syntiant_nn_metadata_v1_t *nn_metadata = &pstate->data.nn_metadata.v1;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-write actions */
    } else if (pstate->expected_bytes != 0) {
        goto error;
    }

#if SYNTIANT_NDP120_DEBUG && SYNTIANT_NDP120_DEBUG_NN_META
    DEBUG_PRINTF("\nParser mode: %d\n", pstate->mode);
    DEBUG_PRINTF("\tParser_params:\n");
    DEBUG_PRINTF("\t\tcurrent network: %d\n", nn_metadata->parser.cur_nn);
    DEBUG_PRINTF("\t\tcurrent layer: %d\n", nn_metadata->parser.cur_layer);
    DEBUG_PRINTF("\t\tparsing: %d\n", nn_metadata->parser.parsing);
    DEBUG_PRINTF("\t\tparsed: %d\n", nn_metadata->parser.parsed);
    switch (nn_metadata->parser.parsed) {
    case NNM_PARSE_HEADER:
        DEBUG_PRINTF("\tInitialial Meta content:\n");
        DEBUG_PRINTF("\t\tNum NN: %d\n", nn_metadata->initial_meta.nn_num);
        DEBUG_PRINTF("\t\tDnn mode: %#x\n", nn_metadata->initial_meta.dnn_mode);
        break;
    case NNM_PARSE_BASE_META:
        DEBUG_PRINTF("\tBase Meta Content:\n");
        DEBUG_PRINTF("\t\tNum layers: %d\n", nn_metadata->base_meta.num_layers);
        DEBUG_PRINTF("\t\tIs cached: %d\n", nn_metadata->base_meta.is_nn_cached);
        DEBUG_PRINTF("\t\tInput ISA index: %d\n", nn_metadata->base_meta.nn_input_isa_idx);
        DEBUG_PRINTF("\t\tOutput ISA index: %d\n", nn_metadata->base_meta.nn_output_isa_idx);
        DEBUG_PRINTF("\t\tInput layer type: %d\n", nn_metadata->base_meta.nn_input_layer_type);
        break;
    case NNM_PARSE_INP_SIZE:
        DEBUG_PRINTF("\tInput Size Content:\n");
        DEBUG_PRINTF("\t\tx: %d, y: %d, z: %d\n",
            nn_metadata->inp_size.x, nn_metadata->inp_size.y, nn_metadata->inp_size.z);
        break;
    case NNM_PARSE_COORD:
        DEBUG_PRINTF("\tCoordinates content:\n");
        DEBUG_PRINTF("\t\tInput: 0x%X, Output: 0x%X\n",
            nn_metadata->coords.input_coord, nn_metadata->coords.output_coord);
        break;
    case NNM_PARSE_CACHE_INST:
        DEBUG_PRINTF("\tCache instructions content:\n");
        DEBUG_PRINTF("\t\tinput_base_coord_max: %d\n", nn_metadata->cache_params.input_base_coord_max);
        DEBUG_PRINTF("\t\toutput_base_coord_max: %d\n", nn_metadata->cache_params.output_base_coord_max);
        DEBUG_PRINTF("\t\tinput_base_coord_add: %d\n", nn_metadata->cache_params.input_base_coord_add);
        DEBUG_PRINTF("\t\tinput_offset_add: %d\n", nn_metadata->cache_params.input_offset_add);
        DEBUG_PRINTF("\t\tinput_offset_max: %d\n", nn_metadata->cache_params.input_offset_max);
        DEBUG_PRINTF("\t\toutput_base_coord_add: %d\n", nn_metadata->cache_params.output_base_coord_add);
        DEBUG_PRINTF("\t\toutput_base_coord_stride: %d\n", nn_metadata->cache_params.output_base_coord_stride);
        break;
    }
#endif

    memset(&config, 0, sizeof(syntiant_ndp120_nn_metadata_t));
    config.header.version = NDP120_DNN_METADATA_VERSION_V1;
    switch (nn_metadata->parser.parsed) {
        case NNM_PARSE_HEADER:
            config.set = SYNTIANT_NDP120_SET_META_HEADER;
            config.header.nn_num = (uint8_t)nn_metadata->initial_meta.nn_num;
            config.header.dnn_mode = nn_metadata->initial_meta.dnn_mode;
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_BASE_META:
            config.set = SYNTIANT_NDP120_SET_LAYER_PER_NN |
                SYNTIANT_NDP120_SET_IS_NN_CACHED |
                SYNTIANT_NDP120_SET_INPUT_ISA_IDX |
                SYNTIANT_NDP120_SET_OUTPUT_ISA_IDX |
                SYNTIANT_NDP120_SET_INPUT_LAYER_TYPE;
            config.nn_idx = nn_metadata->parser.cur_nn - 1;
            config.layers_per_nn = nn_metadata->base_meta.num_layers;
            config.is_nn_cached = nn_metadata->base_meta.is_nn_cached;
            config.nn_input_isa_idx = nn_metadata->base_meta.nn_input_isa_idx;
            config.nn_output_isa_idx = nn_metadata->base_meta.nn_output_isa_idx;
            config.nn_input_layer_type = nn_metadata->base_meta.nn_input_layer_type;
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_INP_SIZE:
            config.set = SYNTIANT_NDP120_SET_INPUT_SIZE;
            config.nn_idx = nn_metadata->parser.cur_nn - 1;
            memcpy(&config.nn_input_layer_size, &nn_metadata->inp_size,
                sizeof(nn_metadata->inp_size));
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_COORD:
            config.set = SYNTIANT_NDP120_SET_INPUT_COORD |
                SYNTIANT_NDP120_SET_OUTPUT_COORD;
            config.nn_idx = nn_metadata->parser.cur_nn - 1;
            config.layer_idx = nn_metadata->parser.cur_layer - 1;
            config.input_coord = nn_metadata->coords.input_coord;
            config.output_coord = nn_metadata->coords.output_coord;
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_CACHE_INST:
            config.set = SYNTIANT_NDP120_SET_CACHE_INST;
            config.nn_idx = nn_metadata->parser.cur_nn - 1;
            config.layer_idx = nn_metadata->parser.cur_layer - 1;
            memcpy(&config.cache_inst, &nn_metadata->cache_params,
                sizeof(nn_metadata->cache_params));
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;
    }

    if (pstate->mode == PACKAGE_MODE_TAG_START) {
        ndp120->nn_metadata_loaded = 1;
        maybe_send_prepare(ndp);
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_nn_metadata_v2(struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_nn_metadata_t config;
    syntiant_nn_metadata_v2_t *meta = &pstate->data.nn_metadata.v2;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    int i = 0;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-write actions */
    } else if (pstate->expected_bytes != 0) {
        goto error;
    }

#if SYNTIANT_NDP120_DEBUG && SYNTIANT_NDP120_DEBUG_NN_META
    DEBUG_PRINTF("\nParser mode: %d\n", pstate->mode);
    DEBUG_PRINTF("\tParser_params:\n");
    DEBUG_PRINTF("\t\tcurrent network: %d\n", meta->parser.cur_subgraph);
    DEBUG_PRINTF("\t\tcurrent layer: %d\n", meta->parser.cur_layer);
    DEBUG_PRINTF("\t\tparsing: %d\n", meta->parser.parsing);
    DEBUG_PRINTF("\t\tparsed: %d\n", meta->parser.parsed);
    switch (meta->parser.parsed) {
    case NNM_PARSE_HEADER:
        DEBUG_PRINTF("\tInitial Meta Content:\n");
        DEBUG_PRINTF("\t\tNum NN: %d\n", meta->initial_meta.nn_num);
        DEBUG_PRINTF("\t\tDnn mode: %#x\n", meta->initial_meta.dnn_mode);
        break;
    case NNM_PARSE_BASE_META:
        DEBUG_PRINTF("\tBase Meta Content:\n");
        DEBUG_PRINTF("\t\tNum layers: %d\n", meta->base_meta.num_layers);
        DEBUG_PRINTF("\t\tNum inputs: %d\n", meta->base_meta.num_inputs);
        DEBUG_PRINTF("\t\tNum outputs: %d\n", meta->base_meta.num_outputs);
        DEBUG_PRINTF("\t\tIs cached: %d\n", meta->base_meta.is_cached);
        break;
    case NNM_PARSE_INDICES_LAYER:
        DEBUG_PRINTF("\t\tLayer Indices: ");
        for(i=0; i < METADATA_MAX_NUM_LAYERS; i++) {
            DEBUG_PRINTF("%u ", meta->layers[i]);
        }
        DEBUG_PRINTF("\n");
        break;
    case NNM_PARSE_INDICES_INPUT:
        DEBUG_PRINTF("\t\tInput Indices: ");
        for(i=0; i < METADATA_MAX_NUM_INPUTS; i++) {
            DEBUG_PRINTF("%u ", meta->inputs[i]);
        }
        DEBUG_PRINTF("\n");
        break;
    case NNM_PARSE_INDICES_OUTPUT:
        DEBUG_PRINTF("\t\tOutput Indices: ");
        for(i=0; i < METADATA_MAX_NUM_OUTPUTS; i++) {
            DEBUG_PRINTF("%u ", meta->outputs[i]);
        }
        DEBUG_PRINTF("\n");
        break;
    case NNM_PARSE_CACHE_INST:
        DEBUG_PRINTF("\tCache instructions content:\n");
        DEBUG_PRINTF("\t\tinput_base_coord_max: %d\n", meta->cache_params.input_base_coord_max);
        DEBUG_PRINTF("\t\toutput_base_coord_max: %d\n", meta->cache_params.output_base_coord_max);
        DEBUG_PRINTF("\t\tinput_base_coord_add: %d\n", meta->cache_params.input_base_coord_add);
        DEBUG_PRINTF("\t\tinput_offset_add: %d\n", meta->cache_params.input_offset_add);
        DEBUG_PRINTF("\t\tinput_offset_max: %d\n", meta->cache_params.input_offset_max);
        DEBUG_PRINTF("\t\toutput_base_coord_add: %d\n", meta->cache_params.output_base_coord_add);
        DEBUG_PRINTF("\t\toutput_base_coord_stride: %d\n", meta->cache_params.output_base_coord_stride);
        break;
    }
#endif

    memset(&config, 0, sizeof(syntiant_ndp120_nn_metadata_t));
    config.nn_idx = meta->parser.cur_subgraph;
    config.layer_idx = meta->parser.cur_layer;
    config.header.version = NDP120_DNN_METADATA_VERSION_V2;
    switch (meta->parser.parsed) {
        case NNM_PARSE_HEADER:
            config.set = SYNTIANT_NDP120_SET_META_HEADER;
            config.header.nn_num = (uint8_t)meta->initial_meta.nn_num;
            config.header.dnn_mode = meta->initial_meta.dnn_mode;
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_BASE_META:
            config.set = SYNTIANT_NDP120_SET_META_BASE;
            config.meta_base.num_layers = (uint8_t)meta->base_meta.num_layers;
            config.meta_base.is_cached = (uint8_t)meta->base_meta.is_cached;
            config.meta_base.num_inputs = (uint8_t)meta->base_meta.num_inputs;
            config.meta_base.num_outputs = (uint8_t)meta->base_meta.num_outputs;

            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_INDICES_LAYER:
            config.set = SYNTIANT_NDP120_SET_LAYER_INDICES;
            for (i = 0; i < NDP120_DNN_MAX_NUM_LAYERS; i++) {
                config.layer_indices[i] = (uint8_t)meta->layers[i];
            }
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_INDICES_INPUT:
            config.set = SYNTIANT_NDP120_SET_INPUT_INDICES;
            for (i = 0; i < NDP120_DNN_MAX_NUM_INPUTS; i++) {
                config.input_indices[i] = (uint8_t)meta->inputs[i];
            }
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_INDICES_OUTPUT:
            config.set = SYNTIANT_NDP120_SET_OUTPUT_INDICES;
            for (i = 0; i < NDP120_DNN_MAX_NUM_OUTPUTS; i++) {
                config.output_indices[i] = (uint8_t)meta->outputs[i];
            }
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;

        case NNM_PARSE_CACHE_INST:
            config.set = SYNTIANT_NDP120_SET_CACHE_INST;
            memcpy(&config.cache_inst, &meta->cache_params,
                sizeof(meta->cache_params));
            s = syntiant_ndp120_config_nn_metadata_no_sync(ndp, &config);
            if (s) goto error;
            break;
    }

    if (pstate->mode == PACKAGE_MODE_TAG_START) {
        ndp120->nn_metadata_loaded = 1;
        maybe_send_prepare(ndp);
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_mcu_orch_params(struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    uint32_t *node_count = pstate->metadata.scratch_metadata;
    syntiant_mcu_orch_v1_t * parsed_node;
    syntiant_ndp120_mcu_orch_t config;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-read actions */
        *node_count = 0;
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_node = &(pstate->data.mcu_orchestrator.v1);
        (*node_count) += 1;

        memset(&config, 0, sizeof(config));
        config.set = SYNTIANT_NDP120_SET_NODE;
        config.node_idx = *node_count - 1;
        config.id = parsed_node->id;
        config.flow_id = parsed_node->flow_id;
        config.status = parsed_node->status;
        config.type = parsed_node->type;
        config.action = parsed_node->action;
        config.num_inputs = parsed_node->num_inputs;
        config.num_outputs = parsed_node->num_outputs;
        memcpy(&config.input_edges, &parsed_node->input_edges,
            sizeof(parsed_node->input_edges));
        memcpy(&config.next_ids, &parsed_node->next_ids,
            sizeof(parsed_node->next_ids));

        s = syntiant_ndp120_config_mcu_orchestrator_no_sync(ndp, &config);
        if (s) goto error;

#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nNode:\n");
        DEBUG_PRINTF("\tid: %d\n", parsed_node->id);
        DEBUG_PRINTF("\tflow id: %d\n", parsed_node->flow_id);
        DEBUG_PRINTF("\tstatus: %d\n", parsed_node->status);
        DEBUG_PRINTF("\ttype: %d\n", parsed_node->type);
        DEBUG_PRINTF("\taction: %d\n", parsed_node->action);
        DEBUG_PRINTF("\tnum inputs: %d\n", parsed_node->num_inputs);
        DEBUG_PRINTF("\tnum outputs: %d\n", parsed_node->num_outputs);
#endif
    }

    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        /* post-read actions */
        memset(&config, 0, sizeof(config));
        config.set = SYNTIANT_NDP120_SET_NUM_NODES | SYNTIANT_NDP120_SET_FLOWMAP;
        config.num_nodes = (uint8_t) (*node_count & 0xff);
        config.flowmap = 0x1;    /* SET0 is the starting set */
        /* DEBUG_PRINTF("\nNum nodes: %d\n", *node_count); */

        s = syntiant_ndp120_config_mcu_orchestrator_no_sync(ndp, &config);
        if (s) goto error;
    }

error:
    return s;
}

static int
syntiant_ndp120_pkg_parse_mcu_orch_params_v3(struct syntiant_ndp_device_s *ndp)
{
    int s = NDP_MB_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    uint32_t *node_count = pstate->metadata.scratch_metadata;
    syntiant_mcu_orch_v3_t * parsed_node;
    syntiant_ndp120_mcu_orch_t config;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* pre-read actions */
        *node_count = 0;
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {

        parsed_node = &(pstate->data.mcu_orchestrator.v3);
        (*node_count) += 1;

        memset(&config, 0, sizeof(config));
        config.set = SYNTIANT_NDP120_SET_NODE;
        config.node_idx = *node_count - 1;
        config.id = parsed_node->node_id;
        config.flow_id = parsed_node->dsp_flow_id;
        config.status = parsed_node->status;
        config.type = parsed_node->type;
        config.action = parsed_node->action;
        config.num_inputs = parsed_node->num_inputs;
        config.num_outputs = parsed_node->num_outputs;
        config.preroll = parsed_node->preroll;
        config.orch_timeout = parsed_node->orch_timeout;

        memcpy(&config.input_edges, &parsed_node->input_edges,
            sizeof(parsed_node->input_edges));
        memcpy(&config.next_ids, &parsed_node->next_ids,
            sizeof(parsed_node->next_ids));

        s = syntiant_ndp120_config_mcu_orchestrator_no_sync(ndp, &config);
        if (s) goto error;

#if SYNTIANT_NDP120_DEBUG
        DEBUG_PRINTF("\nNode:\n");
        DEBUG_PRINTF("\tid: %d\n", parsed_node->node_id);
        DEBUG_PRINTF("\tflow id: %d\n", parsed_node->dsp_flow_id);
        DEBUG_PRINTF("\tstatus: %d\n", parsed_node->status);
        DEBUG_PRINTF("\ttype: %d\n", parsed_node->type);
        DEBUG_PRINTF("\taction: %d\n", parsed_node->action);
        DEBUG_PRINTF("\tnum inputs: %d\n", parsed_node->num_inputs);
        DEBUG_PRINTF("\tnum outputs: %d\n", parsed_node->num_outputs);
#endif
    }

    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        /* post-read actions */
        memset(&config, 0, sizeof(config));
        config.set = SYNTIANT_NDP120_SET_NUM_NODES | SYNTIANT_NDP120_SET_FLOWMAP;
        config.num_nodes = (uint8_t) (*node_count & 0xff);
        config.flowmap = 0x1;    /* SET0 is the starting set */
        /* DEBUG_PRINTF("\nNum nodes: %d\n", *node_count); */

        s = syntiant_ndp120_config_mcu_orchestrator_no_sync(ndp, &config);
        if (s) goto error;
    }

error:
    return s;
}

static int
ndp120_pkg_validate_pdm_cfg_v1(struct ndp120_pdm_params_v1 *pdm)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned base_mic_id, i;

    DEBUG_PRINTF(
        ">>> PDM: aud_id=%u mode=%u clk_mode=%u clk_rate=%u sample_rate=%u "
        "bypass_farrow=%u frphasestep=%#x dcstaticremval=%u dcremovalmode=%u\n",
        pdm->aud_id, pdm->mode, pdm->clk_mode, pdm->clk_rate, pdm->sample_rate,
        pdm->bypass_farrow, pdm->frphasestep, pdm->dcstaticremval, pdm->dcremovalmode);

    if (pdm->aud_id > NDP120_TLV_PDM_V1_AUD1 ||
        pdm->mode > NDP120_TLV_PDM_V1_AUD_MODE_STEREO ||
        pdm->clk_mode > NDP120_TLV_PDM_V1_AUD_CLK_MODE_DUAL_INTERNAL ||
        pdm->clk_rate < NDP120_TLV_PDM_V1_AUD_CLK_RATE_LOW ||
        pdm->clk_rate > NDP120_TLV_PDM_V1_AUD_CLK_RATE_HIGH ||
        pdm->sample_rate < NDP120_TLV_PDM_V1_AUD_SAMPLE_RATE_LOW ||
        pdm->sample_rate > NDP120_TLV_PDM_V1_AUD_SAMPLE_RATE_HIGH ||
        pdm->frphasestep > NDP120_TLV_PDM_V1_FARROW_PHASE_STEP_HIGH ||
        pdm->dcremovalmode > NDP120_TLV_PDM_V1_REMOVAL_MODE_ON ||
        pdm->bypass_farrow > NDP120_TLV_PDM_V1_FARROW_BYPASS_ENABLE) {
        s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
        SYNTIANT_PRINTF("pdm: config param out of range\n");
        goto done;
    }

    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct pdm_params_channel_v1 *ch = pdm->channel + i;
        DEBUG_PRINTF(
            ">>> [%u] mic=%u num_delay=%u decimation(inshift=%u outshift=%u) "
            "agc(shiftdir=%u shiftcnt=%u finegrainmul=%#x) zcgainchange=%u\n",
            i, base_mic_id+i, ch->numdelay, ch->decimation_inshift, ch->decimation_outshift,
            ch->agcshiftdir, ch->agcshiftcnt, ch->agcfinegrainmul, ch->zcgainchange);
        if (ch->decimation_inshift > NDP120_TLV_PDM_V1_DECIMATION_INSHIFT_HIGH ||
            ch->decimation_outshift > NDP120_TLV_PDM_V1_DECIMATION_OUTSHIFT_HIGH ||
            ch->numdelay > NDP120_TLV_PDM_V1_AUD_NUM_DELAY_MAX ||
            ch->agcshiftdir > NDP120_TLV_PDM_V1_GAIN_SHIFT_RIGHT ||
            ch->agcshiftcnt > NDP120_TLV_PDM_V1_GAIN_SHIFT_CNT_MAX ||
            ch->zcgainchange > NDP120_TLV_PDM_V1_GAIN_CHANGE_TRUE ||
            ch->agcfinegrainmul > NDP120_TLV_PDM_V1_GAIN_FINEGRAIN_MUL_HIGH) {
            s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
            SYNTIANT_PRINTF("pdm: config channel %u param out of range\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_pdm_cfg_params_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    struct ndp120_pdm_params_v1 *pdm = &pstate->data.pdm_cfg.tlv85;
    syntiant_ndp120_config_pdm_t config_pdm;
    syntiant_ndp120_config_farrow_t config_farrow;
    syntiant_ndp120_config_decimation_t config_decimation;
    syntiant_ndp120_config_gain_t config_gain;
    unsigned base_mic_id, i;

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    s = ndp120_pkg_validate_pdm_cfg_v1(pdm);
    if (s) {
        goto done;
    }

    memset(&config_pdm, 0, sizeof(config_pdm));
    config_pdm.interface = pdm->aud_id;
    config_pdm.sample_rate = pdm->sample_rate;
    config_pdm.pdm_rate = pdm->clk_rate;
    config_pdm.clk_mode = pdm->clk_mode;
    config_pdm.mode = pdm->mode;
    config_pdm.set =
        SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_MODE;
    s = syntiant_ndp120_config_pdm_no_sync(ndp, &config_pdm);
    if (s) {
        SYNTIANT_PRINTF("pdm: config pdm fail\n");
        goto done;
    }

    memset(&config_farrow, 0, sizeof(config_farrow));
    config_farrow.interface = pdm->aud_id;
    config_farrow.bypass = pdm->bypass_farrow;
    config_farrow.phase_step = pdm->frphasestep;
    config_farrow.set =
        SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS |
        SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP;
    s = syntiant_ndp120_config_farrow_no_sync(ndp, &config_farrow);
    if (s) {
        SYNTIANT_PRINTF("pdm: config farrow fail\n");
        goto done;
    }

    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct pdm_params_channel_v1 *ch = pdm->channel + i;

        memset(&config_decimation, 0, sizeof(config_decimation));
        config_decimation.mic = base_mic_id + i;
        config_decimation.inshift = ch->decimation_inshift;
        config_decimation.outshift = ch->decimation_outshift;
        config_decimation.set =
            SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT |
            SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT;
        s = syntiant_ndp120_config_decimation_no_sync(ndp, &config_decimation);
        if (s) {
            SYNTIANT_PRINTF("pdm: config decimation channel %u fail\n", i);
            goto done;
        }

        memset(&config_gain, 0, sizeof(config_gain));
        config_gain.mic = base_mic_id + i;
        config_gain.dcremovalmode = pdm->dcremovalmode;
        config_gain.dcstaticremval = pdm->dcstaticremval;
        config_gain.agcshiftdir = ch->agcshiftdir;
        config_gain.agcshiftcnt = ch->agcshiftcnt;
        config_gain.agcfinegrainmul = ch->agcfinegrainmul;
        config_gain.zcgainchange = ch->zcgainchange;
        config_gain.set =
            SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_DCSTATICREMVAL;
        s = syntiant_ndp120_config_gain_no_sync(ndp, &config_gain);
        if (s) {
            SYNTIANT_PRINTF("pdm: config gain channel %u fail\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
ndp120_pkg_validate_pdm_cfg_v2(struct ndp120_pdm_params_v2 *pdm)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned base_mic_id, i;

    DEBUG_PRINTF(
        ">>> PDM: aud_id=%u mode=%u clk_mode=%u clk_rate=%u sample_rate=%u "
        "bypass_farrow=%u frphasestep=%#x\n",
        pdm->aud_id, pdm->mode, pdm->clk_mode, pdm->clk_rate, pdm->sample_rate,
        pdm->bypass_farrow, pdm->frphasestep);

    if (pdm->aud_id > NDP120_TLV_PDM_V2_AUD1 ||
        pdm->mode > NDP120_TLV_PDM_V2_AUD_MODE_STEREO ||
        pdm->clk_mode > NDP120_TLV_PDM_V2_AUD_CLK_MODE_DUAL_INTERNAL ||
        pdm->clk_rate < NDP120_TLV_PDM_V2_AUD_CLK_RATE_LOW ||
        pdm->clk_rate > NDP120_TLV_PDM_V2_AUD_CLK_RATE_HIGH ||
        pdm->bypass_farrow > NDP120_TLV_PDM_V2_FARROW_BYPASS_ENABLE ||
        pdm->sample_rate < NDP120_TLV_PDM_V2_AUD_SAMPLE_RATE_LOW ||
        pdm->sample_rate > NDP120_TLV_PDM_V2_AUD_SAMPLE_RATE_HIGH ||
        pdm->frphasestep > NDP120_TLV_PDM_V2_FARROW_PHASE_STEP_HIGH) {
        s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V2;
        SYNTIANT_PRINTF("pdm: config param out of range\n");
        goto done;
    }

    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct pdm_params_channel_v2 *ch = pdm->channel + i;
        DEBUG_PRINTF(
            ">>> [%u] mic=%u num_delay=%u decimation(inshift=%u outshift=%u) "
            "agc(shiftdir=%u shiftcnt=%u finegrainmul=%#x) zcgainchange=%u "
            "dcstaticremval=%u dcremovalmode=%u bwcoeff(a1=%u a2=%u b0=%u b1=%u b2=%u)\n",
            i, base_mic_id+i, ch->numdelay, ch->decimation_inshift, ch->decimation_outshift,
            ch->agcshiftdir, ch->agcshiftcnt, ch->agcfinegrainmul, ch->zcgainchange,
            ch->dcstaticremval, ch->dcremovalmode, ch->a1, ch->a2, ch->b0, ch->b1, ch->b2);
        if (ch->decimation_inshift > NDP120_TLV_PDM_V2_DECIMATION_INSHIFT_HIGH ||
            ch->decimation_outshift > NDP120_TLV_PDM_V2_DECIMATION_OUTSHIFT_HIGH ||
            ch->numdelay > NDP120_TLV_PDM_V2_AUD_NUM_DELAY_MAX ||
            ch->agcshiftdir > NDP120_TLV_PDM_V2_GAIN_SHIFT_RIGHT ||
            ch->agcshiftcnt > NDP120_TLV_PDM_V2_GAIN_SHIFT_CNT_MAX ||
            ch->zcgainchange > NDP120_TLV_PDM_V2_GAIN_CHANGE_TRUE ||
            ch->agcfinegrainmul > NDP120_TLV_PDM_V2_GAIN_FINEGRAIN_MUL_HIGH ||
            ch->a1 > NDP120_TLV_PDM_V2_BW_COEFF_HIGH ||
            ch->a2 > NDP120_TLV_PDM_V2_BW_COEFF_HIGH ||
            ch->b0 > NDP120_TLV_PDM_V2_BW_COEFF_HIGH ||
            ch->b1 > NDP120_TLV_PDM_V2_BW_COEFF_HIGH ||
            ch->b2 > NDP120_TLV_PDM_V2_BW_COEFF_HIGH) {
            s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V2;
            SYNTIANT_PRINTF("pdm: config channel %u param out of range\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_pdm_cfg_params_v2(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    struct ndp120_pdm_params_v2 *pdm = &pstate->data.pdm_cfg.tlv96;
    syntiant_ndp120_config_pdm_t config_pdm;
    syntiant_ndp120_config_farrow_t config_farrow;
    syntiant_ndp120_config_decimation_t config_decimation;
    syntiant_ndp120_config_gain_t config_gain;
    syntiant_ndp120_config_bwcoef_t bwcoef;
    unsigned base_mic_id, i;

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    s = ndp120_pkg_validate_pdm_cfg_v2(pdm);
    if (s) {
        goto done;
    }

    memset(&config_pdm, 0, sizeof(config_pdm));
    config_pdm.interface = pdm->aud_id;
    config_pdm.sample_rate = pdm->sample_rate;
    config_pdm.pdm_rate = pdm->clk_rate;
    config_pdm.clk_mode = pdm->clk_mode;
    config_pdm.mode = pdm->mode;
    config_pdm.set =
        SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE |
        SYNTIANT_NDP120_CONFIG_SET_PDM_MODE;
    s = syntiant_ndp120_config_pdm_no_sync(ndp, &config_pdm);
    if (s) {
        SYNTIANT_PRINTF("pdm: config pdm fail\n");
        goto done;
    }

    memset(&config_farrow, 0, sizeof(config_farrow));
    config_farrow.interface = pdm->aud_id;
    config_farrow.bypass = pdm->bypass_farrow;
    config_farrow.phase_step = pdm->frphasestep;
    config_farrow.set =
        SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS |
        SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP;
    s = syntiant_ndp120_config_farrow_no_sync(ndp, &config_farrow);
    if (s) {
        SYNTIANT_PRINTF("pdm: config farrow fail\n");
        goto done;
    }

    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct pdm_params_channel_v2 *ch = pdm->channel + i;

        memset(&config_decimation, 0, sizeof(config_decimation));
        config_decimation.mic = base_mic_id + i;
        config_decimation.inshift = ch->decimation_inshift;
        config_decimation.outshift = ch->decimation_outshift;
        config_decimation.set =
            SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT |
            SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT;
        s = syntiant_ndp120_config_decimation_no_sync(ndp, &config_decimation);
        if (s) {
            SYNTIANT_PRINTF("pdm: config decimation channel %u fail\n", i);
            goto done;
        }

        memset(&config_gain, 0, sizeof(config_gain));
        config_gain.mic = base_mic_id + i;
        config_gain.dcremovalmode = ch->dcremovalmode;
        config_gain.dcstaticremval = ch->dcstaticremval;
        config_gain.agcshiftdir = ch->agcshiftdir;
        config_gain.agcshiftcnt = ch->agcshiftcnt;
        config_gain.agcfinegrainmul = ch->agcfinegrainmul;
        config_gain.zcgainchange = ch->zcgainchange;
        config_gain.set =
            SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_DCSTATICREMVAL;
        s = syntiant_ndp120_config_gain_no_sync(ndp, &config_gain);
        if (s) {
            SYNTIANT_PRINTF("pdm: config gain channel %u fail\n", i);
            goto done;
        }

        memset(&bwcoef, 0, sizeof(bwcoef));
        bwcoef.mic = base_mic_id + i;
        bwcoef.a1 = ch->a1;
        bwcoef.a2 = ch->a2;
        bwcoef.b0 = ch->b0;
        bwcoef.b1 = ch->b1;
        bwcoef.b2 = ch->b2;
        bwcoef.set =
            SYNTIANT_NDP120_CONFIG_SET_BWCOEF_A1 |
            SYNTIANT_NDP120_CONFIG_SET_BWCOEF_A2 |
            SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B0 |
            SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B1 |
            SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B2;
        s = syntiant_ndp120_config_bwcoef_no_sync(ndp, &bwcoef);
        if (s) {
            SYNTIANT_PRINTF("pdm: config bwcoef channel %u fail\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_pin_mux_cfg_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_pin_mux_config_t *pin_mux = &pstate->data.pin_mux_cfg;
    syntiant_ndp115_pinmux_config_t pcfg;

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    pcfg.aud1_config = pin_mux->aud1_pin_sel;
    pcfg.mspi_config = pin_mux->mspi_pin_sel;
    s = syntiant_ndp115_config_pinmux(ndp, &pcfg);
    if (s) {
        goto done;
    }

done:
    return s;
}

static int ndp120_pkg_validate_i2s_in_cfg(
    const struct ndp120_i2s_in_params *pdm)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned base_mic_id, i;

    DEBUG_PRINTF(
        ">>> I2S_IN: aud_id=%u edge=%u mode=%u clk_rate=%u framesize=%u "
        "samplesize=%u msb_index=%u delayed_flop_sensitivity=%u packed=%u "
        "leftchenable=%u rightchenable=%u bypass_farrow=%u frphasestep=%#x\n",
        pdm->aud_id, pdm->edge, pdm->mode, pdm->clk_rate, pdm->framesize,
        pdm->samplesize, pdm->msb_index, pdm->delayed_flop_sensitivity, pdm->packed,
        pdm->leftchenable, pdm->rightchenable, pdm->bypass_farrow, pdm->frphasestep);

    if (pdm->aud_id > NDP120_TLV_I2S_IN_AUD1 ||
        pdm->edge > NDP120_TLV_I2S_IN_I2S_EDGE_NEG ||
        pdm->mode > NDP120_TLV_I2S_IN_I2S_MODE_PDM ||
        pdm->clk_rate < NDP120_TLV_IS2_IN_SAMPLE_RATE_LOW ||
        pdm->clk_rate > NDP120_TLV_IS2_IN_SAMPLE_RATE_HIGH ||
        pdm->framesize < NDP120_TLV_I2S_IN_FRAMESIZE_LOW ||
        pdm->framesize > NDP120_TLV_I2S_IN_FRAMESIZE_HIGH ||
        pdm->samplesize < NDP120_TLV_I2S_IN_SAMPLESIZE_LOW ||
        pdm->samplesize > NDP120_TLV_I2S_IN_SAMPLESIZE_HIGH ||
        pdm->msb_index < NDP120_TLV_I2S_IN_MSB_INDEX_LOW ||
        pdm->msb_index > NDP120_TLV_I2S_IN_MSB_INDEX_HIGH ||
        pdm->delayed_flop_sensitivity > NDP120_TLV_I2S_IN_I2S_DELAYED_SENS_DELAYED ||
        pdm->packed > NDP120_TLV_I2S_IN_I2S_PACKED_MONO ||
        pdm->leftchenable > NDP120_TLV_I2S_IN_I2S_ENABLE_TRUE ||
        pdm->rightchenable > NDP120_TLV_I2S_IN_I2S_ENABLE_TRUE ||
        pdm->bypass_farrow >NDP120_TLV_I2S_IN_FARROW_BYPASS_ENABLE ||
        pdm->frphasestep > NDP120_TLV_I2S_IN_FARROW_PHASE_STEP_HIGH) {
        s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
        SYNTIANT_PRINTF("i2s_in: config param out of range\n");
        goto done;
    }

    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        const struct i2s_in_channel *ch = pdm->channel + i;
        DEBUG_PRINTF(
            ">>> [%u] mic=%u num_delay=%u agcshiftdir=%u "
            "agcshiftcnt=%u agcfinegrainmul=%#x zcgainchange=%u\n",
            i, base_mic_id+i, ch->numdelay, ch->agcshiftdir,
            ch->agcshiftcnt, ch->agcfinegrainmul, ch->zcgainchange);
        if (ch->numdelay > NDP120_TLV_I2S_IN_AUD_NUM_DELAY_MAX ||
            ch->agcshiftdir > NDP120_TLV_I2S_IN_GAIN_SHIFT_RIGHT ||
            ch->agcshiftcnt > NDP120_TLV_I2S_IN_GAIN_SHIFT_CNT_MAX ||
            ch->zcgainchange > NDP120_TLV_I2S_IN_GAIN_CHANGE_TRUE ||
            ch->agcfinegrainmul > NDP120_TLV_I2S_IN_GAIN_FINEGRAIN_MUL_HIGH) {
            s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
            SYNTIANT_PRINTF("i2s_in: config channel %u param out of range\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_i2s_in_cfg_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    struct ndp120_i2s_in_params *pdm = &pstate->data.pdm_cfg.tlv86;
    syntiant_ndp120_config_farrow_t config_farrow;
    /* syntiant_ndp120_config_gain_t config_gain;  */
    syntiant_ndp120_config_i2s_t config_i2s;
    /* unsigned base_mic_id, i; */

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    s = ndp120_pkg_validate_i2s_in_cfg(pdm);
    if (s) {
        goto done;
    }

    memset(&config_i2s, 0, sizeof(config_i2s));
    config_i2s.interface = pdm->aud_id;
    config_i2s.freq = pdm->clk_rate; /* sample_rate */
    config_i2s.mode = pdm->mode;
    config_i2s.framesize = pdm->framesize;
    config_i2s.samplesize = pdm->samplesize;
    config_i2s.msb_index = pdm->msb_index;
    config_i2s.packed = pdm->packed;
    config_i2s.edge = pdm->edge;
    config_i2s.delayed_flop_sensitivity = pdm->delayed_flop_sensitivity;
    config_i2s.leftchenable = pdm->leftchenable;
    config_i2s.rightchenable = pdm->rightchenable;
    config_i2s.set =
        SYNTIANT_NDP120_CONFIG_SET_I2S_MODE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_FRAMESIZE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_SAMPLESIZE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_MSB_INDEX |
        SYNTIANT_NDP120_CONFIG_SET_I2S_PACKED |
        SYNTIANT_NDP120_CONFIG_SET_I2S_EDGE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY |
        SYNTIANT_NDP120_CONFIG_SET_I2S_LEFTCHENABLE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_RIGHTCHENABLE;
    s = syntiant_ndp120_config_i2s_no_sync(ndp, &config_i2s);
    if (s) {
        SYNTIANT_PRINTF("i2s_in: config i2s fail\n");
        goto done;
    }

    memset(&config_farrow, 0, sizeof(config_farrow));
    config_farrow.interface = pdm->aud_id;
    config_farrow.bypass = pdm->bypass_farrow;
    config_farrow.phase_step = pdm->frphasestep;
    config_farrow.set =
        SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS |
        SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP;
    s = syntiant_ndp120_config_farrow_no_sync(ndp, &config_farrow);
    if (s) {
        SYNTIANT_PRINTF("i2s_in: config farrow fail\n");
        goto done;
    }
/*
    base_mic_id = pdm->aud_id * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct i2s_in_channel *ch = pdm->channel + i;

        memset(&config_gain, 0, sizeof(config_gain));
        config_gain.mic = base_mic_id + i;
        config_gain.dcremovalmode = 0;
        config_gain.agcshiftdir = ch->agcshiftdir;
        config_gain.agcshiftcnt = ch->agcshiftcnt;
        config_gain.agcfinegrainmul = ch->agcfinegrainmul;
        config_gain.zcgainchange = ch->zcgainchange;
        config_gain.set =
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE;
        s = syntiant_ndp120_config_gain_no_sync(ndp, &config_gain);
        if (s) {
            SYNTIANT_PRINTF("i2s_in: config gain channel %u fail\n", i);
            goto done;
        }
    }
*/
done:
    return s;
}

static int ndp120_pkg_validate_i2s_out_cfg(
    const struct ndp120_i2s_out_params *pdm)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned base_mic_id, i;

    DEBUG_PRINTF(
        ">>> I2S_OUT: clk_rate=%u edge=%u mode=%u framesize=%u samplesize=%u "
        "msb_index=%u delayed_flop_sensitivity=%u packed=%u "
        "leftchenable=%u rightchenable=%u aud2_out_mode=%u aud2clkoutneeded=%u "
        "audio_out_fs_ext_enable=%u aud2_src_type=%u aud2_src_param=%u "
        "bypass_farrow=%u frphasestep=%#x\n",
        pdm->clk_rate, pdm->edge, pdm->mode, pdm->framesize, pdm->samplesize,
        pdm->msb_index, pdm->delayed_flop_sensitivity, pdm->packed,
        pdm->leftchenable, pdm->rightchenable, pdm->aud2_out_mode,
        pdm->aud2clkoutneeded, pdm->audio_out_fs_ext_enable, pdm->aud2_src_type,
        pdm->aud2_src_param, pdm->bypass_farrow, pdm->frphasestep);

    if (pdm->clk_rate < NDP120_TLV_IS2_OUT_SAMPLE_RATE_LOW ||
        pdm->clk_rate > NDP120_TLV_IS2_OUT_SAMPLE_RATE_HIGH ||
        pdm->edge > NDP120_TLV_IS2_OUT_I2S_EDGE_NEG ||
        pdm->mode > NDP120_TLV_IS2_OUT_I2S_MODE_PDM ||
        pdm->framesize < NDP120_TLV_I2S_OUT_FRAMESIZE_LOW ||
        pdm->framesize > NDP120_TLV_I2S_OUT_FRAMESIZE_HIGH ||
        pdm->samplesize < NDP120_TLV_I2S_OUT_SAMPLESIZE_LOW ||
        pdm->samplesize > NDP120_TLV_I2S_OUT_SAMPLESIZE_HIGH ||
        pdm->msb_index < NDP120_TLV_I2S_OUT_MSB_INDEX_LOW ||
        pdm->msb_index > NDP120_TLV_I2S_OUT_MSB_INDEX_HIGH ||
        pdm->delayed_flop_sensitivity > NDP120_TLV_IS2_OUT_I2S_DELAYED_SENS_DELAYED ||
        pdm->packed > NDP120_TLV_IS2_OUT_I2S_PACKED_MONO ||
        pdm->leftchenable > NDP120_TLV_IS2_OUT_I2S_ENABLE_TRUE ||
        pdm->rightchenable > NDP120_TLV_IS2_OUT_I2S_ENABLE_TRUE ||
        pdm->aud2_out_mode > NDP120_TLV_IS2_OUT_I2S_AUD2_OUT_MODE_TARGET ||
        pdm->aud2clkoutneeded > NDP120_TLV_IS2_OUT_I2S_ENABLE_TRUE ||
        pdm->audio_out_fs_ext_enable > NDP120_TLV_IS2_OUT_I2S_ENABLE_TRUE ||
        pdm->aud2_src_type > NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_TYPE_FUNCTION ||
        pdm->aud2_src_param < NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_PARAM_LOW ||
        pdm->aud2_src_param > NDP120_TLV_IS2_OUT_I2S_AUD2_SRC_PARAM_HIGH ||
        pdm->bypass_farrow >NDP120_TLV_IS2_OUT_FARROW_BYPASS_ENABLE ||
        pdm->frphasestep > NDP120_TLV_I2S_OUT_FARROW_PHASE_STEP_HIGH) {
        s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
        SYNTIANT_PRINTF("i2s_out: config param out of range\n");
        goto done;
    }

    base_mic_id = NDP120_TLV_I2S_OUT_AUD2 * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        const struct i2s_out_channel *ch = pdm->channel + i;
        DEBUG_PRINTF(
            ">>> [%u]: mic=%u num_delay=%u agcshiftdir=%u "
            "agcshiftcnt=%u agcfinegrainmul=%#x zcgainchange=%u\n",
            i, base_mic_id+i, ch->numdelay, ch->agcshiftdir,
            ch->agcshiftcnt, ch->agcfinegrainmul, ch->zcgainchange);
        if (ch->numdelay > NDP120_TLV_I2S_OUT_AUD_NUM_DELAY_MAX ||
            ch->agcshiftdir > NDP120_TLV_IS2_OUT_GAIN_SHIFT_RIGHT ||
            ch->agcshiftcnt > NDP120_TLV_I2S_OUT_GAIN_SHIFT_CNT_MAX ||
            ch->zcgainchange > NDP120_TLV_IS2_OUT_GAIN_CHANGE_TRUE ||
            ch->agcfinegrainmul > NDP120_TLV_I2S_OUT_GAIN_FINEGRAIN_MUL_HIGH) {
            s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
            SYNTIANT_PRINTF("i2s_out: config channel %u param out of range\n", i);
            goto done;
        }
    }

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_i2s_out_cfg_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    struct ndp120_i2s_out_params *pdm = &pstate->data.pdm_cfg.tlv87;
    syntiant_ndp120_config_farrow_t config_farrow;
    /* syntiant_ndp120_config_gain_t config_gain; */
    syntiant_ndp120_config_i2s_t config_i2s;
    /* unsigned base_mic_id, i; */

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    s = ndp120_pkg_validate_i2s_out_cfg(pdm);
    if (s) {
        goto done;
    }

    memset(&config_i2s, 0, sizeof(config_i2s));
    config_i2s.interface = NDP120_TLV_I2S_OUT_AUD2;
    config_i2s.freq = pdm->clk_rate; /* sample_rate */
    config_i2s.mode = pdm->mode;
    config_i2s.framesize = pdm->framesize;
    config_i2s.samplesize = pdm->samplesize;
    config_i2s.msb_index = pdm->msb_index;
    config_i2s.packed = pdm->packed;
    config_i2s.edge = pdm->edge;
    config_i2s.delayed_flop_sensitivity = pdm->delayed_flop_sensitivity;
    config_i2s.audio_out_fs_ext_enable = pdm->audio_out_fs_ext_enable;
    config_i2s.leftchenable = pdm->leftchenable;
    config_i2s.rightchenable = pdm->rightchenable;
    config_i2s.aud2clkoutneeded = pdm->aud2clkoutneeded;
    config_i2s.aud2_out_mode = pdm->aud2_out_mode;
    config_i2s.aud2_src_type = pdm->aud2_src_type;
    config_i2s.aud2_src_param = pdm->aud2_src_param;
    config_i2s.set =
        SYNTIANT_NDP120_CONFIG_SET_I2S_MODE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_FRAMESIZE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_SAMPLESIZE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_MSB_INDEX |
        SYNTIANT_NDP120_CONFIG_SET_I2S_PACKED |
        SYNTIANT_NDP120_CONFIG_SET_I2S_EDGE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY |
        SYNTIANT_NDP120_CONFIG_SET_I2S_AUDIO_OUT_FS_EXT_ENABLE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_LEFTCHENABLE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_RIGHTCHENABLE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2CLKOUTNEEDED |
        SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_OUT_MODE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_TYPE |
        SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_PARAM;
    s = syntiant_ndp120_config_i2s_no_sync(ndp, &config_i2s);
    if (s) {
        SYNTIANT_PRINTF("i2s_out: config i2s out fail\n");
        goto done;
    }

    memset(&config_farrow, 0, sizeof(config_farrow));
    config_farrow.interface = NDP120_TLV_I2S_OUT_AUD2;
    config_farrow.bypass = pdm->bypass_farrow;
    config_farrow.phase_step = pdm->frphasestep;
    config_farrow.set =
        SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS |
        SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP;
    s = syntiant_ndp120_config_farrow_no_sync(ndp, &config_farrow);
    if (s) {
        SYNTIANT_PRINTF("i2s_out: config farrow fail\n");
        goto done;
    }
/*
    base_mic_id = NDP120_TLV_I2S_OUT_AUD2 * SYNTIANT_PDM_CHANNEL_COUNT;
    for (i = 0; i < SYNTIANT_PDM_CHANNEL_COUNT; i++) {
        struct i2s_out_channel *ch = pdm->channel + i;

        memset(&config_gain, 0, sizeof(config_gain));
        config_gain.mic = base_mic_id + i;
        config_gain.agcshiftdir = ch->agcshiftdir;
        config_gain.agcshiftcnt = ch->agcshiftcnt;
        config_gain.agcfinegrainmul = ch->agcfinegrainmul;
        config_gain.zcgainchange = ch->zcgainchange;
        config_gain.set =
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL |
            SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE;
        s = syntiant_ndp120_config_gain_no_sync(ndp, &config_gain);
        if (s) {
            SYNTIANT_PRINTF("i2s_out: config gain channel %u fail\n", i);
            goto done;
        }
    }
*/
done:
    return s;
}

static int ndp120_pkg_validate_dsp_sync_cfg(
    const struct ndp120_dsp_sync_params *pdm)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    DEBUG_PRINTF(
        ">>> DSP_SYNC: sync_mode=%u sync_ref=%u sync_adj=%u "
        "sync_offset=%u sync_chan_delay_ticks=%u\n",
        pdm->sync_mode, pdm->sync_ref, pdm->sync_adj,
        pdm->sync_offset, pdm->sync_chan_delay_ticks);

    if (pdm->sync_mode > NDP120_TLV_DSP_SYNC_DSP_SYNC_MODE_FULL) {
        s = SYNTIANT_PACKAGE_ERROR_PDM_CFG_V1;
        SYNTIANT_PRINTF("dsp_sync: config param out of range\n");
    }

    return s;
}

static int
syntiant_ndp120_pkg_parse_dsp_sync_cfg_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    struct ndp120_dsp_sync_params *pdm = &pstate->data.pdm_cfg.tlv88;

    /* post-read actions */
    if (pstate->partially_read_length != *(uint32_t *)pstate->length) {
        goto done;
    }

    s = ndp120_pkg_validate_dsp_sync_cfg(pdm);

done:
    return s;
}

static int
syntiant_ndp120_pkg_parse_sensor_cfg_v1(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_sensor_config_v1_t *sensor =
        &sensor_cfg_saved[syntiant_sensor_cfg_state.num_sensor_cfg].v1;
    /*syntiant_sensor_config_v1_t *sensor = &pstate->data.sensor_cfg.v1;*/
    syntiant_ndp120_config_i2c_t config;
    syntiant_sensor_ifc_transaction_t *ifc_transaction;
    syntiant_ndp120_config_sensor_t sconfig;
    uint8_t out[SYNTIANT_PACKAGE_NUM_IFC_BYTES_MAX+1];
    uint8_t in[SYNTIANT_PACKAGE_NUM_IFC_BYTES_MAX];
    unsigned int i, j, outlen, inlen;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        goto done;
    } else if (pstate->expected_bytes != 0 ||
               sensor->parser.parsed != SENC_PARSE_DONE) {
        goto done;
    }

    DEBUG_PRINTF(
        "sensor_cfg: num=%u id=%u num_gpio_used=%u gpio_role[0]=%u "
        "intrpt[0]=%u gpio_role[1]=%u intrpt[1]=%u ifc=%u ifc_addr=0x%x "
        "ifc_clk_rate=%u num_axes=%u axis_tank=0x%x axis_input=0x%x\n",
        sensor->base_sensor_config.sensor_num,
        sensor->base_sensor_config.sensor_id,
        sensor->base_sensor_config.num_gpio_used,
        sensor->base_sensor_config.sensor_gpio[0].gpio_role,
        sensor->base_sensor_config.sensor_gpio[0].intrpt,
        sensor->base_sensor_config.sensor_gpio[1].gpio_role,
        sensor->base_sensor_config.sensor_gpio[1].intrpt,
        sensor->base_sensor_config.ifc, sensor->base_sensor_config.ifc_addr,
        sensor->base_sensor_config.ifc_clk_rate,
        sensor->base_sensor_config.num_axes,
        sensor->base_sensor_config.axis_tank,
        sensor->base_sensor_config.axis_input);

    DEBUG_PRINTF(
        "sensor_cfg: num_param=%u param_value=",
        sensor->base_sensor_config.num_param);

    for (i = 0; i < SYNTIANT_CONFIG_SENSOR_PARAMETERS; i++) {
        DEBUG_PRINTF("0x%x, ", sensor->base_sensor_config.param_value[i]);
    }
    DEBUG_PRINTF("/nnum_ifc_transactions=%u\n",
                 sensor->base_sensor_config.num_ifc_transactions);

    /* check for validity of sensor data from TLV */
    if (sensor->base_sensor_config.sensor_num >=
            SYNTIANT_PACKAGE_NUM_SENSORS_MAX ||
        sensor->base_sensor_config.num_gpio_used == 0 ||
        sensor->base_sensor_config.num_gpio_used >
            SYNTIANT_PACKAGE_NUM_GPIO_PINS_MAX ||
        sensor->base_sensor_config.sensor_gpio[0].intrpt >=
            NDP120_FW_GPIO_MAX ||
        sensor->base_sensor_config.sensor_gpio[1].intrpt >=
            NDP120_FW_GPIO_MAX ||
        sensor->base_sensor_config.sensor_gpio[0].gpio_role >
            NDP_FW_GPIO_ROLE_INTLH ||
        sensor->base_sensor_config.sensor_gpio[1].gpio_role >
            NDP_FW_GPIO_ROLE_INTLH ||
        sensor->base_sensor_config.num_axes > SYNTIANT_CONFIG_SENSOR_AXES ||
        sensor->base_sensor_config.num_param >
            SYNTIANT_CONFIG_SENSOR_PARAMETERS ||
        sensor->base_sensor_config.num_ifc_transactions >
            SYNTIANT_PACKAGE_IFC_TRANSACTIONS_MAX) {
        s = SYNTIANT_PACKAGE_ERROR_SENSOR_CONFIG_V1;
        SYNTIANT_PRINTF("sensor: config param out of range\n");
        goto done;
    }

    /* config i2c bus and speed */
    config.set = SYNTIANT_NDP120_CONFIG_SET_I2C_ENABLE |
                 SYNTIANT_NDP120_CONFIG_SET_I2C_CLK_RATE;
    config.get = 0;
    config.enable = 1;
    config.clk_rate = sensor->base_sensor_config.ifc_clk_rate;
    s = syntiant_ndp120_config_i2c(ndp, &config);
    if (s) {
        SYNTIANT_PRINTF("sensor: config i2c fail\n");
        goto done;
    }

    /* config i2c operations */
    for (i = 0; i < sensor->base_sensor_config.num_ifc_transactions; i++) {
        ifc_transaction = &sensor->sensor_trans[i];
        memset((void*)in, 0, sizeof(in));
        if (!ifc_transaction->ifc_transaction_type) { /* write */
            inlen = 0;
            outlen = (unsigned int) ifc_transaction->num_ifc_bytes + 1;
            out[0] = ifc_transaction->ifc_start_addr;
            for (j = 0; j < ifc_transaction->num_ifc_bytes; j++) {
                out[j+1] = ifc_transaction->ifc_bytes[j];
            }
        } else { /* read */
            inlen = (unsigned int) ifc_transaction->num_ifc_bytes;
            outlen = 1;
            out[0] = ifc_transaction->ifc_start_addr;
        }
        s = syntiant_serial_transfer_no_sync(ndp, sensor->base_sensor_config.ifc,
                                     sensor->base_sensor_config.ifc_addr,
                                     out, outlen, in, inlen, 0);
        if (s) {
            SYNTIANT_PRINTF("sensor: i2c transaction %d fail\n", i);
            goto done;
        }
        /* verify content for read operation */
        if (ifc_transaction->ifc_transaction_type) {
            for (j = 0; j < ifc_transaction->num_ifc_bytes; j++) {
                if (in[j] != ifc_transaction->ifc_bytes[j]) {
                    SYNTIANT_PRINTF("sensor: i2c read transaction %d fail\n",
                                    i);
                    goto done;
                }
            }
        }
    }

    /* config sensor, get existing sensor config first */
    memset(&sconfig, 0, sizeof(sconfig));
    s = syntiant_ndp_config_get_sensor(ndp, &sconfig);
    if (s) {
        SYNTIANT_PRINTF("sensor: get sensor config fail\n");
        goto done;
    }

    sconfig.set = 1;

    /* config sensor id */
    sconfig.sensor_id[sensor->base_sensor_config.sensor_num] =
        sensor->base_sensor_config.sensor_id;

    /* config gpio interrupt pins. need at least 1 gpio pin for interrupt */
    /* set true/false flag */
    sconfig.sensor_int[sensor->base_sensor_config.sensor_num] = 1;
    sconfig.sensor_int_gpio[sensor->base_sensor_config.sensor_num] =
        sensor->base_sensor_config.sensor_gpio[0].intrpt;

    /* set true/false flag */
    sconfig.sensor_int1[sensor->base_sensor_config.sensor_num] =
        (sensor->base_sensor_config.num_gpio_used == 2) ? 1: 0;
    sconfig.sensor_int_gpio1[sensor->base_sensor_config.sensor_num] =
        sensor->base_sensor_config.sensor_gpio[1].intrpt;
    sconfig.gpio_role[sensor->base_sensor_config.sensor_gpio[0].intrpt] =
        sensor->base_sensor_config.sensor_gpio[0].gpio_role;
    sconfig.gpio_role[sensor->base_sensor_config.sensor_gpio[1].intrpt] =
        sensor->base_sensor_config.sensor_gpio[1].gpio_role;

    /* config sensor interface */
    sconfig.sensor_interface[sensor->base_sensor_config.sensor_num] =
        sensor->base_sensor_config.ifc;
    sconfig.sensor_interface_address[sensor->base_sensor_config.sensor_num] =
        sensor->base_sensor_config.ifc_addr;

    /* config axis related data */
    sconfig.sensor_axes[sensor->base_sensor_config.sensor_num] =
        (uint8_t) sensor->base_sensor_config.num_axes;
    for (i = 0; i < SYNTIANT_CONFIG_SENSOR_AXES; i++) {
        sconfig.sensor_axis_tank[sensor->base_sensor_config.sensor_num][i] =
            (sensor->base_sensor_config.axis_tank >> i) & 0x1;
        sconfig.sensor_axis_input[sensor->base_sensor_config.sensor_num][i] =
            (sensor->base_sensor_config.axis_input >> i) & 0x1;
    }

    /* config parameters related data */
    for (i = 0; i < SYNTIANT_CONFIG_SENSOR_PARAMETERS; i++) {
        sconfig.sensor_parameter[sensor->base_sensor_config.sensor_num][i] =
            sensor->base_sensor_config.param_value[i];
    }

    s = syntiant_ndp_config_set_sensor(ndp, &sconfig);
    if (s) {
        SYNTIANT_PRINTF("sensor: config sensor fail\n");
        goto done;
    }

done:
    return s;
}

int syntiant_ndp120_finalize_parse_actions(void)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    while (syntiant_sensor_cfg_state.num_sensor_cfg) {
        syntiant_sensor_cfg_state.num_sensor_cfg--;
        s = syntiant_ndp120_pkg_parse_sensor_cfg_v1(syntiant_sensor_cfg_state.ndp);
        if (s) goto error;
    }

error:
    return s;
}

const char *syntiant_ndp120_get_config_devtype(
    uint32_t device_id, enum pbi_version_chip_type_t_ *chip_type)
{
    const char *s = "unknown";
    if (!chip_type) {
      goto out;
    }
    *chip_type = 0;

    switch (device_id) {
        case 0x30: /* fall through */
        case 0x31: /* fall through */
        case 0x32: /* fall through */
        case 0x33:
            s = "NDP120-A0 ES";
            *chip_type = PBI_CHIP_TYPE_NDP120_B0;
            break;

        case 0x34: /* fall through */
        case 0x35: /* fall through */
        case 0x36: /* fall through */
        case 0x37: /* fall through */
        case 0x38: /* fall through */
        case 0x39: /* fall through */
        case 0x3A: /* fall through */
        case 0x3B: /* fall through */
        case 0x3C: /* fall through */
        case 0x3D: /* fall through */
        case 0x3E: /* fall through */
        case 0x3F:
            s = "NDP120-B0";
            *chip_type = PBI_CHIP_TYPE_NDP120_B0;
            break;

        case 0x40: /* fall through */
        case 0x41: /* fall through */
        case 0x42: /* fall through */
        case 0x43:
            s = "NDP200-A0";
            *chip_type = PBI_CHIP_TYPE_NDP200_A0;
            break;

        case 0x48: /* fall through */
        case 0x49: /* fall through */
        case 0x4A: /* fall through */
        case 0x4B:
            s = "NDP115-A0";
            *chip_type = PBI_CHIP_TYPE_NDP115_A0;
            break;
    }

out:
    return s;
}

static int
syntiant_ndp120_access_config(struct syntiant_ndp_device_s *ndp,
                              struct syntiant_ndp_config_s *config) {
    uint32_t length_in_scratch, len, valid, labels_len, firmware_version_len,
        parameters_version_len, pkg_version_len, dsp_firmware_version_len;
    int s = SYNTIANT_NDP_ERROR_NONE;
    char *buffer = NULL;

    DEBUG_PRINTF("--> syntiant_ndp120_get_config...\n");

    /* check valid bits */
    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;

    labels_len = config->labels_len;
    firmware_version_len = config->firmware_version_len;
    parameters_version_len = config->parameters_version_len;
    pkg_version_len = config->pkg_version_len;
    dsp_firmware_version_len = config->dsp_firmware_version_len;

    config->labels_len = 0;
    config->firmware_version_len = 0;
    config->parameters_version_len = 0;
    config->pkg_version_len = 0;
    config->dsp_firmware_version_len = 0;

    if ((labels_len & 3) || (firmware_version_len & 3) || (parameters_version_len & 3) ||
        (pkg_version_len & 3) || (dsp_firmware_version_len & 3)) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!valid) goto error;

    /* labels */
    if (valid & SYNTIANT_CONFIG_LABELS_VALID) {
        s = ndp_mcu_read(SCRATCH_VARIABLE_ADX(label_size), &length_in_scratch);
        if (s) goto error;

        buffer = config->labels;
        len = ndp_umin(labels_len, length_in_scratch);
        if (buffer && len > 0) {
            s = ndp_mcu_read_block(SCRATCH_VARIABLE_ADX(labels), buffer, len);
            if (s) goto error;
        }
        config->labels_len = length_in_scratch;
    }

    /*fw version*/
    if (valid & SYNTIANT_CONFIG_FW_VERSION_VALID) {
        s = syntiant_ndp120_read(ndp, 1, SCRATCH_VARIABLE_ADX(fw_version_size), &length_in_scratch);
        if (s) goto error;
        if (length_in_scratch < NDP120_ILIB_SCRATCH_LENGTH) {
            buffer = config->firmware_version;
            len = ndp_umin(firmware_version_len, length_in_scratch);
            if (buffer && 0 < len) {
                s = ndp_mcu_read_block(SCRATCH_VARIABLE_ADX(fw_version) , buffer, len);
                if (s) goto error;
            }
            config->firmware_version_len = length_in_scratch;
        }
    }

    /*dsp fw version*/
    if (valid & SYNTIANT_CONFIG_DSP_FW_VERSION_VALID) {
        s = ndp_mcu_read(SCRATCH_VARIABLE_ADX(dsp_fw_version_size), &length_in_scratch);
        if (s) goto error;
        if (length_in_scratch < NDP120_ILIB_SCRATCH_LENGTH) {
            buffer = config->dsp_firmware_version;
            len = ndp_umin(dsp_firmware_version_len, length_in_scratch);
            if (buffer && 0 < len) {
                s = ndp_mcu_read_block(SCRATCH_VARIABLE_ADX(dsp_fw_version), buffer, len);
                if (s) goto error;
            }
            config->dsp_firmware_version_len = length_in_scratch;
        }
    }

    /*pkg version*/
    if (valid & SYNTIANT_CONFIG_PKG_VERSION_VALID) {
        s = ndp_mcu_read(SCRATCH_VARIABLE_ADX(pkg_version_size), &length_in_scratch);
        if (s) goto error;
        if (length_in_scratch < NDP120_ILIB_SCRATCH_LENGTH) {
            buffer = config->pkg_version;

            len = ndp_umin(pkg_version_len, length_in_scratch);
            if (buffer && 0 < len) {
                s = ndp_mcu_read_block (SCRATCH_VARIABLE_ADX(pkg_version), buffer, len);
                if (s) goto error;
            }
            config->pkg_version_len = length_in_scratch;
        }
    }

error:
    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    return s;
}

static int
syntiant_ndp120_verify_configuration(struct syntiant_ndp_device_s *ndp){

    int s;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    struct syntiant_ndp_config_s config = {0};

    s = syntiant_ndp120_access_config(ndp, &config);
    if (s) {
        DEBUG_PRINTF("access_config failed s=%d\n", s);
        goto error;
    }

    /* label length */
    if (config.labels_len &&
        config.labels_len != ndp120->labels_len) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("labels config.len=%d, internal_len=%d failed\n",
            config.labels_len, ndp120->labels_len);
        goto error;
    }

    /* firmware version length */
    if (config.firmware_version_len &&
        config.firmware_version_len != ndp120->fwver_len) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("fw_ver");
        DEBUG_PRINTF("firmware_version config.len=%d, internal_len=%d failed\n",
            config.firmware_version_len, ndp120->fwver_len);
        goto error;
    }

    /* dsp firmware version length */
    if (config.dsp_firmware_version_len &&
        config.dsp_firmware_version_len != ndp120->dspfwver_len) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("dsp_firmware_version_ config.len=%d, internal_len=%d failed\n",
            config.dsp_firmware_version_len, ndp120->dspfwver_len);
        goto error;
    }

    /* params version length */
    if (config.parameters_version_len &&
        config.parameters_version_len != ndp120->paramver_len) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("parameters_version config.len=%d, internal_len=%d failed\n",
            config.parameters_version_len, ndp120->paramver_len);
        goto error;
    }

    /* pkg version length */
    if (config.pkg_version_len &&
        config.pkg_version_len != ndp120->pkgver_len) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("pkg_version config.len=%d, internal_len=%d failed\n",
            config.pkg_version_len, ndp120->pkgver_len);
        goto error;
    }

error:
    if (s) {
        DEBUG_PRINTF("syntiant_ndp_120_verify_config failed s=%d\n", s);
    }
    return s;
}

static int
syntiant_ndp120_pkg_parse_board_calibration_params_v1_v2_tlv(
    struct syntiant_ndp_device_s *ndp, int padded)
{

    int s = SYNTIANT_NDP_ERROR_NONE;
    (void)ndp;
    (void)padded;
#if 0
    syntiant_ndp120_config_t ndp120_config;
    int i = 0;

    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    uint8_t *dnn_input = padded ?
                        &pstate->data.board_params.board_params_v2.dnn_input :
                        &pstate->data.board_params.board_params_v1.dnn_input;

    if (pstate->mode == PACKAGE_MODE_TAG_START &&
        pstate->partially_read_length == *(uint32_t*)pstate->length) {

        /* translate dnn inputs */
        switch (*dnn_input) {
        case SYNTIANT_PACKAGE_DNN_INPUT_NONE:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_NONE;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_PDM0:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM0;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_PDM1:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM1;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_PDM_SUM:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM_SUM;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_I2S_LEFT:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_LEFT;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_I2S_RIGHT:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_RIGHT;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_I2S_SUM:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_SUM;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_I2S_MONO:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_MONO;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_I2S_DIRECT:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_DIRECT;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_SPI:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_SPI;
            break;

        case SYNTIANT_PACKAGE_DNN_INPUT_SPI_DIRECT:
            *dnn_input = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_SPI_DIRECT;
            break;

        default:
            s = SYNTIANT_NDP_ERROR_PACKAGE;
            goto error;
        }
        memset(&ndp120_config, 0, sizeof(syntiant_ndp120_config_misc_t));
        ndp120_config.dnn_input = *dnn_input;

        ndp120_config.input_clock_rate = padded ?
            pstate->data.board_params.board_params_v2.input_clock_rate:
            pstate->data.board_params.board_params_v1.input_clock_rate;

        ndp120_config.pdm_clock_rate = padded ?
                pstate->data.board_params.board_params_v2.pdm_clock_rate:
                pstate->data.board_params.board_params_v1.pdm_clock_rate;

        for (i = 0 ; i < 2; i++){
            ndp120_config.pdm_dc_offset[i] = (padded ?
                pstate->data.board_params.board_params_v2.pdm_dc_offset[i]:
                pstate->data.board_params.board_params_v1.pdm_dc_offset[i]);
        }

        ndp120_config.pdm_clock_ndp = padded ?
            pstate->data.board_params.board_params_v2.pdm_clock_ndp:
            pstate->data.board_params.board_params_v1.pdm_clock_ndp;

        ndp120_config.power_offset = padded ?
            pstate->data.board_params.board_params_v2.power_offset:
            pstate->data.board_params.board_params_v1.power_offset;

        ndp120_config.preemphasis_exponent = padded ?
            pstate->data.board_params.board_params_v2.preemphasis_exponent:
            pstate->data.board_params.board_params_v1.preemphasis_exponent;

        for (i = 0 ; i < 2; i++){
            ndp120_config.pdm_in_shift[i] = (padded ?
                pstate->data.board_params.board_params_v2.cal_pdm_in_shift[i]:
                pstate->data.board_params.board_params_v1.cal_pdm_in_shift[i]);
        }

        for (i = 0 ; i < 2; i++){
            ndp120_config.pdm_out_shift[i] = (padded ?
                pstate->data.board_params.board_params_v2.cal_pdm_out_shift[i]:
                pstate->data.board_params.board_params_v1.cal_pdm_out_shift[i]);
        }

        ndp120_config.power_scale_exponent = padded ?
            pstate->data.board_params.board_params_v2.power_scale_exponent:
            pstate->data.board_params.board_params_v1.power_scale_exponent;

        ndp120_config.agc_on = padded ?
            pstate->data.board_params.board_params_v2.agc:
            pstate->data.board_params.board_params_v1.agc;

        for (i = 0 ; i < NDP120_CONFIG_SET_MISC_MAX_BINS ; i++){
            ndp120_config.filter_eq[i] = padded ?
                pstate->data.board_params.board_params_v2.equalizer[i]:
                pstate->data.board_params.board_params_v1.equalizer[i];
        }

        ndp120_config.set = (NDP120_CONFIG_SET_MISC_SET_INPUT |
            NDP120_CONFIG_SET_MISC_INPUT_CLOCK_RATE |
            NDP120_CONFIG_SET_MISC_PDM_CLOCK_RATE |
            NDP120_CONFIG_SET_MISC_PDM_DC_OFFSET |
            NDP120_CONFIG_SET_MISC_PDM_CLOCK_NDP |
            NDP120_CONFIG_SET_MISC_POWER_OFFSET |
            NDP120_CONFIG_SET_MISC_PREEMPHASIS_EXPONENT |
            NDP120_CONFIG_SET_MISC_PDM_IN_SHIFT |
            NDP120_CONFIG_SET_MISC_PDM_OUT_SHIFT |
            NDP120_CONFIG_SET_MISC_POWER_SCALE_EXPONENT |
            NDP120_CONFIG_SET_MISC_AGC_ON |
            NDP120_CONFIG_SET_MISC_FILTER_EQ );
        s = syntiant_ndp120_config_no_sync(ndp, &ndp120_config);
        if (s) goto error;
    }
error:
#endif
    return s;
}

static int
syntiant_ndp120_pkg_parse_nn_params_v1_tlv(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_multisegment_state_t *ms_state = &ndp->d.ndp120.ms_state;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        /* NN pre-write code */
        memset(ms_state, 0, sizeof(syntiant_ndp120_multisegment_state_t));
    } else if (pstate->value_mode == PACKAGE_MODE_VALID_PARTIAL_VALUE) {
        s = process_multisegment(ndp);
        if (s) goto error;
    }

    /* post-write actions */
    if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
        ndp120->nn_params_loaded = 1;
        s = maybe_send_prepare(ndp);
        if (s) goto error;
    }
error:
    return s;
}

static int
syntiant_ndp120_parse_tag_values(struct syntiant_ndp_device_s *ndp)
{
#if SYNTIANT_NDP120_DEBUG
    static uint32_t last_tag = 0xFFFFFFFF;
#endif
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    int aligned_value = 0;

#if SYNTIANT_NDP120_DEBUG
    if (last_tag != *(uint32_t *)pstate->tag) {
        DEBUG_PRINTF("processing tag %d\n", *(uint32_t *)pstate->tag);
        last_tag = *(uint32_t *)pstate->tag;
    }
#endif

    switch (*(uint32_t *)pstate->tag) {

    case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
        s = syntiant_ndp120_pkg_pbi_version_tlv(ndp);
        break;

    case TAG_FIRMWARE_VERSION_STRING_V1:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
    case TAG_NN_VERSION_STRING_V1:
    case TAG_PACKAGE_VERSION_STRING:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
    case TAG_FIRMWARE_VERSION_STRING_V2:
    case TAG_PACKAGE_VERSION_STRING_V2:
        s = syntiant_ndp120_pkg_parse_version_string_v1_tlv(ndp);
        break;

    case TAG_BOARD_CALIBRATION_PARAMS_V2:
        aligned_value = 1;
        fallthrough;
    case TAG_BOARD_CALIBRATION_PARAMS_V1:
        s = syntiant_ndp120_pkg_parse_board_calibration_params_v1_v2_tlv(
            ndp, aligned_value);
        break;

    case TAG_NN_LABELS_V1:
    case TAG_NN_LABELS_V3:
        s = syntiant_ndp120_pkg_parse_nn_labels_v1_v3_tlv(ndp);
        break;

    case TAG_NN_PHP_COLLECTION_V7:
        s = syntiant_ndp120_pkg_parse_php_collection_v7(ndp);
        break;

    case TAG_NDP120_B0_NN_PARAMETERS_V1:
    case TAG_NDP115_A0_NN_PARAMETERS_V1:
        s = syntiant_ndp120_pkg_parse_nn_params_v1_tlv(ndp);
        break;

    case TAG_NDP120_B0_MCU_ORCHESTRATOR_V1:
        s = syntiant_ndp120_pkg_parse_mcu_orch_params(ndp);
        break;

    case TAG_NDP120_B0_MCU_ORCHESTRATOR_V3:
        s = syntiant_ndp120_pkg_parse_mcu_orch_params_v3(ndp);
        break;

    case TAG_CORE2_PDM_CFG_V1:
        s = syntiant_ndp120_pkg_parse_pdm_cfg_params_v1(ndp);
        break;

    case TAG_CORE2_PDM_CFG_V2:
        s = syntiant_ndp120_pkg_parse_pdm_cfg_params_v2(ndp);
        break;

    case TAG_CORE2_I2S_IN_CFG_V1:
        s = syntiant_ndp120_pkg_parse_i2s_in_cfg_v1(ndp);
        break;

    case TAG_CORE2_I2S_OUT_CFG_V1:
        s = syntiant_ndp120_pkg_parse_i2s_out_cfg_v1(ndp);
        break;

    case TAG_CORE2_DSP_SYNC_V1:
        s = syntiant_ndp120_pkg_parse_dsp_sync_cfg_v1(ndp);
        break;

    /* Firmware */
    case TAG_FIRMWARE:
        ERROR_PRINTF(
            "warning: found 'TAG_FIRMWARE' tag, not a CORE2 tag");
        s = SYNTIANT_NDP_ERROR_PACKAGE;
        break;

    case TAG_UNENCRYPTED_FIRMWARE_V1:
        s = syntiant_ndp120_pkg_parse_firmware_tlv(ndp);
        break;

    case TAG_NDP120_B0_DSP_FIRMWARE_V1:
    case TAG_NDP115_A0_DSP_FIRMWARE_V1:
        s = syntiant_ndp120_pkg_parse_dsp_firmware_tlv(ndp);
        break;

    case TAG_CORE2_SENSOR_CONFIG_V1:
        if (!syntiant_sensor_cfg_state.ndp) {
            syntiant_sensor_cfg_state.ndp = ndp;
        }
        /*s = syntiant_ndp120_pkg_parse_sensor_cfg_v1(ndp);*/
        break;

    case TAG_NDP115_A0_PIN_MUX_CONFIG_V1:
        s = syntiant_ndp120_pkg_parse_pin_mux_cfg_v1(ndp);
        break;

    case TAG_NDP120_B0_NN_METADATA:
        s = syntiant_ndp120_pkg_parse_nn_metadata(ndp);
        break;

    case TAG_NDP120_B0_NN_METADATA_V2:
        s = syntiant_ndp120_pkg_parse_nn_metadata_v2(ndp);
        break;

    case TAG_CORE2_DSP_ALGO_ATTACH_V1:
        s = syntiant_ndp120_pkg_parse_algo_attach_v1(ndp);
        break;
    case TAG_NDP120_B0_DSP_FLOW_COLLECTION_V1:
        s = syntiant_ndp120_pkg_parse_dsp_flow_collection_v1(ndp);
        break;

    case TAG_NDP120_B0_DSP_FLOW_COLLECTION_V2:
        s = syntiant_ndp120_pkg_parse_dsp_flow_collection_v2(ndp);
        break;

    case TAG_NDP120_B0_DSP_FE_CONFIG_V3:
        s = syntiant_ndp120_pkg_parse_dsp_fe_config_v3(ndp);
        break;

    case TAG_NDP120_B0_SOFTMAX_V1:
        s = syntiant_ndp120_pkg_parse_nn_softmax(ndp);
        break;

    case TAG_NDP120_B0_DNN_POWER_CFG_V1:
        s = syntiant_ndp120_pkg_parse_dnn_power_config_v1(ndp);
        break;

    case TAG_PACKAGERLIB_VERSION_STRING:
        /* irrelevant to ilib. Used for determining packager version. */
        break;

    case TAG_INTERRUPT_AUTO_CLEAR:
        s = syntiant_ndp120_pkg_parse_auto_clear_interrupt_config(ndp);
        break;

    case TAG_CORE2_DSP_MCU_SECONDARY_VALIDATION_V1:
        s = syntiant_ndp120_pkg_parse_dsp_mcu_secondary_valildation_v1(ndp);
        break;

    case TAG_CORE2_MCU_SECONDARY_VALIDATION_V1:
        s = syntiant_ndp120_pkg_parse_mcu_secondary_valildation_v1(ndp);
        break;

    case TAG_CORE2_MIC_SETTINGS:
        s = syntiant_ndp120_pkg_parse_mic_settings(ndp);
        break;

    case TAG_DSP_ALGO_PARAMS_V1:
        s = syntiant_ndp120_pkg_parse_dsp_algo_params_v1(ndp);
        break;
    case TAG_HEADER:
        /* intentionally ignore */
        break;

    case TAG_CHECKSUM:
        /* intentionally ignore */
        break;

    case TAG_OBJECTDECODER_PH_PARAMETERS_V1:
        /* used to store vision post-processing parameters, intentionally ignore here */
        break;

    default:
        s = SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV;
        DEBUG_PRINTF("%s:%d Unknown TLV %d\n", __FILE__, __LINE__,
            *(uint32_t *)pstate->tag);
        break;
    }
    return s;
}

int syntiant_ndp120_test_function(struct syntiant_ndp_device_s *ndp)
{
    uint32_t valid;

    return syntiant_ndp120_scratch_get_valid(ndp, &valid);
}

/**
 * @brief function to store the PBI version information to MCU memory.
 */
static int
syntiant_ndp120_write_pbi_version(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pbi_version_params_t pbi_ver;
    uint32_t tag = TAG_PACKAGE_BINARY_INTERFACE_VERSION;

    syntiant_ndp_ilib_pbi_version(ndp, &pbi_ver);
    s = syntiant_ndp120_pkg_parse_store_version(ndp, tag,
            (uint8_t *) &pbi_ver, (uint32_t) sizeof(pbi_ver));

    return s;
}

/**
 * @brief function to verify that the PBI version tag was present.
 * The PBI validation done for all Core types will validate the PBI version
 * information for this tag.
 */
static int syntiant_ndp120_validate_pbi_tag(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (!ndp->pstate.package_pbi_version.chip_type) {
        /* The PBI version tag is missing */
        s = SYNTIANT_NDP_ERROR_PACKAGE_PBI_TAG;
    }

    return s;
}

static int ndp120_dsp_memory_init(struct syntiant_ndp_device_s *ndp)
{
    uint32_t i;
    uint32_t data_32 = 0;
    uint32_t dummy[16] = {0}; /* keep dummy size low to minimize stack space usage */
    uint32_t addr, size;
    int s = SYNTIANT_NDP_ERROR_NONE;

    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_EN_DSP_CLK_INSERT(data_32,1); /* enable DSP clock */
    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_RUN_STALL_INSERT(data_32,1);
    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_BRESET_INSERT(data_32,1);
    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_DRESET_INSERT(data_32,1);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_CFG, data_32);
    if (s) goto error;

    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_DRESET_MASK_INSERT(data_32,0);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_CFG, data_32);
    if (s) goto error;

    data_32 = NDP120_CHIP_CONFIG_DSP_CFG_BRESET_MASK_INSERT(data_32,0);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_CFG, data_32);
    if (s) goto error;
    /* turn on DSP memories */
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_MEM_PGEN, 0x00);
    if (s) goto error;

    /* set scratch ram to 0 */
    addr = ndp_get_scratch_address(ndp);
    size = NDP120_ILIB_SCRATCH_LENGTH/sizeof(dummy);
    for (i = 0; i < size; i++) {
        s = syntiant_ndp120_write_block(ndp, 1, addr, (void*) dummy,
            sizeof(dummy));
        addr += (uint32_t) sizeof(dummy);
        if (s) goto error;
    }
error:
    return s;
}

/* ========================= */
/* EXPORTED DRIVER FUNCTIONS */
/* ========================= */

int syntiant_ndp120_init(
    struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp_init_mode_e init_mode)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data;
    int secure;
    uint8_t spi_data;
    uint8_t auth;
    struct ndp120_dsp_state dstate = {0};
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    int mbout = 0;

    /* remember the ndp boot_from_flash flag */
    int boot_from_flash = syntiant_ndp_boot_from_flash_flag(ndp, 0);

    /* this memset clears the ndp boot_from_flash flag */
    memset(ndp120, 0, sizeof(*ndp120));

    ndp120->init_mode = init_mode;

    DEBUG_PRINTF("->> syntiant_ndp120_init (init_mode=%d)...\n", init_mode);

    switch (init_mode) {
    case SYNTIANT_NDP_INIT_MODE_NO_TOUCH:
        break;

    case SYNTIANT_NDP_INIT_MODE_RESTART:
        s = syntiant_ndp120_get_secured_status_no_sync(ndp, &secure);
        if (s) goto error;
        if (secure) {
            /* Resync to device state */
            s = restore_mb_state(ndp);
            goto error;
        }
        s = ndp_spi_read(NDP120_SPI_INTCTL, &spi_data);
        if (s) goto error;
        spi_data = NDP120_SPI_INTCTL_WM_INTEN_INSERT(spi_data, 1);
        spi_data = NDP120_SPI_INTCTL_MBOUT_INTEN_INSERT(spi_data, 1);
        spi_data = NDP120_SPI_INTCTL_MBIN_INTEN_INSERT(spi_data, 1);
        s = ndp_spi_write(NDP120_SPI_INTCTL, spi_data);
        if (s) goto error;

        s = ndp_spi_read(NDP120_SPI_CFG, &spi_data);
        if (s) goto error;
        spi_data = NDP120_SPI_CFG_INTEN_INSERT(spi_data, 1);
        s = ndp_spi_write(NDP120_SPI_CFG, spi_data);
        if (s) goto error;

        /* hacky stuff to allow things to "work"

           It's arguable that init -N restart shouldn't
           call sync() or set ndp->init to 0
           ¯\_(ツ)_/¯ */

        s = ndp->iif.unsync(ndp->iif.d); /* allow processing */
        if (s) goto error;
        ndp->init = 1;

        /* end hacky stuff */

        /* clear any outstanding notifications */
        /*
           intentionally not checking return codes here,
           it's probably ok if these fail */

        s = syntiant_ndp120_poll(ndp, &data, 1);
        if (s) goto error;
        mailbox_reset_state(ndp);

        /* since mcu code will always compare mbin to previous mbin to ensure
           toggle bit is changed before further processing, we get previous
           mbin's toggle bit so that next mbin sent from Host to mcu will
           have correct toggle bit */

        s = syntiant_ndp120_read(ndp, 0, NDP120_SPI_MBIN, (uint32_t *)&spi_data);
        /* printf("RESTART mode: mbin=0x%x\n", spi_data); */
        if (s) goto error;

        ndp120->mb_state.mbin_seq = spi_data & 0x80;

        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_MIADDR, NULL);
        if (s) goto error;
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_ADX_LOWER, NULL);
        if (s) goto error;

        if (ndp120->dsp_fw_state_addr) {
            uint32_t adx;
            adx = ndp120->dsp_fw_state_addr +
                (uint32_t) offsetof(ndp120_dsp_fw_base_t,
                    config.func_samp_size_bytes);
            s = ndp_mcu_read(adx, &data);
            if (s) goto error;
            ndp120->max_feature_len = data;
            /* DEBUG_PRINTF("syntiant_ndp120_init: max_feature_len=%d\n",
             * ndp120->max_feature_len); */
            adx = ndp120->dsp_fw_state_addr +
              (uint32_t) offsetof(ndp120_dsp_fw_base_t, dsp_state);
            s = ndp_mcu_read(adx, &dstate);
            if (s) goto error;
            if (dstate.device_state & SYNTIANT_NDP120_DSP_DNN_CONFIGURED) {
              ndp120->nn_metadata_loaded = ndp120->nn_params_loaded = 1;
            }
        }
        else {
            /* default to ndp120 feature extractor length */
            ndp120->max_feature_len = 1028;
        }

        if (ndp120->mcu_fw_state_addr) {
            uint32_t addr;
            addr = ndp120->mcu_fw_state_addr +
                (uint32_t) offsetof(struct ndp120_fw_state_s, match_producer);
            s = ndp_mcu_read(addr, &ndp120->match_producer);
            if (s) goto error;
            memcpy(ndp120->match_consumer, ndp120->match_producer,
                    sizeof(ndp120->match_consumer));
        }
        break;

    case SYNTIANT_NDP_INIT_MODE_RESET:
    case SYNTIANT_NDP_INIT_MODE_RESET_ENFORCE_COMPATIBILITY:
        do {
            /* full POR */
            s = ndp_spi_read(NDP120_SPI_CTL, &data);
            if (s) goto error;
            data = NDP120_SPI_CTL_PORSTN_MASK_INSERT(data, 0);
            s = ndp_spi_write(NDP120_SPI_CTL, data);
            if (s) goto error;
            while(1) {
                s = ndp_spi_read(NDP120_SPI_ID0, &spi_data);
                if (s) goto error;
                if (spi_data != 0xff) break;
            }
            int n = 100;
            while (n--) {
                s = ndp_spi_read(NDP120_SPI_INTSTS, &spi_data);
                if (s) goto error;
                if (spi_data) break;
            }
            mbout = 0;
            s = ndp_spi_read(NDP120_SPI_MBOUT, &mbout);
            if (s) goto error;
        } while(mbout == 0);

        /* Drive the interrupt line output active high interrupts */
        spi_data = NDP120_SPI_CFG_INTEN(1) | NDP120_SPI_CFG_INTNEG(0);
        s = ndp_spi_write(NDP120_SPI_CFG, spi_data);
        if (s) goto error;

        ndp120->nn_params_loaded = 0;
        ndp120->nn_metadata_loaded = 0;

        s = syntiant_ndp120_is_auth(ndp, &auth);
        if (s) goto error;
        if (auth) {
            /* Don't touch the device address of the secured device */
            goto error;
        }
#ifdef EMULATE_SECURE
        goto error;
#endif

        if (boot_from_flash) goto error;

        s = ndp120_dsp_memory_init(ndp);
        if (s) {
            DEBUG_PRINTF("Couldn't power on HiFi3 memory\n");
            goto error;
        }

        /* Write the PBI version information to MCU scratch memory */
        s = syntiant_ndp120_write_pbi_version(ndp);
        if (s) goto error;

        s = syntiant_ndp120_scratch_set_valid(ndp, 0);
        if (s) goto error;

        break;

    default:
        s = SYNTIANT_NDP_ERROR_ARG;
        break;
    }

error:
    ndp->init = 0;
    return s;
}

int syntiant_ndp120_uninit(
    struct syntiant_ndp_device_s *ndp, enum syntiant_ndp_init_mode_e init_mode)
{
    (void)*ndp;
    (void)init_mode;
    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_ndp120_op_size(
    struct syntiant_ndp_device_s *ndp, int mcu, unsigned int *size)
{
    (void)*ndp;
    *size = OP_SIZE(mcu);
    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_ndp120_interrupts(struct syntiant_ndp_device_s *ndp, int *causes)
{
    int s;
    uint8_t intctl, intctl_old;
    int cs = *causes;

    if (cs > SYNTIANT_NDP_INTERRUPT_ALL
        && cs != SYNTIANT_NDP_INTERRUPT_DEFAULT) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
    s = syntiant_ndp120_read(ndp, 0, NDP120_SPI_INTCTL, &intctl);
    if (s) goto error;

    intctl_old = intctl;

    if (cs >= 0) {
        if (cs == SYNTIANT_NDP_INTERRUPT_DEFAULT) {
            intctl = NDP120_SPI_INTCTL_MBIN_INTEN(1)
                | NDP120_SPI_INTCTL_WM_INTEN(1)
                | NDP120_SPI_INTCTL_MBOUT_INTEN(1)
                | NDP120_SPI_INTCTL_AE_INTEN(1)
                | NDP120_SPI_INTCTL_RF_INTEN(1);
        } else {
            intctl = (uint8_t)
                (NDP120_SPI_INTCTL_MBIN_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_MAILBOX_IN))
                | NDP120_SPI_INTCTL_MBOUT_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_MAILBOX_OUT))
                | NDP120_SPI_INTCTL_DNN_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_DNN_FRAME))
                | NDP120_SPI_INTCTL_FEATURE_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_FEATURE))
                | NDP120_SPI_INTCTL_AE_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_ADDRESS_ERROR))
                | NDP120_SPI_INTCTL_WM_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_WATER_MARK))
                | NDP120_SPI_INTCTL_RF_INTEN
                (!!(cs & SYNTIANT_NDP_INTERRUPT_SPI_READ_FAILURE)));
        }
        s = syntiant_ndp120_write(ndp, 0, NDP120_SPI_INTCTL, intctl);
        if (s) goto error;
    }

    cs =
        ((NDP120_SPI_INTCTL_MBIN_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_MAILBOX_IN : 0)
        | ((NDP120_SPI_INTCTL_MBOUT_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_MAILBOX_OUT : 0)
        | ((NDP120_SPI_INTCTL_DNN_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_DNN_FRAME : 0)
        | ((NDP120_SPI_INTCTL_FEATURE_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_FEATURE : 0)
        | ((NDP120_SPI_INTCTL_AE_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_ADDRESS_ERROR : 0)
        | ((NDP120_SPI_INTCTL_WM_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_WATER_MARK : 0)
        | ((NDP120_SPI_INTCTL_RF_INTEN(1) & intctl_old)
           ? SYNTIANT_NDP_INTERRUPT_SPI_READ_FAILURE : 0);

    *causes = cs;

error:
    return s;
}

static int syntiant_load_encrypted_pkgs( struct syntiant_ndp_device_s *ndp,
    const uint8_t *chunk_ptr, uint32_t chunk_len, int mcu_fw)
{
    int s = SYNTIANT_NDP_ERROR_NONE, is_error;
    uint8_t data, error_code;
    uint32_t msg, window_size, bytes_to_copy, sum = 0;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_ndp120_bootloader_state_t *bl_state = &ndp120->bl_state;

    memset(bl_state, 0, sizeof(*bl_state));
    if (mcu_fw) {
        bl_state->window_lower = BOOTLOADER_OPEN_RAM_WINDOW_LOWER;
        bl_state->window_upper = BOOTLOADER_OPEN_RAM_WINDOW_UPPER;
    } else {
        bl_state->window_lower = MCU_OPEN_RAM_WINDOW_LOWER;
        bl_state->window_upper = MCU_OPEN_RAM_WINDOW_UPPER;
    }
    window_size = bl_state->window_upper - bl_state->window_lower;

    ndp120->bl_state.mode = SYNTIANT_NDP120_BOOTLOADER_MODE_IN_PROGRESS;
    /* MCU requires writes to be on 4-byte boundaries.  Oddball chunk sizes
     * remainders are stored in bl_state->remainder */
    if (bl_state->remainder_len > 0) {
        bytes_to_copy = (unsigned int) MIN((int)(4 - bl_state->remainder_len), (int) chunk_len);
        if (bytes_to_copy > 0) {
            /* try to bring remainder up to 4 bytes */
            memcpy(bl_state->remainder + bl_state->remainder_len, chunk_ptr,
                bytes_to_copy);
            chunk_len -= bytes_to_copy;
            chunk_ptr += bytes_to_copy;
            bl_state->remainder_len += bytes_to_copy;
        }
        /* cover edge case where chunks are < 4 bytes */
        if (bl_state->remainder_len == 4)  {
            s = ndp_mcu_write_block(
                bl_state->window_lower + bl_state->window_idx,
                bl_state->remainder, 4);
            bl_state->window_idx += 4;
            bl_state->remainder_len = 0;
        }
    }
    while (chunk_len) {
        if (chunk_len < 4) {
            memcpy(bl_state->remainder, chunk_ptr, chunk_len);
            bl_state->remainder_len = chunk_len;
            chunk_len = 0;
            break;
        }
        bytes_to_copy = MIN(window_size - bl_state->window_idx, chunk_len);
        bytes_to_copy -= bytes_to_copy % 4; /* trim to 4 byte boundary */

        /* this is sometimes 0 for odd chunk sizes*/
        if (bytes_to_copy > 0) {
            s = ndp_mcu_write_block(
                bl_state->window_lower + bl_state->window_idx, chunk_ptr,
                bytes_to_copy);
            if (s) goto error;
            chunk_len -= bytes_to_copy;
            chunk_ptr += bytes_to_copy;
            sum += bytes_to_copy;
            bl_state->window_idx += bytes_to_copy;
            DEBUG_PRINTF("total: %d, ", sum);
        }

        if (bl_state->window_idx == bytes_to_copy) {
            s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_CONT, &msg);
            if (s) goto error;
            bl_state->window_idx = 0;
            data = (uint8_t)msg;

            if (data != SYNTIANT_NDP120_MB_MCU_NOP) {
                DEBUG_PRINTF(
                    "received 0x%02X instead of expected NOP", data);
                s = SYNTIANT_NDP_ERROR_FAIL;
                goto error;
            }
            s = mbout_recv(ndp, &data);
            if (s) goto error;
            s = mbout_send_resp(ndp, SYNTIANT_NDP120_MB_MCU_NOP);
            if (s) goto error;

            is_error = mb_resp_is_error(data, &error_code);
            if (data == SYNTIANT_NDP120_MB_MCU_LOAD_DONE) {
                DEBUG_PRINTF("load done");
                bl_state->mode = SYNTIANT_NDP120_BOOTLOADER_MODE_COMPLETE;
                s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                        SYNTIANT_NDP120_MB_MCU_CONT, NULL);
                if (s) goto error;
            } else if (data == SYNTIANT_NDP120_MB_OTHER_LOAD_DONE) {
                DEBUG_PRINTF("load done for other packages");
                bl_state->mode = SYNTIANT_NDP120_BOOTLOADER_MODE_COMPLETE;
            } else if (is_error) {
#if SYNTIANT_NDP120_DEBUG_MB
                const char *name = syntiant_ndp120_mcu_mb_error_decoder(error_code);
                DEBUG_PRINTF("load error: %s (0x%02X)", name, error_code);
#endif
                s = SYNTIANT_NDP_ERROR_FAIL;
                goto error;
            } else if (data != SYNTIANT_NDP120_MB_MCU_CONT) {
                DEBUG_PRINTF("Received 0x%02X, aborting", data);
                s = SYNTIANT_NDP_ERROR_FAIL;
                goto error;
            }
        } /* if ("window full") */
    }    /* while (chunk_len) */
error:
    if (bl_state->mode == SYNTIANT_NDP120_BOOTLOADER_MODE_COMPLETE) {
        if (mcu_fw) {
            /* Received RUNNING from FW already, issue MIADDR */
            s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                SYNTIANT_NDP120_MB_MCU_MIADDR, NULL);
            if (s) goto error;
            ndp120->mcu_fw_loaded = 1;
        } else if (!ndp120->dsp_fw_loaded) {
            DEBUG_PRINTF("waiting for RUNNING");
            s = syntiant_ndp120_get_dsp_fw_running_state(ndp);
            if (s) goto error;
        }
    }
    return s;
}

static int syntiant_ndp120_load_via_bootloader(
    struct syntiant_ndp_device_s *ndp, const uint8_t *chunk_ptr, uint32_t chunk_len)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t data;
    uint32_t msg;
    int i;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    syntiant_ndp120_bootloader_state_t *bl_state = &ndp120->bl_state;

    if (bl_state->mode == SYNTIANT_NDP120_BOOTLOADER_MODE_START) {
        /* Step 0: Reset the chip, wait for BOOTING on MBOUT */

        /* clear interrupts */
        s = ndp_spi_write(NDP120_SPI_INTSTS, 0xff);
        if(s) goto error;

        /* enable interrupts */
        s = ndp_spi_read(NDP120_SPI_CFG, &data);
        if (s) goto error;

        data = NDP120_SPI_CFG_INTEN_INSERT(data, 1);
        s = ndp_spi_write(NDP120_SPI_CFG, data);
        if (s) goto error;

        /* enable MBIN, MBOUT, WM, and AE interrupts */
        data = 0;
        data = NDP120_SPI_INTCTL_MBIN_INTEN_INSERT(data, 1);
        data = NDP120_SPI_INTCTL_MBOUT_INTEN_INSERT(data, 1);
        data = NDP120_SPI_INTCTL_WM_INTEN_INSERT(data, 1);
        data = NDP120_SPI_INTCTL_AE_INTEN_INSERT(data, 1);
        s = ndp_spi_write(NDP120_SPI_INTCTL, data);
        if(s) goto error;
        mailbox_reset_state(ndp);

        /* Reset the chip */
        data = 0;
        data = NDP120_SPI_CTL_RESETN_INSERT(data, 0);
        data = NDP120_SPI_CTL_FLASHCTL_INSERT(data, NDP120_SPI_CTL_FLASHCTL_DISABLE);
        data = NDP120_SPI_CTL_PORSTN_INSERT(data, 1);
        data = NDP120_SPI_CTL_CLKEN_INSERT(data, 1);
        s = ndp_spi_write(NDP120_SPI_CTL, data);
        if(s) goto error;

        /* deassert reset */
        data = NDP120_SPI_CTL_RESETN_INSERT(data, 1);
        s = ndp_spi_write(NDP120_SPI_CTL, data);
        if(s) goto error;

        s = mbout_recv(ndp, &data);
        if (s) goto error;

        s = mbout_send_resp(ndp, SYNTIANT_NDP120_MB_MCU_ACK);
        if (s) goto error;
        /* Should receive BOOTING now*/
        s = mbout_recv(ndp, &data);
        if (s) goto error;

        if (data != SYNTIANT_NDP120_MB_MCU_BOOTING) {
            DEBUG_PRINTF("expecting BOOTING on mbout; received: 0x%02X", data);
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }
        /* Step 1: Handshake with bootloader */
        /* Handshake 0 */

        for (i = 0; i < 2; ++i) {
            s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_NOP, NULL);
            if (s) goto error;
        }
        DEBUG_PRINTF("handshake complete");
        /* Step 2:
               send LOAD
               await CONT
        */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, SYNTIANT_NDP120_MB_MCU_LOAD, &msg);
        if (s) goto error;
        data = (uint8_t)msg;
        if (data != SYNTIANT_NDP120_MB_MCU_CONT) {
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }

        /* Step 3: get "load area" from
                addrprotlo_0 and
                addrprothi_0
        */
        s = syntiant_load_encrypted_pkgs(ndp, chunk_ptr, chunk_len, 1);
    } else {
        s = SYNTIANT_NDP_ERROR_FAIL;
    }

error:
    return s;
}

static int syntiant_ndp120_secure_load(struct syntiant_ndp_device_s *ndp,
    const uint8_t *chunk, uint32_t chunk_len)
{
    uint8_t data;
    uint32_t msg;
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;

    if (!ndp120->mcu_fw_loaded) {
        /* load encrypted MCU fw via boot loader */
        DEBUG_PRINTF("load encrypted MCU fw via boot loader");
        s = syntiant_ndp120_load_via_bootloader(ndp, chunk, chunk_len);
    } else {
        /* load other synpkgs via MCU fw */
        DEBUG_PRINTF("load encrypted DSP fw/ NN synpkg via MCU fw");
        if (!chunk_len) {
            s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                    SYNTIANT_NDP120_MB_MCU_LOAD, &msg);
            if (s) goto error;
            data = (uint8_t) msg;
            if (data != SYNTIANT_NDP120_MB_MCU_CONT) {
                s = SYNTIANT_NDP_ERROR_FAIL;
                DEBUG_PRINTF("Error, didn't receive cont, got: %x", data);
                goto error;
            }
            s = SYNTIANT_NDP_ERROR_MORE;
        } else {
            /* continue load process */
            s = syntiant_load_encrypted_pkgs(ndp, chunk, chunk_len, 0);
        }
    }
error:
    return s;
}


int syntiant_ndp120_load(struct syntiant_ndp_device_s *ndp, void *chunk, int len)
{
    uint32_t data;
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pkg_parser_state_t *pstate = &ndp->pstate;
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    uint32_t ulen = (uint32_t)len;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    DEBUG_PRINTF("syntiant_ndp120_load: %d bytes\n", len);
    /* enforce FW version compatibility check, if chosen so by the app */
    if (ndp120->init_mode ==
            SYNTIANT_NDP_INIT_MODE_RESET_ENFORCE_COMPATIBILITY) {
        pstate->fw_compatibility_check =
              SYNTIANT_NDP_ENFORCE_FW_COMPATIBILITY_CHECK;
    }
    if (secure_boot) {
        return syntiant_ndp120_secure_load(ndp, chunk, (uint32_t) len);
    }
    if (ulen == 0) {
        /* check if fw or NN params are already there, not board params*/
        int is_any_pkg_loaded = (ndp120->paramver_len || ndp120->fwver_len);
        memset(&ndp120->bl_state, 0, sizeof(ndp120->bl_state));

        /* check to see if the chip requires secure boot */
        /* check to see if the chip requires secure boot */
        s = ndp_spi_read(NDP120_SPI_ID0, &data);
        if (s) goto error;
        ndp120->secure_boot_required
            = NDP120_SPI_ID0_AUTH_FIRMWARE_EXTRACT(data);
#ifdef EMULATE_SECURE
        ndp120->secure_boot_required = 1;
#endif
        /* check to see if the chip is locked */
        s = ndp_spi_read(NDP120_SPI_CTL, &data);
        if (s) goto error;
        s = device_locked(ndp, &ndp120->chip_is_locked);
        if (s) goto error;

        if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
            s = SYNTIANT_NDP_ERROR_MORE;
            goto error;
        }
        syntiant_pkg_parser_init(pstate);
        syntiant_pkg_parser_reset(pstate);
        /* verify configuration and ndp120 matches */
        if (is_any_pkg_loaded) {
            s = syntiant_ndp120_verify_configuration(ndp);
            if ((s && s != SYNTIANT_NDP_ERROR_PACKAGE)
                || (s && s == SYNTIANT_NDP_ERROR_FAIL)) {
                    DEBUG_PRINTF("config verify error s=%d\n", s);
                goto error;
            }
        }
        s = SYNTIANT_NDP_ERROR_MORE;
        goto error;
    } else {
        /* process package */
        syntiant_pkg_preprocess_chunk(pstate, chunk, len, 0);
        syntiant_pkg_parser_reset(pstate);
        while (pstate->ptr < pstate->open_ram_end
            && pstate->mode != PACKAGE_MODE_DONE) {
            s = syntiant_pkg_parse_chunk(pstate, 0);
            if (s) {
                s = SYNTIANT_NDP_ERROR_PACKAGE;
                goto error;
            }
            if (pstate->mode == PACKAGE_MODE_TAG_START
                || pstate->mode == PACKAGE_MODE_VALUE_START
                || pstate->mode == PACKAGE_MODE_VALUE_CONT) {
                s = syntiant_ndp120_parse_tag_values(ndp);
                if (s) {
                    DEBUG_PRINTF("%s:%d Got error %d from "
                                 "syntiant_ndp120_parse_tag_values, tag: %d\n",
                        __FILE__, __LINE__, s, *(uint32_t *)pstate->tag);
                    goto error;
                }

                if (pstate->ptr == pstate->open_ram_end
                    && pstate->mode != PACKAGE_MODE_DONE) {
                    s = SYNTIANT_NDP_ERROR_MORE;
                    goto error;
                }
            }
        }
        if (!s) {
            /* Validate the PBI version tag for Core 2 */
            s = syntiant_ndp120_validate_pbi_tag(ndp);
        }
    }

error:
    return s;
}

int syntiant_ndp120_get_config(struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp_config_s *config)
{
    syntiant_ndp120_device_t *ndp120 = &ndp->d.ndp120;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t network_id = ndp120->last_network_id;
    enum pbi_version_chip_type_t_ chip_type;

    DEBUG_PRINTF("--> syntiant_ndp120_get_config...\n");

#if !SYNTIANT_NDP120_DEBUG_SECURE
    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }
#endif
    config->device_type = syntiant_ndp120_get_config_devtype(ndp->device_type,
        &chip_type);
    DEBUG_PRINTF("--> chip_type=%#x device_type=%s\n",
        chip_type, config->device_type);
    config->classes = ndp120->classes[network_id];

    if (
           !config->labels_len
        && !config->firmware_version_len
        && !config->parameters_version_len
        && !config->pkg_version_len
        && !config->dsp_firmware_version_len
    ) {
        /* retrieve all the version lengths */
        config->labels_len = ndp120->labels_len;
        config->firmware_version_len = ndp120->fwver_len;
        config->parameters_version_len = ndp120->paramver_len;
        config->pkg_version_len = ndp120->pkgver_len;
        config->dsp_firmware_version_len = ndp120->dspfwver_len;

        if (ndp120->init_mode != SYNTIANT_NDP_INIT_MODE_RESTART) {
            goto error;
        }
    }

    s = syntiant_ndp120_access_config(ndp, config);
error:
    return s;
}

int syntiant_ndp120_send_data(struct syntiant_ndp_device_s *ndp,
    const uint8_t *data, unsigned int len, int type, uint32_t offset)
{
    int s;

    /* boundary check for DNN static feature */
    if ((type == SYNTIANT_NDP_SEND_DATA_TYPE_STREAMING) && offset) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
#if SYNTIANT_NDP120_DEBUG_SEND
    DEBUG_PRINTF("ndp120_send_data, sending %d bytes", len);
#endif

    s = syntiant_ndp120_write_block(ndp, 0, NDP120_SPI_SAMPLE, data, len);

error:
    return s;
}

/*
static uint32_t bufdelta(uintptr_t cur, uintptr_t last, size_t bufsize) {
    cur += (cur < last : bufsize : 0);
    return cur-last;
}
*/

int syntiant_ndp120_extract_data(struct syntiant_ndp_device_s *ndp, int type,
                             int from, uint8_t *data, unsigned int *lenp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    /* transient variables */
    uint32_t adx;
    uint32_t read_len_bytes;

    /* sample size vars */
    uint32_t sample_size, effective_sample_size;

    /* dsp buffer info */
    uint32_t prod_ptr, cons_ptr, last_ptr = 0;
    uint32_t buf_start, buf_end;
    uint32_t ann_buf_start = 0;
    uint32_t buf_size_bytes;
    uint32_t dsp_lock;

    uint32_t bytes_available, effective_bytes_available;
    uint32_t chunk_count;

    /* buf read location info */
    size_t offset;
    uint32_t read_start_ptr, match_ptr;
    uint32_t samp_cap = 0;
    int extract_all = 0;
    uint32_t preroll_len;
    uint32_t winstep_us;
    uint32_t frequency = CPU_FREQUENCY;
    uint32_t cycles_per_us, cycles_per_winstep, total_cycles = 0;

    /* device info & fw state info*/
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    ndp120_dsp_config_t  fw_config;
    ndp120_dsp_buffers_t fw_buffers = { 0 };
    syntiant_ndp120_config_tank_t tankcfg = {0};

    struct recording_metadata {
        uint32_t prod_ptr;
        uint32_t cons_ptr;
        uint32_t sample_size;
        uint32_t buf_start;
        uint32_t samp_cap;
        uint32_t ann_buf_start;
    } rec_metadata;

    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        switch (type) {
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED:
            /* audio metadata is dumped at NDP120_MCU_OPEN_RAM_RESULTS */
            adx = NDP120_MCU_OPEN_RAM_RESULTS;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_SENSOR:
            /* sensor metadata is dumped after audio metadata */
            adx = NDP120_MCU_OPEN_RAM_RESULTS + sizeof(rec_metadata);
            break;

        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
            break;
        }

        s = syntiant_ndp120_read_block(ndp, 1, adx, &rec_metadata,
            sizeof(rec_metadata));
        if (s) goto error;

        prod_ptr = rec_metadata.prod_ptr;
        cons_ptr = rec_metadata.cons_ptr;
        sample_size = rec_metadata.sample_size;
        buf_start = rec_metadata.buf_start;
        samp_cap = rec_metadata.samp_cap;
        buf_size_bytes = sample_size * samp_cap;
        switch (type) {
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT:
            /* fallthru */
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED:
            last_ptr = ndp120->dsp_pcm_audio_sample_last_ptr;
            ann_buf_start = rec_metadata.ann_buf_start;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_SENSOR:
            last_ptr = ndp120->dsp_sensor_sample_last_ptr;
            break;
        }

    } else {
        if (lenp == NULL) {
            return SYNTIANT_NDP_ERROR_ARG;
        }
        /* get fw state info */
        adx = ndp120->dsp_fw_state_addr +
                (uint32_t)offsetof(ndp120_dsp_fw_base_t, buffers);
        s = ndp_mcu_read_block((uint32_t)adx, &fw_buffers, sizeof(fw_buffers));
        if (s) goto error;

        adx = ndp120->dsp_fw_state_addr +
                (uint32_t)offsetof(ndp120_dsp_fw_base_t, config);
        s = ndp_mcu_read_block((uint32_t)adx, &fw_config, sizeof(fw_config));
        if (s) goto error;

        switch (type) {
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT:
            /* fallthru */
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED:
            /* DEBUG_PRINTF("extract_data: audio\n"); */
            prod_ptr = fw_buffers.aud_samp_buf_prod;
            cons_ptr = fw_buffers.aud_samp_buf_cons;
            last_ptr = ndp120->dsp_pcm_audio_sample_last_ptr;
            sample_size = fw_config.aud_samp_size_bytes;
            buf_start = fw_buffers.aud_samp_buf_ptr;
            samp_cap = fw_config.aud_samp_cap;
            buf_size_bytes = sample_size * samp_cap;
            ann_buf_start = fw_buffers.aud_annotation_buf_ptr;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_FEATURES:
            /* DEBUG_PRINTF("extract_data: feature\n"); */
            prod_ptr = fw_buffers.func_samp_buf_prod;
            cons_ptr = fw_buffers.func_samp_buf_cons;
            last_ptr = ndp120->dsp_function_sample_last_ptr;
            sample_size = fw_config.func_samp_size_bytes;
            buf_start = fw_buffers.func_samp_buf_ptr;
            buf_size_bytes = sample_size * fw_config.func_samp_cap;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_RAW_ACCEL:
            prod_ptr = fw_buffers.sensor_samp_buf_prod;
            cons_ptr = fw_buffers.sensor_samp_buf_cons;
            last_ptr = ndp120->dsp_sensor_sample_last_ptr;
            sample_size = fw_config.sensor_samp_size_bytes;
            buf_start = fw_buffers.sensor_samp_buf_ptr;
            buf_size_bytes = sample_size * fw_config.sensor_samp_cap;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_SENSOR:
            /* DEBUG_PRINTF("extract_data: image\n"); */
            prod_ptr = fw_buffers.sensor_samp_buf_prod;
            cons_ptr = fw_buffers.sensor_samp_buf_cons;
            last_ptr = ndp120->dsp_sensor_sample_last_ptr;
            sample_size = fw_config.sensor_samp_size_bytes;
            buf_start = fw_buffers.sensor_samp_buf_ptr;
            buf_size_bytes = sample_size * fw_config.sensor_samp_cap;
            buf_end = buf_start + buf_size_bytes;
            break;


        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    }

    buf_end = buf_start + buf_size_bytes;

    DEBUG_PRINTF("buf_start: 0x%X\n", buf_start);
    DEBUG_PRINTF("  buf_end: 0x%X\n", buf_end);
    DEBUG_PRINTF(" prod_ptr: 0x%X\n", prod_ptr);
    DEBUG_PRINTF(" cons_ptr: 0x%X\n", cons_ptr);

    effective_sample_size = sample_size;
    if (type == SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED) {
        effective_sample_size += (uint32_t)sizeof(ndp120_dsp_audio_sample_annotation_t);
    }

    /* guard against buffer overflow or 0 length */
    if (*lenp > (buf_end - buf_start) || !*lenp) {
        *lenp = buf_end - buf_start;
        extract_all = 1;
    }

    if (fw_buffers.sample_reset_flag || extract_all) {
        last_ptr = 0;
        adx = ndp120->dsp_fw_state_addr
            + (uint32_t) offsetof(ndp120_dsp_fw_base_t,
                buffers.sample_reset_flag);
        s = ndp_mcu_write(adx, 0);
        if (s) goto error;
    }

    if (last_ptr == 0) { /* will be 0 on system start */
        last_ptr = cons_ptr;
    }
    /* round *lenp to even multiple of sample size. */
    *lenp = *lenp / effective_sample_size * effective_sample_size;

    /* calculate data available */
    switch (from) {
        case SYNTIANT_NDP_EXTRACT_FROM_MATCH: {
            bytes_available = (uint32_t)BUFDELTA(prod_ptr,
                buf_start + ndp120->tankptr_match, buf_size_bytes);
            extract_all = 1;
            break;
        }

        case SYNTIANT_NDP_EXTRACT_FROM_NEWEST:
        case SYNTIANT_NDP_EXTRACT_FROM_OLDEST:
            bytes_available = (uint32_t)BUFDELTA(prod_ptr, cons_ptr, buf_size_bytes);
            break;

        /* from the oldest unread */
        case SYNTIANT_NDP_EXTRACT_FROM_UNREAD:
            bytes_available = (uint32_t)BUFDELTA(prod_ptr, last_ptr, buf_size_bytes);
            break;

        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
    }

    chunk_count = bytes_available / sample_size;
    effective_bytes_available = chunk_count * effective_sample_size;
    DEBUG_PRINTF("bytes available:%d\n", bytes_available);
    DEBUG_PRINTF("*lenp interim: %d\n", *lenp);

    /* Extract only whats available from the tank instead of a full sink
     */
    if (!extract_all) {
        *lenp = MIN(*lenp, effective_bytes_available);
    }

    switch (from) {
        case SYNTIANT_NDP_EXTRACT_FROM_MATCH: {
            /* preroll len should be multiple of sample size */
            preroll_len = (*lenp / effective_sample_size) * sample_size;
            match_ptr = buf_start + ndp120->tankptr_match;
            read_start_ptr = ((match_ptr - preroll_len) < buf_start)
                ? (buf_end - preroll_len + ndp120->tankptr_match)
                : (match_ptr - preroll_len);
            break;
        }

        /* from the oldest unread */
        case SYNTIANT_NDP_EXTRACT_FROM_UNREAD:
            read_start_ptr = last_ptr;
            DEBUG_PRINTF("using last_ptr of 0x%x\n", last_ptr);
            break;

        /* from the oldest recorded */
        case SYNTIANT_NDP_EXTRACT_FROM_OLDEST:
            read_start_ptr = cons_ptr;
            break;

        case SYNTIANT_NDP_EXTRACT_FROM_NEWEST:
            if (prod_ptr == buf_start) {
                read_start_ptr = buf_end - sample_size;
            } else {
                read_start_ptr = prod_ptr - sample_size;
            }
            break;

        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
    }

    if (data == NULL) {
        /* update last ptr */
        if (from == SYNTIANT_NDP_EXTRACT_FROM_MATCH &&
            (type == SYNTIANT_NDP_EXTRACT_TYPE_INPUT ||
            type == SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED)) {
            ndp120->dsp_pcm_audio_sample_last_ptr = read_start_ptr;
        }
        goto error;
    }
    if (!ndp120->dsp_sampletank_mem_type) {
        tankcfg.get = 1;
        s = syntiant_ndp120_config_dsp_tank_memory(ndp, &tankcfg);
        if (s) {
            goto error;
        }
    }

    if (ndp120->dsp_sampletank_mem_type == NDP120_DSP_MEM_TYPE_DNN_DATA) {
        adx = ndp120->dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, max_winstep_us);
        s = syntiant_ndp120_read_block(ndp, 1, adx, &winstep_us,
                                   sizeof(winstep_us));
        adx = NDP120_CHIP_CONFIG_SPARE(0);
        dsp_lock = NDP_HOST_EXTRACTION_LOCK_SIG;
        s = syntiant_ndp120_write_block(ndp, 1, adx, &dsp_lock,
            sizeof(dsp_lock));
        if (s) {
            goto error;
        }
        /* 1 ms delay */
        ndp->iif.udelay(1000);
        adx = NDP120_CHIP_CONFIG_SPARE(1);
        cycles_per_us = (frequency / 1000000);
        cycles_per_winstep = cycles_per_us * winstep_us;
        while(1) {
            s = syntiant_ndp120_read_block(ndp, 1, adx, &dsp_lock,
                sizeof(dsp_lock));
            if (s) {
                goto error;
            }
            if (dsp_lock == 0) {
                break;
            }
            if (total_cycles >= cycles_per_winstep) {
                DEBUG_PRINTF("DSP is not freeing the Extraction Lock\n");
                adx = NDP120_CHIP_CONFIG_SPARE(0);
                dsp_lock = 0;
                s = syntiant_ndp120_write_block(ndp, 1, adx, &dsp_lock,
                    sizeof(dsp_lock));
                s = SYNTIANT_NDP_ERROR_EXTRACTION_LOCK_TIMEOUT;
                goto error;
            }
            ndp->iif.udelay(1);
            total_cycles += cycles_per_us;
        }
    }

    /* read out the actual data */
    chunk_count = *lenp / effective_sample_size;
    offset = 0;
    read_len_bytes = chunk_count * sample_size;
    adx = read_start_ptr;

    if (adx + read_len_bytes > buf_end) {
        read_len_bytes = buf_end - adx;
        s = ndp_mcu_read_block(adx, data, read_len_bytes);
        if (s) goto error;
        offset = read_len_bytes;
        read_len_bytes = (chunk_count * sample_size) - read_len_bytes;
        adx = buf_start;
    }
    s = ndp_mcu_read_block(adx, data + offset, read_len_bytes);
    if (s) goto error;

    last_ptr = adx + read_len_bytes;
    DEBUG_PRINTF("setting last_ptr to 0x%x\n", last_ptr);

    /* Annotation extraction and interleaving */
    if (type == SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED) {
        uint32_t chunk_start_index = (read_start_ptr - buf_start) / sample_size;

        uint32_t ann_buf_sample_size = (uint32_t) sizeof(ndp120_dsp_audio_sample_annotation_t);
        uint32_t ann_buf_end = ann_buf_start + ann_buf_sample_size * samp_cap;

        uint32_t src_adx = ann_buf_start + chunk_start_index *
            (uint32_t) sizeof(ndp120_dsp_audio_sample_annotation_t);
        uint8_t   *dst_adx = data + chunk_count * sample_size;
        DEBUG_PRINTF("annotate ...\n");
        DEBUG_PRINTF("read_start_ptr: 0x%x\n", read_start_ptr);
        DEBUG_PRINTF("chunk_count: %u chunk_start_index: %u\n", chunk_count, chunk_start_index);
        DEBUG_PRINTF("ann_buf_start: 0x%x, ann_buf_end: 0x%x\n", ann_buf_start, ann_buf_end);
        DEBUG_PRINTF("src_adx: 0x%x dst_adx: %p\n", src_adx, dst_adx);

        /* extract */

        read_len_bytes = (uint32_t)(chunk_count * sizeof(ndp120_dsp_audio_sample_annotation_t));

        if (src_adx + read_len_bytes > ann_buf_end) {
            uint32_t read_len_bytes_temp = ann_buf_end - src_adx;
            s = ndp_mcu_read_block(src_adx, dst_adx, read_len_bytes_temp);
            if (s) goto error;
            read_len_bytes -= read_len_bytes_temp;
            src_adx = ann_buf_start;
            dst_adx += read_len_bytes_temp;
        }
        s = ndp_mcu_read_block(src_adx, dst_adx, read_len_bytes);
        if (s) goto error;

        /* interleave */

        if (chunk_count > 1) {
            ndp120_dsp_audio_sample_annotation_t *ann_ptr =
                (ndp120_dsp_audio_sample_annotation_t *)(data + chunk_count * sample_size);
            ndp120_dsp_audio_sample_annotation_t ann;
            uint8_t *src, *dst;
            uint32_t i;
            uint32_t len_bytes;
            DEBUG_PRINTF("data ptr: %p\n", data);
#if 0
            for (i = 0; i < chunk_count; ++i) {
                ann = *ann_ptr;
                src = data + (i+1) * sample_size;
                dst = data + (i+1) * effective_sample_size;
                len_bytes = (chunk_count - (i+1)) * sample_size;
                DEBUG_PRINTF("src: %p dst: %p cnt: %d\n", src, dst, len_bytes);
                memmove(dst, src, len_bytes);
                *(ndp120_dsp_audio_sample_annotation_t *)(dst-sizeof(ann)) = ann;
                ++ann_ptr;
            }
#endif
            for (i = 1; i < chunk_count; ++i) {
                ann = *ann_ptr;
                src = data + i * sample_size + (i - 1) * sizeof(ann);
                dst = data + i * effective_sample_size;
                len_bytes = (chunk_count - i) * sample_size;
                DEBUG_PRINTF("src: %p dst: %p cnt: %d\n", src, dst, len_bytes);
                memmove(dst, src, len_bytes);
                *(ndp120_dsp_audio_sample_annotation_t *)(dst-sizeof(ann)) = ann;
                ++ann_ptr;
            }
        }
    }
    if (ndp120->dsp_sampletank_mem_type == NDP120_DSP_MEM_TYPE_DNN_DATA) {
        adx = NDP120_CHIP_CONFIG_SPARE(0);
        dsp_lock = 0;
        s = syntiant_ndp120_write_block(ndp, 1, adx, &dsp_lock,
            sizeof(dsp_lock));
        if (s) {
            goto error;
        }
    }


    if (last_ptr >= buf_end) {
        last_ptr -= buf_size_bytes;
    }

    /* update last ptr */
    switch (type) {
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT:
        case SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED:
            ndp120->dsp_pcm_audio_sample_last_ptr = last_ptr;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_FEATURES:
            ndp120->dsp_function_sample_last_ptr = last_ptr;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_RAW_ACCEL:
            ndp120->dsp_sensor_sample_last_ptr = last_ptr;
            break;

        case SYNTIANT_NDP_EXTRACT_TYPE_SENSOR:
            ndp120->dsp_sensor_sample_last_ptr = last_ptr;
            break;
    }

error:
    if (*lenp == 0) {
        s = SYNTIANT_NDP_ERROR_DATA_REREAD;
        DEBUG_PRINTF("syntiant_ndp120_extract_data: reread error: \n");
    }
    return s;
}


static int syntiant_ndp120_get_result_offset(struct syntiant_ndp_device_s *ndp,
    uint32_t *addr, uint32_t network_id)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t result_base_addr;
    if (*addr) {
        *addr += (uint32_t) offsetof(struct ndp120_fw_state_s, result);
        *addr += (uint32_t) sizeof(uint32_t) * network_id;
        s = syntiant_ndp120_read_block(ndp, 1, *addr,
            &result_base_addr, sizeof(result_base_addr));
        if (s) {
            goto error;
        }
        *addr = result_base_addr;
    }

error:
    return s;
}

int syntiant_ndp120_get_match_summary(
    struct syntiant_ndp_device_s *ndp, uint32_t *summary)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t addr = ndp120->mcu_fw_state_addr;
    uint32_t network_id = ndp120->last_network_id;
    uint32_t cons;
    uint32_t address = NDP120_MCU_OPEN_RAM_MATCH_RESULTS;
    struct syntiant_ndp120_secure_match_data secure_match;
    *summary = 0;

    if (addr) {
        cons = ndp120->match_consumer[network_id];
        if (ndp120->match_producer[network_id] != cons) {
            /* read from Open RAM */
            if (syntiant_ndp_boot_from_flash_flag(ndp, 0)) {
                address = NDP120_FLASHBOOT_OPEN_RAM_MATCH_RESULTS;
            }
            s = syntiant_ndp120_read_block(ndp, 1, address,
                &secure_match, sizeof(secure_match));
            if (s) goto error;
            ndp120->tankptr_match = secure_match.tank_ptr & ~0x3U;
            ndp120->match_frame_count = secure_match.match_frame_count;
            ndp120->sec_val_status = secure_match.sec_val_status;
            *summary = secure_match.summary;
            ndp120->algo_match_summary = secure_match.algo_match_summary;
            if (s) {
                DEBUG_PRINTF("Match results read error, %d\n", s);
                goto error;
            }
            DEBUG_PRINTF("ALGO MATCH SUMMARY: 0x%x\n", secure_match.algo_match_summary);
            DEBUG_PRINTF("tankptr at match: 0x%x\n", secure_match.tank_ptr);
            cons++;
            cons = cons == NDP120_MATCH_RING_SIZE ? 0 : cons;
            ndp120->match_consumer[network_id] = cons;
        }

    } else {
        s = ndp_spi_read(NDP120_SPI_MATCHSTS, summary);
        if(s) goto error;
   }

error:
    return s;
}

int syntiant_ndp120_get_match_binary(
    struct syntiant_ndp_device_s *ndp, uint8_t *matches, unsigned int len)
{
    int s;
    uint32_t addr = ndp->d.ndp120.mcu_fw_state_addr;
    uint32_t network_id = ndp->d.ndp120.last_network_id;

    if (8 < len || len % 4 != 0) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
    if (ndp->d.ndp120.secure_boot_required || ndp->d.ndp120.chip_is_locked) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    if (addr) {
        s = syntiant_ndp120_get_result_offset(ndp, &addr, network_id);
        if (s) {
            goto error;
        }
        addr += (uint32_t) offsetof(struct ndp120_dnn_result_s, winner_one_hot);
        s = syntiant_ndp120_read_block(ndp, 1, addr, matches, len);
        if (s) goto error;
    } else {
        /* FAIL instead of UNINIT to match 10x ilib code */
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }

error:
    return s;
}

static int  syntiant_ndp120_get_activation_size(
        struct syntiant_ndp_device_s *ndp, uint32_t network_id, uint8_t *act)
{
    int s;
    struct ndp120_nn_output_cache_s nn_cache;
    uint32_t addr = ndp->d.ndp120.mcu_fw_state_addr +
                (uint32_t) offsetof(struct ndp120_fw_state_s, nn_output_cache) +
                (uint32_t) sizeof(nn_cache) * network_id;
    struct syntiant_ndp120_secure_match_data secure_match;

    if (ndp->d.ndp120.secure_boot_required || ndp->d.ndp120.chip_is_locked) {
    /* read from Open RAM */
        s = syntiant_ndp120_read_block(ndp, 1, NDP120_MCU_OPEN_RAM_MATCH_RESULTS,
                                           &secure_match, sizeof(secure_match));
        if (s) goto error;
        *act = (uint8_t) secure_match.activation_type;
    } else {
        s = syntiant_ndp120_read_block(ndp, 1, addr, (void *) &nn_cache,
            sizeof(nn_cache));
        if (s) goto error;

        switch (nn_cache.activation_type) {
        case NDP120_DNN_ISA_COMP0_ACTIVATION_RELU:
        case NDP120_DNN_ISA_COMP0_ACTIVATION_SIGMOID0:
        case NDP120_DNN_ISA_COMP0_ACTIVATION_SIGMOID1:
            *act = sizeof(((ndp120_raw_strengths_t *)0)->u8[0]);
            break;
        case NDP120_DNN_ISA_COMP0_ACTIVATION_LINEAR:
        case NDP120_DNN_ISA_COMP0_ACTIVATION_TANH0:
        case NDP120_DNN_ISA_COMP0_ACTIVATION_TANH1:
            *act = sizeof(((ndp120_raw_strengths_t *)0)->s8[0]);
            break;
        case NDP120_DNN_ISA_COMP0_ACTIVATION_LINEAR_16:
            *act = sizeof(((ndp120_raw_strengths_t *)0)->s16[0]);
            break;
        default:
            DEBUG_PRINTF("Unknown activation type: %d\n",
                    nn_cache.activation_type);
            s = SYNTIANT_NDP_ERROR_FAIL;
            break;
        }
    }
error:
    return s;
}

int syntiant_ndp120_get_num_activations(struct syntiant_ndp_device_s *ndp,
    unsigned int *len)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct ndp120_nn_output_cache_s nn_cache;
    uint32_t addr;
    uint32_t network_id = ndp->d.ndp120.last_network_id;
    addr = ndp->d.ndp120.mcu_fw_state_addr +
           (uint32_t)offsetof(struct ndp120_fw_state_s, nn_output_cache) +
           (uint32_t)sizeof(nn_cache) * network_id;
    s = syntiant_ndp120_read_block(ndp, 1, addr, (void *)&nn_cache, sizeof(nn_cache));
    if (s) {
        goto error;
    }
    *len = (unsigned int)(((nn_cache.num_entries + 3) / 4) * 4);

error:
    return s;
}

int syntiant_ndp120_get_match_strength(struct syntiant_ndp_device_s *ndp,
    uint8_t *strengths, unsigned int len, int type)

{
    int s;
    uint8_t act_size = 0;
    uint32_t addr = ndp->d.ndp120.mcu_fw_state_addr, raw_strengths_addr;
    uint32_t softmax_strengths_addr;
    uint32_t network_id = ndp->d.ndp120.last_network_id;

    if (type == SYNTIANT_NDP_STRENGTH_RAW) {
        s = syntiant_ndp120_get_activation_size(ndp, network_id, &act_size);
        if (s) goto error;
        len = len * act_size;
        if (len > NDP120_DNN_RESULT_MAX_NUM_ACTIVATIONS * (int) sizeof(uint16_t) ||
                len % 4 != 0) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    } else if (type == SYNTIANT_NDP_STRENGTH_SOFTMAX) {
        if (len > NDP120_DNN_RESULT_MAX_NUM_CLASSES * (int) sizeof(uint32_t) ||
                len % 4 != 0) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    } else {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (type == SYNTIANT_NDP_STRENGTH_RAW) {
        if (ndp->d.ndp120.secure_boot_required || ndp->d.ndp120.chip_is_locked) {
            addr = NDP120_MCU_OPEN_RAM_RESULTS +
                (uint32_t) sizeof (struct syntiant_ndp120_secure_match_data);
            s = syntiant_ndp120_read_block(ndp, 1, addr, strengths, len);
            /* fallthru */
            goto error;
        }

        if (addr) {
            s = syntiant_ndp120_get_result_offset(ndp, &addr, network_id);
            if (s) {
                goto error;
            }
            addr += (uint32_t) offsetof(struct ndp120_dnn_result_s, raw_strengths_ptr);
            s = syntiant_ndp120_read_block(ndp, 1, addr, &raw_strengths_addr,
                sizeof(raw_strengths_addr));
            if (s) {
                goto error;
            }
            s = syntiant_ndp120_read_block(ndp, 1, raw_strengths_addr, strengths, len);
            if (s) goto error;
        } else {
            /* FAIL instead of UNINIT to match 10x ilib code */
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }
    } else {
        if (ndp->d.ndp120.secure_boot_required || ndp->d.ndp120.chip_is_locked) {
            s = syntiant_ndp120_get_activation_size(ndp, network_id, &act_size);
            if (s) goto error;
            len = len * act_size;
            if (len > NDP120_DNN_RESULT_MAX_NUM_CLASSES * (int) sizeof(uint16_t) ||
                len % 4 != 0) {
                s = SYNTIANT_NDP_ERROR_ARG;
                goto error;
            }
            /* softmax strength follows raw strengths */
            addr = NDP120_MCU_OPEN_RAM_RESULTS +
                (uint32_t) sizeof(struct syntiant_ndp120_secure_match_data) + len;
            s = syntiant_ndp120_read_block(ndp, 1, addr, strengths, len);
            /* fallthru */
            goto error;
        }
        if (addr) {
            s = syntiant_ndp120_get_result_offset(ndp, &addr, network_id);
            if (s) {
                goto error;
            }
            addr += (uint32_t) offsetof(struct ndp120_dnn_result_s,
                    softmax_strengths_ptr);
            s = syntiant_ndp120_read_block(ndp, 1, addr,
                &softmax_strengths_addr, sizeof(softmax_strengths_addr));
            if (s) {
                goto error;
            }
            s = syntiant_ndp120_read_block(ndp, 1, softmax_strengths_addr,
                strengths, len);
            if (s) {
                goto error;
            }
        } else {
        /* FAIL instead of UNINIT to match 10x ilib code */
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }
    }

error:
    return s;
}

int syntiant_ndp120_get_activation_size_bytes(struct syntiant_ndp_device_s *ndp,
    uint8_t *act_size)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t network_id = ndp->d.ndp120.last_network_id;

    s = syntiant_ndp120_get_activation_size(ndp, network_id, act_size);
    if (s) {
        DEBUG_PRINTF("Error : unable to get activation size in bytes\n");
    }
    return s;
}

int syntiant_ndp120_get_network_match_strength(
    struct syntiant_ndp_device_s *ndp, uint8_t *strengths,
    int net_id, unsigned int len, int type)
{
    int s;
    uint8_t act_size = 0;
    uint32_t network_id;
    uint32_t addr = ndp->d.ndp120.mcu_fw_state_addr, raw_strengths_addr;
    uint8_t num_networks;

    if ((net_id == -1) || (type != SYNTIANT_NDP_STRENGTH_RAW)) {
        s = syntiant_ndp120_get_match_strength(ndp, strengths, len, type);
        /* fallthru */
        goto error;
    }

    s = syntiant_ndp120_num_networks(ndp, &num_networks);
    if (s) goto error;

    if ((net_id >= 0) && (net_id < (int) num_networks)) {
        network_id = (uint32_t) net_id;
    } else {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    /* check and adjust len if needed */
    s = syntiant_ndp120_get_activation_size(ndp, network_id, &act_size);
    if (s) goto error;
    len = len * act_size;
    if (len > NDP120_DNN_RESULT_MAX_NUM_CLASSES * (int) sizeof(uint16_t) ||
            len % 4 != 0) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (ndp->d.ndp120.secure_boot_required || ndp->d.ndp120.chip_is_locked) {
        addr = NDP120_MCU_OPEN_RAM_RESULTS +
            (uint32_t) sizeof (struct syntiant_ndp120_secure_match_data);
        s = syntiant_ndp120_read_block(ndp, 1, addr, strengths, len);
        /* fallthru */
        goto error;
    }

    if (addr) {
        s = syntiant_ndp120_get_result_offset(ndp, &addr, network_id);
        if (s) {
            goto error;
        }
        addr += (uint32_t) offsetof(struct ndp120_dnn_result_s, raw_strengths_ptr);
        s = syntiant_ndp120_read_block(ndp, 1, addr, &raw_strengths_addr,
            sizeof(raw_strengths_addr));
        if (s){
            goto error;
        }
        s = syntiant_ndp120_read_block(ndp, 1, raw_strengths_addr, strengths, len);
        if (s) {
            goto error;
        }
    } else {
        /* FAIL instead of UNINIT to match 10x ilib code */
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }

error:
    return s;
}

int syntiant_ndp120_init_ring_buffer_pointers_no_sync(struct syntiant_ndp_device_s *ndp,
    int reset)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t address = NDP120_DSP_OPEN_RAM_RESULTS;

    if (reset) {
        ndp120->dsp_pcm_audio_sample_last_ptr = 0;
        ndp120->dsp_function_sample_last_ptr = 0;
        ndp120->dsp_pcm_audio_annotation_last_ptr = 0;
        ndp120->dsp_sensor_sample_last_ptr = 0;
        goto error;
    }

    /*send EXT MB command*/
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_INIT_RING_BUF_PTR, NULL);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_init_ring_buffer_pointers_no_sync\n");
        goto error;
    }
    /*read block*/
    s = syntiant_ndp120_read_block(ndp, 1, address, &ring_buf_ptrs, sizeof(ring_buf_ptrs));
    if (s) goto error;
    /* reinitialize last_ptr to be equal to producer pointer to discard prior
       data, if any */
    ndp120->dsp_pcm_audio_sample_last_ptr = ring_buf_ptrs.pcm_audio_sample_last_ptr;
    ndp120->dsp_function_sample_last_ptr = ring_buf_ptrs.function_sample_last_ptr;
    ndp120->dsp_pcm_audio_annotation_last_ptr = ring_buf_ptrs.pcm_audio_annotation_last_ptr;
    ndp120->dsp_sensor_sample_last_ptr = ring_buf_ptrs.sensor_sample_last_ptr;

error:
    return s;
}

int syntiant_ndp120_init_ring_buffer_pointers(struct syntiant_ndp_device_s *ndp, int reset)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_init_ring_buffer_pointers_no_sync(ndp, reset);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

enum syntiant_transer_max_block_size_ {
    SYNTIANT_TRANSFER_MAX_BLOCK_SIZE = 2048
};

int syntiant_ndp120_read_block(struct syntiant_ndp_device_s *ndp,
    int mcu, uint32_t address, void *value, unsigned int count)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned chunk;

    while (count) {
        chunk = SYNTIANT_TRANSFER_MAX_BLOCK_SIZE -
            (address % SYNTIANT_TRANSFER_MAX_BLOCK_SIZE);
        if (count < chunk) {
            chunk = count;
        }
        s = ndp->iif.transfer(ndp->iif.d, mcu, address, NULL, value, chunk);
        if (s) break;
        value = (uint8_t *)value + chunk;
        address += chunk;
        count -= chunk;
    }

    return s;
}

int syntiant_ndp120_write_block(struct syntiant_ndp_device_s *ndp,
    int mcu, uint32_t address, const void *value, unsigned int count)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned chunk;

    while (count) {
        chunk = SYNTIANT_TRANSFER_MAX_BLOCK_SIZE -
            (address % SYNTIANT_TRANSFER_MAX_BLOCK_SIZE);
        if (count < chunk) {
            chunk = count;
        }
        s = ndp->iif.transfer(ndp->iif.d, mcu, address, value, NULL, chunk);
        if (s) break;
        value = (uint8_t *)value + chunk;
        if (mcu || address != NDP120_SPI_SAMPLE) {
            address += chunk;
        }
        count -= chunk;
    }

    return s;
}

int syntiant_ndp120_read(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, void *value)
{
    return syntiant_ndp120_read_block(ndp, mcu, address, value, OP_SIZE(mcu));
}

int
syntiant_ndp120_write(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, uint32_t value)
{
    return syntiant_ndp120_write_block(ndp, mcu, address, &value, OP_SIZE(mcu));
}

int syntiant_ndp120_config_gpio(
    struct syntiant_ndp_device_s *ndp, const syntiant_ndp120_config_gpio_t *config)
{
    uint32_t data, data_old;
    uint32_t mask, mask_lower, mask_upper;
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (!config->set) goto error;

    mask = (uint32_t) (1u << config->gpio_num);
    mask_lower = mask & 0xffff;
    mask_upper = (mask >> 16) & 0xffff;

    s = ndp_mcu_read(NDP120_CHIP_CONFIG_GPIOSEL, &data);
    if (s) goto error;
    data |= mask;
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_GPIOSEL, data);
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GPIO_DIR) {
        if (config->dir == SYNTIANT_NDP120_CONFIG_VALUE_GPIO_DIR_OUT) {
            if (mask_lower) {
                s = ndp_mcu_write(NDP120_GPIO_OUTSET, mask_lower);
                if (s) goto error;
            } else {
                s = ndp_mcu_write(NDP120_GPIO1_OUTSET, mask_upper);
                if (s) goto error;
            }
        } else {
            if (mask_lower) {
                s = ndp_mcu_write(NDP120_GPIO_OUTCLR, mask_lower);
                if (s) goto error;
            } else {

                s = ndp_mcu_write(NDP120_GPIO1_OUTCLR, mask_upper);
                if (s) goto error;
            }
        }
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GPIO_VALUE) {
        s = ndp_mcu_read(
            mask_lower ? NDP120_GPIO_DATAOUT : NDP120_GPIO1_DATAOUT, &data);
        if (s) goto error;
        data_old = data;

        if (config->value) {
            data |= (mask_lower | mask_upper);
        } else {
            data &= ~(mask_lower | mask_upper);
        }

        if (data != data_old) {
            s = ndp_mcu_write(
                mask_lower ? NDP120_GPIO_DATAOUT : NDP120_GPIO1_DATAOUT, data);
            if (s) goto error;
        }
    }

error:
    return s;
}
static int compute_crc_mcu(struct syntiant_ndp_device_s *ndp, uint32_t adx, uint32_t len, uint32_t *crc_out) {
    uint32_t chunk_size;
#if SYNTIANT_NDP120_DEBUG_SIMULATOR
    uint8_t data[1024]; /* sim runs much faster with large chunk sizes */
#else
    uint8_t data[128];
#endif
    int s = 0;

    *crc_out = crc32_no_lib_init();

    while(len) {
        chunk_size = ndp_umin(sizeof(data), len);
        chunk_size = (chunk_size + 3) / 4 * 4;
        s = ndp_mcu_read_block(adx, data, chunk_size);
        if(s) goto error;
        *crc_out = crc32_no_lib_update(*crc_out, data, chunk_size);
        adx += chunk_size;
        len -= chunk_size;
    }
    *crc_out = crc32_no_lib_finalize(*crc_out);
error:
    return s;
}

static int scratch_get_valid(struct syntiant_ndp_device_s *ndp, uint32_t *valid, int check_crc) {
    uint32_t stored_crc;
    uint32_t calc_crc;
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    DEBUG_PRINTF("##> scratch_get_valid...\n");
    if (syntiant_ndp_boot_from_flash_flag(ndp, 0)) {
        check_crc = 0;
    }

    *valid = 0;
    /* scratch RW is NOP for secure boot */
    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        DEBUG_PRINTF("##> scratch_get_valid: secure boot required or chip is locked\n");
        return s;
    }

    if (check_crc) {
        s = ndp_mcu_read(SCRATCH_CHECKSUM_ADX, &stored_crc);
        if (s) goto error;
        s = compute_crc_mcu(ndp, SCRATCH_CRC_REGION_BEGIN_ADX,
            SCRATCH_CRC_REGION_LENGTH, &calc_crc);
        if (s) goto error;
        if (stored_crc != calc_crc && ndp120->mcu_fw_loaded) {
            /*TODO: may need to revisit this part */
            s = SYNTIANT_NDP_ERROR_CRC;
            DEBUG_PRINTF("CRC mismatch in scratch stored: 0x%08X calc: 0x%08X\n",
                stored_crc, calc_crc);
            goto error;
        }
    }
    s = ndp_mcu_read(SCRATCH_VALID_ADX, valid);
    if(s) goto error;

error:
    return s;
}

int syntiant_ndp120_scratch_get_valid(struct syntiant_ndp_device_s *ndp, uint32_t *valid)
{
    return scratch_get_valid(ndp, valid, 1);
}

int syntiant_ndp120_scratch_get_valid_skip_crc(struct syntiant_ndp_device_s *ndp, uint32_t *valid)
{
    return scratch_get_valid(ndp, valid, 0);
}

int syntiant_ndp120_scratch_set_valid(struct syntiant_ndp_device_s *ndp, uint32_t valid)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t calc_crc;

    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    DEBUG_PRINTF("##> scratch_set_valid (%u)...\n", valid);
    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        DEBUG_PRINTF("##> scratch_set_valid secure boot required OR chip locked\n");
        return s;
    }
    s = ndp_mcu_write(SCRATCH_VALID_ADX, valid);
    if(s) goto error;
    s = compute_crc_mcu(ndp, SCRATCH_CRC_REGION_BEGIN_ADX, SCRATCH_CRC_REGION_LENGTH, &calc_crc);
    if(s) goto error;
    s = ndp_mcu_write(SCRATCH_CHECKSUM_ADX, calc_crc);
    if(s) goto error;

error:
    return s;
}

int syntiant_ndp120_debug_extract(struct syntiant_ndp_device_s *ndp, int type,
        void *data, unsigned int *len)
{

    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    unsigned int l = *len;
    uint32_t addr, addr2;
    int s = SYNTIANT_NDP_ERROR_ARG;

    /* Debug extract is not supported for secured device */
    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        goto error;
    }

    if (!ndp120->mcu_fw_state_addr || !ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }
    /* pr_info("mcu fw addr: 0x%x\n", ndp120->mcu_fw_state_addr);
    pr_info("dsp fw addr: 0x%x\n", ndp120->dsp_fw_state_addr);
    */
    switch (type) {
    case SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_FW_STATE:
        if (l != sizeof(struct ndp120_fw_state_s)) {
            goto error;
        }
        addr = ndp120->mcu_fw_state_addr;
        break;
    case SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_PH_STATE:
        if (l != sizeof(struct ndp120_ph_state_s)) {
            goto error;
        }
        /* TODO: this address needs to be adjusted */
        addr2 = (uint32_t)ndp->d.ndp120.mcu_fw_ph_data_coll_addr;
        s = syntiant_ndp120_read(ndp, 1, addr2, &addr);
        if (s) goto error;
        addr += (uint32_t)sizeof(struct ndp120_ph_params_s);
        break;
    case SYNTIANT_NDP120_DEBUG_EXTRACT_NN_NUM:
        *(uint32_t *) data = ndp120->last_network_id;
        goto error;

    case SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_DSP_FW_STATE:
        if (l != sizeof(ndp120_dsp_fw_state_t)) {
            goto error;
        }
        addr = ndp120->dsp_fw_state_addr;
        break;
    default:
        goto error;
    }

    s = syntiant_ndp_read_block(ndp, 1, addr, data, l);

error:
    return s;
}

/* Fetch number of networks present in device memory */
int syntiant_ndp120_num_networks(struct syntiant_ndp_device_s *ndp,
        uint8_t *num_networks)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    syntiant_ndp120_nn_metadata_header_t header;

    uint32_t addr = ndp120->dsp_fw_state_addr +
        (uint32_t) offsetof(ndp120_dsp_fw_base_t, metadata) +
        (uint32_t) offsetof(ndp120_dnn_metadata_t, version);
    s = syntiant_ndp120_read_block(ndp, 1, addr, &header, sizeof(header));
    if (s) goto error;

    *num_networks = header.nn_num & 0xff;

error:
    return s;
}

/* Return last network ran (fetched from mailbox out event) */
uint32_t syntiant_get_last_network_id(struct syntiant_ndp_device_s *ndp)
{
    return ndp->d.ndp120.last_network_id;
}

uint32_t syntiant_ndp120_get_nn_graph(struct syntiant_ndp_device_s *ndp)
{
    return ndp->d.ndp120.mcu_fw_orchestrator_graph_addr;
}
int syntiant_ndp120_read_dnn_state(struct syntiant_ndp_device_s *ndp,
    uint8_t nn_id, ndp120_dnn_network_state_t *dnn_state)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    if (!dnn_state) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
               NDP120_DSP_MB_H2D_EXT_READ_DNN_STATE, NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, NDP120_DSP_OPEN_RAM_RESULTS,
          dnn_state, (uint32_t) (nn_id * sizeof(*dnn_state)));
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_get_dsp_counters(struct syntiant_ndp_device_s *ndp,
        ndp120_dsp_counters_t *dsp_cnts)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    if (!dsp_cnts) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t,
            counters);
    s = syntiant_ndp120_read_block(ndp, 1, adx, dsp_cnts, sizeof(*dsp_cnts));
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_get_dsp_debug_flag(struct syntiant_ndp_device_s *ndp,
        uint32_t *debug_flag)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;

    if (!debug_flag) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t,
            debug_flag);
    s = syntiant_ndp120_read_block(ndp, 1, adx, debug_flag,
            sizeof(*debug_flag));
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_set_dsp_debug_flag(struct syntiant_ndp_device_s *ndp,
        uint32_t debug_flag)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};

    adx = NDP120_DSP_MB_H2D_ADDR;
    payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
            debug_flag);
    s = syntiant_ndp120_write_block(ndp, 1, adx, payload_in_MSB,
            sizeof(payload_in_MSB));
    if (s) {
        goto error;
    }
    s = syntiant_ndp120_do_mailbox_req(ndp,
            NDP120_DSP_MB_H2D_EXT_SET_DSP_DEBUG, NULL);
    if (s) {
        goto error;
    }

error:
    return s;
}

int syntiant_ndp120_get_func_sample_size(struct syntiant_ndp_device_s *ndp,
        uint32_t *sample_size_bytes)
{
    uint32_t adx;
    int s;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    if (!sample_size_bytes) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t,
            config.func_samp_size_bytes);
    s = ndp_mcu_read(adx, sample_size_bytes);
    if (s) goto error;

error:
    return s;
}

int syntiant_ndp120_get_sensor_sample_size(struct syntiant_ndp_device_s *ndp,
        uint32_t *sensor_sample_size)
{
    uint32_t adx;
    int s;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    if (!sensor_sample_size) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    if (!ndp120->dsp_fw_state_addr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx = ndp120->dsp_fw_state_addr + (uint32_t) offsetof(ndp120_dsp_fw_base_t,
            config.sensor_samp_size_bytes);
    s = ndp_mcu_read(adx, sensor_sample_size);
    if (s) goto error;

error:
    return s;
}

/* This API works for both secure and regular app*/
int syntiant_ndp120_enable_disable_sensor(struct syntiant_ndp_device_s *ndp,
        uint8_t sensor_id, int enable)
{
    int s, s0;
    uint32_t adx = NDP120_DSP_MB_H2D_ADDR;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    if (!sensor_id) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
    /* write the msg,sensor_id and enable bit to NDP120_DSP_MB_H2D_ADDR
     * */
    payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
            (uint32_t)sensor_id);
    payload_in_MSB[1] = (uint32_t)enable;
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, adx,
            payload_in_MSB, sizeof(payload_in_MSB));
    if (s) {
        goto error;
    }

    /* send ext MB msg */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
           NDP120_DSP_MB_H2D_EXT_SENSOR_CONTROL, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static void update_nn_input_type(int data_flow_rule_type, int dsp_data_flow_src_type,
    ndp120_dsp_data_flow_setup_t *flow_setup, syntiant_ndp120_flow_src_type_t *nn_input_type)
{
    int i,j;
    uint32_t nn_id = 0;
    for (i = 0; i < data_flow_rule_type; i++) {
        ndp120_dsp_data_flow_rule_t *next_flow_rule = NULL;
        ndp120_dsp_data_flow_rule_t *present_flow_rule = NULL;
        if (data_flow_rule_type == NDP120_SENSOR_DATA_FLOW_RULE_MAX) {
            present_flow_rule = &flow_setup->src_sensor[i];
        } else {
            present_flow_rule = &flow_setup->src_pcm_audio[i];
        }

        if (!NDP120_DSP_FLOW_RULE_IS_VALID(*present_flow_rule)) {
            continue;
        }

        switch (present_flow_rule->dst_type) {
            case NDP120_DSP_DATA_FLOW_DST_TYPE_NN:
                nn_id = present_flow_rule->dst_param;
                break;

            case NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION:
                for (j = 0; j < NDP120_FUNC_DATA_FLOW_RULE_MAX; j++) {
                    next_flow_rule = &flow_setup->src_function[j];
                    if (!NDP120_DSP_FLOW_RULE_IS_VALID(*next_flow_rule)) {
                        continue;
                    }

                    if (present_flow_rule->dst_param == next_flow_rule->src_param &&
                        next_flow_rule->dst_type == NDP120_DSP_DATA_FLOW_DST_TYPE_NN) {
                        nn_id = next_flow_rule->dst_param;
                        break;
                    }
                }
                break;

            default:
                continue;
        }
        nn_input_type[nn_id] = dsp_data_flow_src_type;
    }
}


int syntiant_ndp120_get_nn_input_type(struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_flow_src_type_t *nn_input_type)
{
    int s, s0;
    ndp120_dsp_data_flow_setup_t flow_setup;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_dsp_flow_setup_get_no_sync(ndp, &flow_setup);
    if(s) goto error;

    update_nn_input_type(NDP120_SENSOR_DATA_FLOW_RULE_MAX, NDP120_DSP_DATA_FLOW_SRC_TYPE_SENSOR, &flow_setup, nn_input_type);

    update_nn_input_type(NDP120_PCM_DATA_FLOW_RULE_MAX, NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO, &flow_setup, nn_input_type);

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

#define IS_HOST2_MCU(cmd) (cmd < NDP_MBIN_REQUEST_IGNORE_START)

int syntiant_ndp120_send_secure_command(struct syntiant_ndp_device_s *ndp,
        uint8_t cmd, void *data, uint32_t length)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t adx = MCU_OPEN_RAM_WINDOW_LOWER;
    uint32_t ram_size = MCU_OPEN_RAM_WINDOW_UPPER - adx - (unsigned)sizeof(adx) -
        (unsigned)sizeof(length);

    if (ndp120->secure_boot_required || ndp120->chip_is_locked) {
        if (!IS_HOST2_MCU(cmd)) {
            s = SYNTIANT_NDP_ERROR_UNSUP;
            DEBUG_PRINTF("Didn't send command: 0x%x to secured MCU fw\n", cmd);
            goto out;
        }
        /* write payload to open RAM */
        s = ndp_mcu_write(adx, cmd);
        if (s) goto out;
        if (length && !(length % sizeof(uint32_t)) && length < ram_size && data) {
            adx += (unsigned)sizeof(cmd);
            ndp_mcu_write(adx, length);
            adx += (unsigned)sizeof(length);
            s = ndp_mcu_write_block(adx, data, length);
            if (s) goto out;
        }
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, cmd, NULL);
        if (s) goto out;
    } else {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        DEBUG_PRINTF("Command: 0x%x can't be sent to an unsecured MCU fw\n", cmd);
        goto out;
    }
out:
    return s;
}

int syntiant_ndp120_secure_switch_flowset(struct syntiant_ndp_device_s *ndp,
    uint32_t flow_set_id, uint32_t *input_mode)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd[3] = {SYNTIANT_NDP120_SEC_SWITCH_FLOW_SET_ID,
               sizeof(flow_set_id), 0};
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_switch_dnn_flow\n");
        return s;
    }
    cmd[2] = flow_set_id;

    /* write cmd, length of the payload and payload */
    s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
                sizeof(cmd));
    if (s) goto error;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    /* read input_mode */
    s = syntiant_ndp120_read_block(ndp,1, address, input_mode,
                   sizeof(*input_mode));
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_get_info(struct syntiant_ndp_device_s *ndp,
        struct syntiant_ndp120_secure_info *sec_data)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t ver_len = NDP120_MCU_PBI_VER_MAX_LEN;
    uint32_t address = NDP120_MCU_FW_VER_LEN;
    uint32_t cmd[2] = {SYNTIANT_NDP120_SEC_GET_INFO, 0};
    uint32_t sf_len, sensor_info_len;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot;

    s = syntiant_ndp120_get_secured_status_no_sync(ndp, &secure_boot);
    if (s) goto error;
#ifdef EMULATE_SECURE
    secure_boot = 1;
#endif
    ndp120->secure_boot_required  = secure_boot;
    s = device_locked(ndp, &ndp120->chip_is_locked);
    if (s) goto error;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_get_info\n");
	    return s;
    }
    if (sec_data && sec_data->fw_version && sec_data->dsp_fw_version &&
            sec_data->pkg_version && sec_data->labels) {

        /* write secure cmd data */
        s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
                sizeof(cmd));
        if (s) goto error;

        /* send secure command */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
        if (s) goto error;

        /* read MCU returned data */
        s = syntiant_ndp120_read(ndp, 1, address, &sec_data->fw_version_len);
        if (s) goto error;
        if (sec_data->fw_version_len > NDP120_MCU_FW_VER_MAX_LEN) {
            DEBUG_PRINTF("Firmware sent invalid FW version length:%d bytes\n",
                    sec_data->fw_version_len);
            s = SYNTIANT_NDP_ERROR_INVALID_LENGTH;
            goto error;
        }

        address = NDP120_MCU_DSP_FW_VER_LEN;
        s = syntiant_ndp120_read(ndp, 1, address, &sec_data->dsp_fw_version_len);
        if (s) goto error;
        if (sec_data->fw_version_len > NDP120_MCU_DSP_FW_VER_MAX_LEN) {
            DEBUG_PRINTF("Firmware sent invalid DSP FW version length:%d bytes\n",
                    sec_data->dsp_fw_version_len);
            s = SYNTIANT_NDP_ERROR_INVALID_LENGTH;
            goto error;
        }

        address = NDP120_MCU_PKG_VER_LEN;
        s = syntiant_ndp120_read(ndp, 1, address, &sec_data->pkg_version_len);
        if (s) goto error;
        if (sec_data->pkg_version_len > NDP120_MCU_PKG_VER_MAX_LEN) {
            DEBUG_PRINTF("Firmware sent invalid package version length:%d bytes\n",
                    sec_data->pkg_version_len);
            s = SYNTIANT_NDP_ERROR_INVALID_LENGTH;
            goto error;
        }

        address = NDP120_MCU_LABELS_LEN;
        s = syntiant_ndp120_read(ndp, 1, address, &sec_data->labels_len);
        if (s) goto error;
        if (sec_data->labels_len > NDP120_MCU_LABELS_MAX_LEN) {
            DEBUG_PRINTF("Firmware sent invalid labels length:%d bytes\n",
                    sec_data->labels_len);
            s = SYNTIANT_NDP_ERROR_INVALID_LENGTH;
            goto error;
        }

        address = NDP120_MCU_PBI_VER_LEN;
        s = syntiant_ndp120_read(ndp, 1, address, &ver_len);
        if (s) goto error;
        if (ver_len > NDP120_MCU_PBI_VER_MAX_LEN) {
            DEBUG_PRINTF("Firmware sent invalid PBI version length:%d bytes\n",
                    ver_len);
            s = SYNTIANT_NDP_ERROR_INVALID_LENGTH;
            goto error;
        }

        address += (uint32_t) sizeof(ver_len);
        s = syntiant_ndp120_read_block(ndp, 1, address, sec_data->fw_version,
                sec_data->fw_version_len);
        if (s) goto error;

        address += sec_data->fw_version_len;
        s = syntiant_ndp120_read_block(ndp, 1, address,
                sec_data->dsp_fw_version, sec_data->dsp_fw_version_len);
        if (s) goto error;

        address += sec_data->dsp_fw_version_len;
        s = syntiant_ndp120_read_block(ndp, 1, address,
                sec_data->pkg_version, sec_data->pkg_version_len);
        if (s) goto error;

        address += sec_data->pkg_version_len;
        s = syntiant_ndp120_read_block(ndp, 1, address,
                sec_data->labels, sec_data->labels_len);
        if (s) goto error;

        address += sec_data->labels_len;
        s = syntiant_ndp120_read_block(ndp, 1, address,
                sec_data->pbi, ver_len);
        if (s) goto error;

        address += ver_len;
        s = syntiant_ndp120_read(ndp, 1, address, &sec_data->total_nn);
        if (s) goto error;

        address += (uint32_t)sizeof(uint32_t);
        sf_len = (uint32_t)(sec_data->total_nn * sizeof(uint32_t));
        s = syntiant_ndp120_read_block(ndp, 1, address,
            sec_data->scale_factor, sf_len);
        if (s) goto error;

        address += sf_len;
        sensor_info_len = (uint32_t)(SYNTIANT_CONFIG_SENSOR_SENSORS *
                                     sizeof(sf_len));
        s = syntiant_ndp120_read_block(ndp, 1, address,
            sec_data->sensor_info, sensor_info_len);
        if (s) goto error;
    }
    /* we can assume that NN synpkg has been loaded by now */
    if (sec_data->total_nn) {
        ndp120->nn_params_loaded = 1;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_get_nn_graph(struct syntiant_ndp_device_s *ndp,
        struct ndp120_nn_graph *graph)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd[2] = {SYNTIANT_NDP120_SEC_GET_NN_GRAPH, 0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_get_nn_graph\n");
	    return s;
    }

    if (graph) {
        /* write secure cmd data */
        s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
                sizeof(cmd));
        if (s) goto error;

        /* send secure command */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
        if (s) goto error;

        s = syntiant_ndp120_read_block(ndp, 1, address, graph, sizeof(*graph));
        if (s) goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int syntiant_ndp120_secure_send_dsp_flow_cmd(struct syntiant_ndp_device_s *ndp,
        ndp120_dsp_data_flow_setup_t *flow_setup, uint32_t src)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd = SYNTIANT_NDP120_SEC_GET_DSP_FLOW;

    /* write secure cmd */
    s = ndp_mcu_write(addr, cmd);
    if (s) goto error;
    addr += (uint32_t) sizeof(cmd);
    /* write the length of payload */
    s = ndp_mcu_write(addr, (uint32_t) sizeof(src));
    if (s) goto error;
    addr += (uint32_t) sizeof(src);
    /* write payload */
    s = ndp_mcu_write_block(addr, &src, sizeof(src));
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    switch (src) {
        case NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO:
        case NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION:
            s = syntiant_ndp120_read_block(ndp, 1, address, &flow_setup->src_pcm_audio,
                         sizeof(flow_setup->src_pcm_audio));
            if (s) goto error;

            address += (uint32_t)sizeof(flow_setup->src_pcm_audio);

            s = syntiant_ndp120_read_block(ndp, 1, address, &flow_setup->src_function,
                         sizeof(flow_setup->src_function));
            if (s) goto error;
            break;

        case NDP120_DSP_DATA_FLOW_SRC_TYPE_NN:
        case NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU:
            s = syntiant_ndp120_read_block(ndp, 1, address, &flow_setup->src_nn,
                         sizeof(flow_setup->src_nn));
            if (s) goto error;

            address += (uint32_t)sizeof(flow_setup->src_nn);

            s = syntiant_ndp120_read_block(ndp, 1, address, &flow_setup->src_mcu,
                         sizeof(flow_setup->src_mcu));
            if (s) goto error;
            break;
    }
error:
    return s;
}

int syntiant_ndp120_secure_get_dsp_flow(struct syntiant_ndp_device_s *ndp,
        ndp120_dsp_data_flow_setup_t *flow_setup)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_get_dsp_flow\n");
        return s;
    }

    if (flow_setup) {

        s = syntiant_ndp120_secure_send_dsp_flow_cmd(ndp, flow_setup, NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO);
        if(s) goto error;

        s = syntiant_ndp120_secure_send_dsp_flow_cmd(ndp, flow_setup, NDP120_DSP_DATA_FLOW_SRC_TYPE_NN);
        if(s) goto error;
    }

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_get_audio_chunk_size(
    struct syntiant_ndp_device_s *ndp,
    uint32_t *audio_chunk_size)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd[2] = {SYNTIANT_NDP120_SEC_GET_AUDIO_CHUNK_SIZE, 0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_send_audio_extract\n");
        return s;
    }
    if(audio_chunk_size) {
        /* write secure cmd */
        s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
                sizeof(cmd));
        if (s) goto error;

        /* send secure command */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
        if (s) goto error;

        s = syntiant_ndp120_read_block(ndp, 1, address, audio_chunk_size, sizeof(*audio_chunk_size));
        if (s) goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_config_misc_interrupts(
    struct syntiant_ndp_device_s *ndp, uint32_t interrupt, uint32_t enable)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr;
    uint32_t cmd[4] = {SYNTIANT_NDP120_SEC_CFG_MISC_INTERRUPTS,
            sizeof(uint32_t) * 2, 0 , 0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_send_audio_extract\n");
        return s;
    }

    cmd[2] = interrupt;
    cmd[3] = enable;

    addr = NDP120_MCU_OPEN_RAM_BGN;
    s = syntiant_ndp120_write_block(ndp, 1, addr, cmd, sizeof(cmd));
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_get_sensor_sample_size(
    struct syntiant_ndp_device_s *ndp, uint32_t *sensor_sample_size)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd[2] = {SYNTIANT_NDP120_SEC_GET_SNSR_SMP_SIZE, 0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_send_audio_extract\n");
        return s;
    }

    /* write secure cmd */
    s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
            sizeof(cmd));
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, address, sensor_sample_size, sizeof(*sensor_sample_size));
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_get_debug(struct syntiant_ndp_device_s *ndp,
        struct syntiant_ndp120_secure_debug *mcu_dsp_dbg_cnts)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd[2] = {NDP_MBIN_REQUEST_SECURE_GET_DEBUG, 0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t size = sizeof(struct ndp120_debug_cnt_s) + sizeof(ndp120_dsp_counters_t);
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_get_nn_graph\n");
	    return s;
    }

    if (mcu_dsp_dbg_cnts) {
        /* write secure cmd data */
        s = syntiant_ndp120_write_block(ndp, 1, NDP120_MCU_OPEN_RAM_BGN, cmd,
                sizeof(cmd));
        if (s) goto error;

        /* send secure command */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
        if (s) goto error;

        s = syntiant_ndp120_read_block(ndp, 1, address, mcu_dsp_dbg_cnts, size);
        if (s) goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_secure_input_config(struct syntiant_ndp_device_s *ndp,
             uint32_t input_mode)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t cmd = SYNTIANT_NDP120_SEC_INPUT_CONFIG;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;
    uint32_t length = sizeof(input_mode);
    uint32_t configured_input = SYNTIANT_NDP120_GET_INPUT_CONFIG;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_config_secure_input_config\n");
        return s;
    }
    /* write secure cmd */
    s = ndp_mcu_write(addr, cmd);
    if (s) goto error;
    addr += (uint32_t) sizeof(cmd);
    /* write the length of payload */
    s = ndp_mcu_write(addr, length);
    if (s) goto error;
    addr += (uint32_t) sizeof(length);
    /* write payload */
    s = ndp_mcu_write_block(addr, &configured_input, sizeof(configured_input));
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, address, &configured_input, sizeof(configured_input));
    if (s) goto error;

    if (configured_input != input_mode) {
        addr = NDP120_MCU_OPEN_RAM_BGN;
        /* write secure cmd */
        s = ndp_mcu_write(addr, cmd);
        if (s) goto error;
        addr += (uint32_t) sizeof(cmd);
        /* write the length of payload */
        s = ndp_mcu_write(addr, length);
        if (s) goto error;
        addr += (uint32_t) sizeof(length);
        /* write payload */
        s = ndp_mcu_write_block(addr, &input_mode, sizeof(input_mode));
        if (s) goto error;

        /* send secure command */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
        if (s) goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

/* configure posterior class specified by indexed ph/state/class */
int syntiant_ndp120_secure_posterior_config(struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_secure_posterior_config_s *cfg)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_SEC_POSTERIOR_CONFIG, sizeof(*cfg)};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_posterior_config\n");
        return s;
    }

    /* write command and payload length */
    s = ndp_mcu_write_block(addr, cmd, sizeof(cmd));
    if (s) goto error;
    addr += (uint32_t)sizeof(cmd);
    /* write payload */
    s = ndp_mcu_write_block(addr, cfg, sizeof(*cfg));
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    memset(cfg, 0, sizeof(*cfg));
    s = ndp_mcu_read_block(NDP120_MCU_OPEN_RAM_RESULTS, cfg, sizeof(*cfg));
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

/* configure arbitrary posterior handler */
int syntiant_ndp120_secure_custom_posterior_handler(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_posterior_handler_s *cfg)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_SEC_POSTERIOR_HANDLER, sizeof(*cfg)};
    void *payload = cfg + 1;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_posterior_config\n");
        return s;
    }

    /* include payload length */
    cmd[1] += cfg->length;

    /* write command */
    s = ndp_mcu_write_block(addr, cmd, sizeof(cmd));
    if (s) goto error;
    addr += (uint32_t)sizeof(cmd);
    /* write payload */
    s = ndp_mcu_write_block(addr, cfg, cmd[1]);
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    memset(payload, 0, cfg->length);
    s = ndp_mcu_read_block(NDP120_MCU_OPEN_RAM_RESULTS, payload, cfg->length);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

/* modify algo config area */
int syntiant_ndp120_secure_algo_config_area(struct syntiant_ndp_device_s *ndp,
    struct syntiant_algo_area_s *cfg)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_SEC_ALGO_CONFIG_AREA, sizeof(*cfg)};
    void *payload = cfg + 1;
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_posterior_config\n");
        return s;
    }

    /* include payload length */
    cmd[1] += cfg->length;

    /* write command */
    s = ndp_mcu_write_block(addr, cmd, sizeof(cmd));
    if (s) goto error;
    addr += (uint32_t)sizeof(cmd);
    /* write payload */
    s = ndp_mcu_write_block(addr, cfg, cmd[1]);
    if (s) goto error;

    /* send secure command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        SYNTIANT_NDP120_MB_MCU_SECURE_CMD, NULL);
    if (s) goto error;

    memset(payload, 0, cfg->length);
    s = ndp_mcu_read_block(NDP120_MCU_OPEN_RAM_RESULTS, payload, cfg->length);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_config_tank_size(struct syntiant_ndp_device_s *ndp,
             uint32_t pcm_tank_size_in_msec)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx = NDP120_DSP_MB_H2D_ADDR;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_WORDS] = {0};
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    int secure_boot = ndp120->secure_boot_required || ndp120->chip_is_locked;

    if (!secure_boot) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_config_tank_size\n");
        return s;
    }

    payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
            pcm_tank_size_in_msec);
    s = syntiant_ndp120_write_block(ndp, 1, adx, payload_in_MSB,
            sizeof(payload_in_MSB));
    if (s) goto error;

    s = syntiant_ndp120_do_mailbox_req(ndp,
            NDP120_DSP_MB_H2D_EXT_SET_TANK_SIZE, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_spi_direct_config(struct syntiant_ndp_device_s *ndp,
             uint32_t threshold_bytes, uint32_t *fifo_threshold_value)
{
    int s0;
    uint32_t msg;
    uint8_t data;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t address = NDP120_MCU_OPEN_RAM_RESULTS;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    /*uint32_t address = SYNTIANT_NDP120_OPEN_RAM_RESULTS;*/
    uint32_t cmd[3] = {SYNTIANT_NDP120_SEC_SPI_DIRECT, sizeof(threshold_bytes), 0};
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_spi_direct_config\n");
        return s;
    }
    cmd[2] = threshold_bytes;
    /* write cmd */
    s = syntiant_ndp120_write_block(ndp, 1, addr, cmd,
						sizeof(cmd));
    if (s) goto error;
    /* send command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
			SYNTIANT_NDP120_MB_MCU_SECURE_CMD, &msg);
    if (s) goto error;

    s = syntiant_ndp120_read_block(ndp, 1, address, fifo_threshold_value,
            sizeof(uint32_t));
    if (s) {
        DEBUG_PRINTF("Error reading the fifo threshold value from open ram\n");
        goto error;
    }

    data = (uint8_t)msg;
    if(data != SYNTIANT_NDP120_MB_MCU_NOP) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        DEBUG_PRINTF("Error in configuring the fifo threshold register\n");
        goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_secure_init_extraction(struct syntiant_ndp_device_s *ndp)
{
    int s0;
    uint32_t msg;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t addr = NDP120_MCU_OPEN_RAM_BGN;
    uint32_t cmd[] = {SYNTIANT_NDP120_SEC_INIT_EXTRACTION, 0};

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_secure_init_extraction\n");
        return s;
    }

    /* write cmd */
    s = syntiant_ndp120_write_block(ndp, 1, addr, cmd, sizeof(cmd));
    if (s) goto error;

    /* send command */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            SYNTIANT_NDP120_MB_MCU_SECURE_CMD, &msg);
    if (s) goto error;


error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int syntiant_ndp120_alloc_algo_config_area_no_sync(
        struct syntiant_ndp_device_s *ndp,
        uint32_t algo_config_idx, uint32_t size)
{
    uint32_t adx;
    uint32_t total_size = 0;
    int i, s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};
    uint32_t algo_config_sizes[NDP120_DSP_ALGO_CONFIG_MAX_COUNT];

    /* validate algo config index and algo area size alignment */
    /* read total size of allocated algo config areas in th device */
    adx = ndp->d.ndp120.dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, algo_config_size);
    s = syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_MCU, adx,
            algo_config_sizes, sizeof(algo_config_sizes));
    if (s) goto out;
    if (algo_config_sizes[algo_config_idx]) {
        s = SYNTIANT_NDP_WARN_CONFIG_EXISTS;
        goto out;
    }
    for (i = 0; i < NDP120_DSP_ALGO_CONFIG_MAX_COUNT; i++) {
        total_size += algo_config_sizes[i];
    }
    if (UNALIGNED_SIZE(size, SYNTIANT_NDP120_DSP_ALGO_CONFIG_AREA_SIZE_ALIGN) ||
          NDP120_DSP_ALGO_CONFIG_TOTAL_SIZE <= total_size) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    /* write the msg and allocation size to NDP120_DSP_MB_H2D_ADDR
     * Ideally, we should add a new mailbox API for extended messages
     * */
    adx = NDP120_DSP_MB_H2D_ADDR;
    payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
            algo_config_idx);
    payload_in_MSB[1] = size;
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, adx,
            payload_in_MSB, sizeof(payload_in_MSB));
    if (s) goto out;

    /* send ext MB msg */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
           NDP120_DSP_MB_H2D_EXT_ALGO_CONFIG_ALLOC, NULL);
    if (s) goto out;

out:
    return s;
}

int syntiant_ndp120_alloc_algo_config_area(struct syntiant_ndp_device_s *ndp,
        uint8_t algo_config_idx, uint32_t size)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    if (algo_config_idx > NDP120_DSP_ALGO_CONFIG_MAX_COUNT) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_alloc_algo_config_area\n");
        goto out;
    }
    s = syntiant_ndp120_alloc_algo_config_area_no_sync(ndp, algo_config_idx,
            size);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

static int syntiant_ndp120_erase_algo_config_areas_no_sync(
    struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};

    /* write the msg and allocation size to NDP120_DSP_MB_H2D_ADDR
     * Ideally, we should add a new mailbox API for extended messages
     * */
    adx = NDP120_DSP_MB_H2D_ADDR;
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, adx,
            payload_in_MSB, sizeof(payload_in_MSB));
    if (s) goto out;
    /* send ext MB msg */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
           NDP120_DSP_MB_H2D_EXT_RESET_ALGO_CONFIG, NULL);
    if (s) goto out;
    /* Issue DSP restart */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
           NDP120_DSP_MB_H2D_RESTART, NULL);
    if (s) goto out;
out:
    return s;
}

int syntiant_ndp120_erase_algo_config_areas(struct syntiant_ndp_device_s *ndp)
{
    int s0, s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_erase_algo_config_areas\n");
        goto out;
    }
    s = syntiant_ndp120_erase_algo_config_areas_no_sync(ndp);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

static int syntiant_ndp120_add_flow_input_no_sync(
    struct syntiant_ndp_device_s *ndp, const ndp120_dsp_data_flow_rule_t *rule,
    syntiant_algo_multi_config_params_t *input_data, uint8_t input_count)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t algo_config_sizes[NDP120_DSP_ALGO_CONFIG_MAX_COUNT] = {0};
    syntiant_multi_input_algo_config_t algo_config = {0};
    uint32_t algo_config_index;

    if (!input_count || rule->algo_config_index == -1) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    algo_config_index = (uint32_t)rule->algo_config_index;

    algo_config.signature = CUSTOM_MUTLI_INPUT_ALGO_CONFIG_SIGNATURE;
    algo_config.no_of_inputs = input_count;
    algo_config.exec_type = CUSTOM_MUTLI_INPUT_ALGO_CONFIG_EXEC_TYPE;
    memcpy(&algo_config.data, input_data, input_count * sizeof(*input_data));

    s = syntiant_ndp120_get_algo_config_area_sizes(ndp, algo_config_sizes,
        NDP120_DSP_ALGO_CONFIG_MAX_COUNT);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_get_algo_config_area_sizes\n");
        goto out;
    }

    if (!algo_config_sizes[rule->algo_config_index]) {
        s = syntiant_ndp120_alloc_algo_config_area_no_sync(ndp,
            algo_config_index, CUSTOM_MUTLI_INPUT_ALGO_CONFIG_MAX_SIZE);
        if (s) {
            DEBUG_PRINTF("Error in "
            "syntiant_ndp120_alloc_algo_config_area_no_sync\n");
            goto out;
        }
    }

    s = ndp120_read_write_algo_config_no_sync(ndp, algo_config_index, 0,
        &algo_config, CUSTOM_MUTLI_INPUT_ALGO_CONFIG_MAX_SIZE, 0);
    if (s) {
        DEBUG_PRINTF("Error in ndp120_read_write_algo_config_no_sync\n");
        goto out;
    }

out:
    return s;
}

int syntiant_ndp120_add_flow_input(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule,
    syntiant_algo_multi_config_params_t *input_data, uint8_t input_count)
{
    int s0, s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_add_flow_input\n");
        goto out;
    }
    s = syntiant_ndp120_add_flow_input_no_sync(ndp, rule, input_data,
        input_count);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

static int syntiant_ndp120_set_get_dnn_run_delay_no_sync(
    struct syntiant_ndp_device_s *ndp, const ndp120_dsp_data_flow_rule_t *rule,
    syntiant_ndp120_flow_src_type_t src_type, uint32_t *delay, uint32_t set)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx_w = NDP120_DSP_OPEN_RAM_START;
    uint32_t payload[3];

    if (!delay) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    payload[0] = set;
    payload[1] = *delay;
    payload[2] = src_type;

    /* write set/get, delay and src_type at the start of dsp open ram */
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU,
        adx_w, &payload, sizeof(payload));
    if (s) {
        DEBUG_PRINTF("Error in write block while setting dnn run delay\n");
        goto out;
    }
    adx_w += (uint32_t)sizeof(payload);

    /* write flow rule */
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU,
        adx_w, rule, sizeof(*rule));
    if (s) {
        DEBUG_PRINTF("Error in write block while setting dnn run delay\n");
        goto out;
    }

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_SET_DNN_RUN_DELAY, NULL);
    if (s) {
        DEBUG_PRINTF("Error in mailbox no sync while setting dnn run delay\n");
        goto out;
    }

    if (!set) {
        /* read delay */
        s = syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_MCU,
            NDP120_DSP_OPEN_RAM_RESULTS, delay, sizeof(*delay));
        if (s) {
            DEBUG_PRINTF("Error read block while setting dnn run delay\n");
        }
    }
out:
    return s;
}

int syntiant_ndp120_set_get_dnn_run_delay(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule, syntiant_ndp120_flow_src_type_t
    src_type, uint32_t *delay, uint32_t set)
{
    int s0, s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_set_dnn_run_delay\n");
        goto out;
    }
    s = syntiant_ndp120_set_get_dnn_run_delay_no_sync(ndp, rule, src_type,
        delay, set);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

static int syntiant_ndp120_enable_disable_barge_in_no_sync(struct syntiant_ndp_device_s *ndp,
        int enable)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx;
    uint32_t payload_in_MSB[NDP120_DSP_MB_H2D_SIZE] = {0};

    adx = NDP120_DSP_MB_H2D_ADDR;
    payload_in_MSB[0] = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB[0],
            (uint32_t)enable);
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, adx,
		payload_in_MSB, sizeof(payload_in_MSB));
    if (s) {
        goto error;
    }
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_EXT_SET_BARGE_IN, NULL);
    if (s) {
        goto error;
    }

error:
    return s;
}

int syntiant_ndp120_enable_disable_barge_in(struct syntiant_ndp_device_s *ndp,
        int enable)
{
    int s0, s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_enable_disable_barge_in \n");
        goto out;
    }
    s = syntiant_ndp120_enable_disable_barge_in_no_sync(ndp, enable);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}
static int
syntiant_ndp120_set_get_input_size_no_sync(
    struct syntiant_ndp_device_s *ndp, const ndp120_dsp_data_flow_rule_t *rule,
    syntiant_ndp120_flow_src_type_t src_type, uint32_t *input_size, uint32_t set)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx_w = NDP120_DSP_OPEN_RAM_START;
    uint32_t payload[3];

    if (!input_size) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    payload[0] = set;
    payload[1] = *input_size;
    payload[2] = src_type;

    /* write set/get, input_size and src_type at the start of dsp open ram */
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU,
        adx_w, &payload, sizeof(payload));
    if (s) {
        DEBUG_PRINTF("Error in write block while setting input size\n");
        goto out;
    }
    adx_w += (uint32_t)sizeof(payload);

    /* write flow rule */
    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU,
        adx_w, rule, sizeof(*rule));
    if (s) {
        DEBUG_PRINTF("Error in write block while setting input size\n");
        goto out;
    }

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_MISC_INPUT_SIZE, NULL);
    if (s) {
        DEBUG_PRINTF("Error in mailbox no sync while setting input size\n");
        goto out;
    }

    if (!set) {
        /* read input size */
        s = syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_MCU,
            NDP120_DSP_OPEN_RAM_RESULTS, input_size, sizeof(*input_size));
        if (s) {
            DEBUG_PRINTF("Error read block while getting input size\n");
        }
    }
out:
    return s;
}

int syntiant_ndp120_set_get_input_size(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule, syntiant_ndp120_flow_src_type_t
    src_type, uint32_t *input_size, uint32_t set)
{
    int s0, s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) {
        DEBUG_PRINTF("Error in syntiant_ndp120_set_input_size\n");
        goto out;
    }
    s = syntiant_ndp120_set_get_input_size_no_sync(ndp, rule, src_type,
        input_size, set);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
out:
    return s;
}

#ifdef SYNTIANT_NDP120_DEBUG_NO_TOUCH
void syntiant_ndp120_set_fw_bases(struct syntiant_ndp_device_s *ndp,
                        uint32_t mcu_fw_base, uint32_t dsp_fw_base,
                        uint32_t mcu_fw_dbg_state_base)
{
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    ndp120->mcu_fw_state_addr = mcu_fw_base;
    ndp120->dsp_fw_state_addr = dsp_fw_base;
    ndp120->mcu_fw_dbg_state_addr = mcu_fw_dbg_state_base;
}
#endif

int syntiant_ndp120_status(struct syntiant_ndp_device_s *ndp,
                       struct syntiant_ndp120_status_s *status)
{
    struct syntiant_ndp120_device_s *ndp120 = &ndp->d.ndp120;
    uint32_t a, v;
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (status->set & ~((unsigned int) SYNTIANT_NDP120_STATUS_SET_ALL_M)) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    a = ndp120->mcu_fw_state_addr
        + (unsigned int) offsetof(struct ndp120_fw_state_s,
                                  mb_state.m2h_match_skipped);

    if (status->set & SYNTIANT_NDP120_STATUS_SET_MAILBOX_TRACE) {
        syntiant_ndp120_mailbox_trace = status->mailbox_trace;
    }

    status->mailbox_trace = syntiant_ndp120_mailbox_trace;
    status->h2m_mailbox_req
        = syntiant_ndp120_mailbox_req_number[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_HOST_TO_MCU];
    status->h2m_mailbox_rsp
        = syntiant_ndp120_mailbox_rsp_number[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_HOST_TO_MCU];
    status->h2m_mailbox_unexpected
        = syntiant_ndp120_mailbox_unexpected[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_HOST_TO_MCU];
    status->h2m_mailbox_error
        = syntiant_ndp120_mailbox_error[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_HOST_TO_MCU];
    status->m2h_mailbox_req
        = syntiant_ndp120_mailbox_req_number[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST];
    status->m2h_mailbox_rsp
        = syntiant_ndp120_mailbox_rsp_number[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST];
    status->m2h_mailbox_unexpected
        = syntiant_ndp120_mailbox_unexpected[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST];
    status->m2h_mailbox_error
        = syntiant_ndp120_mailbox_error[
            SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST];

    if (ndp120->mcu_fw_state_addr) {
        s = syntiant_ndp_read(ndp, 1, a, &v);
        if (s) goto error;
        status->missed_frames = v;
    } else {
        status->missed_frames = 0;
    }

    if (status->set & SYNTIANT_NDP120_STATUS_SET_CLEAR) {
        memset(syntiant_ndp120_mailbox_req_number, 0,
               sizeof(syntiant_ndp120_mailbox_req_number));
        memset(syntiant_ndp120_mailbox_rsp_number, 0,
               sizeof(syntiant_ndp120_mailbox_rsp_number));
        memset(syntiant_ndp120_mailbox_unexpected, 0,
               sizeof(syntiant_ndp120_mailbox_unexpected));
        memset(syntiant_ndp120_mailbox_error, 0,
               sizeof(syntiant_ndp120_mailbox_error));
        if (ndp120->mcu_fw_state_addr) {
            s = syntiant_ndp_write(ndp, 1, a, 0);
            if (s) goto error;
        }
    }

error:
    return s;
}

int syntiant_ndp_boot_from_flash_flag(struct syntiant_ndp_device_s *ndp, int set)
{
    if (set) {
        ndp->d.ndp120.boot_from_flash = 1;
    }
    return ndp->d.ndp120.boot_from_flash != 0;
}

static int syntiant_ndp120_soft_flash_boot(struct syntiant_ndp_device_s *ndp)
{
    int s;
    uint32_t data = NDP120_SOFT_FLASH_BOOT_SIG;

    s = syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU,
            NDP120_SOFT_FLASH_BOOT_ADDR, &data, sizeof(data));
    if (s) {
        goto done;
    }

done:
    return s;
}

int syntiant_ndp120_boot_from_flash(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t req;
    uint32_t msg = 0, notify;

    if (ndp->d.ndp120.boot_flag != SYNTIANT_NDP120_SERIAL_BOOT) {
        /* indicate to mcufw to do soft flash boot */
        s = syntiant_ndp120_soft_flash_boot(ndp);
        if (s) {
            DEBUG_PRINTF("set soft flash boot failed %d\n", s);
            /* fall thru anyway */
        }

        /* wait for bootloader to be in BOOTING loop */
        notify = SYNTIANT_NDP_NOTIFICATION_BOOTING;
        s = syntiant_ndp120_poll_notification(ndp, notify);
        if (s) {
            DEBUG_PRINTF("poll notification %#x failed %d\n", notify, s);
            goto error;
        }

        mailbox_reset_state(ndp);

        /* tell bootloader to boot mcufw from flash */
        req = SYNTIANT_NDP120_MB_MCU_BOOT_FROM_FLASH;
        s = syntiant_ndp120_do_mailbox_req(ndp, req, &msg);
        if (s) {
            DEBUG_PRINTF("mb cmd %#x failed %d\n", req, s);
            goto error;
        }
    }

    /* wait until mcufw has loaded dnn synpkg (from flash) */
    notify = SYNTIANT_NDP_NOTIFICATION_OTHER_LOAD_DONE;
    s = syntiant_ndp120_poll_notification(ndp, notify);
    if (s) {
        DEBUG_PRINTF("poll notification %#x failed %d\n", notify, s);
        goto error;
    }

    req = SYNTIANT_NDP120_MB_MCU_MIADDR;
    s = syntiant_ndp120_do_mailbox_req(ndp, req, &msg);
    if (s) {
        DEBUG_PRINTF("mb cmd %#x failed %d\n", req, s);
        goto error;
    }

    req = SYNTIANT_NDP120_MB_DSP_ADX_LOWER;
    s = syntiant_ndp120_do_mailbox_req(ndp, req, &msg);
    if (s) {
        DEBUG_PRINTF("mb cmd %#x failed %d\n", req, s);
        goto error;
    }

    /* boot from flash succeeded */
    syntiant_ndp_boot_from_flash_flag(ndp, 1);

error:
    return s;
}

int syntiant_ndp120_handle_image(struct syntiant_ndp_device_s *ndp)
{
    if (0x40 <= ndp->device_type && ndp->device_type <= 0x43) {
        return 1;
    } else {
        return 0;
    }
}

int syntiant_ndp120_read_scratch(struct syntiant_ndp_device_s *ndp, void* in,
    uint32_t length)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    if (length > NDP120_ILIB_SCRATCH_LENGTH) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    }
    s = ndp_mcu_read_block(ndp_get_scratch_address(ndp), in, length);
out:
    return s;
}

int syntiant_ndp120_get_active_configurations(
    struct syntiant_ndp_device_s *ndp, int *active_channels)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t idx, mask;
    uint32_t src_params = 0;
    ndp120_dsp_data_flow_setup_t setup;
    int channels_cnt = 0;
    s = syntiant_ndp120_dsp_flow_setup_get(ndp, &setup);
    if (s) {
        DEBUG_PRINTF("Error syntiant_ndp120_dsp_flow_setup_get\n");
        goto out;
    }
    for (idx = 0; idx < NDP120_PCM_DATA_FLOW_RULE_MAX; idx++) {
        if (setup.src_pcm_audio[idx].dst_type ==
            NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT) {
            src_params |= setup.src_pcm_audio[idx].src_param;
        }
    }

    for (idx = 0; idx < FIFO_MIC_CNT; idx++) {
        mask = (uint8_t)(1 << idx);
        if (src_params & mask) {
            channels_cnt++;
        }
    }

    for (idx = 0; idx < NDP120_FUNC_DATA_FLOW_RULE_MAX; idx++) {
        ndp120_dsp_data_flow_rule_t *rule = &setup.src_function[idx];
        if (NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) {
            if (rule->dst_type ==
                NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT &&
                rule->dst_param == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO) {
                channels_cnt++;
            }
        }
    }
    *active_channels =  channels_cnt;

out:
    return s;
}


/* ========================= */
/* EXPORTED DRIVER FUNCTIONS */
/* ========================= */

struct syntiant_ndp_driver_s syntiant_ndp120_driver = {
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_MAJOR_VERSION,    /* major version */
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_MINOR_VERSION,    /* minor version */
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_PATCH_VERSION,    /* patch number */
    syntiant_ndp120_device_types,
    SYNTIANT_NDP_FAMILY_CORE_2,
    syntiant_ndp120_init,
    syntiant_ndp120_uninit,
    syntiant_ndp120_op_size,
    syntiant_ndp120_interrupts,
    syntiant_ndp120_poll,
    syntiant_ndp120_load,
    syntiant_ndp120_get_config,
    syntiant_ndp120_send_data,
    syntiant_ndp120_extract_data,
    syntiant_ndp120_get_match_summary,
    syntiant_ndp120_get_match_binary,
    syntiant_ndp120_get_match_strength,
    syntiant_ndp120_read_block,
    syntiant_ndp120_write_block,
    NULL
};

