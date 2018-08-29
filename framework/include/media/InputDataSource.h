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
 * @file media/InuptDataSource.h
 * @brief Media InuptDataSource APIs
 */

#ifndef __MEDIA_INPUTDATASOURCE_H
#define __MEDIA_INPUTDATASOURCE_H

#include <fstream>
#include <memory>
#include <pthread.h>
#include <media/DataSource.h>
#include <media/BufferObserverInterface.h>

namespace media {
class Decoder;
class MediaPlayerImpl;

typedef enum buffer_state_e {
	BUFFER_STATE_EMPTY,
	BUFFER_STATE_BUFFERING,
	BUFFER_STATE_BUFFERED,
	BUFFER_STATE_FULL,
} buffer_state_t;

namespace stream {
class StreamBuffer;
class StreamBufferReader;
class StreamBufferWriter;

/**
 * @class
 * @brief This class is input data structure
 * @details @b #include <media/InputDataSource.h>
 * @since TizenRT v2.0 PRE
 */
class InputDataSource
	: public DataSource
	, public BufferObserverInterface
{
public:
	/**
	 * @brief Constructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource();
	/**
	 * @brief Copy constructs for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource(const InputDataSource &source);
	/**
	 * @brief Operator= for InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	InputDataSource &operator=(const InputDataSource &source);
	/**
	 * @brief Deconstructs an empty InputDataSource.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ~InputDataSource();

	/**
	 * @brief Gets the stream data
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual ssize_t read(unsigned char *buf, size_t size);

	/**
	 * @brief Set the offset
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual int seek(long offset, int origin) = 0;

	/**
	 * @brief Gets the data start from offset
	 * @details @b #include <media/InputDataSource.h>
	 * @param[in] offset The offset from origin
	 * @param[in] origin Position used as reference
	 * @param[out] buf The buf that read the data and fill it into the buffer
	 * @param[in] size The size that the size of the buffer
	 * @return if failed, it returns -1, else readead size returns
	 * @since TizenRT v2.0
	 */
	virtual int readAt(long offset, int origin, unsigned char *buf, size_t size);

	/**
	 * @brief Register current player to get data souce state and other informations.
	 * @details @b #include <media/InputDataSource.h>
	 * @since TizenRT v2.0 PRE
	 */
	void setPlayer(std::shared_ptr<MediaPlayerImpl> mp) { mPlayer = mp; }

	/* BufferObserverInterface
	 * Override these methods in derived class if needed.
	 */
	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

protected:
	bool start();
	bool stop();
	virtual ssize_t onStreamBufferWritable() = 0;

protected:
	void setAudioType(audio_type_t audioType);
	audio_type_t getAudioType();
	void registerDecoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate);
	void unregisterDecoder();
	size_t getDecodeFrames(unsigned char *buf, size_t *size);

	void setStreamBuffer(std::shared_ptr<StreamBuffer>);
	std::shared_ptr<StreamBuffer> getStreamBuffer() { return mStreamBuffer; }
	void setBufferState(buffer_state_t state);
	std::shared_ptr<MediaPlayerImpl> getPlayer() { return mPlayer.lock(); }
	void createWorker();
	void destoryWorker();
	ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

private:
	audio_type_t mAudioType;
	std::shared_ptr<Decoder> mDecoder;
	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;
	std::weak_ptr<MediaPlayerImpl> mPlayer;

	buffer_state_t mState;
	size_t mTotalBytes;
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
