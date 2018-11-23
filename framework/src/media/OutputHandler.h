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
#ifndef __MEDIA_OUTPUTHANDLER_H
#define __MEDIA_OUTPUTHANDLER_H

#include <sys/types.h>
#include <memory>
#include <media/OutputDataSource.h>
#include <media/BufferObserverInterface.h>

#include "Encoder.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"

namespace media {
class MediaRecorderImpl;
namespace stream {
class OutputHandler : public BufferObserverInterface
{
public:
	OutputHandler();
	virtual ~OutputHandler() = default;
	void setOutputDataSource(std::shared_ptr<OutputDataSource> source);
	std::shared_ptr<OutputDataSource> getOutputDataSource() { return mOutputDataSource; }
	bool open();
	bool close();
	ssize_t write(unsigned char *buf, size_t size);
	bool start();
	bool stop();
	void flush();
	void createWorker();
	void destroyWorker();
	static void *workerMain(void *arg);
	void sleepWorker();
	void wakenWorker();

	std::shared_ptr<StreamBuffer> getStreamBuffer() { return mStreamBuffer; }
	void setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer);

	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

	void setRecorder(std::shared_ptr<MediaRecorderImpl> mr) { mRecorder = mr; }
	std::shared_ptr<MediaRecorderImpl> getRecorder() { return mRecorder.lock(); }

	ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

	bool registerEncoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate);
	void unregisterEncoder();

private:
	void writeToSource(size_t size);
	std::shared_ptr<OutputDataSource> mOutputDataSource;
	std::shared_ptr<Encoder> mEncoder;
	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;

	pthread_t mWorker;
	bool mIsFlushing;
	bool mIsWorkerAlive;
	std::mutex mMutex;
	std::condition_variable mCondv;
	std::mutex mFlushMutex;
	std::condition_variable mFlushCondv;

	std::weak_ptr<MediaRecorderImpl> mRecorder;
};
} // namespace stream
} // namespace media
#endif
