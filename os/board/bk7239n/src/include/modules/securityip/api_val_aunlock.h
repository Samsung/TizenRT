/**
 * @file api_val_aunlock.h
 *
 * @brief VAL API - Authenticated Unlock and Secure Debug related services
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

#ifndef INCLUDE_GUARD_API_VAL_AUNLOCK_H
#define INCLUDE_GUARD_API_VAL_AUNLOCK_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t, ValSize_t
#include "api_val_asset.h"          // ValAssetId_t, ValAssetNumber_t
#include "api_val_asym.h"           // ValAsymBigInt_t


/*-----------------------------------------------------------------------------
 * val_AuthenticatedUnlockStart
 */
/**
 * This function initializes and starts the Authenticated Unlocked scheme.
 *
 * @param [in] AuthKeyNumber
 *     The asset number to search for in the (static) Asset Store that contains
 *     the Authenticated Unlocked public key. Note that the supported values
 *     for AuthKeyNumber are defined by the device manufacturer and vendor
 *     and can differ from product to product.
 *
 * @param [out] AuthStateASId_p
 *     Pointer to the memory location where the AssetId for Authenticated
 *     Unlock state will be written. The AssetId is needed
 *     for all Authenticated Unlock related operations.
 *
 * @param [out] Nonce_p
 *     Pointer to the memory location where the HW Nonce will be written for
 *     the signing and is later on used for the verification.
 *
 * @param [in,out] NonceLength_p
 *     Pointer to the memory location where the size of the Nonce in bytes
 *     will be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AuthenticatedUnlockStart(
        ValAssetNumber_t AuthKeyNumber,
        ValAssetId_t * AuthStateASId_p,
        ValOctetsOut_t * Nonce_p,
        ValSize_t * NonceLength_p);


/*-----------------------------------------------------------------------------
 * val_AuthenticatedUnlockVerify
 */
/**
 * This function verifies the signature to get to the unlocked state.
 *
 * @param [in] AuthStateASId
 *     Reference to the Authenticated Unlock state Asset.
 *     This reference is provided by val_AuthenticatedUnlockStart().
 *
 * @param [in] Signature_p
 *     Pointer to the memory location where the signature is located for
 *     the verification.
 *
 * @param [in] Nonce_p
 *     Pointer to the memory location where the host Nonce is located for
 *     the verification.
 *
 * @param [in] NonceLength
 *     Size of the Nonce in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AuthenticatedUnlockVerify(
        const ValAssetId_t AuthStateASId,
        ValAsymBigInt_t * const Signature_p,
        ValOctetsIn_t * Nonce_p,
        const ValSize_t NonceLength);


/*-----------------------------------------------------------------------------
 * val_AuthenticatedUnlockRelease
 */
/**
 * This function release the Authenticated Unlock state Asset.
 *
 * @param [in] AuthStateASId
 *     Reference to the Authenticated Unlock state Asset.
 *     This reference is provided by val_AuthenticatedUnlockStart().
 *
 * @return One of the ValStatus_t values.
 */
#define val_AuthenticatedUnlockRelease(AuthStateASId) val_AssetFree(AuthStateASId)


/*-----------------------------------------------------------------------------
 * val_SecureDebug
 */
/**
 * This function set the Secure Debug port state after a successfull
 * authenticated unlock operation.
 *
 * @param [in] AuthStateASId
 *     Reference to the Authenticated Unlock state Asset.
 *     This reference is provided by val_AuthenticatedUnlockStart().
 *
 * @param [in] fSet
 *     Indication to enable (true) or disable (false) the debug port.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SecureDebug(
        const ValAssetId_t AuthStateASId,
        const bool fSet);


#endif /* Include Guard */

/* end of file api_val_aunlock.h */
