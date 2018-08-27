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

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is buffer Input data structure
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
	BufferInputDataSource();
	/**
	 * @brief Constructs an empty BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource(audio_type_t audio_type);
	/**
	 * @brief Copy constructs for BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource(const BufferInputDataSource &source);
	/**
	 * @brief Operator= for BufferInputDataSource.
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	BufferInputDataSource &operator=(const BufferInputDataSource &source);
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
	 * @brief set the file offset
	 * @details @b #include <media/BufferInputDataSource.h>
	 * @param[in] offset The offset from origin
	 * @param[in] origin Position used as reference
	 * @return if failed, it returns a minus value, else 0 returns
	 * @since TizenRT v2.0
	 */
	int seek(long offset, int origin) override;
protected:
	ssize_t onStreamBufferWritable() override;
};

} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
