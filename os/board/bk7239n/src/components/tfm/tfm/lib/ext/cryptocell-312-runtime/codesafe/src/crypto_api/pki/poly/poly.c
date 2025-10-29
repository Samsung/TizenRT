/*
 * Copyright (c) 2001-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "cc_pal_types.h"
#include "cc_pal_mem.h"
#include "pka.h"
#include "pka_error.h"
#include "pka_hw_defs.h"
#include "mbedtls_cc_poly.h"
#include "mbedtls_cc_poly_error.h"
#include "cc_common.h"
#include "poly.h"

/* Declaration of global buffers defined in common/pka.c */
extern const int8_t regTemps[PKA_MAX_COUNT_OF_PHYS_MEM_REGS];

/* PKA registers required for the operation. We require R0-R5 registers plus
 * the two reserved registers that are always required, PKA_REG_T0/PKA_REG_T1
 */
#define  PRIME_REG  regTemps[0]
#define  NP_REG     regTemps[1]
#define  ACC_REG    regTemps[2]
#define  KEY_R_REG  regTemps[3]
#define  KEY_S_REG  regTemps[4]
#define  DATA_REG   regTemps[5]
#define  POLY_PKA_REGS_NUM (6+2) // +2 temp registers

/* Macro for read non aligned word from RAM */
#define GET_NON_ALIGNED_WORD(m_w0, m_w1, m_shift, m_shift_) \
    (((m_w0)>>(m_shift)) | ((m_w1)<<(m_shift_)))

/* Macro to calc full data blocks from a generic data size */
#define GET_FULL_DATA_BLOCKS_SIZE(data_size) \
    ((data_size/CC_POLY_BLOCK_SIZE_IN_BYTES)*CC_POLY_BLOCK_SIZE_IN_BYTES)

/* Mask for Key "r" buffer: clearing 4 high bits of bytes 3, 7, 11, 15;
 * clearing low 2 bits of bytes 4,8,12
 */
static const uint32_t g_PolyMaskKeyR[CC_POLY_KEY_SIZE_IN_WORDS/2] = {
    0x0fffffff,
    0x0ffffffc,
    0x0ffffffc,
    0x0ffffffc
};
/* POLY PRIME: 2^130-5 = 0x3_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFB */
static const uint32_t g_PolyPrime[POLY_PRIME_SIZE_IN_WORDS] = {
    0xfffffffb,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0x3
};
/* Barrett tag Np = 0x80_00000000_00000000*/
static const uint32_t g_PolyNp[] = {0x00000000, 0x00000000, 0x00000080};

/**
 * @brief The function loads the remaining bytes (not a full block) of data
 *        together with setting of extra bit according to Poly1305 algorithm.
 *
 * @note  Assuming register size is 128+64 bits according to PKA multiplier
 *        64x16, and assumes the PKA engine is already working
 *
 * @param[in] pSrc           Pointer to source (little endian) buffer.
 * @param[in] sizeBytes      Size of remaining data in bytes, should be in range
 *                           [1...15]
 * @param[in] isPolyAeadMode Flag indicating whether to consider the block as a
 *                           16-bytes block padded with zeros towards the MSB
 *
 * @return None
 */
static void PolyAccRemainBlock(const uint8_t *pSrc,
                               uint32_t sizeBytes,
                               bool isPolyAeadMode)
{
    uint32_t tmp[CC_POLY_BLOCK_SIZE_IN_WORDS+2] = {0};
    uint32_t i;
    uint32_t dataRegSramAddr;

    /* load block into words buffer and set high bit 1 */
    CC_PalMemCopy((uint8_t*)tmp, pSrc, sizeBytes);
    if(isPolyAeadMode) {
        sizeBytes = CC_POLY_BLOCK_SIZE_IN_BYTES;
    }
    ((uint8_t*)tmp)[sizeBytes] = 1;

    /* get DATA_REG address */
    PKA_GET_REG_ADDRESS(DATA_REG, dataRegSramAddr/*reg.addr*/);

    /* set address */
    PKA_WAIT_ON_PKA_DONE();
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_ADDR), dataRegSramAddr);

    /* load block into PKA reg. */
    for(i = 0; i < CC_POLY_BLOCK_SIZE_IN_WORDS+2; i++) {
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), tmp[i]);
    }

    /* acumulate the data and do modular multiplication by keyR */
    PKA_MOD_ADD(LEN_ID_N_BITS, ACC_REG, ACC_REG, DATA_REG);
    PKA_MOD_MUL(LEN_ID_N_BITS, ACC_REG, ACC_REG, KEY_R_REG);

    return;
}

