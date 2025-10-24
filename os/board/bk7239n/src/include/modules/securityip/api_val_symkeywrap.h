/**
 * @file api_val_symkeywrap.h
 *
 * @brief VAL API - Symmetric Crypto based key wrap related services
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

#ifndef INCLUDE_GUARD_API_VAL_SYMKEYWRAP_H
#define INCLUDE_GUARD_API_VAL_SYMKEYWRAP_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t
#include "api_val_asset.h"          // ValAssetId_t


/*-----------------------------------------------------------------------------
 * val_SymAesKeyWrap
 */
/**
 * This function wraps the plaintext (key) data according to the method that
 * is specified in SP800-38F, RFC3394 or RFC5649 based on AES algorithm.
 *
 * @param [in] KeyAssetId
 *     If applicable, the AssetId of the key to use otherwise
 *     VAL_ASSETID_INVALID to indicate that a plaintext key is used
 *     specified with Key_p.
 *
 * @param [in] Key_p
 *     If applicable, the pointer to the plaintext key buffer otherwise NULL.
 *
 * @param [in] KeySize
 *     The size of the key (KeyLength).
 *
 * @param [in] SrcData_p
 *     Pointer to the buffer with the key data that needs to be wrapped.
 *
 * @param [in] SrcDataSize
 *     The size of data to be wrapped.
 *
 * @param [out] DstData_p
 *     Pointer to the buffer in which the wrapped data needs to be written.
 *
 * @param [in,out] DstDataSize_p
 *     Pointer to the size of data buffer, upon return it is updated with
 *     the actual size of the wrapped data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SymAesKeyWrap(
        ValAssetId_Optional_t KeyAssetId,
        ValOctetsIn_Optional_t * const Key_p,
        const ValSize_t KeySize,
        ValOctetsIn_t * const SrcData_p,
        const ValSize_t SrcDataSize,
        ValOctetsOut_t * const DstData_p,
        ValSize_t * const DstDataSize_p);


/*-----------------------------------------------------------------------------
 * val_SymAesKeyUnwrap
 */
/**
 * This function unwraps the encrypted (key) data according to the method that
 * is specified in SP800-38F, RFC3394 or RFC5649 based on AES algorithm.
 *
 * @param [in] KeyAssetId
 *     If applicable, the AssetId of the key to use otherwise
 *     VAL_ASSETID_INVALID to indicate that a plaintext key is used
 *     specified with Key_p.
 *
 * @param [in] Key_p
 *     If applicable, the pointer to the plaintext key buffer otherwise NULL.
 *
 * @param [in] KeySize
 *     The size of the key (KeyLength).
 *
 * @param [in] SrcData_p
 *     Pointer to the buffer with the key data that needs to be unwrapped.
 *
 * @param [in] SrcDataSize
 *     The size of data to be unwrapped.
 *
 * @param [out] DstData_p
 *     Pointer to the buffer in which the unwrapped data needs to be written.
 *
 * @param [in,out] DstDataSize_p
 *     Pointer to the size of data buffer, upon return it is updated with
 *     the actual size of the unwrapped data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SymAesKeyUnwrap(
        ValAssetId_Optional_t KeyAssetId,
        ValOctetsIn_Optional_t * const Key_p,
        const ValSize_t KeySize,
        ValOctetsIn_t * const SrcData_p,
        const ValSize_t SrcDataSize,
        ValOctetsOut_t * const DstData_p,
        ValSize_t * const DstDataSize_p);


/*-----------------------------------------------------------------------------
 * val_EncryptVectorForPKI
 */
/**
 * This function encrypts the specified asset with the Key Decrypt Key (Black
 * Key) when the exportable bit of the Asset Policy is set so that is can be
 * used as an encrypted vector for the PKI.
 * Note: The function can return a length error, in that case the required
 *       buffer size will be returned.
 *
 * @param [in] AssetId
 *     The AssetId of the vector (Key, etc.) to encrypt to get an encrypted
 *     vector.
 *
 * @param [in] AssetNumber
 *     The asset number of the Key Decryption Key (Black Key) that must be
 *     used to encrypt the vector to get an encrypted vector.
 *
 * @param [out] EncVectorData_p
 *     Pointer to the buffer in which the encrypted vector data needs to be
 *     written.
 *
 * @param [in,out] EncVectorDataSize_p
 *     Pointer to the size of encrypted vector data buffer, upon return it is
 *     updated with the actual or the required size of the encrypted vector
 *     data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_EncryptVectorForPKI(
        const ValAssetId_t AssetId,
        const ValAssetNumber_t AssetNumber,
        ValOctetsOut_t * const EncVectorData_p,
        ValSize_t * const EncVectorDataSize_p);


#endif /* Include Guard */


/* end of file api_val_symkeywrap.h */
