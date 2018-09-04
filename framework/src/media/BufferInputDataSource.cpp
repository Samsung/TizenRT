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

#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <media/BufferInputDataSource.h>
#include "MediaPlayerImpl.h"
#include "Decoder.h"
#include "StreamBuffer.h"
#include "StreamBufferWriter.h"

#ifndef CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_SIZE
#define CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_THRESHOLD
#define CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_THRESHOLD 1
#endif

namespace media {
namespace stream {
BufferInputDataSource::BufferInputDataSource()
	: InputDataSource()
{
}

BufferInputDataSource::BufferInputDataSource(audio_type_t audio_type)
	: InputDataSource()
{
	setAudioType(audio_type);
}

BufferInputDataSource::BufferInputDataSource(const BufferInputDataSource &source)
	: InputDataSource(source)
{
}

BufferInputDataSource &BufferInputDataSource::operator=(const BufferInputDataSource &source)
{
	InputDataSource::operator=(source);
	return *this;
}

bool BufferInputDataSource::open()
{

	auto streamBuffer = StreamBuffer::Builder()
							.setBufferSize(CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_SIZE)
							.setThreshold(CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_THRESHOLD)
							.build();
	if (registerStream(streamBuffer) == false) {
		meddbg("%s[line : %d] Fail : registerStream() failed\n", __func__, __LINE__);
		return false;
	}

	registerDecoder(getAudioType(), getChannels(), getSampleRate());
	start();
	return true;
}

bool BufferInputDataSource::close()
{
	unregisterDecoder();
	stop();
	unregisterStream();
	return true;
}

bool BufferInputDataSource::isPrepare()
{
	return isStreamReady();
}

ssize_t BufferInputDataSource::onStreamBufferWritable()
{
	auto writer = getBufferWriter();
	auto size = writer->sizeOfSpace();
	unsigned char *buffer = new unsigned char[size];
	if (buffer == nullptr) {
		return -1;
	}

	auto player = getPlayer();
	if (player) {
		player->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_DATAREACHED, buffer, size);
	}
	ssize_t ret = writeToStreamBuffer(buffer, size);
	delete[] buffer;
	return ret;
}

int BufferInputDataSource::seek(long offset, int origin)
{
	return -1;
}

BufferInputDataSource::~BufferInputDataSource()
{
	close();
}

} // namespace stream
} // namespace media
