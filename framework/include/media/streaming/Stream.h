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

#ifndef __MEDIA_STREAM_H
#define __MEDIA_STREAM_H

#include <memory>
#include <mutex>
#include <condition_variable>

#include "StreamObserverInterface.h"

struct rb_s;

namespace media {
namespace stream {

typedef enum stream_observer_command_e {
	STREAM_OBSERVER_COMMAND_OVERRUN,
	STREAM_OBSERVER_COMMAND_UNDERRUN
} stream_observer_command_t;


class StreamReader;
class StreamWriter;
class Stream : public std::enable_shared_from_this<Stream> {
public:
    Stream();
    virtual ~Stream();

public:
    void init(size_t size);
    int id();

    std::unique_ptr<StreamReader> getReader();
    std::unique_ptr<StreamWriter> getWriter();

    std::mutex &getMutex() { return mMutex; }
    std::condition_variable &getCondv() { return mCondv; }

    size_t read(unsigned char *buf, size_t size);
	size_t write(unsigned char *buf, size_t size);

	size_t sizeOfSpace();
	size_t sizeOfData();

	void setEndOfStream();
	bool isEndOfStream();

	void setObserver(std::shared_ptr<StreamObserverInterface> observer);
	void notifyObserver(stream_observer_command_t cmd);

private:
    std::mutex mMutex;
	std::condition_variable mCondv;
    std::unique_ptr<StreamReader> mReader;
    std::unique_ptr<StreamWriter> mWriter;
	std::shared_ptr<StreamObserverInterface> mObserver;

    rb_s *mRingBuf;
	bool mEOS;
	int mId;
};

} // namespace media
} // namespace stream

#endif
