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
 * @file media/InuptDataSource.h
 * @brief Media InuptDataSource APIs
 */

#ifndef __MEDIA_INPUTDATASOURCE_H
#define __MEDIA_INPUTDATASOURCE_H

#include <memory>
#include <media/DataSource.h>

namespace media {
namespace stream {

/**
 * @class
 * @brief This class is input data structure
 * @details @b #include <media/InputDataSource.h>
 * @since TizenRT v2.0
 */
class InputDataSource
	: public DataSource
{
public:
	/**
	 * @brief Constructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	InputDataSource();
	/**
	 * @brief Copy constructs for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	InputDataSource(const InputDataSource &source);
	/**
	 * @brief Operator= for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	InputDataSource &operator=(const InputDataSource &source);
	/**
	 * @brief Deconstructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ~InputDataSource();

	/**
	 * @brief Gets the stream data
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ssize_t read(unsigned char *buf, size_t size) = 0;
};

} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
