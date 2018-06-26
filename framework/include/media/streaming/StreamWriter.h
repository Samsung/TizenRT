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

#ifndef __MEDIA_STREAMWRITER_H
#define __MEDIA_STREAMWRITER_H

#include "Stream.h"

namespace media {
namespace stream {

class StreamWriter {
public:
    StreamWriter();

public:
    void setStream(std::shared_ptr<Stream> stream);
    size_t write(unsigned char *buf, size_t size, bool sync = false);
    size_t sizeOfSpace();
    size_t sizeOfData();
	void setEndOfStream();

private:
    std::shared_ptr<Stream> mStream;
};

} // namespace media
} // namespace stream

#endif
