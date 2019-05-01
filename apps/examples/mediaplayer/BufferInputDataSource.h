/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __BUFFERINPUTDATASOURCE_H
#define __BUFFERINPUTDATASOURCE_H

#include <media/InputDataSource.h>
#include <functional>

class BufferInputDataSource : public media::stream::InputDataSource
{
public:
	BufferInputDataSource();
	virtual ~BufferInputDataSource() = default;
	bool isPrepared() override;
	bool open() override;
	bool close() override;
	ssize_t read(unsigned char *buf, size_t size) override;

private:
	FILE *mFp;
	unsigned char *mSrcBuf;
	size_t mSrcSize;
	std::function<ssize_t(unsigned char *, size_t, unsigned char *, size_t)> mBufferFiller;
};
#endif
