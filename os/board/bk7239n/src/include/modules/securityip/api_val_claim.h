/**
 * @file api_val_claim.h
 *
 * @brief VAL API - Mailbox locking control services
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

#ifndef INCLUDE_GUARD_API_VAL_CLAIM_H
#define INCLUDE_GUARD_API_VAL_CLAIM_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "api_val_result.h"         // ValStatus_t


/**
 * Next functions are intended to control an exclusive lock of the EIP-13x
 * mailbox for the calling process. Generally, the mailbox is locked for each
 * operation and unlocked immediately afterwards, but with these functions the
 * mailbox can be locked exclusively for the calling process so that a sequence
 * of operations can be performed.
 * The main procedure is:
 * - val_Claim                  exclusively locks the mailbox for use
 * - ...                        various operations
 * - val_ServiceZeroizeMailbox  if needed, clears security relevant information
 * - val_ClaimRelease           releases the exclusive mailbox lock
 */

/*-----------------------------------------------------------------------------
 * val_Claim
 */
/**
 * This function claims exclusive use of the mailbox for the calling process,
 * if possible.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_Claim(void);

/*-----------------------------------------------------------------------------
 * val_ClaimOverrule
 */
/**
 * This function claims exclusive use of the mailboxfor the calling process.
 * Note that this function overrules any existing claim and can only be
 * performed by processes running on the master CPU (Host).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ClaimOverrule(void);

/*-----------------------------------------------------------------------------
 * val_Release
 */
/**
 * This function releases the exclusive use of the mailbox, if one was pending
 * for the calling process.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ClaimRelease(void);


#endif /* Include Guard */

/* end of file api_val_claim.h */