/**
 * @brief Function to accumulate full blocks (i.e. multiples of 16 bytes only)
 *        in the Poly1305 algorithm
 *
 * @note  This function assumes the PKA engine is already working.
 *
 * @param[in] pSrc The pointer to the data buffer.
 * @param[in] size The size in bytes of data.
 * 
 * @return CC_OK or error code.
 */
static CCError_t PolyAccCalcFullBlocks(const uint8_t *pSrc, size_t size)
{
    uint32_t shift, shift_; /* shift left (in bits) needed for aligning data to 32-bit words */
    uint32_t remSize, blocksCount;
    uint32_t *pSrc32;
    uint32_t i;
    uint32_t dataRegSramAddr;
    uint32_t word0 = 0;

    /* count of full blocks */
    blocksCount = size / CC_POLY_BLOCK_SIZE_IN_BYTES;
    /* remining data: count of bytes in not full 32-bit word */
    remSize = size % CC_POLY_BLOCK_SIZE_IN_BYTES;
    /* Calling this function for not an integer number of blocks is an error */
    if (remSize) {
        return CC_POLY_DATA_INVALID_ERROR;
    }
    /* Calling this function for zero input size, do nothing and return */
    if (!blocksCount) {
        return CC_OK;
    }

    /* count of non aligned bytes and aligned pointer */
    shift = (size_t)pSrc % CC_32BIT_WORD_SIZE;
    shift_ = CC_32BIT_WORD_SIZE - shift;
    pSrc32 = (uint32_t*)((uint32_t)pSrc - shift);

    /* set first non aligned bytes into word0 */
    if(shift) {

        word0 = (uint32_t)pSrc[0];
        if(shift_ > 1){
            word0 |= ((uint32_t)pSrc[1] << 8);
        }
        if(shift_ > 2){
            word0 |= ((uint32_t)pSrc[2] << 16);
        }

        shift = shift << 3; /*now shift is in bits*/
        shift_ = shift_ << 3;
        word0 <<= shift;
    }

    /*---------------------*/
    /* process full blocks */
    /*---------------------*/

    /* get DATA_REG address */
    PKA_GET_REG_ADDRESS(DATA_REG, dataRegSramAddr/*reg.addr*/);

    for(i = 0; i < blocksCount; i++) {

        /* set address of DATA_REG PKA register */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_ADDR), dataRegSramAddr);
        PKA_WAIT_ON_PKA_DONE();

        /* load block of 4 words */
        if(shift) {
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA),
                                  (uint32_t)GET_NON_ALIGNED_WORD(word0,     pSrc32[1], shift, shift_));
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA),
                                  (uint32_t)GET_NON_ALIGNED_WORD(pSrc32[1], pSrc32[2], shift, shift_));
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA),
                                  (uint32_t)GET_NON_ALIGNED_WORD(pSrc32[2], pSrc32[3], shift, shift_));
            word0  = pSrc32[4];
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA),
                                  (uint32_t)GET_NON_ALIGNED_WORD(pSrc32[3], word0, shift, shift_));
        } else {
            /* write data block */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), pSrc32[0]);
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), pSrc32[1]);
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), pSrc32[2]);
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), pSrc32[3]);
        }
        pSrc32 += CC_POLY_BLOCK_SIZE_IN_WORDS;

        /* set MSBit 129 and zeroe other high bits of register */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), 1UL);
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, PKA_SRAM_WDATA), 0UL);

        /* acumulate the data and do modular multiplication by keyR */
        PKA_MOD_ADD(LEN_ID_N_BITS, ACC_REG, ACC_REG, DATA_REG);
        PKA_MOD_MUL(LEN_ID_N_BITS, ACC_REG, ACC_REG, KEY_R_REG);
    }

    return CC_OK;
}

