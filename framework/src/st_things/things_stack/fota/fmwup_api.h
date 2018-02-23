/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _FMWUP_API_H_
#define _FMWUP_API_H_

//#include <st_things/st_things.h>
#include <st_things/st_things_types.h>
#include "things_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for XXX.
 */
typedef enum {
	FMWUP_ERROR_NONE = 0,			  /**< Successful */
	FMWUP_ERROR_INVALID_PARAMETER = ST_THINGS_ERROR_INVALID_PARAMETER,
	/**< Invalid parameter (If parameter is null or empty)*/
	FMWUP_ERROR_OPERATION_FAILED = ST_THINGS_ERROR_OPERATION_FAILED,/**< Operation Failed */
	FMWUP_ERROR_MEMORY_ERROR = -3,		  /**< Allocation Failed */
} fmwup_things_error_e;

/**
 * @brief Enumeration for XXX.
 */
typedef enum {
	FMWUP_RESULT_INIT = 0,					/**< 0: Initial value. When updating process started (Download /Update), result property MUST set to Initial. */
	FMWUP_RESULT_SUCCESS,					/**< 1: Firmware updated successfully */
	FMWUP_RESULT_NOT_ENOUGH_MEMORY,			/**< 2: Not enough flash memory for the new firmware package. */
	FMWUP_RESULT_OUT_OF_RAM,				/**< 3. Out of RAM during downloading process. */
	FMWUP_RESULT_CONNECTION_LOST,			/**< 4: Connection lost during downloading process. */
	FMWUP_RESULT_INTEGRITY_FAIL,			/**< 5: Integrity check failure for new downloaded package.  */
	FMWUP_RESULT_INVALID_URI,				/**< 6: Invalid URI */
	FMWUP_RESULT_UPDATE_FAILED,				/**< 7: Firmware update failed */
	FMWUP_RESULT_UPDATE_UNSUPPORTED_PROTOCOL/**< 8: Unsupported protocol. */
} fmwup_result_e;

/**
 * @brief Enumeration for XXX.
 */
typedef enum {
	FMWUP_STATE_IDLE = 0,		/**< 0: Idle (before downloading or after successful updating) */
	FMWUP_STATE_DOWNLOADING,	/**< 1: Downloading (The data sequence is on the way) */
	FMWUP_STATE_DOWNLOADED,		/**< 2: Downloaded */
	FMWUP_STATE_UPDATING		/**< 3: Updating */
} fmwup_state_e;

/**
 * @brief Enumeration for XXX.
 */
typedef enum {
	FMWUP_COMMAND_INIT = 0,					/**< 0: Initial value */
	FMWUP_COMMAND_CHECK,
	FMWUP_COMMAND_DOWNLOAD,
	FMWUP_COMMAND_UPDATE,
	FMWUP_COMMAND_DOWNLOADUPDATE,
	FMWUP_COMMAND_UNKNOWN
} fmwup_update_e;

int fmwup_get_data(things_resource_s *target_resource);

int fmwup_set_data(things_resource_s *target_res);

int fmwup_check_firmware_upgraded(void);

int fmwup_initialize(void);

#ifdef __cplusplus
}
#endif
#endif							// _FMWUP_API_H_
