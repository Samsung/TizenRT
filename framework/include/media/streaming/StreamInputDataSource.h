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

#ifndef __MEDIA_STREAMINPUTDATASOURCE_H
#define __MEDIA_STREAMINPUTDATASOURCE_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Stream.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include <media/InputDataSource.h>

namespace media {
namespace stream {

typedef enum stream_status {
	BUFFER_STREAM_EMPTY,
	BUFFER_STREAM_FULL,
    BUFFER_STREAM_BUFFERING,
    BUFFER_STREAM_BUFFERED,
    BUFFER_STREAM_UNKNOWN
} stream_status_t;


class StreamInputDataSource : public InputDataSource {
public:
    StreamInputDataSource();
    virtual ~StreamInputDataSource();

public:
    ssize_t read(unsigned char *buf, size_t size, bool sync = true) override;
    int readAt(long offset, int origin, unsigned char* buf, size_t size) override;
	void setObserver(std::shared_ptr<StreamObserverInterface> observer);
	int getStreamId();
	bool start();
	bool stop();
    void setThreshold(size_t sec);
    size_t getThreshold();

    virtual void onStreamWritable() = 0;
    virtual bool endOfStream() = 0;

protected:
    void createWriterWorker();
    void stopWriterWorker();

private:
	void mSleepWriterWorker();
	void mWakeupWriterWorker();

private:
    std::shared_ptr<Stream> mStream;
    std::unique_ptr<StreamReader> mStreamReader;
    std::thread mWorker;
    bool mIsWorkerAlive;

	std::mutex mSyncMtx;
	std::condition_variable mSyncCV;

    size_t mThreshold;
    stream_status_t mStatus;

protected:
    std::unique_ptr<StreamWriter> mStreamWriter;
};

} // namespace stream
} // namespace media

#endif // __MEDIA_STREAMINPUTDATASOURCE_H
