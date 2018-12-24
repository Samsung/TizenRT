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

#ifndef __MEDIA_HTTPSTREAM_H
#define __MEDIA_HTTPSTREAM_H

#include <chrono>
#include <string>
#include <curl/curl.h>
#include <debug.h>

#ifndef CONFIG_ENABLE_CURL
#error CONFIG_ENABLE_CURL should be turn on to use HttpStream class.
#endif

namespace media {
namespace stream {

class HttpStream : public std::enable_shared_from_this<HttpStream>
{
public:
	/**
	 * callback function type.
	 * size_t callback(char *ptr, size_t size, size_t nmemb, void *userdata)
	 * The callbacks CANNOT be non-static class member functions.
	 */
	using CallbackFunc = size_t (*)(char *ptr, size_t size, size_t nmemb, void *userdata);

public:
	static std::shared_ptr<HttpStream> create();
	~HttpStream();

	/*
	 * Adds an Http level header
	 */
	bool addHeader(const std::string &header);

	/*
	 * Sets the callback for HTTP header data
	 */
	bool setHeaderCallback(CallbackFunc callback, void *userdata);

	/*
	 * Sets the callback for downloading response data
	 */
	bool setWriteCallback(CallbackFunc callback, void *userdata);

	/*
	 * Sets the callback for uploading local data
	 */
	bool setReadCallback(CallbackFunc callback, void *userdata);

	/*
	 * Sets the callback for uploading local data
	 */
	bool download(const std::string &url);

	/*
	 * Sets the callback for uploading local data
	 */
	bool upload(const std::string &url);

private:
	HttpStream();
	bool init();
	void cleanup();
	bool perform();

	// curl handle
	CURL *mCurl;
	// http level headers
	curl_slist *mHttpHeaders;

	bool mInitializeFlag;
	static int mInitializeCount;
};

} // namespace stream
} // namespace media

#endif  // __MEDIA_HTTPSTREAM_H
