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
 * @file media/HttpInputDataSource.h
 * @brief Media HttpInputDataSource APIs
 */

#ifndef __MEDIA_HTTPINPUTDATASOURCE_H
#define __MEDIA_HTTPINPUTDATASOURCE_H

#include <media/InputDataSource.h>
#include <media/BufferObserverInterface.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

namespace media {
namespace stream {

class HttpStream;
class StreamBuffer;
class StreamBufferReader;
class StreamBufferWriter;

/**
 * @class
 * @brief This class is http input data structure
 * @details @b #include <media/HttpInputDataSource.h>
 * @since TizenRT v2.0
 */
class HttpInputDataSource : public InputDataSource, public BufferObserverInterface
{
public:
	/**
	 * @brief Constructs an empty HttpInputDataSource.
	 * @details @b #include <media/HttpInputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0
	 */
	HttpInputDataSource() = delete;
	/**
	 * @brief Deconstructs an HttpInputDataSource.
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ~HttpInputDataSource();
	/**
	 * @brief Constructs a new object provide with url
	 * @details @b #include <media/HttpInputDataSource.h>
	 * param[in] url The URL of the internet audio stream
	 * @since TizenRT v2.0
	 */
	HttpInputDataSource(const std::string &url);
	/**
	 * @brief Copy constructs for HttpInputDataSource.
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @since TizenRT v2.0
	 */
	HttpInputDataSource(const HttpInputDataSource &source);
	/**
	 * @brief Operator= for HttpInputDataSource.
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @since TizenRT v2.0
	 */
	HttpInputDataSource &operator=(const HttpInputDataSource &source);

	/**
	 * @brief Whether http source is ready to be read.
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @return True is ready, False is not ready
	 * @since TizenRT v2.0
	 */
	bool isPrepared() override;
	/**
	 * @brief Open the http source
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool open() override;
	/**
	 * @brief Close the http source
	 * @details @b #include <media/HttpInputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool close() override;
	/**
	 * @brief Read the audio stream data
	 * @details @b #include <media/HttpInputDataSource.h>
	 * param[in] buf  pointer to a buffer
	 * param[in] size size to be read
	 * @return The number of elements read. On failure, it return negative value.
	 * @since TizenRT v2.0
	 */
	ssize_t read(unsigned char *buf, size_t size) override;

public:
	/**
	 * BufferObserverInterface
	 */
	void onBufferOverrun() override;
	void onBufferUnderrun() override;
	void onBufferUpdated(ssize_t change, size_t current) override;

private:
	static size_t HeaderCallback(char *data, size_t size, size_t nmemb, void *userp);
	static size_t WriteCallback(char *data, size_t size, size_t nmemb, void *userp);
	static void *workerMain(void *arg);

private:
	std::string mContentType;
	std::string mUrl;
	pthread_t mThread;
	std::mutex mMutex;
	std::condition_variable mCondv;
	bool mIsHeaderReceived;
	bool mIsDataReceived;
	std::shared_ptr<HttpStream> mHttpStream;
	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;
};

} // namespace stream
} // namespace media

#endif // __MEDIA_HTTPINPUTDATASOURCE_H
/** @} */ // end of MEDIA group
