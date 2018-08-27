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
class Encoder;
class MediaRecorderImpl;
namespace stream {
class StreamBuffer;
class StreamBufferReader;
class StreamBufferWriter;

/**
 * @class
 * @brief This class is output data structure
 * @details @b #include <media/OutputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class OutputDataSource
	: public DataSource
	, public BufferObserverInterface
{
public:
	/**
	 * @brief Constructs an empty OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	OutputDataSource();
	/**
	 * @brief Constructs a new object provide with audio configuration
	 * @details @b #include <media/OutputDataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * @since TizenRT v2.0 PRE
	 */
	OutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat);

	/**
	 * @brief Copy constructs for OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	OutputDataSource(const OutputDataSource& source);
	/**
	 * @brief Operator= for OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	OutputDataSource& operator=(const OutputDataSource& source);
	/**
	 * @brief Deconstructs an empty OutputDataSource.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~OutputDataSource();

public:
	/**
	 * @brief Puts the stream data
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ssize_t write(unsigned char *buf, size_t size);

	/**
	 * @brief Register current recorder to get data souce state and other infomations.
	 * @details @b #include <media/OutputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	void setRecorder(std::shared_ptr<MediaRecorderImpl> mr) { mRecorder = mr; }

public:
	/* BufferObserverInterface
	 * Override these methods in derived class if needed.
	 */
	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

protected:
	bool start();
	bool stop();
	void flush();
	virtual ssize_t onStreamBufferReadable(bool isFlush) = 0;

protected:
	const std::shared_ptr<Encoder> getEncoder();
	void setEncoder(std::shared_ptr<Encoder> encoder);
	void setAudioType(audio_type_t audioType);
	audio_type_t getAudioType();

	void setStreamBuffer(std::shared_ptr<StreamBuffer>);
	std::shared_ptr<StreamBuffer> getStreamBuffer() { return mStreamBuffer; }
	std::shared_ptr<StreamBufferReader> getBufferReader() { return mBufferReader; }
	std::shared_ptr<MediaRecorderImpl> getRecorder() { return mRecorder.lock(); }
	void createWorker();
	void destoryWorker();
	ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

private:
	audio_type_t mAudioType;
	std::shared_ptr<Encoder> mEncoder;
	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;
	std::weak_ptr<MediaRecorderImpl> mRecorder;

	bool mIsFlushing;
	bool mIsWorkerAlive;
	pthread_t mWorker;
	std::mutex mMutex;
	std::condition_variable mCondv;

	void sleepWorker();
	void wakenWorker();
	static void *workerMain(void *arg);
};
} // namespace stream
} // namespace media

#endif
/** @} */ // end of MEDIA group
