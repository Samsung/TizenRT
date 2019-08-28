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
#include "Section.h"
#include "TableBase.h"

TableBase::TableBase()
	: mVersion(INVALID_VN)
	, mLastSectionNum(0)
	, mMultiSectionCRC(nullptr)
	, mMultiSectionFlag(nullptr)
{
}

TableBase::~TableBase()
{
	resetTable();
}

int TableBase::checkSection(uint8_t version, uint8_t sectionNum, uint8_t lastSectionNum, uint32_t crc32)
{
	if (sectionNum > lastSectionNum) {
		meddbg("Ignore invalid section! sectionNum[%x] > lastSectionNum[%x]\n", sectionNum, lastSectionNum);
		return SECTION_IGNORE;
	}

	if (mVersion == INVALID_VN) {
		if (!initTable(version, sectionNum, lastSectionNum, crc32)) {
			meddbg("init section failed!\n");
			return SECTION_IGNORE;
		}
		return SECTION_INITIAL;
	}

	// Check if table version changed
	if (isTableChanged(version, sectionNum, lastSectionNum, crc32)) {
		deleteInfo();
		if (!initTable(version, sectionNum, lastSectionNum, crc32)) {
			meddbg("init section failed!\n");
			return SECTION_IGNORE;
		}

		medwdbg("section change, version %d section %d/%d crc32 0x%x\n", version, sectionNum, lastSectionNum, crc32);
		return SECTION_CHANGE;
	}

	// Check if the section is present
	if (mMultiSectionFlag[sectionNum] && (mMultiSectionCRC[sectionNum] == crc32)) {
		return SECTION_PRESENT;
	}

	// Accept the section
	mMultiSectionFlag[sectionNum] = true;
	mMultiSectionCRC[sectionNum] = crc32;
	return SECTION_APPEND;
}

bool TableBase::initTable(uint8_t version, uint8_t sectionNumber, uint8_t lastSectionNumber, uint32_t crc32)
{
	int i;

	resetTable();

	mMultiSectionFlag = new bool[lastSectionNumber + 1];
	if (!mMultiSectionFlag) {
		meddbg("Out of memory! lastSectionNumber 0x%x\n", lastSectionNumber);
		return false;
	}

	mMultiSectionCRC = new uint32_t[lastSectionNumber + 1];
	if (!mMultiSectionCRC) {
		meddbg("Out of memory! lastSectionNumber 0x%x\n", lastSectionNumber);
		resetTable();
		return false;
	}

	for (i = 0; i <= lastSectionNumber; i++) {
		mMultiSectionFlag[i] = false;
		mMultiSectionCRC[i] = 0;
	}

	mVersion = version;
	mLastSectionNum = lastSectionNumber;

	mMultiSectionFlag[sectionNumber] = true;
	mMultiSectionCRC[sectionNumber] = crc32;

	return true;
}

bool TableBase::isTableChanged(uint8_t version, uint8_t sectionNum, uint8_t lastSectionNum, uint32_t crc32)
{
	if ((mVersion != version) ||
		(mLastSectionNum != lastSectionNum) ||
		((mVersion == version) && (mMultiSectionFlag[sectionNum]) && (mMultiSectionCRC[sectionNum] != crc32))) {
		return true;
	}

	return false;
}

bool TableBase::isCompleted(void)
{
	uint8_t i;

	if (!mMultiSectionFlag) {
		meddbg("mMultiSectionFlag is nullptr");
		return false;
	}

	for (i = 0; i <= mLastSectionNum; i++) {
		if (mMultiSectionFlag[i] != true) {
			return false;
		}
	}

	return true;
}

void TableBase::resetTable(void)
{
	if (mMultiSectionFlag) {
		delete[] mMultiSectionFlag;
		mMultiSectionFlag = nullptr;
	}

	if (mMultiSectionCRC) {
		delete[] mMultiSectionCRC;
		mMultiSectionCRC= nullptr;
	}

	mVersion = INVALID_VN;
	mLastSectionNum = 0;
}
