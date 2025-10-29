/**
 * @file api_val_nop.h
 *
 * @brief VAL API - NOP service
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

#ifndef INCLUDE_GUARD_API_VAL_NOP_H
#define INCLUDE_GUARD_API_VAL_NOP_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t


/*-----------------------------------------------------------------------------
 * val_NOP
 */
/**
 * This function allows data to be copied with the DMA engine.
 *
 * @param [in] SrcData_p
 *     Pointer to the buffer with the data that needs to be copied.
 *
 *
 * @param [in] DstData_p
 *     Pointer to the buffer in which the data needs to be copied.
 *
 * @param [in] DataSize
 *     The size of data to be copied.
 *     Note that the size is equal for the Source and Destination Data and
 *     will be round-up to a multiple of 4 bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_NOP(
        ValOctetsIn_t * const  SrcData_p,
        ValOctetsOut_t * const DstData_p,
        const ValSize_t  DataSize);


#endif /* Include Guard */

/* end of file api_val_nop.h */
