/******************************************************************
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

#ifndef __MEDIA_STREAMBUFFERREADER_H
#define __MEDIA_STREAMBUFFERREADER_H

#include <memory>

namespace media {
namespace stream {
class StreamBuffer;

class StreamBufferReader
{
public:
	StreamBufferReader(std::shared_ptr<StreamBuffer> stream);

public:
	virtual size_t read(unsigned char *buf, size_t size, bool sync = true);
	virtual size_t sizeOfData();

public:
	bool isEndOfStream();

private:
	std::shared_ptr<StreamBuffer> mStream;
};

} // namespace stream
} // namespace media

#endif
