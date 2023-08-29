/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_OSAL_ERRNOS_H
#define AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_OSAL_ERRNOS_H

#include "audio_hw_compat.h"
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	HAL_OSAL_OK                      = 0,    // Everything's swell.


	HAL_OSAL_ERR_UNKNOWN_ERROR       = (-2147483647 - 1), // INT32_MIN value

	HAL_OSAL_ERR_NO_MEMORY           = -ENOMEM,
	HAL_OSAL_ERR_INVALID_OPERATION   = -ENOSYS,
	HAL_OSAL_ERR_INVALID_PARAM       = -EINVAL,
	HAL_OSAL_ERR_NAMERR_NOT_FOUND    = -ENOENT,
	HAL_OSAL_ERR_PERMISSION_DENIED   = -EPERM,
	HAL_OSAL_ERR_NO_INIT             = -ENODEV,
	HAL_OSAL_ERR_ALREADY_EXISTS      = -EEXIST,
	HAL_OSAL_ERR_DEAD_OBJECT         = -EPIPE,
	HAL_OSAL_ERR_BAD_INDEX           = -EOVERFLOW,
	HAL_OSAL_ERR_NOT_ENOUGH_DATA     = -ENODATA,
	HAL_OSAL_ERR_WOULD_BLOCK         = -EWOULDBLOCK,
	HAL_OSAL_ERR_TIMED_OUT           = -ETIMEDOUT,
	HAL_OSAL_ERR_UNKNOWN_TRANSACTION = -EBADMSG,

	HAL_OSAL_ERR_FAILURE             = (HAL_OSAL_ERR_UNKNOWN_ERROR + 1),
	HAL_OSAL_ERR_NOT_SUPPORT         = (HAL_OSAL_ERR_UNKNOWN_ERROR + 2),
};

#ifdef __cplusplus
}
#endif

#endif // AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_OSAL_ERRNOS_H