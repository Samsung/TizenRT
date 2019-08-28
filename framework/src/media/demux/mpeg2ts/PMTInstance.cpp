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

#include <debug.h>

#include "Mpeg2TsTypes.h"
#include "PMTElementary.h"
#include "PMTInstance.h"

#define PMT_PROG_INFO_LENGTH(buffer)       (((buffer[0] & 0x0F) << 8) + buffer[1])
#define LENGTH_FIELD_BYTES                 (2)
#define PMT_PCR_PID(buffer)                (((buffer[0] & 0x1F) << 8) + buffer[1])
#define PID_FIELD_BYTES                    (2)
#define PMT_CRC_BYTES                      (4)

PMTInstance::PMTInstance(ts_pid_t pid)
	: mPid(pid)
	, mProgramNumber(INVALID_PN)
	, mPcrPid(INVALID_PID)
	, mProgramInfoLength(0)
{
}

PMTInstance::~PMTInstance(void)
{
	deleteInfo();
}

void PMTInstance::deleteInfo(void)
{
	mElementaryStreams.clear();
	mProgramInfoLength = 0;
}

bool PMTInstance::parseInfo(uint8_t *pData, uint32_t size)
{
	mProgramInfoLength = PMT_PROG_INFO_LENGTH(pData);
	pData += LENGTH_FIELD_BYTES;
	size -= LENGTH_FIELD_BYTES;

	if (mProgramInfoLength + (uint32_t)PMT_CRC_BYTES > size) {
		meddbg("mProgramInfoLength=%d invalid\n", mProgramInfoLength);
		return false;
	}

	parseProgramInfo(pData, mProgramInfoLength);
	pData += mProgramInfoLength;
	size -= mProgramInfoLength;

	int32_t length = (int32_t)(size - (uint32_t)PMT_CRC_BYTES);
	while (length > 0) {
		auto stream = std::make_shared<PMTElementary>();
		if (!stream) {
			meddbg("Run out of memory!\n");
			return false;
		}

		int32_t len = stream->parseES(pData, (uint32_t)length);
		mElementaryStreams.push_back(stream);
		length -= len;
		pData += len;
	}

	return true;
}

bool PMTInstance::parseProgramInfo(uint8_t *pData, uint32_t size)
{
	// Ignore program info descriptors...
	// Add related descrioptor parer in future.
	return true;
}

bool PMTInstance::parse(uint8_t *pData, uint32_t size, prog_num_t programNum,
						int8_t versionNumber, uint8_t sectionNumber,
						uint8_t lastSectionNumber, uint32_t crc32, bool currentNextIndicator)
{
	mProgramNumber = programNum;
	mPcrPid = PMT_PCR_PID(pData);
	pData += PID_FIELD_BYTES;
	size -= PID_FIELD_BYTES;

	auto sectionState = checkSection(versionNumber, sectionNumber, lastSectionNumber, crc32);
	switch (sectionState) {
	case SECTION_INITIAL: // fall through
	case SECTION_CHANGE:  // fall through
	case SECTION_APPEND:
		if (parseInfo(pData, size)) {
			return isCompleted();
		}
		meddbg("parse program info failed!\n");
		return false;
	case SECTION_IGNORE:  // fall through
	case SECTION_PRESENT: // fall through
	default:
		medvdbg("PMT section state : %d\n", sectionState);
		return false;
	}
}

size_t PMTInstance::numOfElementary(void)
{
	return mElementaryStreams.size();
}

std::shared_ptr<PMTElementary> PMTInstance::getPMTElementary(uint32_t index)
{
	if ((size_t)index >= mElementaryStreams.size()) {
		return nullptr;
	}

	return mElementaryStreams[index];
}
