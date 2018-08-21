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

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/BufferInputDataSource.h
 * @brief Media BufferInputDataSource APIs
 */

#ifndef __MEDIA_BUFFERINPUTDATASOURCE_H
#define __MEDIA_BUFFERINPUTDATASOURCE_H

#include <media/InputDataSource.h>

typedef ssize_t(*OnBufferInputDataReached)(void* buffer, size_t size);
//TODO - The Callback mechanism will be modified.

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is buffer output data structure
 * @details @b #include <media/BufferInputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class BufferInputDataSource : public InputDataSource
{
public:
	/**
	 * @brief Constructs an empty BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource() = delete;
	/**
	 * @brief Constructs an empty BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * param[in] callback The callback that the callback function
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource(OnBufferInputDataReached callback);
	/**
	 * @brief Copy constructs for BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource(const BufferInputDataSource& source);
	/**
	 * @brief Operator= for BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource& operator=(const BufferInputDataSource& source);
	/**
	 * @brief Deconstructs an empty BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~BufferInputDataSource();

	/**
	 * @brief Whether file is ready to be write.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @return True.
	 * @since TizenRT v2.0 PRE
	 */
	bool isPrepare() override;
	/**
	 * @brief Open the file
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @return True.
	 * @since TizenRT v2.0 PRE
	 */
	bool open() override;
	/**
	 * @brief Close the file
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @return True.
	 * @since TizenRT v2.0 PRE
	 */
	bool close() override;

	/**
	 * @brief Gets the data
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @param[out] buf The buf that read the data and fill it into the buffer
	 * @param[in] size The size that the size of the buffer
	 * @return if there is nothing to read, it returns 0
	 *         if error occurred, it returns -1, else readead size returns
	 * @since TizenRT v2.0 PRE
	 */
	ssize_t read(unsigned char *buf, size_t size) override;

	/**
	 * @brief set the offset
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @param[in] offset The offset from origin
	 * @param[in] origin Position used as reference
	 * @return if failed, it returns a minus value, else 0 returns
	 * @since TizenRT v2.0 PRE
	 */
	int seek(long offset, int origin) override;

private:
	OnBufferInputDataReached mCallback;
};
} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
