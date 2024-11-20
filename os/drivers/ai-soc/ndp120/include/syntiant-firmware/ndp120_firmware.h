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

#ifndef NDP120_FIRMWARE_H
#define NDP120_FIRMWARE_H

#include "syntiant-firmware/ndp120_result.h"
#include "syntiant-firmware/ndp120_mb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FW_VER "##FIRMWARE_VERSION##"

#define NDP115_ILIB_SCRATCH_ORIGIN (0x300a3bc0U)
#define NDP120_ILIB_SCRATCH_ORIGIN (0x300afbc0U)
#ifdef NDP115A0
#define NDP_ILIB_SCRATCH_ORIGIN   (NDP115_ILIB_SCRATCH_ORIGIN)
#else
#define NDP_ILIB_SCRATCH_ORIGIN   (NDP120_ILIB_SCRATCH_ORIGIN)
#endif

#define NDP120_ILIB_SCRATCH_LENGTH 0x00440

/* mcu open ram contiguous memory space */
#define NDP120_MCU_OPEN_RAM_BGN     (NDP120_MCU_OPEN_RAM_START + NDP120_DSP_MB_H2D_SIZE + NDP120_DSP_MB_D2H_SIZE)
#define NDP120_MCU_OPEN_RAM_MATCH_RESULTS (0x20007794U)
/* last 8 bytes of open ram is used to store mb state in ilib */
#define NDP120_MCU_OPEN_RAM_MATCH_RESULTS_END (0x200077F8U)
#define NDP120_MCU_OPEN_RAM_END     (0x20007800U)
#define NDP120_MCU_TINY_OPEN_RAM_END (0x20007C00U)
#define NDP120_MCU_TINY_OPEN_RAM_MATCH_RESULTS (0x20007B94U)
#define NDP120_MCU_TINY_OPEN_RAM_MATCH_RESULTS_END (0x20007BF8U)

/* soft boot from flash (initiated by tiny app) */
#define NDP120_SOFT_FLASH_BOOT_ADDR    (NDP120_CHIP_CONFIG_SPARE(0))
#define NDP120_SOFT_FLASH_BOOT_SIG      0x53594E54U

/* secure exec */
#define NDP120_MCU_FW_VER_MAX_LEN       (0x20)
#define NDP120_MCU_DSP_FW_VER_MAX_LEN   (0x20)
#define NDP120_MCU_PKG_VER_MAX_LEN      (0x30)
#define NDP120_MCU_LABELS_MAX_LEN       (0x200)
#define NDP120_MCU_PBI_VER_MAX_LEN      (0x10)

/* Open RAM area for retrieving device strings from the host (secure exec) */
#define NDP120_MCU_FW_VER_LEN           (NDP120_MCU_OPEN_RAM_BGN)
#define NDP120_MCU_DSP_FW_VER_LEN       (NDP120_MCU_FW_VER_LEN + sizeof(uint32_t))
#define NDP120_MCU_PKG_VER_LEN          (NDP120_MCU_DSP_FW_VER_LEN + sizeof(uint32_t))
#define NDP120_MCU_LABELS_LEN           (NDP120_MCU_PKG_VER_LEN + sizeof(uint32_t))
#define NDP120_MCU_PBI_VER_LEN          (NDP120_MCU_LABELS_LEN + sizeof(uint32_t))

enum {
    NDP120_FW_STATE_ADDRESS_INDEX_FW_STATE              = 0x0,
    NDP120_FW_STATE_ADDRESS_INDEX_PH_DATA_COLL          = 0x1,
    NDP120_FW_STATE_ADDRESS_INDEX_SMAX_SMOOTHER         = 0x2,
    NDP120_FW_STATE_ADDRESS_INDEX_ORCHESTRATOR_PARAMS   = 0x3,
    NDP120_FW_STATE_ADDRESS_INDEX_DBG_STATE             = 0x4,
    NDP120_FW_STATE_ADDRESS_INDEX_MAX_CNT               = 0x8
};
struct ndp120_fw_state_pointers_s {
    uint32_t addresses[NDP120_FW_STATE_ADDRESS_INDEX_MAX_CNT];
};

enum ndp120_firmware_constants_e {
    NDP120_MATCH_RING_SIZE = 2, /* Match ring size */
    MAX_NNETWORKS = 2,          /* Maximum number of networks */
    MAX_PH_ALGOS = 2,           /* Maximum number of algorithm types*/
    NDP120_MATCH_CONFIDENCE_SHIFT = 8,  /* Shift of confidence level */
    NDP120_MATCH_MISC_EVENT_SHIFT = 16, /* device event shift */
    NDP120_MATCH_MISC_DATA_SHIFT = 24   /* device event payload shift */
};