/**
 * @brief performs internal operations on PKA buffer to calculate the POLY
 *        accumulator, i.e. Acc = ((Acc+block)*r) % p.
 *
 * @param[in] pSrc           The pointer to the data buffer
 * @param[in] size           The size in bytes of data
 * @param[in] isPolyAeadMode Flag indicating whether to consider the last
 *                           (partial) block as a full block padded with
 *                           zeros towards the MSB
 *
 * @return CC_OK On success, otherwise indicates failure
 */
static uint32_t PolyAccCalc(const uint8_t *pSrc, size_t size,
                            bool isPolyAeadMode)
{
    uint32_t blocksCount = size / CC_POLY_BLOCK_SIZE_IN_BYTES;
    uint32_t remSize = size % CC_POLY_BLOCK_SIZE_IN_BYTES;

    CCError_t ret = PolyAccCalcFullBlocks(pSrc, blocksCount * CC_POLY_BLOCK_SIZE_IN_BYTES);
    if (ret != CC_OK) {
        return ret;
    }

    /*-----------------------------------*/
    /* process remaining (not full) block */
    /*-----------------------------------*/
    if (remSize) {
        PolyAccRemainBlock(pSrc + blocksCount*CC_POLY_BLOCK_SIZE_IN_BYTES,
                           remSize, isPolyAeadMode);
    }
    return CC_OK;
}

/**
 * @brief This function performs clamping of the first 128 bit of the key
 *        pair (r,s), i.e. r, as specified by RFC7539.
 *
 * @param[in] pKeyR Pointer to a buffer containing the r value, i.e. 128 bits
 *
 * @return None.
 */
static void poly_clamp_r(uint32_t *pKeyR)
{
    if (pKeyR == NULL) {
        return;
    }

    /* clamp "KeyR"  */
    for (int i = 0; i < CC_POLY_KEY_SIZE_IN_WORDS/2; i++) {
        pKeyR[i] = pKeyR[i] & g_PolyMaskKeyR[i];
    }
}

/**
 * @brief This function performs context restoration on the PKA registers. It
 *        must be called with the PKA Mutex already taken to avoid undefined
 *        behaviour. The key in the ctx must be already clamped as per RFC7539
 *
 * @param[out] ctx Pointer to the PKA context to be used to restore the
 *                 registers from, i.e. (ACC_REG, KEY_R_REG, KEY_S_REG)
 *
 * @return  CC_OK On success, otherwise indicates failure
 */
static CCError_t poly_restore_context(PolyPkaContext_t *ctx)
{
    if (ctx == NULL) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    uint32_t *pKeyR = ctx->key;
    uint32_t *pKeyS = ctx->key + CC_POLY_KEY_SIZE_IN_WORDS/2;
    uint32_t *acc = ctx->acc;

    /* clear ACC_REG register */
    PKA_CLEAR(LEN_ID_MAX_BITS, ACC_REG );

    /* set values in PKA register for the MAC operation: */
    /* Set the prime number to ((1<<130) -5) */
    PkaCopyDataIntoPkaReg(PRIME_REG, LEN_ID_MAX_BITS, g_PolyPrime, CALC_32BIT_WORDS_FROM_BYTES(sizeof(g_PolyPrime)));

    /* Calculate NP for modulus operation */
    PkaCopyDataIntoPkaReg(NP_REG, LEN_ID_MAX_BITS, g_PolyNp, CALC_32BIT_WORDS_FROM_BYTES(sizeof(g_PolyNp)));

    /* Copy accumulator value into PKA accumulator register */
    PkaCopyDataIntoPkaReg(ACC_REG, LEN_ID_MAX_BITS, acc, CALC_32BIT_WORDS_FROM_BYTES(sizeof(ctx->acc)));
    /* Copy pKeyR to PKA register #2 */
    PkaCopyDataIntoPkaReg(KEY_R_REG, LEN_ID_MAX_BITS, pKeyR, CC_POLY_KEY_SIZE_IN_WORDS/2);
    /* Copy pKeyS to PKA register #3 */
    PkaCopyDataIntoPkaReg(KEY_S_REG, LEN_ID_MAX_BITS, pKeyS, CC_POLY_KEY_SIZE_IN_WORDS/2);

    /* clear other registers  */
    PKA_CLEAR(LEN_ID_MAX_BITS, DATA_REG);
    PKA_CLEAR(LEN_ID_MAX_BITS, PKA_REG_T0);
    PKA_CLEAR(LEN_ID_MAX_BITS, PKA_REG_T1);

    return CC_OK;
}

