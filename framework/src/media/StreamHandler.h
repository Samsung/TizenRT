/* ****************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef __MEDIA_STREAMHANDLER_H
#define __MEDIA_STREAMHANDLER_H

#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <media/DataSource.h>
#include <media/BufferObserverInterface.h>

#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"

namespace media {
namespace stream {
class StreamHandler : public BufferObserverInterface
{
public:
	StreamHandler();

	virtual bool open();
	virtual bool close();
	virtual bool start();
	virtual bool stop();

	std::shared_ptr<DataSource> getDataSource()
	{
		return mDataSource;
	}
protected:
	void setDataSource(std::shared_ptr<DataSource> dataSource)
	{
		mDataSource = dataSource;
	}
	void setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer);
	std::shared_ptr<StreamBuffer> getStreamBuffer()
	{
		return mStreamBuffer;
	}

	virtual void resetWorker() = 0;
	virtual bool processWorker() = 0;
	void wakenWorker();
	virtual void sleepWorker();

	virtual bool probeDataSource() = 0;
	virtual bool registerCodec(audio_type_t audioType, unsigned int channels, unsigned int sampleRate) = 0;
	virtual void unregisterCodec() = 0;
	virtual const char *getWorkerName() const = 0;

	std::shared_ptr<StreamBuffer> mStreamBuffer;
	std::shared_ptr<StreamBufferReader> mBufferReader;
	std::shared_ptr<StreamBufferWriter> mBufferWriter;

	pthread_t mWorker;
	size_t mWorkerStackSize;
	bool mIsWorkerAlive;
private:
	void createWorker();
	void destroyWorker();
	static void *workerMain(void *arg);

	std::mutex mMutex;
	std::condition_variable mCondv;

	std::shared_ptr<DataSource> mDataSource;
};
} // namespace stream
} // namespace media
#endif
