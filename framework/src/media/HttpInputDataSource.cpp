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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>
#include <unistd.h>
#include <assert.h>
#include <media/HttpInputDataSource.h>
#include <chrono>

#include "utils/MediaUtils.h"
#include "HttpStream.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"


#ifndef CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_SIZE
#define CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_THRESHOLD
#define CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_THRESHOLD 2048
#endif

#ifndef CONFIG_HTTPSOURCE_DOWNLOAD_STACKSIZE
#define CONFIG_HTTPSOURCE_DOWNLOAD_STACKSIZE 8192
#endif

namespace media {
namespace stream {

// Content-Type tag
static const std::string TAG_CONTENT_TYPE = "Content-Type:";

static const std::chrono::seconds WAIT_HEADER_TIMEOUT = std::chrono::seconds(3);
static const std::chrono::seconds WAIT_DATA_TIMEOUT = std::chrono::seconds(3);

HttpInputDataSource::HttpInputDataSource(const std::string &url)
	: InputDataSource(), mUrl(url), mThread((pthread_t)0)
{
	medvdbg("url: %s\n", mUrl.c_str());
}

HttpInputDataSource::HttpInputDataSource(const HttpInputDataSource &source)
	: InputDataSource(source), mUrl(source.mUrl), mThread((pthread_t)0)
{
}

HttpInputDataSource &HttpInputDataSource::operator=(const HttpInputDataSource &source)
{
	InputDataSource::operator=(source);
	return *this;
}

bool HttpInputDataSource::open()
{
	medvdbg("HttpInputDataSource::open!\n");

	if (isPrepared()) {
		medvdbg("HttpInputDataSource is already opened!\n");
		return true;
	}

	if (mStreamBuffer == nullptr) {
		mStreamBuffer = StreamBuffer::Builder()
								.setBufferSize(CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_SIZE)
								.setThreshold(CONFIG_HTTPSOURCE_DOWNLOAD_BUFFER_THRESHOLD)
								.build();

		if (mStreamBuffer == nullptr) {
			meddbg("mStreamBuffer is nullptr!\n");
			return false;
		}

		mStreamBuffer->setObserver(this);
		mBufferReader = std::make_shared<StreamBufferReader>(mStreamBuffer);
		mBufferWriter = std::make_shared<StreamBufferWriter>(mStreamBuffer);
	}

	if (mHttpStream == nullptr) {
		mHttpStream = HttpStream::create();
		if (mHttpStream == nullptr) {
			meddbg("mHttpStream is nullptr!\n");
			return false;
		}
	}

	std::unique_lock<std::mutex> lock(mMutex);
	mIsHeaderReceived = false;
	mIsDataReceived = false;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, CONFIG_HTTPSOURCE_DOWNLOAD_STACKSIZE);
	struct sched_param sparam;
	sparam.sched_priority = 100;
	pthread_attr_setschedparam(&attr, &sparam);

	int iRet = pthread_create(&mThread, &attr, static_cast<pthread_startroutine_t>(workerMain), this);
	if (iRet != OK) {
		meddbg("Fail to create download thread, err:%d\n", iRet);
		return false;
	}
	pthread_setname_np(mThread, "HttpSourceDownloader");

	// wait for Content-Type header
	if (!mCondv.wait_for(lock, WAIT_HEADER_TIMEOUT, [=]{ return mIsHeaderReceived; })) {
		meddbg("download:: wait header timeout!\n");
		mBufferWriter->setEndOfStream();
		return false;
	}

	auto audioType = utils::getAudioTypeFromMimeType(mContentType);

	switch (audioType) {
	case AUDIO_TYPE_MP3:
	case AUDIO_TYPE_AAC: {
		// wait for audio stream data
		if (!mCondv.wait_for(lock, WAIT_DATA_TIMEOUT, [=]{ return mIsDataReceived; })) {
			meddbg("download:: wait audio data timeout!\n");
			mBufferWriter->setEndOfStream();
			return false;
		}

		size_t templen = mBufferReader->sizeOfData();
		unsigned char *tempbuf = new unsigned char[templen];
		if (tempbuf == nullptr) {
			meddbg("memory allocation failed! size 0x%x\n", templen);
			mBufferWriter->setEndOfStream();
			return false;
		}

		size_t dlen = mBufferReader->copy(tempbuf, templen);
		unsigned int channel;
		unsigned int sampleRate;
		bool ret = utils::header_parsing(tempbuf, dlen, audioType, &channel, &sampleRate, NULL);
		delete[] tempbuf;

		if (!ret) {
			meddbg("header parsing failed\n");
			mBufferWriter->setEndOfStream();
			return false;
		}

		medvdbg("audioType:%d,setSampleRate:%u,setChannels:%u\n", (int)audioType, sampleRate, channel);
		setAudioType(audioType);
		setSampleRate(sampleRate);
		setChannels(channel);
		break;
	}

	default:
		/* unsupported audio type */
		meddbg("HttpInputDataSource::open, unsupported audio type %d\n", (int)audioType);
		mBufferWriter->setEndOfStream();
		return false;
	}

