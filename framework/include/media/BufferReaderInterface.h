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
 * @file media/BufferReaderInterface.h
 * @brief Media BufferReaderInterface APIs
 */

#ifndef __MEDIA_BUFFERREADERINTERFACE_H
#define __MEDIA_BUFFERREADERINTERFACE_H

namespace media {
/**
 * @class
 * @brief This class is buffer reader data structure
 * @details @b #include <media/BufferReaderInterface.h>
 * @since TizenRT v2.0 PRE
 */
class BufferReaderInterface
{
public:
	/**
	 * Virtual destructor to assure proper cleanup of derived types.
	 */
	virtual ~BufferReaderInterface() = default;
	/**
	 * @brief Read data and save to the given buffer.
	 * @details @b #include <media/BufferReaderInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual size_t read(unsigned char *buf, size_t size, bool sync = true) = 0;
	/**
	 * @brief Available data bytes to read.
	 * @details @b #include <media/BufferReaderInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual size_t sizeOfData() = 0;
};

} // namespace media
#endif
/** @} */ // end of MEDIA group
