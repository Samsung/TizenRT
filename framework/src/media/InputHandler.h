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
#include <mutex>
#include <condition_variable>
#include <sys/types.h>

#include <media/InputDataSource.h>
#include "StreamHandler.h"

#include "Decoder.h"
#include "Demuxer.h"

namespace media {
class MediaPlayerImpl;
namespace stream {

typedef enum buffer_state_e : int {
	BUFFER_STATE_EMPTY,
	BUFFER_STATE_BUFFERING,
	BUFFER_STATE_BUFFERED,
	BUFFER_STATE_FULL,
} buffer_state_t;

class InputHandler : public StreamHandler
{
public:
	InputHandler();
	void setInputDataSource(std::shared_ptr<InputDataSource> source);
	bool doStandBy();
	bool open() override;
	bool close() override;
	ssize_t read(unsigned char *buf, size_t size);

	void setBufferState(buffer_state_t state);

	virtual void onBufferOverrun() override;
	virtual void onBufferUnderrun() override;
	virtual void onBufferUpdated(ssize_t change, size_t current) override;

	void setPlayer(std::shared_ptr<MediaPlayerImpl> mp) { mPlayer = mp; }
	std::shared_ptr<MediaPlayerImpl> getPlayer() { return mPlayer.lock(); }

	size_t getAvailSpace();
	ssize_t writeToStreamBuffer(unsigned char *buf, size_t size);

private:
	bool probeDataSource() override;
	bool registerCodec(audio_type_t audioType, unsigned int channels, unsigned int sampleRate) override;
	void unregisterCodec() override;
	size_t getDecodeFrames(unsigned char *buf, size_t *size);

	void resetWorker() override;
	void sleepWorker() override;
	bool processWorker() override;
	const char *getWorkerName(void) const override { return "InputHandler"; };
	ssize_t getElementaryStream(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect);
	ssize_t getPCM(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect);
	size_t fetchData(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect);
	ssize_t readFromSource(unsigned char *buf, size_t size);

	std::mutex mMutex;
	std::condition_variable mCondv;
	std::shared_ptr<StreamBuffer> mPreloadBuffer;
	std::shared_ptr<InputDataSource> mInputDataSource;
	std::shared_ptr<Decoder> mDecoder;
	std::shared_ptr<Demuxer> mDemuxer;
	std::weak_ptr<MediaPlayerImpl> mPlayer;

	buffer_state_t mState;
	size_t mTotalBytes;
};
} // namespace stream
} // namespace media
#endif