	medvdbg("HttpInputDataSource::open! exit\n");
	return true;
}

bool HttpInputDataSource::close()
{
	medvdbg("HttpInputDataSource::close enter\n");
	if (mBufferWriter) {
		mBufferWriter->setEndOfStream();
	}

	if (mThread != (pthread_t)0) {
		pthread_join(mThread, NULL);
		mThread = (pthread_t)0;
	}

	mHttpStream = nullptr;
	mStreamBuffer = nullptr;
	mBufferReader = nullptr;
	mBufferWriter = nullptr;
	setAudioType(AUDIO_TYPE_UNKNOWN);
	medvdbg("HttpInputDataSource::close exit!\n");
	return true;
}

bool HttpInputDataSource::isPrepared()
{
	return ((mStreamBuffer != nullptr) && (mHttpStream != nullptr) && (getAudioType() != AUDIO_TYPE_UNKNOWN));
}

ssize_t HttpInputDataSource::read(unsigned char *buf, size_t size)
{
	if (!isPrepared()) {
		meddbg("[line:%d] Fail : HttpInputDataSource is not prepared\n", __LINE__);
		return EOF;
	}

	if (buf == nullptr) {
		meddbg("[line:%d] Fail : buf is nullptr\n", __LINE__);
		return EOF;
	}

	size_t rlen = 0;
	if (mBufferReader) {
		rlen = mBufferReader->read(buf, size);
	}

	medvdbg("read size: %d\n", rlen);
	return rlen;
}

void HttpInputDataSource::onBufferOverrun()
{
}

void HttpInputDataSource::onBufferUnderrun()
{
}

void HttpInputDataSource::onBufferUpdated(ssize_t change, size_t current)
{
	if (!mIsDataReceived) {
		if (current >= mStreamBuffer->getThreshold()) {
			medvdbg("Enough data received!\n");
			std::lock_guard<std::mutex> lock(mMutex);
			mIsDataReceived = true;
			mCondv.notify_one();
		}
	}
}

size_t HttpInputDataSource::HeaderCallback(char *data, size_t size, size_t nmemb, void *userp)
{
	auto source = static_cast<HttpInputDataSource *>(userp);
	if (source->mBufferReader->isEndOfStream()) {
		medwdbg("end-of-stream:true\n");
		return 0;
	}

	size_t totalsize = size * nmemb;
	std::string header(data, totalsize);
	medvdbg("%s\n", header.c_str());
	auto pos = header.find(TAG_CONTENT_TYPE);
	if (pos != std::string::npos) {
		pos = header.find_first_not_of(' ', pos + TAG_CONTENT_TYPE.length());
		auto end = header.find((char)0x0d, pos); // CR: 0x0d
		source->mContentType = header.substr(pos, end - pos);
		if (!source->mIsHeaderReceived) {
			std::lock_guard<std::mutex> lock(source->mMutex);
			source->mIsHeaderReceived = true;
			source->mCondv.notify_one();
		}
	}

	return totalsize;
}

size_t HttpInputDataSource::WriteCallback(char *data, size_t size, size_t nmemb, void *userp)
{
	auto source = static_cast<HttpInputDataSource *>(userp);
	size_t totalsize = size * nmemb;
	return source->mBufferWriter->write((unsigned char *)data, totalsize);
}

void *HttpInputDataSource::workerMain(void *arg)
{
	medvdbg("download thread enter!\n");
	auto source = static_cast<HttpInputDataSource *>(arg);

	//mHttpStream->addHeader("Icy-MetaData:1"); // not support now
	source->mHttpStream->setHeaderCallback(HeaderCallback, arg);
	source->mHttpStream->setWriteCallback(WriteCallback, arg);
	if (!source->mHttpStream->download(source->mUrl)) {
		medwdbg("download failed or terminated!\n");
		// TODO: send network error code to upper layer later
	}

	source->mBufferWriter->setEndOfStream();
	medvdbg("download thread exit!\n");
	return NULL;
}

HttpInputDataSource::~HttpInputDataSource()
{
	if (isPrepared()) {
		close();
	}
}

} // namespace stream
} // namespace media
