/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include "Mpeg2TsTypes.h"
#include "PMTElementary.h"

#define PMT_ES_HEAD_BYTES               (5)
#define PMT_STREAM_TYPE(buffer)         (buffer[0])
#define PMT_ELEMENTARY_PID(buffer)      (((buffer[1] & 0x1F) << 8) + buffer[2])
#define PMT_ES_INFO_LENGTH(buffer)      (((buffer[3] & 0x0F) << 8) + buffer[4])

PMTElementary::PMTElementary()
	: mStreamType(0)
	, mElementaryPID(INVALID_PID)
	, mESInfoLength(0)
{
}

PMTElementary::~PMTElementary()
{
}

int32_t PMTElementary::parseES(uint8_t *pData, uint32_t size)
{
	mStreamType = PMT_STREAM_TYPE(pData);
	mElementaryPID = PMT_ELEMENTARY_PID(pData);
	mESInfoLength = PMT_ES_INFO_LENGTH(pData);

	// currently, ignore ES info descriptors...
	// add descriptor parser if necessary in furture

	return PMT_ES_HEAD_BYTES + mESInfoLength;
}
