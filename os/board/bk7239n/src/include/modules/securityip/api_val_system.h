/**
 * @file api_val_system.h
 *
 * @brief VAL API - System related services
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

#ifndef INCLUDE_GUARD_API_VAL_SYSTEM_H
#define INCLUDE_GUARD_API_VAL_SYSTEM_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t
#include "api_val_asset.h"          // ValAssetId_t


/*-----------------------------------------------------------------------------
 * val_SystemGetVersion
 */
/**
 * This function retrieves the version information and memory size from the
 * EIP-13x device and returns this in a string.
 *
 * @param [out] Version_p
 *     Pointer to the buffer in which the version information and memory size
 *     needs to be returned.
 *
 * @param [in,out] VersionSize_p
 *     Pointer to the size of buffer, upon return it is updated with
 *     the actual size of the version information.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemGetVersion(
        ValOctetsOut_t * const Version_p,
        ValSize_t * const VersionSize_p);


/*-----------------------------------------------------------------------------
 * val_SystemGetState
 */
/**
 * This function retrieves the version information from the EIP-13x device and
 * returns this in a string.
 *
 * @param [out] OtpErrorCode_p
 *     Pointer to the variable in which the OTP scan error must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] OtpErrorLocation_p
 *     Pointer to the variable in which the first location of OTP scan error
 *     must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] Mode_p
 *     Pointer to the variable in which the mode must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be:
 *     0   : Non-FIPS mode
 *     4..6: Error mode (see Firmware Reference Manual for details)
 *     15  : FIPS mode
 *
 * @param [out] SelfTestError_p
 *     Pointer to the variable in which the self-test error must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     Note: Information only valid when Error mode is active and is caused
 *           by a self-test.
 *
 * @param [out] CryptoOfficer_p
 *     Pointer to the variable in which the availability of the Crypto Officer
 *     ID must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be:
 *     0 : Not available
 *     !0: Available
 *
 * @param [out] HostID_p
 *     Pointer to the variable in which the host ID of the caller must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be in the range [0..7].
 *
 * @param [out] NonSecure_p
 *     Pointer to the variable in which the secure/non-secure state of the
 *     caller must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be:
 *     0 : Secure
 *     !0: Non-Secure
 *
 * @param [out] Identity_p
 *     Pointer to the variable in which the identity of the caller must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemGetState(
        uint8_t * const OtpErrorCode_p,
        uint16_t * const OtpErrorLocation_p,
        uint8_t * const Mode_p,
        uint8_t * const SelfTestError_p,
        uint8_t * const CryptoOfficer_p,
        uint8_t * const HostID_p,
        uint8_t * const NonSecure_p,
        uint32_t * const Identity_p);


/*-----------------------------------------------------------------------------
 * val_SystemReset
 */
/**
 * This function resets the EIP-13x firmware.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemReset(void);


/*-----------------------------------------------------------------------------
 * val_SystemSelfTest
 */
/**
 * This function starts the self test either to switch to FIPS mode or recheck
 * the system.
 * NOTE: The function switches to FIPS mode in which strict users checking is
 *       performed. Therefore, this function is classified as FIPS function
 *       and can only be used FIPS mode is enabled.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemSelfTest(void);


/*-----------------------------------------------------------------------------
 * val_SystemDefineUsers
 */
/**
 * This function defines the allowed users for the FIPS mode.
 * NOTE: The function can only be used by the Crypto Officer and is intended
 *       to configure FIPS mode functionality, therefore this function can only
 *       be used when FIPS and Crypto Officer functionality are enabled.
 *
 * @param [in] User1
 *     The identity of User1.
 *
 * @param [in] User2
 *     The identity of User2.
 *
 * @param [in] User3
 *     The identity of User3.
 *
 * @param [in] User4
 *     The identity of User4.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemDefineUsers(
        const uint32_t User1,
        const uint32_t User2,
        const uint32_t User3,
        const uint32_t User4);


/*-----------------------------------------------------------------------------
 * val_SystemSetTime
 */
/**
 * This function set/initializes the (UTC) time of the EIP-13x hardware.
 *
 * @param [in] Seconds
 *     This specifies the time in seconds relative to 1 January 1970.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemSetTime(
        const uint32_t Seconds);


/*-----------------------------------------------------------------------------
 * val_SystemSleep
 */
/**
 * This function forces the EIP-13x hardware to go to sleep mode. This means
 * that after successful execution, the EIP-13x hardware with exception of the
 * DataRAM and optional the FW-RAM can be powered off.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemSleep(void);


/*-----------------------------------------------------------------------------
 * val_SystemResumeFromSleep
 */
/**
 * This function resumes the EIP-13x hardware from sleep mode and switches to
 * normal operation again.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemResumeFromSleep(void);


/*-----------------------------------------------------------------------------
 * val_SystemHibernation
 */
/**
 * This function forces the EIP-13x hardware to go to hibernation mode. This
 * means that after successful execution, the EIP-13x hardware can be powered
 * off.
 *
 * @param [in] StateAssetId
 *     Asset ID of the Asset that holds the state.
 *
 * @param [in] KEKAssetId
 *     Reference to the Key Encryption Key Asset to be used when generating
 *     the data blob.
 *
 * @param [in] AssociatedData_p
 *     Pointer to the Associated Data used for the data blob authentication.
 *     The exact same Associated Data must be provided for the Resume From
 *     Hibernation operation.
 *
 * @param [in] AssociatedDataSize
 *     The size in bytes for the Associated Data used for the data blob
 *     authentication. (AssociatedData_p)
 *
 * @param [out] DataBlob_p
 *     Pointer to the data blob buffer.
 *
 * @param [in,out] DataBlobSize_p
 *     Size of the buffer intended for the data blob (DataBlob_p).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemHibernation(
        const ValAssetId_t StateAssetId,
        const ValAssetId_t KEKAssetId,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_t * const DataBlob_p,
        ValSize_t * const DataBlobSize_p);


/*-----------------------------------------------------------------------------
 * val_SystemResumeFromHibernation
 */
/**
 * This function resumes the EIP-13x hardware from hibernation mode and
 * switches to normal operation again.
 *
 * @param [in] StateAssetId
 *     Asset ID of the Asset that holds the state.
 *
 * @param [in] KEKAssetId
 *     Reference to the Key Encryption Key Asset to be used when generating
 *     the data blob.
 *
 * @param [in] AssociatedData_p
 *     Pointer to the Associated Data used for the data blob authentication.
 *     The exact same Associated Data must be provided for the Hibernation
 *     operation.
 *
 * @param [in] AssociatedDataSize
 *     The size in bytes for the Associated Data used for the data blob
 *     authentication. (AssociatedData_p)
 *
 * @param [in] DataBlob_p
 *     Pointer to the data blob buffer.
 *
 * @param [in] DataBlobSize
 *     Size of the buffer of the data blob (DataBlob_p).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SystemResumeFromHibernation(
        const ValAssetId_t StateAssetId,
        const ValAssetId_t KEKAssetId,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsIn_t * const DataBlob_p,
        const ValSize_t DataBlobSize);


/*-----------------------------------------------------------------------------
 * val_IsAccessSecure
 */
/**
 * This function returns an indication of the device access is secure or
 * non-secure.
 * Note: If no device connection exists, the function will assume Non-secure
 *       access.
 *
 * @return true  = Secure.
 * @return false = Non-secure.
 */
bool
val_IsAccessSecure(void);


#endif /* Include Guard */

/* end of file api_val_system.h */
