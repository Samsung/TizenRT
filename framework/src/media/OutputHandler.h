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

#include "StreamHandler.h"
#include "Encoder.h"

namespace media {
class MediaRecorderImpl;
namespace stream {
class OutputHandler : public StreamHandler
{
public:
	OutputHandler();
	void setOutputDataSource(std::shared_ptr<OutputDataSource> source);
	ssize_t write(unsigned char *buf, size_t size);
	bool stop() override;
	void flush();

	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

	void setRecorder(std::shared_ptr<MediaRecorderImpl> mr) { mRecorder = mr; }
	std::shared_ptr<MediaRecorderImpl> getRecorder() { return mRecorder.lock(); }

	ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

private:
	bool probeDataSource() override;
	bool registerCodec(audio_type_t audioType, unsigned int channels, unsigned int sampleRate) override;
	void unregisterCodec() override;
	void resetWorker() override;
	void sleepWorker() override;
	virtual bool processWorker() override;
	const char *getWorkerName(void) const override { return "OutputHandler"; };
	void writeToSource(size_t size);
	std::shared_ptr<OutputDataSource> mOutputDataSource;
	std::shared_ptr<Encoder> mEncoder;

	bool mIsFlushing;
	std::mutex mFlushMutex;
	std::condition_variable mFlushCondv;

	std::weak_ptr<MediaRecorderImpl> mRecorder;
};
} // namespace stream
} // namespace media
#endif
