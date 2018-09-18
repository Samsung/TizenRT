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
 * @file media/SocketOutputDataSource.h
 * @brief Media SocketOutputDataSource APIs
 */

#ifndef __MEDIA_SOCKETOUTPUTDATASOURCE_H
#define __MEDIA_SOCKETOUTPUTDATASOURCE_H

#include <media/OutputDataSource.h>

#ifndef CONFIG_NET
#error CONFIG_NET should be turn on to use SocketOutputDataSource class.
#endif

namespace media {
namespace stream {
/**
 * @class
 * @brief This class is file output data structure
 * @details @b #include <media/SocketOutputDataSource.h>
 * @since TizenRT v2.0
 */
class SocketOutputDataSource : public OutputDataSource
{
public:
	/**
	 * @brief Constructs an empty SocketOutputDataSource.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * Delete the default construct
	 * @since TizenRT v2.0
	 */
	SocketOutputDataSource() = delete;
	/**
	 * @brief Constructs an empty SocketOutputDataSource.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * param[in] ipAddr     The IP address for connecting to the socket server
	 * param[in] port       The port number for connecting to the socket server
	 * @since TizenRT v2.0
	 */
	SocketOutputDataSource(const std::string& ipAddr, const uint16_t port);
	/**
	 * @brief Constructs a new object provide with audio configuration
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * param[in] ipAddr     The IP address for connecting to the socket server
	 * param[in] port       The port number for connecting to the socket server
	 * @since TizenRT v2.0
	 */
	SocketOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat, const std::string& ipAddr, const uint16_t port);
	/**
	 * @brief Copy constructs for SocketOutputDataSource.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	SocketOutputDataSource(const SocketOutputDataSource& source);
	/**
	 * @brief Operator= for SocketOutputDataSource.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	SocketOutputDataSource& operator=(const SocketOutputDataSource& source);
	/**
	 * @brief Deconstructs an empty SocketOutputDataSource.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual ~SocketOutputDataSource();

	/**
	 * @brief Whether file is ready to be write.
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @return True is ready, False is not ready
	 * @since TizenRT v2.0
	 */
	bool isPrepare() override;
	/**
	 * @brief Open the file
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool open() override;
	/**
	 * @brief Close the file
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @return True is Success, False is Fail
	 * @since TizenRT v2.0
	 */
	bool close() override;

	/**
	 * @brief Puts the file data
	 * @details @b #include <media/SocketOutputDataSource.h>
	 * @param[in] buf The buf that buffer to be written to the file
	 * @param[in] size The size that the size of the buffer
	 * @return if there is nothing to write, it returns 0
	 *         if error occurred, it returns -1, else written size returns
	 * @since TizenRT v2.0
	 */
	ssize_t write(unsigned char* buf, size_t size) override;

private:
	std::string mIpAddr;
	uint16_t mPort;
	int mSockFd;
};
} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
