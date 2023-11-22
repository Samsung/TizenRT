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

#include <stdlib.h>
#include "stdio.h"
#include "stdint.h"
#include "sys/types.h"

namespace aifw {

/**
 * @class AIDataBufferNode
 * @brief Interface class for necessary fields of an AIDataBuffer node.
 */
class AIDataBufferNode
{
public:
	bool mIsEmpty;
	float *mData;
	AIDataBufferNode *mPrev;
	AIDataBufferNode *mNext;

	/**
	 * @brief Construct the AIDataBufferNode class instance.
	 */
	AIDataBufferNode(uint16_t rowSize) :
		mIsEmpty(true), mData(NULL), mPrev(NULL), mNext(NULL)
	{
		mData = (float *)calloc(rowSize, sizeof(float));
	}

	/**
	 * @brief AIDataBufferNode destructor.
	 */
	~AIDataBufferNode()
	{
		free(mData);
	}
};

} // namespace aifw

