/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef DUBHE_HASH_H
#define DUBHE_HASH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_CE_HASH_MAGIC 0xdeadbeee
#define DBH_HASH_INFO_WM      ( 0x13000 )
/* Error number */
#define DBH_HASH_MEM_ALLOC_ERR -0x13001
#define DBH_HASH_VECT_SRC_ERR -0x13002
#define DBH_HASH_MODE_ERR -0x13003
#define DBH_HASH_PARAM_INVAL_ERR -0x13004
#define DBH_HASH_BUS_ERR -0x13006
#define DBH_HASH_INVALID_CMD_ERR -0x13007
#define DBH_HASH_PADDING_ERR -0x13008

/* HASH interrupt state */
#define DBH_HASH_CMD_INTR 0x01
#define DBH_HASH_INVALID_CMD_INTR 0x02
#define DBH_HASH_PADDING_ERR_INTR 0x04
#define DBH_HASH_BUS_ERR_INTR 0x08
#define DBH_HASH_WM_INTR 0x10

#if defined( DUBHE_FOR_RUNTIME )
#define INTR_THRESHOLD 512
#endif

/* Hash hardware command opcode */
#define DBH_HASH_INIT_OPCODE 0x80
#define DBH_HASH_PROCESS_OPCODE 0x40
#define DBH_HASH_FINISH_OPCODE 0x20

#define DBH_HASH_OPCODE_BIT_SHIFT 0x18
#define DBH_HASH_MODE_BIT_SHIFT 0x5

#define DBH_HASH_VECT_SRC_INTERNAL 0x00
#define DBH_HASH_VECT_SRC_EXTERNAL 0x10

#define DBH_HASH_ENGINE_BUSY -1
#define DBH_HASH_ENGINE_IDLE 0

#define DBH_HASH_SHA1_TEMP_DATA_LEN 0x14
#define DBH_HASH_SHA224_TEMP_DATA_LEN 32
#define DBH_HASH_SHA256_TEMP_DATA_LEN 32
#define DBH_HASH_SM3_TEMP_DATA_LEN 32

#define DBH_HASH_IV_IS_ADDR 0x1
#define DBH_HASH_IV_IS_VALUE 0x0

#define ARM_HASH_IV_IS_ADDR DBH_HASH_IV_IS_ADDR
#define ARM_HASH_IV_IS_VALUE DBH_HASH_IV_IS_VALUE

/* Queue depth */
#define DBH_HASH_QUEUE_DEPTH 0x2
#define HASH_BUF_LEN 64

typedef enum {
    ARM_HASH_VECT_DEFAULT = 0,
    ARM_HASH_VECT_EXTERNAL,
} arm_ce_hash_vect_source_t;

typedef enum {
    ARM_HASH_MODE_SHA1 = 0,
    ARM_HASH_MODE_SHA224,
    ARM_HASH_MODE_SHA256,
    ARM_HASH_MODE_SM3,
} arm_ce_hash_mode_t;

typedef enum {
    ARM_CE_HASH_ENG_IDLE,
    ARM_CE_HASH_ENG_WAIT_FOR_CMD,
    ARM_CE_HASH_ENG_NORMAL_OPTS,
} arm_ce_hash_engine_status_t;

typedef struct arm_ce_hash_context {
    volatile uint32_t count;   /* Used to store the extra data count */
    uint32_t initialized_flag; /* Indicates whether dubhe is initialized */
    unsigned char extra_data[HASH_BUF_LEN]; /* Buffer to store the extra data*/
    unsigned char
        hash_temp[HASH_BUF_LEN / 2]; /* Buffer to store hash temp value */
    uint32_t hash_temp_valid;
    volatile uint64_t total_bits_num; /* Total process data bits number */
    arm_ce_hash_mode_t mode;
    uint32_t finish_flag; /* This flag indicates if the context need to finish.
                           * 0 not, 1 need to do. */
    void *prv;
} arm_ce_hash_context_t;

/**
 * \brief          This function initializes Dubhe HASH module.
 *
 */

void arm_ce_hash_driver_init( void );

/**
 * \brief          This function initializes a arm_ce_hash_context.
 *
 * \param ctx      The arm_ce_hash_context to initialize.
 *
 */
int arm_ce_hash_init( arm_ce_hash_context_t *ctx );

/**
 * \brief          This function clone the state of a  arm_ce_hash_context
 * context.
 *
 * \param dst      The arm_ce_hash_context to clone to.
 * \param src      The arm_ce_hash_context to clone from.
 *
 */
int arm_ce_hash_clone( arm_ce_hash_context_t *dst, arm_ce_hash_context_t *src );

/**
 * \brief          This function starts a hash checksum calculation.
 *
 * \param ctx      The arm_ce_hash_context to initialize.
 *
 * \return         \c 0 on success.
 *
 */
int arm_ce_hash_start( arm_ce_hash_context_t *ctx,
                       arm_ce_hash_vect_source_t vector,
                       int vect_is_addr,
                       unsigned char *external_vector,
                       size_t iv_len,
                       arm_ce_hash_mode_t hash_mode );

/**
 * \brief          This function feeds an input buffer into Dubhe engine for an
 * ongoing hash checksum calculation.
 *
 * \param ctx      The arm_ce_hash_context.
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 *
 * \return         \c 0 on success.
 */

int arm_ce_hash_update( arm_ce_hash_context_t *ctx,
                        const unsigned char *input,
                        size_t ilen );

/**
 * \brief          This function finishes the hash operation, and writes
 *                 the result to the output buffer.
 * \param ctx      The arm_ce_hash_context.
 * \param output   The hash checksum result.
 *
 * \return         \c 0 on success.
 */

int arm_ce_hash_finish( arm_ce_hash_context_t *ctx, unsigned char *output );

/**
 * \brief          This function clears a hash context, flush all the data
 *                 in Dubhe HASH status.
 *
 * \param ctx      The arm_ce_hash_context to clear.
 *
 */
void arm_ce_hash_free( arm_ce_hash_context_t *ctx );

#ifdef __cplusplus
}
#endif

#endif /* DUBHE_HASH_H */
/*************************** The End Of File*****************************/
