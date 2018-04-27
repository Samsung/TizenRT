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

#ifndef __MEDIA_FILEINPUTDATASOURCE_H
#define __MEDIA_FILEINPUTDATASOURCE_H

#include <media/InputDataSource.h>
#include <media/MediaUtils.h>

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is file input data structure
 * @details @b #include <media/FileInputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class FileInputDataSource : public InputDataSource
{
public:
	/**
	 * @brief Constructs an empty FileInputDataSource.
	 * @details @b #include <media/FileInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	FileInputDataSource();
	/**
	 * @brief Deconstructs an empty FileInputDataSource.
	 * @details @b #include <media/FileInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~FileInputDataSource();
	/**
	 * @brief Constructs a new object provide with path
	 * @details @b #include <media/FileInputDataSource.h>
	 * param[in] dataPath The dataPath that the path of data
	 * @since TizenRT v2.0 PRE
	 */
	FileInputDataSource(const std::string& dataPath);
	/**
	 * @brief Copy constructs for FileInputDataSource.
	 * @details @b #include <media/FileInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	FileInputDataSource(const FileInputDataSource& source);
	/**
	 * @brief Operator= for FileInputDataSource.
	 * @details @b #include <media/FileInputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	FileInputDataSource& operator=(const FileInputDataSource& source);

	/**
	 * @brief Whether file is ready to be read.
	 * @details @b #include <media/FileInputDataSource.h>
	 * @return True is ready, False is not ready
	 * @since TizenRT v2.0 PRE
	 */
	bool isPrepare() override;
	/**
	 * @brief Open the file
	 * @details @b #include <media/FileInputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0 PRE
	 */
	bool open() override;
	/**
	 * @brief Close the file
	 * @details @b #include <media/FileInputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0 PRE
	 */
	bool close() override;

	/**
	 * @brief Gets the file data
	 * @details @b #include <media/FileInputDataSource.h>
	 * @param[out] buf The buf that read the data and fill it into the buffer
	 * @param[in] size The size that the size of the buffer
	 * @return if there is nothing to read, it returns 0, else readead size returns
	 * @since TizenRT v2.0 PRE
	 */
	size_t read(unsigned char* buf, size_t size) override;

	/**
	 * @brief Gets the file data
	 * @details @b #include <media/FileInputDataSource.h>
	 * @param[in] offset The offset from origin
	 * @param[in] origin Position used as reference
	 * @param[out] buf The buf that read the data and fill it into the buffer
	 * @param[in] size The size that the size of the buffer
	 * @return if failed, it returns -1, else readead size returns
	 * @since TizenRT v2.0
	 */
	int readAt(long offset, int origin, unsigned char* buf, size_t size) override;

private:
	std::string mDataPath;
	FILE* mFp;
};
} // namespace stream
} // namespace media

#endif
