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
#include "SectionParser.h"

// Section Info macros
#define TABILE_ID(buffer)                   (buffer[0])
#define SECTION_SYNTAX_INDICATOR(buffer)    ((buffer[1] >> 7) & 1)
#define PRIVATE_INDICATOR(buffer)           ((buffer[1] >> 6) & 1)
#define SECTION_LENGTH(buffer)              (((buffer[1] & 0x0F) << 8) + buffer[2])
#define SI_TABLE_ID_EXT(buffer)             ((buffer[3] << 8) + buffer[4])
#define SI_VERSION_NUMBER(buffer)           ((buffer[5] >> 1) & 0x1F)
#define SI_CURRENT_NEXT_INDICATOR(buffer)   (buffer[5] & 0x01)
#define SI_SECTION_NUMBER(buffer)           (buffer[6])
#define SI_LAST_SECTION_NUMBER(buffer)      (buffer[7])
#define SI_CRC(buffer,len)                  ((buffer[(len) - 4] << 24) + \
											(buffer[(len) - 3] << 16) + \
											(buffer[(len) - 2] << 8) + \
											(buffer[(len) - 1]))
#define PSI_DATA(buffer)                    (&(buffer[8]))
#define SHORT_FORM(buffer)                  (&(buffer[3]))


SectionParser::SectionParser(table_id_t tableId)
	: mIsRecv(false)
	, mSectionData(nullptr)
	, mPid(INVALID_PID)
	, mTableId(tableId)
	, mSectionSyntaxIndicator(false)
	, mPrivateIndicator(false)
	, mSectionLength(0)
	, mTableIdExtension(0)
	, mVersionNumber(INVALID_VN)
	, mCurrentNextIndicator(false)
	, mSectionNumber(0)
	, mLastSectionNumber(0)
	, mProtocolVersion(0)
	, mCrc32(0)
{
}

SectionParser::~SectionParser()
{
}

void SectionParser::Initialize(void)
{
	clearParser();

	mIsRecv = false;
	mSectionData = nullptr;
	mSectionLength = 0;
}

bool SectionParser::parseSection(ts_pid_t pid, uint8_t *pData)
{
	mIsRecv = true;
	mPid = pid;
	mSectionData = pData;
	mTableId = TABILE_ID(pData);
	mSectionSyntaxIndicator = SECTION_SYNTAX_INDICATOR(pData);
	mPrivateIndicator = PRIVATE_INDICATOR(pData);
	mSectionLength = SECTION_LENGTH(pData);

	if (mSectionSyntaxIndicator) {
		mTableIdExtension = SI_TABLE_ID_EXT(pData);
		mVersionNumber = SI_VERSION_NUMBER(pData);
		mCurrentNextIndicator = SI_CURRENT_NEXT_INDICATOR(pData);
		mSectionNumber = SI_SECTION_NUMBER(pData);
		mLastSectionNumber = SI_LAST_SECTION_NUMBER(pData);
		mCrc32 = SI_CRC(pData,mSectionLength + SECTION_HEADER_LENGTH);
		return parseInfo(PSI_DATA(pData), mSectionLength - SectionParser::LONG_FORM_HEADER_LENGTH);
	} else {
		meddbg("Short form syntax is not supported now!\n");
		return false;
	}
}
