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
#include <assert.h>
#include <debug.h>
#include <unistd.h>
#include <media/OutputDataSource.h>
#include "MediaRecorderImpl.h"

#ifndef CONFIG_OUTPUT_DATASOURCE_STACKSIZE
#define CONFIG_OUTPUT_DATASOURCE_STACKSIZE 4096
#endif

namespace media {
namespace stream {
OutputDataSource::OutputDataSource() :
	DataSource()
{
}

OutputDataSource::OutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat) :
	DataSource(channels, sampleRate, pcmFormat)
{
}

OutputDataSource::OutputDataSource(const OutputDataSource &source) :
	DataSource(source)
{
}

OutputDataSource &OutputDataSource::operator=(const OutputDataSource &source)
{
	DataSource::operator=(source);
	return *this;
}

OutputDataSource::~OutputDataSource()
{
}

} // namespace stream
} // namespace media

