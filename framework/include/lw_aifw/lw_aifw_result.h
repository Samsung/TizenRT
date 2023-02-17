/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * @ingroup lw_aifw
 * @{
 */

/**
 * @file lw_aifw/lw_aifw_result.h
 * @brief On-Device AI Framework Result Enumeration
 */

#ifndef __LW_AIFW_LW_AIFW_H__
#define __LW_AIFW_LW_AIFW_H__

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Common set of return values used in Light Weight AI Framework functions.
 */
typedef enum _LW_AIFW_RESULT {
	LW_AIFW_OK = 0, /* OK: Without any error  */
	LW_AIFW_ERROR = -1, /* ERROR: All other types of error not specified by any following enum */
	LW_AIFW_NO_MEM = -ENOMEM, /* Memory allocation (malloc/calloc) failed */
	LW_AIFW_ERROR_FILE_ACCESS = -3, /* File access error */
	LW_AIFW_BUFFER_EMPTY = -4, /* No data availbale in Read/write/other data buffer */
	LW_AIFW_READ_ALREADY = -5, /* all data already read from a read/write/other buffer */
	LW_AIFW_NO_SPACE = -ENOSPC, /* read/write/other buffer has empty space less than required size */
	LW_AIFW_SOURCE_EOF = -7,  /* End Of File or End of Source data */
} LW_AIFW_RESULT;

#ifdef __cplusplus
}
#endif

#endif /* __LW_AIFW_LW_AIFW_H__ */

/** @} */ /* end of lw_aifw group */

