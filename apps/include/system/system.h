/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
#include <utility>
#define _MOV std::move
#else
#define _MOV
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief boolean constant
 *
 */
typedef enum {
	SYSTEM_FALSE = 0,	/**<  bool FALSE constant  */
	SYSTEM_TRUE = 1	/**<  bool TRUE constant   */
} system_bool;


/**
 * @brief time out option (used by message queue, uart, semaphore, mutex)
 *
 */
typedef enum {
	SYSTEM_NO_WAIT = 0,	/**<  no wait constant         */
	SYSTEM_FOREVER = -1,	/**<  wait until job finished  */
} system_timeout_option;


/**
 * @brief function result constant
 *
 */
typedef enum {
	SYSTEM_FAIL = -1,			/**<  fail				*/
	SYSTEM_SUCCESS = 0,			/**<  success				*/
	SYSTEM_INVALID_ARGS = 1,		/**<  invalid parameter (argument)	*/
	SYSTEM_TIMEOUT = 2,			/**<  occur time out			*/
	SYSTEM_RESOURCE_BUSY = 3,		/**<  resource using another task	*/
	SYSTEM_TLS_CERT_VERIFY_FAIL =100,	/**< TLS Cert fail			*/
} system_result;

#define SYSTEM_PLATFORM_VERSION "2.0"

#if !defined(_WIN32)
#define __FILENAME__ (strrchr(__FILE__, '/') ? (char *)(strrchr(__FILE__, '/') + 1) : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (char *)(strrchr(__FILE__, '\\') + 1) : __FILE__)
#endif
#define FILE_NAME   __FILENAME__	/** @def source file name macro for debug log */
#define LINE_NUM	__LINE__	/** @def source file line number macro for debug log */
#define FUNC_NAME	__func__	/** @def function name macro for debug log */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
