/**
 * @file api_val_service.h
 *
 * @brief VAL API - Service related services
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

#ifndef INCLUDE_GUARD_API_VAL_SERVICE_H
#define INCLUDE_GUARD_API_VAL_SERVICE_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t


/*-----------------------------------------------------------------------------
 * val_ServiceRegisterRead
 */
/**
 * This function reads the specified internal register (address) of EIP-13x
 * hardware.
 *
 * @param [in] Address
 *     Address of the internal register to write.
 *
 * @param [out] Value_p
 *     Pointer to the buffer in which the value needs to be returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceRegisterRead(
        uint32_t         Address,
        uint32_t * const Value_p);


/*-----------------------------------------------------------------------------
 * val_ServiceRegisterWrite
 */
/**
 * This function writes a value to the specified internal register (address)
 * of EIP-13x hardware.
 *
 * @param [in] Address
 *     Address of the internal register to write.
 *
 * @param [in] Value
 *     Value to write.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceRegisterWrite(
        uint32_t Address,
        uint32_t Value);


/*-----------------------------------------------------------------------------
 * val_ServiceZeroizeMailbox
 */
/**
 * This function zeroizes the output mailbox to make sure that the output
 * mailbox does not hold sensitive information when the mailbox is released.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceZeroizeMailbox(void);


/*-----------------------------------------------------------------------------
 * val_ServiceSelectOTPZeroize
 */
/**
 * This function selects the OTP zeroize functionality only. To execute the
 * actual OTP zeroize operation, this function call must be followed by calling
 * the function val_ServiceZeroizeOTP().
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceSelectOTPZeroize(void);


/*-----------------------------------------------------------------------------
 * val_ServiceZeroizeOTP
 */
/**
 * This function executes the OTP zeroize operation, when the OTP zeroize
 * functionality was previously selected with the val_ServiceSelectOTPZeroize()
 * function.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceZeroizeOTP(void);


/*-----------------------------------------------------------------------------
 * val_ServiceClockSwitch
 */
/**
 * This function controls the switch that activates the Encryption, Hash, TRNG,
 * PKCP, Sequencer and Custom Engine clocks. Normally, the EIP-13x firmware
 * switches the clocks automatically for optimized power consumption.
 * Notes:
 * - Function is only intended for debug
 * - All clocks with 0b’s in the ClocksForcedOn and ClocksForcedOff bitmask are
 *   set to 'Automatic mode'.
 *
 * @param [in] ClocksForcedOn
 *     Bitmask specifying (with 1b’s) the clocks that needs to be forced ON
 *     (always ON).
 *
 * @param [in] ClocksForcedOff
 *     Bitmask specifying (with 1b’s) the clocks that needs to be forced OFF
 *     (always OFF).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ServiceClockSwitch(
        uint16_t ClocksForcedOn,
        uint16_t ClocksForcedOff);


#endif /* Include Guard */


/* end of file api_val_service.h */
