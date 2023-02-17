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

#pragma once

#include "lw_aifw/lw_aifw_result.h"

namespace lw_aifw {

	class AIDataBuffer {
	private:
		uint16_t mFirstDataOffset;
		uint16_t m_WriteOffset;
		char *m_Buffer;
		uint16_t m_BytesPerRow;
		uint16_t m_NotUsedBuffer;
		pthread_mutex_t m_Lock;
		int mReadOffset;
		uint16_t mBufferDataSetCount;
	public:
		AIDataBuffer();
		LW_AIFW_RESULT init(void);
		LW_AIFW_RESULT clear(void);
		~AIDataBuffer();
		uint16_t getBytesPerRow(void) const;
		void setBufferSize(uint16_t maxBufferCount);
		LW_AIFW_RESULT write(float *data, uint16_t dataPerSet);
		int getDataSetCount();
		LW_AIFW_RESULT read(float *data, uint16_t dataPerSet, uint16_t setCount);
		LW_AIFW_RESULT read(float *result, uint16_t index);
	};

} /* lw_aifw */

