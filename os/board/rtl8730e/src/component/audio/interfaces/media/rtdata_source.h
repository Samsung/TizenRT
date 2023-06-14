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

/**
 * @addtogroup Media
 * @{
 *
 * @brief Declares APIs for RTDataSource.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file rtdata_source.h
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDATA_SOURCE_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDATA_SOURCE_H

#include <stddef.h>

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RTDataSource RTDataSource;
struct RTDataSource {
	/**
	 * @brief Check whether the data source init success.
	 *
	 * @param source The RTDataSource object pointer.
	 * @return Returns a value listed below: \n
	 * rt_status_t | Description
	 * ----------------------| -----------------------
	 * OSAL_OK | prepare success.
	 * OSAL_ERR_NO_INIT | source init fail.
	 * @since 1.0
	 * @version 1.0
	 */
	rt_status_t (*CheckPrepared)(const RTDataSource *source);

	/**
	 * @brief Returns the number of bytes read, or error value on failure. It's not an error if
	 * this returns zero; it just means the given offset is equal to, or
	 * beyond, the end of the source.
	 *
	 * @param source The RTDataSource object pointer.
	 * @param offset The offset from the beginning of the source.
	 * @param data The pointer that stores the data.
	 * @param size The size need to read.
	 * @return Returns the number of bytes read, or error value on failure: \n
	 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
	 * RTDATA_SOURCE_EOF | read meet the end of stream.
	 * RTDATA_SOURCE_READ_AGAIN | need to read again.
	 * @since 1.0
	 * @version 1.0
	 */
	ssize_t (*ReadAt)(const RTDataSource *source, off_t offset, void *data, size_t size);

	/**
	 * @brief Get the whole length of the source.
	 *
	 * @param source The RTDataSource object pointer.
	 * @param size The size of the source.
	 * @return Returns a value listed below: \n
	 * rt_status_t | Description
	 * ----------------------| -----------------------
	 * OSAL_OK | the operation is successful.
	 * RTDATA_SOURCE_FAIL | operation is failed.
	 * RTDATA_SOURCE_UNKNOWN_LENGTH | this is an unknown length source.
	 * @since 1.0
	 * @version 1.0
	 */
	rt_status_t (*GetLength)(const RTDataSource *source, off_t *size);
};

enum {
	RTDATA_SOURCE_OK = 0,

	RTDATA_SOURCE_ERROR_BASE = -1000,
	RTDATA_SOURCE_READ_AGAIN = RTDATA_SOURCE_ERROR_BASE - 1,
	RTDATA_SOURCE_FAIL = RTDATA_SOURCE_ERROR_BASE - 2,
	RTDATA_SOURCE_EOF = RTDATA_SOURCE_ERROR_BASE - 3,
	RTDATA_SOURCE_UNKNOWN_LENGTH = RTDATA_SOURCE_ERROR_BASE - 4,
};

#ifdef __cplusplus
}
#endif

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDATA_SOURCE_H
/** @} */