enum ndp120_secondary_val_type_e {
    NDP120_SEC_VAL_TYPE_NONE = 0,
    NDP120_SEC_VAL_TYPE_ONE_SHOT,
    NDP120_SEC_VAL_TYPE_PATTERN,
    NDP120_SEC_VAL_TYPE_MAX = NDP120_SEC_VAL_TYPE_PATTERN
};

/* @brief Match related data
 */
struct ndp120_fw_match_s {
    uint32_t summary;           /**< match summary */
    uint32_t tankptr;           /**< tank pointer at match */
    uint32_t match_frame_count; /**< frame count at match */
    uint32_t sec_val_status;    /**< indicates status of secondary validation */
};

struct ndp120_misc_posterior {
    uint32_t spl_thresh;
};

/* @brief Parameters for Auto Clearance of Interrupts
 */
struct ndp120_auto_clear {
    uint16_t num_frames;      /**< frame count for interrupt clearance */
    uint16_t match_clear;     /**< auto clear after a match */
    uint16_t int_count;       /**< number of self clearance events happened */
    uint16_t match;           /**< working state of a match detection */
};

struct ndp120_val_type_one_shot_s {
    uint32_t threshold;        /**< threshold for sec val */
    uint32_t ratio_threshold;  /**< ratio threshold for sec val */
    uint16_t num_frames;       /**< gets validation data after num_frames from the match */
    uint8_t notification_type; /**< Indicates to send primary and secondary matches to host */
    uint8_t pad;
};

struct ndp120_val_type_pattern_s {
    uint8_t num_occurrences;   /**< Number of occurrences of matches */
    uint8_t notification_type; /**< Indicates to send primary and secondary matches to host */
    uint16_t frame_range_low;  /**< Minimum frame count gap between each occurrence */
    uint16_t frame_range_high; /**< Maximum frame count gap between each occurrence */
    uint8_t pad[2];
};

struct ndp120_val_patterned_counters_s {
    uint32_t nn_frame_cnt;     /**< stores frame count of each NN */
    uint32_t match_frame_cnt;  /**< stores the latest match frame count */
    uint32_t num_occurrences;  /**< num of occurences of matches */
};

/**
 * @brief Data Structure for storing firmware state.
 * Note: Please do not change this data struture because this struture is used
 * by ilib running on host for operational or debugging purposes.
 * Please add your data after this data structure and set the address for your
 * data in fw_state_pointers.
 *
 */
struct ndp120_fw_state_s {
    /*
     * These members must be in fixed locations and never change.
     * The uILib relies on these members and does not include
     * the firmware header files.
     */
    uint32_t tankptr;     /* tank pointer with all 17 bits */
    uint32_t match_producer[MAX_NNETWORKS];
    struct ndp120_fw_match_s match_ring[MAX_NNETWORKS][NDP120_MATCH_RING_SIZE];

    /*
     * the remaining members should be kept in a stable location but
     * will be accessed through these header files by the full ILib
     * so there is less pain of death to moving them
     */
    uint32_t debug;                   /* debug scratch area */
    uint32_t enable;
    uint32_t prev_enable;
    uint32_t reset;

    uint32_t tank_size;
    uint32_t tank_full;
    uint32_t result_fifo_full;

    uint32_t fw_wake_count;

    /* interrupt counters*/
    uint32_t mb_int_count;
    uint32_t freq_int_count;
    uint32_t dnn_int_count;
    uint32_t unknown_int_count;

    /* firmware version */
    char version[NDP120_MCU_FW_VER_MAX_LEN];

    /* mailbox state */
    struct ndp120_mb_state_s mb_state;

    /* params needed for reading results */
    struct ndp120_nn_output_cache_s nn_output_cache[MAX_NNETWORKS];
    /* base addresses of the results struct created in the DSP */
    uint32_t result[MAX_NNETWORKS];

    struct ndp120_misc_posterior post_settings;
    /* ph_type of posterior handler */
    uint32_t ph_types[MAX_NNETWORKS];
    /* Number of aconfigured networks, must be < MAX_NNETWORKS */
    uint32_t ph_num;
    /* extended MB area for Host <--> MCU */
    uint32_t host2mcu_mb;
    /* auto clear interrupt after num_frames */
    struct ndp120_auto_clear auto_clear;
    /* secondary validation type */
    uint8_t sec_validation_type[MAX_NNETWORKS];
    /* one shot validation type preset fields */
    struct ndp120_val_type_one_shot_s one_shot[MAX_NNETWORKS];
    /* patterned validation type preset fields */
    struct ndp120_val_type_pattern_s patterned[MAX_NNETWORKS];
};

