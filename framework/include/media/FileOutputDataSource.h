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

#ifndef __MEDIA_FILEOUTPUTDATASOURCE_H
#define __MEDIA_FILEOUTPUTDATASOURCE_H

#include <media/OutputDataSource.h>

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is file output data structure
 * @details @b #include <media/FileOutputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class FileOutputDataSource : public OutputDataSource
{
public:
	/**
	 * @brief Constructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0 PRE
	 */
	FileOutputDataSource() = delete;
	/**
	 * @brief Constructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * param[in] dataPath The dataPath that the path of data
	 * @since TizenRT v2.0 PRE
	 */
	FileOutputDataSource(const std::string& dataPath);
	/**
	 * @brief Constructs a new object provide with audio configuration
	 * @details @b #include <media/FileOutputDataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * param[in] datapath   The datapath that the path of data
	 * @since TizenRT v2.0 PRE
	 */
	FileOutputDataSource(unsigned short channels, unsigned int sampleRate, int pcmFormat, const std::string& dataPath);
	/**
	 * @brief Copy constructs for FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	FileOutputDataSource(const FileOutputDataSource& source);
	/**
	 * @brief Operator= for FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	FileOutputDataSource& operator=(const FileOutputDataSource& source);
	/**
	 * @brief Deconstructs an empty FileOutputDataSource.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~FileOutputDataSource();

	/**
	 * @brief Whether file is ready to be write.
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is ready, False is not ready
	 * @since TizenRT v2.0 PRE
	 */
	bool isPrepare() override;
	/**
	 * @brief Open the file
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0 PRE
	 */
	bool open() override;
	/**
	 * @brief Close the file
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0 PRE
	 */
	bool close() override;
	
	/**
	 * @brief Puts the file data
	 * @details @b #include <media/FileOutputDataSource.h>
	 * @param[in] buf The buf that buffer to be written to the file
	 * @param[in] size The size that the size of the buffer
	 * @return if there is nothing to write, it returns 0, else written size returns
	 * @since TizenRT v2.0 PRE
	 */
	size_t write(unsigned char* buf, size_t size) override;

private:
	std::string mDataPath;
	FILE* mFp;
};
} // namespace stream
} // namespace media

#endif
