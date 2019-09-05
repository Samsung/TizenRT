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
#include "PATParser.h"


#define PAT_UNTIL_LAST_SECTION_NUMBER_LEN       (9)
#define PAT_PROG_NUM_PID_LEN                    (4)
#define PAT_PROGRAM_NUMBER(buffer,idx)          ((buffer[4 * (idx)] << 8) + buffer[4 * (idx) + 1])
#define PAT_PROGRAM_NUMBER_PID(buffer,idx)      (((buffer[4 * (idx) + 2] << 8) + buffer[4 * (idx) + 3]) & 0x1FFF)



PATParser::PATParser()
	: SectionParser(TABLE_ID)
	, mTransportStreamId(0)
	, mNetworkPID(INVALID_PID)
{
}

PATParser::~PATParser()
{
	clearParser();
}

void PATParser::clearParser(void)
{
	TableBase::resetTable();
	deleteInfo();
}

void PATParser::deleteInfo(void)
{
	mTransportStreamId = 0;
	mNetworkPID = INVALID_PID;
	mProgramMap.clear();
}

bool PATParser::parseInfo(uint8_t *pData, uint32_t size)
{
	auto sectionState = checkSection(mVersionNumber, mSectionNumber, mLastSectionNumber, mCrc32);
	switch (sectionState) {
	case SECTION_INITIAL: // fall through
	case SECTION_CHANGE:  // fall through
	case SECTION_APPEND: {
		mTransportStreamId = mTableIdExtension;
		int i;
		for (i = 0; i < ((mSectionLength - PAT_UNTIL_LAST_SECTION_NUMBER_LEN) / PAT_PROG_NUM_PID_LEN); i++) {
			addProgram(PAT_PROGRAM_NUMBER(pData, i), PAT_PROGRAM_NUMBER_PID(pData, i));
		}
		return isCompleted();
	}
	case SECTION_IGNORE:  // fall through
	case SECTION_PRESENT: // fall through
	default:
		medvdbg("PAT section state : %d\n", sectionState);
		return false;
	}
}

size_t PATParser::sizeOfProgram(void)
{
	return mProgramMap.size();
}

prog_num_t PATParser::getProgramNumber(uint32_t index)
{
	if (index >= mProgramMap.size()) {
		return INVALID_PN;
	}

	uint32_t curr = 0;
	auto it = mProgramMap.begin();
	while (it != mProgramMap.end()) {
		if (index == curr++) {
			return (prog_num_t)it->first;
		}
		it++;
	}

	return INVALID_PN;
}

ts_pid_t PATParser::getProgramMapPID(prog_num_t programNumber)
{
	auto it = mProgramMap.find(programNumber);
	if (it == mProgramMap.end()) {
		meddbg("[getProgramMapPID] pn %d -> INVALID_PID\n", programNumber);
		return INVALID_PID;
	}

	medvdbg("[getProgramMapPID] pn %d -> pmt pid 0x%x\n", programNumber, it->second);
	return it->second;
}

void PATParser::addProgram(prog_num_t programNumber, ts_pid_t programPID)
{
	// program number 0 is reserved to specify the network PID
	if (programNumber == PATParser::NETWORK_PID_PN) {
		medvdbg("NIT table pid 0x%04x\n", programPID);
		mNetworkPID = programPID;
		return ;
	}

	medvdbg("program %d pid 0x%x\n", programNumber, programPID);
	mProgramMap[programNumber] = programPID;
}

uint16_t PATParser::getTansportStreamId(void)
{
	return mTransportStreamId;
}

ts_pid_t PATParser::getNetworkPID(void)
{
	return mNetworkPID;
}

bool  PATParser::isRecv(void)
{
	if (SectionParser::isRecv()) {
		return isCompleted();
	}

	return false;
}
