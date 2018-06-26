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

#ifndef __MEDIA_BUFFERSTREAMINPUTDATASOURCE_H
#define __MEDIA_BUFFERSTREAMINPUTDATASOURCE_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <media/streaming/StreamInputDataSource.h>

namespace media {
namespace stream {
class BufferStreamInputDataSource : public StreamInputDataSource {
public:
    BufferStreamInputDataSource(const std::string& httpAddr);
    virtual ~BufferStreamInputDataSource();

    size_t addBufferStreamData(unsigned char *buf, size_t size, bool isEndOfStream = false);
    void setThreshold(size_t sec);
    
    bool open() override;
    bool close() override;
    bool isPrepare() override;
    void onStreamWritable() override;
    bool endOfStream() override;
    void setEndOfStream(bool isEnd);
private:
    std::string mIpAddr;
    std::mutex mMutex;
	std::condition_variable mSyncCV;
    std::thread mWorker;
    
    bool mIsPrepare;

    bool mIsEndOfStream;
};

} // namespace stream
} // namespace media

#endif // __MEDIA_BUFFERSTREAMINPUTDATASOURCE_H
