/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @defgroup DM DM
 * @ingroup DM
 * @{
 */

/**
 * @file dm/dm_error.h
 * @brief Definition of dm_error
 */

#ifndef DM_ERROR_H_
#define DM_ERROR_H_

#include <errno.h>

/**
 * @brief Enumeration for the dm error
 * @details
 * Enumeration Details:\n
 * DM_ERROR_NONE = 0, Successful\n
 * DM_ERROR_INVALID_PARAMETER = -EINVAL, Invalid parameter : -22\n
 * DM_ERROR_NO_DATA = -ENODATA, No data available : -61\n
 * DM_ERROR_TIMED_OUT = -ETIME, Time out : -62\n
 * DM_ERROR_NOT_IMPLEMENTED = -ENOSYS, Function not implemented : -38\n
 * DM_ERROR_NOT_SUPPORTED = -1101, Not supported\n
 * DM_ERROR_ALREADY_STARTED = -1201, Already started\n
 * DM_ERROR_ALREADY_STOPPED = -1202, Already stopped\n
 * DM_ERROR_UNKNOWN = -1999, Unknown error\n
 */
typedef enum {

	DM_ERROR_NONE = 0, /* Successful */

	DM_ERROR_INVALID_PARAMETER = -EINVAL, /* Invalid parameter : -22 */
	DM_ERROR_NO_DATA = -ENODATA, /* No data available : -61*/
	DM_ERROR_TIMED_OUT = -ETIME, /* Time out : -62 */
	DM_ERROR_NOT_IMPLEMENTED = -ENOSYS, /* Function not implemented : -38 */

	DM_ERROR_NOT_SUPPORTED = -1101, /* Not supported */

	DM_ERROR_ALREADY_STARTED = -1201, /* Already started */
	DM_ERROR_ALREADY_STOPPED = -1202, /* Already stopped */

	DM_ERROR_UNKNOWN = -1999, /* Unknown error */
} dm_error_e;

#endif /* DM_ERROR_H_ */

/** @} */ // end of DM group
