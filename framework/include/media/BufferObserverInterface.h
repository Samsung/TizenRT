/******************************************************************
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
 * @file media/BufferObserverInterface.h
 * @brief Media BufferObserverInterface APIs
 */

#ifndef __MEDIA_STREAMOBSERVERINTERFACE_H
#define __MEDIA_STREAMOBSERVERINTERFACE_H

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is buffer observer data structure
 * @details @b #include <media/BufferObserverInterface.h>
 * @since TizenRT v2.0
 */
class BufferObserverInterface
{
public:
	/**
	 * Virtual destructor to assure proper cleanup of derived types.
	 */
	virtual ~BufferObserverInterface() = default;
	/**
	 * @brief Called when buffer overrun
	 * @details @b #include <media/BufferObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onBufferOverrun() = 0;
	/**
	 * @brief Called when buffer underrun
	 * @details @b #include <media/BufferObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onBufferUnderrun() = 0;
	/**
	 * @brief Called when buffer updated
	 * @details @b #include <media/BufferObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onBufferUpdated(ssize_t change, size_t current) = 0;
};

} // namespace stream
} // namespace media
#endif
/** @} */ // end of MEDIA group