/**
 * @brief This function performs context saving from the PKA registers. It
 *        must be called with the PKA Mutex already taken to avoid undefined
 *        behaviour.
 *
 * @param[out] ctx Pointer to the PKA context to be used to store the ACC reg
 *
 * @return CC_OK On success, otherwise indicates failure
 */
static CCError_t poly_save_context(PolyPkaContext_t *ctx)
{
    if (ctx == NULL) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    uint32_t *acc = ctx->acc;

    /* copy ACC register into the PKA context structure */
    PkaCopyDataFromPkaReg(acc, CALC_32BIT_WORDS_FROM_BYTES(sizeof(ctx->acc)), ACC_REG);

    return CC_OK;
}

/**
 * @brief This function performs the setup steps required to implement
 *        the Poly1305 algorithm, i.e. register initialisations. The function
 *        also performs the clamping of the key as specified by RFC7539 before
 *        setting the key onto the PKA regs
 *
 * @note The PKA engine must be already initialised before calling this function
 *
 * @param[in] key Pointer to the buffer containing the 256 bit key pair (r,s)
 *
 * @return None
 */
static void PolyMacCalc_setup(mbedtls_poly_key key, uint32_t *pkaRegsNum)
{
    uint32_t *pKeyR = key;
    uint32_t *pKeyS = pKeyR + CC_POLY_KEY_SIZE_IN_WORDS/2;

    /* Clamp "KeyR" */
    poly_clamp_r(pKeyR);

    /* Set values in PKA register for the MAC operation: */
    /* Set the prime number to ((1<<130) -5) */
    PkaCopyDataIntoPkaReg(PRIME_REG, LEN_ID_MAX_BITS, g_PolyPrime, CALC_32BIT_WORDS_FROM_BYTES(sizeof(g_PolyPrime)));

    /* Calculate NP for modulus operation */
    PkaCopyDataIntoPkaReg(NP_REG, LEN_ID_MAX_BITS, g_PolyNp, CALC_32BIT_WORDS_FROM_BYTES(sizeof(g_PolyNp)));

    /* Copy pKeyR to PKA register #2 */
    PkaCopyDataIntoPkaReg(KEY_R_REG, LEN_ID_MAX_BITS, pKeyR, CC_POLY_KEY_SIZE_IN_WORDS/2);
    /* Copy pKeyS to PKA register #3 */
    PkaCopyDataIntoPkaReg(KEY_S_REG, LEN_ID_MAX_BITS, pKeyS, CC_POLY_KEY_SIZE_IN_WORDS/2);

    /* clear some registers  */
    PKA_CLEAR(LEN_ID_MAX_BITS, DATA_REG);
    PKA_CLEAR(LEN_ID_MAX_BITS, ACC_REG);
    PKA_CLEAR(LEN_ID_MAX_BITS, PKA_REG_T0);
    PKA_CLEAR(LEN_ID_MAX_BITS, PKA_REG_T1);
}

/**
 * @brief This function performs the finalization of the Poly1305 operation by
 *        doing "a += s", i.e. it returns the accumulator value after having
 *        added the 128 bit s part of the key previously setup on PKA registers.
 *        The PKA engine must be explicitly released after a call to this
 *        function.
 *
 * @param[out] macRes Pointer to the buffer to hold the produced 16 bytes tag.
 *
 * @return None
 */
static void PolyMacCalc_finalize(mbedtls_poly_mac macRes)
{
    /* acc = acc+pkeyS */
    PKA_ADD(LEN_ID_N_BITS, ACC_REG, ACC_REG, KEY_S_REG);

    /* copy acc into macRes */
    PkaCopyDataFromPkaReg(macRes, CC_POLY_MAC_SIZE_IN_WORDS, ACC_REG);
}

/*********** Public functions ***********/

