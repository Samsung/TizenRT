/**
 * @file api_val_hardware.h
 *
 * @brief VAL API - Hardware related services
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

#ifndef INCLUDE_GUARD_API_VAL_HARDWARE_H
#define INCLUDE_GUARD_API_VAL_HARDWARE_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t


/*-----------------------------------------------------------------------------
 * val_HW_ModuleStatus
 */
/**
 * This function retrieves the module status from the EIP-13x hardware
 * registers and returns the requested/selected information.
 *
 * @param [out] FIPSmode_p
 *     Pointer to the variable in which FIPS mode indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not active) or 1 (active).
 *
 * @param [out] NonFIPSmode_p
 *     Pointer to the variable in which non-FIPS mode indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not active) or 1 (active).
 *
 * @param [out] FatalError_p
 *     Pointer to the variable in which the fatal error indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (no error) or 1 (error).
 *
 * @param [out] CRC24Ok_p
 *     Pointer to the variable in which the CRC24 Ok indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not Ok) or 1 (Ok).
 *
 * @param [out] CRC24Busy_p
 *     Pointer to the variable in which the CRC24 busy indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not busy) or 1 (busy).
 *
 * @param [out] CRC24Error_p
 *     Pointer to the variable in which the CRC24 error indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (no error) or 1 (error).
 *
 * @param [out] FwImageWritten_p
 *     Pointer to the variable in which the Firmware image written indication
 *     must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not written) or 1 (written).
 *
 * @param [out] FwImageCheckDone_p
 *     Pointer to the variable in which the Firmware image check done
 *     indication must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not checked/check busy) or 1 (check done).
 *
 * @param [out] FwImageAccepted_p
 *     Pointer to the variable in which the Firmware image accepted indication
 *     must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 (not accepted) or 1 (accepted).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_HW_ModuleStatus(
        uint8_t * const FIPSmode_p,
        uint8_t * const NonFIPSmode_p,
        uint8_t * const FatalError_p,
        uint8_t * const CRC24Ok_p,
        uint8_t * const CRC24Busy_p,
        uint8_t * const CRC24Error_p,
        uint8_t * const FwImageWritten_p,
        uint8_t * const FwImageCheckDone_p,
        uint8_t * const FwImageAccepted_p);


/*-----------------------------------------------------------------------------
 * val_HW_EIP_Options
 */
/**
 * This function retrieves the EIP options information from the EIP-13x
 * hardware registers and returns the requested/selected information.
 *
 * @param [out] nMailboxes_p
 *     Pointer to the variable in which the number of mailboxes must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be in the range [1..8].
 *
 * @param [out] MailboxSize_p
 *     Pointer to the variable in which the mailbox size must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 128, 256 (default), 512 and 1024 bytes.
 *
 * @param [out] HostId_p
 *     Pointer to the variable in which the Host Id must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] SecureHostId_p
 *     Pointer to the variable in which the Secure Host Id must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] MasterId_p
 *     Pointer to the variable in which the master host Id must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be in the range [0..7].
 *
 * @param [out] MyHostId_p
 *     Pointer to the variable in which the my host Id (calling host) must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be in the range [0..7].
 *
 * @param [out] ProtectionAvailable_p
 *     Pointer to the variable in which the protection available indication
 *     must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 or 1.
 *
 * @param [out] Protection_p
 *     Pointer to the variable in which the protection indication must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *     The value can be 0 or 1.
 *
 * @param [out] StandardEngines_p
 *     Pointer to the variable in which standard engines, cpu and bus interface
 *     information must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] CustomEngines_p
 *     Pointer to the variable in which custom engines information must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_HW_EIP_Options(
        uint8_t * const nMailboxes_p,
        uint16_t * const MailboxSize_p,
        uint8_t * const HostId_p,
        uint8_t * const SecureHostId_p,
        uint8_t * const MasterId_p,
        uint8_t * const MyHostId_p,
        uint8_t * const ProtectionAvailable_p,
        uint8_t * const Protection_p,
        uint16_t * const StandardEngines_p,
        uint16_t * const CustomEngines_p);


/*-----------------------------------------------------------------------------
 * val_HW_EIP_Version
 */
/**
 * This function retrieves the EIP version and number information from the
 * EIP-13x hardware registers and returns the requested/selected information.
 *
 * @param [out] MajorVersion_p
 *     Pointer to the variable in which the major version number of the
 *     hardware must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] MinorVersion_p
 *     Pointer to the variable in which the minor version number of the
 *     hardware must be returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] PatchLevel_p
 *     Pointer to the variable in which patch level of the hardware must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @param [out] EipNumber_p
 *     Pointer to the variable in which EIP number of the hardware must be
 *     returned.
 *     Note: Pointer may be NULL if information is not required.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_HW_EIP_Version(
        uint8_t * const MajorVersion_p,
        uint8_t * const MinorVersion_p,
        uint8_t * const PatchLevel_p,
        uint8_t * const EipNumber_p);


#endif /* Include Guard */

/* end of file api_val_hardware.h */
