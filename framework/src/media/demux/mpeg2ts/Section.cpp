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
#include <string.h>
#include "Mpeg2TsTypes.h"
#include "Section.h"
#include "SectionParser.h"


#define SECTION_LENGTH(buffer)  ((uint16_t)((buffer[1] & 0x0F) << 8) + (uint16_t)buffer[2])
#define SECTION_HEAD_BYTES      (3)  // table_id + ... + section_length
#define CONTINUITY_COUNTER_MOD  (16) // Continuity counter's module value


std::shared_ptr<Section> Section::create(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size)
{
	auto instance = std::make_shared<Section>();
	if (instance && instance->initialize(pid, continuityCounter, pData, size)) {
		return instance;
	}

	meddbg("create Section instance failed!\n");
	return nullptr;
}

bool Section::initialize(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size)
{
	mSectionDataLen = parseLengthField(pData, size);
	mSectionData = new uint8_t[mSectionDataLen];
	if (!mSectionData) {
		meddbg("Run out of memory! Allocating %d bytes failed!\n", mSectionDataLen);
		return false;
	}

	if (mSectionDataLen < size) {
		// abnormal case, anyway it would be abandoned if it's invalid section.
		mPresentDataLen = mSectionDataLen;
	} else {
		mPresentDataLen = size;
	}
	memcpy(mSectionData, pData, mPresentDataLen);

	mPid = pid;
	mContinuityCounter = continuityCounter;
	medvdbg("initialize new section/packet, pid:0x%x, continuity:%u, data %u/%u\n", mPid, mContinuityCounter, mPresentDataLen, mSectionDataLen);
	return true;
}

Section::Section()
	: mPid(INVALID_PID)
	, mContinuityCounter(0)
	, mSectionData(nullptr)
	, mSectionDataLen(0)
	, mPresentDataLen(0)
{
}

Section::~Section()
{
	if (mSectionData) {
		delete[] mSectionData;
		mSectionData = nullptr;
	}
}

bool Section::appendData(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size)
{
	if (mPid != pid) {
		meddbg("pid(0x%x) do not match, current 0x%x\n", pid, mPid);
		return false;
	}

	if (continuityCounter != ((mContinuityCounter + 1) % CONTINUITY_COUNTER_MOD)) {
		meddbg("continuity counter(0x%x) do not match, current 0x%x\n", continuityCounter, mContinuityCounter);
		return false;
	}

	mContinuityCounter = continuityCounter;

	if (mPresentDataLen + size <= mSectionDataLen) {
		memcpy(mSectionData + mPresentDataLen, pData, size);
		mPresentDataLen += size;
	} else {
		memcpy(mSectionData + mPresentDataLen, pData, mSectionDataLen - mPresentDataLen);
		mPresentDataLen = mSectionDataLen;
	}

	medvdbg("append section/packet, pid:0x%x, continuity:%u, data %u(%u)/%u\n", mPid, mContinuityCounter, mPresentDataLen, size, mSectionDataLen);
	return true;
}

bool Section::verifyCrc32(void)
{
	if (crc32(mSectionData, mPresentDataLen) != 0) {
		meddbg("crc32 verification failed!\n");
		return false;
	}

	return true;
}

bool Section::isCompleted(void)
{
	return ((mSectionDataLen != 0) && (mSectionDataLen == mPresentDataLen));
}

uint16_t Section::parseLengthField(uint8_t *pData, uint16_t size)
{
	return (SECTION_HEAD_BYTES + SECTION_LENGTH(pData));
}

/*
 * Name:   CRC-32/MPEG-2 x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:   0x4C11DB7
 * Init:   0xFFFFFFF
 * Refin:  False
 * Refout: False
 * Xorout: 0x0000000
 */
uint32_t Section::crc32(uint8_t *data, uint32_t length)
{
	uint8_t i;
	uint32_t j = 0;
	uint32_t crc = 0xffffffff;

	while ((length--) != 0) {
		crc ^= (uint32_t)data[j++] << 24;
		for (i = 0; i < 8; ++i) {
			if ((crc & 0x80000000) != 0) {
				crc = (crc << 1) ^ 0x04C11DB7;
			} else {
				crc <<= 1;
			}
		}
	}
	return crc;
}
