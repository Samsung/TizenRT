/**
 * @file api_val_emmc.h
 *
 * @brief VAL API - eMMC related services
 */

/*****************************************************************************
* Copyright (c) 2016-2018 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_API_VAL_EMMC_H
#define INCLUDE_GUARD_API_VAL_EMMC_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t
#include "api_val_asset.h"          // ValAssetId_t


/*-----------------------------------------------------------------------------
 * val_eMMCReadRequest
 */
/**
 * This function is the initial (start) operation for a secure data read
 * of an item stored in the Replay Protected Memory Block (RPMB) of the eMMC.
 * Note that the returned Nonce is required for the actual eMMC read.
 *
 * The returned State Asset is required for all other eMMC operations:
 * - val_eMMCReadVerify()
 *     Verifies the eMMC read response (the secure data).
 * - val_eMMCReadWriteCounterRequest()
 *     Provides a Nonce that is required for the write counter read of the
 *     eMMC item.
 * - val_eMMCReadWriteCounterVerify()
 *     Verifies the eMMC read response (the write counter value)
 * - val_eMMCWriteRequest()
 *     Generates the MAC value for the actual eMMC write.
 * - val_eMMCWriteVerify()
 *     Verifies the eMMC write response (the secure data write)
 *
 * @param [in] KeyAssetId
 *      Reference to the eMMC Authentication Key Asset.
 *      This reference is provided by val_AssetSearch().
 *
 * @param [out] StateASId_p
 *     Pointer to the memory location where the AssetId for eMMC state will
 *     be written. The AssetId is needed for all related eMMC operations.
 *
 * @param [out] Nonce_p
 *     Pointer to the memory location where the Nonce will be written for
 *     the verification later on.
 *
 * @param [in,out] NonceLength_p
 *     Pointer to the memory location where the size of the Nonce in bytes
 *     will be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCReadRequest(
        const ValAssetId_t KeyAssetId,
        ValAssetId_t * const StateASId_p,
        ValOctetsOut_t * const Nonce_p,
        ValSize_t * const NonceLength_p);


/*-----------------------------------------------------------------------------
 * val_eMMCReadVerify
 */
/**
 * This function is the second and final operation for the secure data read
 * operation, which verifies the eMMC read response with the secure data that
 * is read.
 *
 * @param [in] StateASId
 *     Reference to the eMMC State Asset.
 *     This reference is provided by val_eMMCReadRequest().
 *
 * @param [in] Data_p
 *     Pointer to the buffer with the eMMC read response (the secure data read)
 *
 * @param [in] DataSize
 *     The size of the eMMC read response.
 *
 * @param [in] MAC_p
 *     Pointer to a buffer with the MAC value of the eMMC read response that
 *     must be checked.
 *
 * @param [in] MACSize
 *     The size of the MAC value of the eMMC read response.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCReadVerify(
        const ValAssetId_t StateASId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize,
        ValOctetsIn_t * const MAC_p,
        const ValSize_t MACSize);


/*-----------------------------------------------------------------------------
 * val_eMMCReadWriteCounterRequest
 */
/**
 * This function is the initial (start) operation of the write counter read
 * of the item stored in the Replay Protected Memory Block (RPMB) of the eMMC.
 * Note that the returned Nonce is required for the actual eMMC write counter
 * read.
 *
 * @param [in] StateASId
 *     Reference to the eMMC State Asset.
 *     This reference is provided by val_eMMCReadRequest().
 *
 * @param [out] Nonce_p
 *     Pointer to the memory location where the Nonce will be written for the
 *     verification later on.
 *
 * @param [in,out] NonceLength_p
 *     Pointer to the memory location where the size of the Nonce in bytes
 *     will be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCReadWriteCounterRequest(
        const ValAssetId_t StateASId,
        ValOctetsOut_t * const Nonce_p,
        ValSize_t * const NonceLength_p);


/*-----------------------------------------------------------------------------
 * val_eMMCReadWriteCounterVerify
 */
/**
 * This function is the second and final operation of the write counter read
 * of the eMMC item, which verifies the eMMC Write Counter read response.
 *
 * @param [in] StateASId
 *     Reference to the eMMC State Asset.
 *     This reference is provided by val_eMMCReadRequest().
 *
 * @param [in] Data_p
 *     Pointer to the buffer with the eMMC Write Counter read response (the
 *     write counter value)
 *
 * @param [in] DataSize
 *     The size of the eMMC Write Counter read response.
 *
 * @param [in] MAC_p
 *     Pointer to a buffer with the MAC value of the eMMC Write Counter read
 *     response that must be checked.
 *
 * @param [in] MACSize
 *     The size of the MAC value of the eMMC Write Counter read response.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCReadWriteCounterVerify(
        const ValAssetId_t StateASId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize,
        ValOctetsIn_t * const MAC_p,
        const ValSize_t MACSize);


/*-----------------------------------------------------------------------------
 * val_eMMCWriteRequest
 */
/**
 * This function is the initial (start) operation of the secure data write
 * of the eMMC item. The generated MAC value is required for the actual eMMC
 * write operation.
 *
 * @param [in] StateASId
 *     Reference to the eMMC State Asset.
 *     This reference is provided by val_eMMCReadRequest().
 *
 * @param [in] Data_p
 *     Pointer to the buffer with the eMMC Write response (the secure data
 *     must be written)
 *
 * @param [in] DataSize
 *     The size of the eMMC Write response.
 *
 * @param [in] MAC_p
 *     Pointer to a buffer in which the MAC value must be returned which is
 *     required for the actual eMMC Write.
 *     Note: The buffer needs to be big enough to hold the MAC value.
 *
 * @param [in,out] MACSize_p
 *     Pointer to the size of MAC value buffer, upon return it is updated with
 *     the actual size of the MAC value.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCWriteRequest(
        const ValAssetId_t StateASId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize,
        ValOctetsOut_t * const MAC_p,
        ValSize_t * const MACSize_p);


/*-----------------------------------------------------------------------------
 * val_eMMCWriteVerify
 */
/**
 * This function is the second and final operation of the data write of the
 * eMMC item, which verifies the eMMC Write response.
 *
 * @param [in] StateASId
 *     Reference to the eMMC State Asset.
 *     This reference is provided by val_eMMCReadRequest().
 *
 * @param [in] Data_p
 *     Pointer to the buffer with the eMMC Write response (the secure data
 *     that is written)
 *
 * @param [in] DataSize
 *     The size of the eMMC Write response.
 *
 * @param [in] MAC_p
 *     Pointer to a buffer with the MAC value of the eMMC Write response that
 *     must be checked.
 *
 * @param [in] MACSize
 *     The size of the MAC value of the eMMC Write response.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_eMMCWriteVerify(
        const ValAssetId_t StateASId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize,
        ValOctetsIn_t * const MAC_p,
        const ValSize_t MACSize);


#endif /* Include Guard */


/* end of file api_val_emmc.h */