CCError_t PolyMacCalc(mbedtls_poly_key key,
                      const uint8_t *pAddData,
                      size_t addDataSize,
                      const uint8_t *pDataIn,
                      size_t dataInSize,
                      mbedtls_poly_mac macRes,
                      bool isPolyAeadMode)
{
    uint32_t rc = CC_FAIL;
    uint32_t lastBlock[CC_POLY_BLOCK_SIZE_IN_WORDS];
    uint32_t pkaRegsNum = POLY_PKA_REGS_NUM;

    /* verify inputs for the key and output tag */
    if ((key == NULL) || (macRes == NULL)) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    /* Check that if pointers are NULL sizes are also zero for data and aad */
    if (((pDataIn == NULL) && (dataInSize != 0)) ||
        ((pAddData == NULL) && (addDataSize != 0))) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    /* Initialize the PKA engine and obtain the mutex on success */
    rc = PkaInitAndMutexLock(POLY_PRIME_SIZE_IN_BITS, &pkaRegsNum);
    if (rc != CC_OK) {
        return rc;
    }

    /* Setup the registers of the PKA engine. It also clamps the key as
     * specified by RFC7539. This procedure can't return failure.
     */
    PolyMacCalc_setup(key, &pkaRegsNum);

    /* process the additional Data */
    if (addDataSize) {
        rc = PolyAccCalc(pAddData, addDataSize, isPolyAeadMode);
        if (rc != CC_OK) {
            goto end_func;
        }
    }
    /*  process the DataIn input */
    if (dataInSize) {
        rc = PolyAccCalc(pDataIn, dataInSize, isPolyAeadMode);
        if (rc != CC_OK) {
            goto end_func;
        }
    }

    /* The last block needs to be formatted as RFC7539 if AEAD mode is set */
    if (isPolyAeadMode) {
        /* Fill lastBlock with 64-bit LE words: addDataSize | dataInSize */
        lastBlock[0] = addDataSize;
        lastBlock[1] = 0;
        lastBlock[2] = dataInSize;
        lastBlock[3] = 0;

        rc = PolyAccCalc((uint8_t*)lastBlock, CC_POLY_BLOCK_SIZE_IN_BYTES, false);
        if (rc != CC_OK) {
            goto end_func;
        }
    }

    /* Finalize macRes by adding KEY_S_REG to ACC_REG. Can't fail. */
    PolyMacCalc_finalize(macRes);

end_func:
    PkaFinishAndMutexUnlock(pkaRegsNum);

    return rc;
}

CCError_t PolyInit(PolyState_t *state, const uint8_t *key, size_t key_size)
{
    if ((key == NULL) || (key_size != CC_POLY_KEY_SIZE_IN_BYTES)) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    /* Copy the 256-bit pair (r,s) in the context and clamp first 128 bits */
    CC_PalMemCopy(state->context.key, key, sizeof(state->context.key));
    poly_clamp_r(state->context.key);

    /* Re-set ACC value in the context */
    CC_PalMemSetZero(state->context.acc, sizeof(state->context.acc));

    /* Reset the stored message chunk, CC_POLY_BLOCK_SIZE_IN_BYTES bytes */
    CC_PalMemSetZero(state->msg_state, sizeof(state->msg_state));

    /* Reset the current size of the buffered message */
    state->msg_state_size = 0;

    return CC_OK;
}

