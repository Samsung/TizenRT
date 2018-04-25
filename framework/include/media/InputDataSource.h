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

#ifndef __MEDIA_INPUTDATASOURCE_H
#define __MEDIA_INPUTDATASOURCE_H

#include <fstream>
#include <memory>
#include <media/DataSource.h>
#include <media/MediaUtils.h>

namespace media {
class Decoder;
namespace stream {
/**
 * @class
 * @brief This class is input data structure
 * @details @b #include <media/InputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class InputDataSource : public DataSource
{
public:
	/**
	 * @brief Constructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource();
	/**
	 * @brief Copy constructs for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource(const InputDataSource& source);
	/**
	 * @brief Operator= for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource& operator=(const InputDataSource& source);
	/**
	 * @brief Deconstructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~InputDataSource();
  
  	/**
	 * @brief Gets the stream data
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual size_t read(unsigned char* buf, size_t size) = 0;

  	/**
	 * @brief Gets the stream data from offset
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual int readAt(long offset, int origin, unsigned char* buf, size_t size) = 0;

	/**
	 * @brief Gets current audio decoder. The audio decoder is set by each data source.
	 * @details @b #include <media/InputDataSource.h>
	 * @return Current audio decoder instance.
	 * @since TizenRT v2.0 PRE
	 */
	const std::shared_ptr<Decoder> getDecoder();

protected:
	void setDecoder(std::shared_ptr<Decoder> decoder);
	void setAudioType(utils::audio_type_t audioType);
	utils::audio_type_t getAudioType();

private:
	utils::audio_type_t mAudioType;
	std::shared_ptr<Decoder> mDecoder;
};

} // namespace stream
} // namespace media

#endif
