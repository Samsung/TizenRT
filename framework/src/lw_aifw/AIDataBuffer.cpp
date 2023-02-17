/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include "lw_aifw/lw_aifw_log.h"
#include "AIDataBuffer.h"

namespace lw_aifw {

#define _UNLOCK { \
		int status = pthread_mutex_unlock(&m_Lock);\
		if (status != 0) {\
			LW_AIFW_LOGE("Unlock failed");\
		}\
	}

#define _LOCK { \
		int status = pthread_mutex_lock(&m_Lock);\
		if (status != 0) {\
			LW_AIFW_LOGE("Lock acquire failed");\
			return LW_AIFW_ERROR;\
		}\
	}

	AIDataBuffer::AIDataBuffer() : mFirstDataOffset(0), m_WriteOffset(0), m_Buffer(NULL), m_BytesPerRow(0), m_NotUsedBuffer(0), m_Lock(PTHREAD_MUTEX_INITIALIZER), mReadOffset(0), mBufferDataSetCount(0)
	{
		LW_AIFW_LOGV("AIDataBuffer created buffer size %d", CONFIG_LW_AIFW_BUFFER_SIZE);
	}

	LW_AIFW_RESULT AIDataBuffer::init(void)
	{
		mFirstDataOffset = 0;
		m_WriteOffset = 0;
		m_BytesPerRow = 0;
		m_NotUsedBuffer = 0;
		mReadOffset = 0;
		mBufferDataSetCount = 0;
		m_Buffer = NULL;
		m_Buffer = new char[CONFIG_LW_AIFW_BUFFER_SIZE];
		if (!m_Buffer) {
			LW_AIFW_LOGE("Streaming buffer creation failed");
			return LW_AIFW_NO_MEM;
		}
		return LW_AIFW_OK;
	}

	uint16_t AIDataBuffer::getBytesPerRow(void) const
	{
		return m_BytesPerRow;
	}

	LW_AIFW_RESULT AIDataBuffer::clear(void)
	{
		mFirstDataOffset = 0;
		m_WriteOffset = 0;
		m_BytesPerRow = 0;
		m_NotUsedBuffer = 0;
		mReadOffset = 0;
		mBufferDataSetCount = 0;
		memset(m_Buffer, '\0', CONFIG_LW_AIFW_BUFFER_SIZE);
		return LW_AIFW_OK;
	}

	AIDataBuffer::~AIDataBuffer()
	{
		LW_AIFW_LOGV("Destructor");
		if (m_Buffer) {
			delete[] m_Buffer;
			m_Buffer = NULL;
		}
	}

	LW_AIFW_RESULT AIDataBuffer::write(float *data, uint16_t dataPerSet)
	{
		_LOCK
		if (!m_Buffer) {
			LW_AIFW_LOGE("no buffer allocated");
			_UNLOCK
			return LW_AIFW_ERROR;
		}
		LW_AIFW_LOGV("write offset %d", m_WriteOffset);
		m_BytesPerRow = sizeof(float) * dataPerSet;
		if (CONFIG_LW_AIFW_BUFFER_SIZE - m_WriteOffset < m_BytesPerRow) {
			m_NotUsedBuffer = CONFIG_LW_AIFW_BUFFER_SIZE - m_WriteOffset;
			m_WriteOffset = 0;
			LW_AIFW_LOGV("write offset reset to 0");
		}
		memset(m_Buffer + m_WriteOffset, '\0', m_BytesPerRow);
		memcpy(m_Buffer + m_WriteOffset, data, m_BytesPerRow);
		m_WriteOffset += m_BytesPerRow;

		if (mBufferDataSetCount < CONFIG_LW_AIFW_BUFFER_SIZE / m_BytesPerRow) {
			mBufferDataSetCount++;
		}

		LW_AIFW_LOGV("write offset %d", m_WriteOffset);
		_UNLOCK
		return LW_AIFW_OK;
	}

	int AIDataBuffer::getDataSetCount()
	{
		return mBufferDataSetCount;
	}

	LW_AIFW_RESULT AIDataBuffer::read(float *data, uint16_t dataPerSet, uint16_t setCount)
	{
		_LOCK
		LW_AIFW_LOGV("Requested sensor count %d, rows count %d", dataPerSet, setCount);
		if (m_WriteOffset == 0) {
			LW_AIFW_LOGE("buffer empty");
			_UNLOCK
			return LW_AIFW_BUFFER_EMPTY;
		}
		uint16_t tempOffset = m_WriteOffset;
		if (mFirstDataOffset == m_WriteOffset) {
			LW_AIFW_LOGI("All data read WriteOffset=%d ReadOffset=%d", m_WriteOffset, mFirstDataOffset);
			_UNLOCK
			return LW_AIFW_READ_ALREADY;
		}
		for (uint16_t i = 0; i < setCount; i++) {
			if (tempOffset == 0) {
				tempOffset = CONFIG_LW_AIFW_BUFFER_SIZE - m_NotUsedBuffer;
			}
			uint16_t startOffset = tempOffset - m_BytesPerRow;
			for (uint16_t k = 0; k < dataPerSet; k++) {
				float val;
				val = *((float*)(m_Buffer + startOffset + (k * sizeof(float))));
				*(data + (i * dataPerSet) + k) = val;
				LW_AIFW_LOGV("value read %f %f", val, *(data + (i * dataPerSet) + k));
			}
			tempOffset = tempOffset - m_BytesPerRow;
		}
		mFirstDataOffset = m_WriteOffset;
		LW_AIFW_LOGV("read api write offset %d", m_WriteOffset);
		_UNLOCK
		return LW_AIFW_OK;
	}

	LW_AIFW_RESULT AIDataBuffer::read(float *result, uint16_t index)
	{
		_LOCK
		if (m_WriteOffset == 0) {
			LW_AIFW_LOGE("buffer empty");
			_UNLOCK
			return LW_AIFW_BUFFER_EMPTY;
		}
		if (index == 0) {
			mFirstDataOffset = m_WriteOffset - m_BytesPerRow;
		}
		uint16_t dataPerSet = m_BytesPerRow / sizeof(float);
		mReadOffset = mFirstDataOffset - index * m_BytesPerRow;
		if (mReadOffset < 0) {
			mReadOffset = CONFIG_LW_AIFW_BUFFER_SIZE - m_NotUsedBuffer + mReadOffset;
		}
		for (uint16_t k = 0; k < dataPerSet; k++) {
			float val;
			val = *((float*)(m_Buffer + mReadOffset + (k * sizeof(float))));
			*(result + k) = val;
			LW_AIFW_LOGV("value read %f %f", val, *(result + k));
		}
		_UNLOCK
		return LW_AIFW_OK;
	}
} /* lw_aifw */

