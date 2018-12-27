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

#include <curl/curl.h>
#include <curl/easy.h>
#include <debug.h>

#include "HttpStream.h"

namespace media {
namespace stream {

#define SET_OPTION(curl, opt, val) \
	do {\
		auto result = curl_easy_setopt(curl, opt, val);\
		if (result != CURLE_OK) {\
			meddbg("curl_easy_setopt(%d...) failed!\n", (int)opt);\
			return false;\
		}\
	} while (0)

int HttpStream::mInitializeCount = 0;

std::shared_ptr<HttpStream> HttpStream::create()
{
	std::shared_ptr<HttpStream> stream(new HttpStream());
	if (!stream->init()) {
		meddbg("HttpStream instance create failed!\n");
		return nullptr;
	}

	return stream;
}

HttpStream::HttpStream() :
	mCurl(nullptr), mHttpHeaders(nullptr), mInitializeFlag(false)
{
}

HttpStream::~HttpStream()
{
	cleanup();
}

bool HttpStream::addHeader(const std::string &header) {
	mHttpHeaders = curl_slist_append(mHttpHeaders, header.c_str());
	if (!mHttpHeaders) {
		meddbg("curl_slist_append failed, header %s\n", header.c_str());
		return false;
	}

	return true;
}

bool HttpStream::setHeaderCallback(CallbackFunc callback, void *userdata)
{
	SET_OPTION(mCurl, CURLOPT_HEADERFUNCTION, callback);
	SET_OPTION(mCurl, CURLOPT_HEADERDATA, userdata);
	return true;
}

bool HttpStream::setWriteCallback(CallbackFunc callback, void *userdata)
{
	SET_OPTION(mCurl, CURLOPT_WRITEFUNCTION, callback);
	SET_OPTION(mCurl, CURLOPT_WRITEDATA, userdata);
	return true;
}

bool HttpStream::setReadCallback(CallbackFunc callback, void *userdata)
{
	SET_OPTION(mCurl, CURLOPT_READFUNCTION, callback);
	SET_OPTION(mCurl, CURLOPT_READDATA, userdata);
	return true;
}

bool HttpStream::init()
{
	if (mInitializeCount == 0) {
		if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
			meddbg("curl_global_init failed\n");
			return false;
		}
	}
	mInitializeCount++;
	mInitializeFlag = true;

	mCurl = curl_easy_init();
	if (mCurl == nullptr) {
		meddbg("HttpStream init failed!\n");
		return false;
	}

	return true;
}

void HttpStream::cleanup()
{
	if (mHttpHeaders) {
		curl_slist_free_all(mHttpHeaders);
		mHttpHeaders = nullptr;
	}

	if (mCurl != nullptr) {
		curl_easy_cleanup(mCurl);
		mCurl = nullptr;
	}

	if (mInitializeFlag) {
		mInitializeCount--;
		if (mInitializeCount == 0) {
			curl_global_cleanup();
		}
		mInitializeFlag = false;
	}
}

bool HttpStream::perform()
{
	if (mHttpHeaders) {
		SET_OPTION(mCurl, CURLOPT_HTTPHEADER, mHttpHeaders);
	}

	CURLcode result = curl_easy_perform(mCurl);
	if (result != CURLE_OK) {
		meddbg("curl_easy_perform failed, result %d - %s\n", result, curl_easy_strerror(result));
		return false;
	}

	long response_code = 0;
	result = curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &response_code);
	if (result != CURLE_OK) {
		meddbg("Get response failed! result[%d] response[%ld]\n", result, response_code);
		return false;
	}

	return true;
}

bool HttpStream::download(const std::string &url)
{
	SET_OPTION(mCurl, CURLOPT_HTTPGET, 1L);

	SET_OPTION(mCurl, CURLOPT_URL, url.c_str());

	SET_OPTION(mCurl, CURLOPT_SSL_VERIFYPEER, 0L);

	SET_OPTION(mCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	if (!perform()) {
		meddbg("http get failed!\n");
		return false;
	}

	return true;
}

bool HttpStream::upload(const std::string &url)
{
	SET_OPTION(mCurl, CURLOPT_UPLOAD, 1L);

	SET_OPTION(mCurl, CURLOPT_URL, url.c_str());

	if (!perform()) {
		meddbg("upload failed!\n");
		return false;
	}

	return true;
}

} // namespace stream
} // namespace media