/* max 256 bytes */
struct ndp120_debug_cnt_s {
   uint32_t signature;          /* identifier for this struct */
   uint32_t frame_cnt;          /* Frame count */
   uint32_t dsp2mcu_intr_cnt;   /* DSP 2 MCU int cnt */
   uint32_t dsp2mcu_nn_done_cnt;/* DSP 2 MCU done cnt */
   uint32_t mcu2host_match_cnt; /* MCU 2 Host match cnt */
   uint32_t mcu2host_mpf_cnt;   /* MCU 2 Host match per frame cnt */
   uint32_t mcu2dsp_done_cnt;   /* MCU 2 DSP done cnt */
   uint32_t matches;            /* Matches detected in posterior handler */
   uint32_t dsp2mcu_queue_cnt;  /* DSP 2 MCU queue cnt */
   uint32_t nn_orch_flwchg_cnt; /* Flow map change cnt in NN orchestrator */
   uint32_t unknown_activation_cnt;/* Unknown activation count */
   uint32_t unknown_int_count;  /* Unknown interrupt count */
   uint32_t inv_nn_orch_node_cnt;/* Invalid NN graph node error */
   uint32_t accumulator_error;  /* Counter for tracking accumulator error in softmax */
   uint32_t mbin_int_cnt;       /* MBIN int count */
   uint32_t mbout_int_cnt;      /* MBOUT int count */
   uint32_t buffer_bgn;         /* Debug buffer begin */
   uint32_t buffer_end;         /* Debug buffer end */
   uint32_t curr_ptr;           /* Debug buffer current pointer */
   uint32_t inv_num_class_cnt;  /* Invalid number of classes */
   uint32_t num_frames;         /* maximum frames for logging */
   uint32_t dbg1;
   uint32_t dbg2;
   uint32_t ph_heap_allocation_error; /* Counter for tracking ph heap allocation error */
   uint32_t enable;             /* Debug logging enable flag */
};

typedef enum ndp120_debug_tag_t_ {
   NDP120_MCU_MATCH_TAG = 0xEA,
   NDP120_MCU_PH_TAG = 0xEB,
   NDP120_MCU_ACTIVATION_TAG = 0xEF,
   NDP120_MCU_ORCH_TAG = 0xEC,
   NDP120_MCU_SMOOTHER_TAG = 0xEE,
   NDP120_MCU_SOFTMAX_TAG = 0xE9,
   NDP120_MCU_FRAME_TAG = 0xE8
} ndp120_debug_tag_t;

enum {
    NDP120_DEBUG_STRUCT_SIGNATURE = 0x7EADDEB6U,
    NDP120_DEBUG_TLV_LENGTH = 4,
    NDP120_DEBUG_ENABLE_PH = (1 << 1),
    NDP120_DEBUG_ENABLE_ACTIVATION = (1 << 2),
    NDP120_DEBUG_ENABLE_ORCH = (1 << 3),
    NDP120_DEBUG_ENABLE_SOFTMAX = (1 << 4),
    NDP120_DEBUG_ENABLE_SMOOTHER = (1 << 5),
    NDP120_DEBUG_ENABLE_VERBOSE = (NDP120_DEBUG_ENABLE_PH |
                                NDP120_DEBUG_ENABLE_ACTIVATION |
                                NDP120_DEBUG_ENABLE_ORCH |
                                NDP120_DEBUG_ENABLE_SOFTMAX |
                                NDP120_DEBUG_ENABLE_SMOOTHER
                                )
};

/**
 * @brief enum values to enable/disable various components of firmware. This
 * constants will also be used by ilib code to enable/disable the components.
 *
 * Note: please release header files to ilib if you change this constants.
 */
enum ndp120_fw_state_address_enable_e {
    NDP120_FW_STATE_ENABLE_POSTERIOR = 0x1,
    NDP120_FW_STATE_ENABLE_SMAX_SMOOTHER = 0x2
};

/**
 * @brief enum values to reset various components of firmware. This
 * constants will also be used by ilib code to reset the components.
 *
 * Note: please release header files to ilib if you change this constants.
 */
enum ndp120_fw_state_address_reset_e {
    NDP120_FW_STATE_RESET_POSTERIOR_STATE = 0x1,
    NDP120_FW_STATE_RESET_SMAX_SMOOTHER = 0x2,
    NDP120_FW_STATE_RESET_POSTERIOR_PARAMS = 0x4
};
#ifdef __cplusplus
}
#endif
#endif /* NDP120_FIRMWARE_H */
