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
 * @file media/OutputDataSource.h
 * @brief Media OutputDataSource APIs
 */

#ifndef __MEDIA_OUTPUTDATASOURCE_H
#define __MEDIA_OUTPUTDATASOURCE_H

#include <pthread.h>
#include <media/DataSource.h>
#include <media/BufferObserverInterface.h>

namespace media {
class MediaRecorderImpl;
namespace stream {

/**
 * @class
 * @brief This class is output data structure
 * @details @b #include <media/OutputDataSource.h>
 * @since TizenRT v2.0
 */
class OutputDataSource : public DataSource
{
public:
	/**
	 * @brief Constructs an empty OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	OutputDataSource();
	/**
	 * @brief Constructs a new object provide with audio configuration
	 * @details @b #include <media/OutputDataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * @since TizenRT v2.0
	 */
	OutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat);

	/**
	 * @brief Copy constructs for OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	OutputDataSource(const OutputDataSource& source);
	/**
	 * @brief Operator= for OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	OutputDataSource& operator=(const OutputDataSource& source);
	/**
	 * @brief Deconstructs an empty OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ~OutputDataSource();

public:
	/**
	 * @brief Puts the stream data
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ssize_t write(unsigned char *buf, size_t size) = 0;

	/**
	 * @brief Register current recorder to get data souce state and other infomations.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	void setRecorder(std::shared_ptr<MediaRecorderImpl> mr) { mRecorder = mr; }

protected:
	std::shared_ptr<MediaRecorderImpl> getRecorder() { return mRecorder.lock(); }

private:
	std::weak_ptr<MediaRecorderImpl> mRecorder;
};
} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
