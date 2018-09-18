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
 * @file media/FileOutputDataSource.h
 * @brief Media FileOutputDataSource APIs
 */

#ifndef __MEDIA_FILEOUTPUTDATASOURCE_H
#define __MEDIA_FILEOUTPUTDATASOURCE_H

#include <media/OutputDataSource.h>

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is file output data structure
 * @details @b #include <media/FileOutputDataSource.h>
 * @since TizenRT v2.0
 */
class FileOutputDataSource : public OutputDataSource
{
public:
	/**
	 * @brief Constructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0
	 */
	FileOutputDataSource() = delete;
	/**
	 * @brief Constructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * param[in] dataPath The dataPath that the path of data
	 * @since TizenRT v2.0
	 */
	FileOutputDataSource(const std::string& dataPath);
	/**
	 * @brief Constructs a new object provide with audio configuration
	 * @details @b #include <media/FileOutputDataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * param[in] datapath   The datapath that the path of data
	 * @since TizenRT v2.0
	 */
	FileOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat, const std::string& dataPath);
	/**
	 * @brief Copy constructs for FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	FileOutputDataSource(const FileOutputDataSource& source);
	/**
	 * @brief Operator= for FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	FileOutputDataSource& operator=(const FileOutputDataSource& source);
	/**
	 * @brief Deconstructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ~FileOutputDataSource();

	/**
	 * @brief Whether file is ready to be write.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is ready, False is not ready
	 * @since TizenRT v2.0
	 */
	bool isPrepare() override;
	/**
	 * @brief Open the file
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool open() override;
	/**
	 * @brief Close the file
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool close() override;
	/**
	 * @brief Write the file
	 * @details @b #include <media/FileOutputDataSource.h>
	 * param[in] buf poiter to a buffer
	 * param[in] size Number of size to write
	 * @return Data size written to buf.
	 * @since TizenRT v2.0
	 */
	ssize_t write(unsigned char *buf, size_t size) override;

private:
	std::string mDataPath;
	FILE* mFp;
};
} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