CCError_t PolyUpdate(
    PolyState_t *state,
    const uint8_t *data,
    size_t data_size,
    bool isPolyAeadMode)
{
    size_t data_to_add = 0;
    uint8_t *pStateByte = NULL;
    CCError_t rc = CC_FAIL;
    uint32_t pkaRegsNum = POLY_PKA_REGS_NUM;

    if ((state == NULL) || (data == NULL && data_size != 0)) {
        return CC_POLY_DATA_INVALID_ERROR;
    }
    pStateByte = (uint8_t *) &state->msg_state[0];

    /* Ad additional data of size 0 is a legitimate case */
    if (data_size == 0) {
        return CC_OK;
    }

    /* If we don't need to pad to a block for this call, it's fine to just
     * cache the bytes received and leave the accumulation to a later call
     */
    if (!isPolyAeadMode) {
        if (data_size + state->msg_state_size < CC_POLY_BLOCK_SIZE_IN_BYTES) {
            for (int i=0; i<data_size; i++) {
                pStateByte[i + state->msg_state_size] = data[i];
            }
            state->msg_state_size += data_size;
            return CC_OK;
        }
    }

    if (state->msg_state_size != 0) {
        /* Fill one block of cached data */
        data_to_add = CC_POLY_BLOCK_SIZE_IN_BYTES - state->msg_state_size;
        if (data_to_add > data_size) {
            /* This can happen only when isPolyAeadMode is set, and the
             * difference between data_to_add and data_size must be padded with
             * zeros in the cached data state
             */
            size_t num_of_zeros = data_to_add - data_size;
            for (int i = num_of_zeros; i != 0; i--) {
                pStateByte[CC_POLY_BLOCK_SIZE_IN_BYTES-i] = 0;
            }
            /* Copy all the available data in this case, but no more */
            data_to_add = data_size;
        }
        CC_PalMemCopy(&pStateByte[state->msg_state_size], data, data_to_add);

        data += data_to_add;
        data_size -= data_to_add;

        state->msg_state_size = CC_POLY_BLOCK_SIZE_IN_BYTES;
    }

    /* Init PKA engine and take the mutex */
    rc = PkaInitAndMutexLock(POLY_PRIME_SIZE_IN_BITS, &pkaRegsNum);
    if (rc != CC_OK) {
        return rc;
    }

    /* Restore the context on the PKA engine */
    poly_restore_context(&state->context);

    if (state->msg_state_size == CC_POLY_BLOCK_SIZE_IN_BYTES) {
        rc = PolyAccCalcFullBlocks(pStateByte, CC_POLY_BLOCK_SIZE_IN_BYTES);
        if (rc != CC_OK) {
            goto cleanup;
        }
        /* The state has been consumed */
        state->msg_state_size = 0;
    }

    /* Again if we don't need to pad to a block for this call, just cache the
     * remaining data and leave the accumulation to a later call
     */
    if (!isPolyAeadMode) {
        if (data_size < CC_POLY_BLOCK_SIZE_IN_BYTES) {
            CC_PalMemCopy(&pStateByte[0], data, data_size);
            state->msg_state_size = data_size;
            goto store_and_cleanup;
        }
    }

    rc = PolyAccCalcFullBlocks(data, GET_FULL_DATA_BLOCKS_SIZE(data_size));
    if (rc != CC_OK) {
        goto cleanup;
    }

    data += GET_FULL_DATA_BLOCKS_SIZE(data_size);
    data_size -= GET_FULL_DATA_BLOCKS_SIZE(data_size);

    if (data_size) {
        if (!isPolyAeadMode) {
            CC_PalMemCopy(&pStateByte[0], data, data_size);
            state->msg_state_size = data_size;
        } else {
            /* If isPolyAeadMode is true, that means we need to pad the block
             * to 16 bytes with zeros. This should happen only when the AD
             * data or the payload data authentication get to its last call
             */
            PolyAccRemainBlock(data, data_size, true);
        }
    }

store_and_cleanup:
    poly_save_context(&state->context);
cleanup:
    PkaFinishAndMutexUnlock(pkaRegsNum);
    return rc;
}

CCError_t PolyFinish(PolyState_t *state, uint8_t *tag, size_t tag_size)
{
    CCError_t rc = CC_FAIL;
    uint32_t pkaRegsNum = POLY_PKA_REGS_NUM;

    if ((tag == NULL) || (tag_size < CC_POLY_BLOCK_SIZE_IN_BYTES)) {
        return CC_POLY_DATA_INVALID_ERROR;
    }

    /* Init PKA engine and take the mutex */
    rc = PkaInitAndMutexLock(POLY_PRIME_SIZE_IN_BITS, &pkaRegsNum);
    if (rc != CC_OK) {
        return rc;
    }

    /* Restore the context on the PKA engine */
    poly_restore_context(&state->context);

    /* Accumulate the last partial block */
    if (state->msg_state_size) {
        PolyAccRemainBlock((const uint8_t *)state->msg_state,
                           state->msg_state_size, false);
    }

    /* Add the s part of the key to the value of the accumulator */
    PKA_ADD(LEN_ID_N_BITS, ACC_REG, ACC_REG, KEY_S_REG);

    poly_save_context(&state->context);

    PkaFinishAndMutexUnlock(pkaRegsNum);

    CC_PalMemCopy(tag, state->context.acc, sizeof(state->context.acc));

    return CC_OK;
}
