/**
 * @file api_val_random.h
 *
 * @brief VAL API - Random related services
 */

/*****************************************************************************
* Copyright (c) 2014-2018 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef INCLUDE_GUARD_API_VAL_RANDOM_H
#define INCLUDE_GUARD_API_VAL_RANDOM_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, u, uint32_t, etc.

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t
#include "api_val_asset.h"          // ValAssetId_t


/** ValTrngDrbgPPTest_t - TRNG DRBG Post-Processing test to perform */
typedef enum
{
    VAL_TRNG_DRBGPP_CF_12B_REPEAT_PATTERN = 0,
    VAL_TRNG_DRBGPP_CF_EXTERNAL_NOISE,
    VAL_TRNG_DRBGPP_KNOWN_ANSWER,
} ValTrngDrbgPPTest_t;


/*-----------------------------------------------------------------------------
 * val_RandomData
 */
/**
 * This function generates a number of bytes (random number/data) from the
 * Deterministic Random Bit Generator (DRBG) into the buffer at Data_p.
 * The implementation is expected to perform post processing as well.
 *
 * @param [in] DataSize
 *     Number of random data bytes.
 *     Note: DataSize <= 65535.
 *
 * @param [in,out] Data_p
 *     Pointer to buffer in which random data will be returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_RandomData(
        const ValSize_t DataSize,
        ValOctetsOut_t * const Data_p);

/*-----------------------------------------------------------------------------
 * val_RandomReseed
 */
/**
 * This function triggers an internal re-seed of the Deterministic Random Bit
 * Generator (DRBG). Use this function to guarantee fresh seed and key material
 * for the post-processor.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_RandomReseed(void);

/*-----------------------------------------------------------------------------
 * val_TrngConfig
 */
/**
 * This function configures and starts the True Random Number Generator (TRNG)
 * and Deterministic Random Bit Generator (DRBG).\n
 * NOTE: The function can only be used by the Crypto Officer, therefore this
 *       function can only be used when Crypto Officer functionality is
 *       enabled.
 *
 * @param [in] AutoSeed
 *     Setting that defines the automatic reseed after 'AutoSeed' times
 *     64K Bytes of DRBG random generation.
 *
 * @param [in] SampleCycles
 *     Setting that controls the number of (XOR-ed) FRO samples XOR-ed
 *     together to generate a single 'noise' bit. This value must be
 *     set such that the total amount of entropy in 8 'noise' bits
 *     equals at least 1 bit.
 *
 * @param [in] SampleDiv
 *     Setting that controls the number of module clock cycles between
 *     samples taken from the FROs.
 *
 * @param [in] NoiseBlocks
 *     Setting that defines number of 512 bit 'noise' blocks to process
 *     through the SHA-256 Conditioning function to generate a single
 *     256 bits 'full entropy' result for (re-)seeding the DRBG.
 *
 * @param [in] fReseed
 *     Re-seed of the DRBG.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_TrngConfig(
        uint8_t  AutoSeed,
        uint16_t SampleCycles,
        uint8_t  SampleDiv,
        uint8_t  NoiseBlocks,
        uint8_t  Scale,			    ////V3.0.2
        bool     fReseed);

/*-----------------------------------------------------------------------------
 * val_TrngDrbgPostProcessing
 */
/**
 * This function performs the TRNG post-processing verification operation that
 * can be used to verify the SHA-2 Conditioning Function and AES-256 based
 * CTR-DRBG random number generator.
 *
 * @param [in] Test
 *     Test to perform:
 *     0 - Conditioning Function with 12 bits repeating pattern
 *     1 - Conditioning Function with externally supplied 'noise' bits
 *     2 - DRBG data generation using known input data
 *
 * @param [in] Pattern
 *     Repeating input pattern for the 'Conditioning Function with 12 bits
 *     repeating pattern' test. Please, read the Firmware Reference Manual
 *     for details.
 *
 * @param [in] Size
 *     The actual value of Size depends on the test that is performed:
 *     0 - Number of 384 bits V & Key output blocks to generate in range 1-255.
 *     1 - Number of 384 bits V & Key output blocks to generate in range 1-255;
 *         the number of 512 bit 'noise blocks' to read equals the value given
 *         here times the NoiseBlocks value in the TRNG configuration token.
 *     2 - Splits the DataSize field in separate sub-fields:
 *         Bits [3:0]   = Total number of 384 bits seed blocks to read (range
 *                        1-15). The first is used for an 'Instantiate'
 *                        function, the others for a 're-seed' function.
 *         Bits [7:4]   = Number of 128-bit DRBG output words to generate in a
 *                        single data block (values 0-15 = 1-16 words).
 *         Bits [11:8]  = Number of data blocks to generate before each re-seed
 *                        (values 0-14 = 1-15 blocks, value 15 = 0 blocks).
 *         Bits [15:12] = Number of data blocks to generate after the last
 *                        re-seed (or 'Instantiate' if bits [3:0] are value 1).
 *                        Values 0-15 generate 1-16 blocks.
 *         Note: Using these sub-fields, several test-scenarios can be
 *               executed. Value 0x0002 performs an 'Instantiate-generate-
 *               reseed-generate' sequence while value 0x1F02 performs an
 *               'Instantiate-reseed-generate-generate' sequence.
 *
 * @param [in] InputData_p
 *     Pointer to buffer with the input data.
 *
 * @param [in] InputDataSize
 *     The size of input data.
 *
 * @param [out] OutputData_p
 *     Pointer to buffer in which the output data must be written.
 *
 * @param [in] OutputDataSize
 *     The size of output data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_TrngDrbgPostProcessing(
        const ValTrngDrbgPPTest_t Test,
        const uint16_t Pattern,
        const ValSize_t Size,
        ValOctetsIn_t * const InputData_p,
        const ValSize_t InputDataSize,
        ValOctetsOut_t * const OutputData_p,
        const ValSize_t OutputDataSize);

/*-----------------------------------------------------------------------------
 * val_TrngNrbgSelfTest
 */
