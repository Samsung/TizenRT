/**
 * @mainpage VAL API
 * @section Introduction
 * This documentation covers the API for the EIP-13x HW that each application
 * should use.
 *
 * @file api_val.h
 *
 * @brief VAL API
 *
 * This API covers all functionality that is provided by the EIP-13x HW. It
 * implements an Abstraction Layer that runs on top of the layer that performs
 * the actual communication (exchange) with the EIP-13x HW.
 */

/*****************************************************************************
* Copyright (c) 2014-2019 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_API_VAL_H
#define INCLUDE_GUARD_API_VAL_H

#define VAL_API_VERSION "2.3.2"

/** VAL API specifics per category */
#include "api_val_result.h"
#include "api_val_buffers.h"
#include "api_val_asset.h"
#include "api_val_securetimer.h"
#include "api_val_asym.h"
#include "api_val_aunlock.h"
#include "api_val_nop.h"
#include "api_val_random.h"
#include "api_val_specfuncs.h"
#include "api_val_sym.h"
#include "api_val_symkeywrap.h"
#include "api_val_service.h"
#include "api_val_system.h"
#include "api_val_hardware.h"
#include "api_val_emmc.h"
#include "api_val_claim.h"

#endif /* Include Guard */

/* end of file api_val.h */
