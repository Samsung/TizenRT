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
#ifndef __MEDIA_INPUTHANDLER_H
#define __MEDIA_INPUTHANDLER_H

#include <memory>

#include <sys/types.h>

#include <media/InputDataSource.h>
#include <media/BufferObserverInterface.h>

#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"
#include "Decoder.h"

namespace media {
class MediaPlayerImpl;
namespace stream {

typedef enum buffer_state_e : int {
	BUFFER_STATE_EMPTY,
	BUFFER_STATE_BUFFERING,
	BUFFER_STATE_BUFFERED,
	BUFFER_STATE_FULL,
} buffer_state_t;

class InputHandler : public BufferObserverInterface
{
public:
    InputHandler();
    virtual ~InputHandler() = default;
    void setInputDataSource(std::shared_ptr<InputDataSource> source);
    bool open();
    bool close();
    ssize_t read(unsigned char *buf, size_t size);
    bool start();
    bool stop();
    void createWorker();
    void destroyWorker();
    static void *workerMain(void *arg);
    void sleepWorker();
    void wakenWorker();

	std::shared_ptr<StreamBuffer> getStreamBuffer() { return mStreamBuffer; }
    void setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer);
    void setBufferState(buffer_state_t state);

	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

    void setPlayer(std::shared_ptr<MediaPlayerImpl> mp) { mPlayer = mp; }
	std::shared_ptr<MediaPlayerImpl> getPlayer() { return mPlayer.lock(); }

    ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

	bool registerDecoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate);
	void unregisterDecoder();
	size_t getDecodeFrames(unsigned char *buf, size_t *size);

    std::shared_ptr<InputDataSource> getInputDataSource() { return mInputDataSource; }

private:
    std::shared_ptr<InputDataSource> mInputDataSource;
	std::shared_ptr<Decoder> mDecoder;
	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;
	pthread_t mWorker;
    bool mIsWorkerAlive;
	std::weak_ptr<MediaPlayerImpl> mPlayer;

	buffer_state_t mState;
	size_t mTotalBytes;

	std::mutex mMutex;
	std::condition_variable mCondv;
};
} // namespace stream
} // namespace media
#endif