/**
 * This function performs a TRNG hardware self-test verification operation.
 * This test focuses on the following hardware-related functions:
 * - The NIST SP800-90B proposed 'Repetition Count' test on the noise source.
 * - The NIST SP800-90B proposed 'Adaptive Proportion' tests (with 64 and 4K
 *   window sizes) on the noise source. and starts the True Random Number
 *   Generator (TRNG) and Deterministic Random Bit Generator (DRBG).
 *
 * @param [in] InputData_p
 *     Pointer to buffer with the input data.
 *
 * @param [in] Size
 *     Size of the input data in bytes.
 *
 * @param [in,out] RepCntCutoff_p
 *     Pointer to the 'Cutoff' value for the NIST SP800-90B 'Repetition Count'
 *     test. The default setting for this value is 31, unless overruled here.
 *     Setting this value to zero disables the 'Repetition Count' test.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] RepCntCount_p
 *     Pointer to the initial counter value for the NIST SP800-90B 'Repetition
 *     Count' test. Although it is possible to pre-load this counter, it will
 *     be reset to one (1) at the start of the test run.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] RepCntData_p
 *     Pointer to the initial compare data value for the NIST SP800-90B
 *     'Repetition Count' test. At the start of the test, this value indicates
 *     the last 8-bits 'noise sample' value detected for repetition counting.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp64Cutoff_p
 *     Pointer to the 'Cutoff' value for the NIST SP800-90B 'Adaptive
 *     Proportion' test with a 64 'noise samples' window. The default setting
 *     for this value is 51, unless overruled here. Setting this value to zero
 *     disables the 'Adaptive Proportion' test with a 64 'noise samples' window.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp64Count_p
 *     Pointer to the initial counter value for the NIST SP800-90B 'Adaptive
 *     Proportion' test with a 64 'noise samples' window. Although it is
 *     possible to pre-load this counter, it will be reset to zero at the
 *     start of the test run.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp64Data_p
 *     Pointer to the initial compare data value for the NIST SP800-90B
 *     'Adaptive Proportion' test with a 64 'noise samples' window. Although
 *     this value is loaded into the HW compare register before the test run
 *     start, it will be overwritten immediately with the first 8-bits 'noise
 *     sample' in the test data stream.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp4kCutoff_p
 *     Pointer to the 'Cutoff' value for the NIST SP800-90B 'Adaptive
 *     Proportion' test with a 4096 'noise samples' window. The default
 *     setting for this value is 2240, unless overruled here. Setting this
 *     value to zero disables the 'Adaptive Proportion' test with a 4096
 *     'noise samples' window.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp4kCount_p
 *     Pointer to the initial counter value for the NIST SP800-90B 'Adaptive
 *     Proportion' test with a 4096 'noise samples' window. Although it is
 *     possible to pre-load this counter, it will be reset to zero at the
 *     start of the test run.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [in,out] AdaptProp4kData_p
 *     Pointer to the initial compare data value for the NIST SP800-90B
 *     'Adaptive Proportion' test with a 4096 'noise samples' window. Although
 *     this value is loaded into the HW compare register before the test run
 *     start, it will be overwritten immediately with the first 8-bits 'noise
 *     sample' in the test data stream.
 *     On success, the engine value at end of the test run is returned.
 *
 * @param [out] AdaptProp64Fail_p
 *     Pointer to the buffer where the failure indication for the NIST SP800-90B
 *     'Adaptive Proportion' test with a 64 'noise samples' window must be
 *     written.
 *
 * @param [out] AdaptProp4kFail_p
 *     Pointer to the buffer where the failure indication for the NIST SP800-90B
 *     'Adaptive Proportion' test with a 4096 'noise samples' window must be
 *     written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_TrngNrbgSelfTest(
        ValOctetsIn_t * const InputData_p,
        const ValSize_t Size,
        uint8_t * const RepCntCutoff_p,
        uint8_t * const RepCntCount_p,
        uint8_t * const RepCntData_p,
        uint8_t * const AdaptProp64Cutoff_p,
        uint8_t * const AdaptProp64Count_p,
        uint8_t * const AdaptProp64Data_p,
        uint16_t * const AdaptProp4kCutoff_p,
        uint16_t * const AdaptProp4kCount_p,
        uint8_t * const AdaptProp4kData_p,
        uint8_t * const AdaptProp64Fail_p,
        uint8_t * const AdaptProp4kFail_p);


#endif /* Include Guard */

/* end of file api_val_random.h */
