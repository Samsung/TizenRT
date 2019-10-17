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

#include "Section.h"
#include "PMTInstance.h"
#include "PMTParser.h"

PMTParser::PMTParser()
	: SectionParser(TABLE_ID)
{
}

PMTParser::~PMTParser()
{
	Initialize();
}

void PMTParser::clearParser(void)
{
	mPMTInstances.clear();
	mPMTElements.clear();
}

bool PMTParser::parseInfo(uint8_t *pData, uint32_t size)
{
	// table id extension filed is the program nubmer in PMT
	prog_num_t programNumber = (prog_num_t)mTableIdExtension;
	medvdbg("Parsing PMT of programnumber %d, pid 0x%x\n", programNumber, mPid);

	if (!mPMTElements.empty()) {
		auto it = mPMTElements.find(makeKey(mPid, programNumber));
		if (it == mPMTElements.end()) {
			meddbg("Can not find PMT element: programnumber %d, pid 0x%x\n", programNumber, mPid);
			return false;
		}
	}

	auto pInstance = mPMTInstances[programNumber];
	if (!pInstance) {
		pInstance = std::make_shared<PMTInstance>(mPid);
		if (!pInstance) {
			meddbg("Run out of memory! Allocate PMT instance failed!\n");
			return false;
		}
		// add PMT instance
		mPMTInstances[programNumber] = pInstance;
		medvdbg("Add PMT instance: programnumber %d, pid 0x%x\n", programNumber, mPid);
	}

	return pInstance->parse(pData, size, mTableIdExtension, mVersionNumber,
							mSectionNumber, mLastSectionNumber, mCrc32, mCurrentNextIndicator);
}

size_t PMTParser::numOfPMTInstance(void)
{
	return mPMTInstances.size();
}

std::shared_ptr<PMTInstance> PMTParser::getPMTInstance(prog_num_t programNumber)
{
	auto it = mPMTInstances.find(programNumber);
	if (it != mPMTInstances.end()) {
		return it->second;
	}

	meddbg("Can not find PMT instance by programnumber %d\n", programNumber);
	return nullptr;
}

void PMTParser::updatePMTElements(std::map<int, ts_pid_t> &pmtMap)
{
	mPMTElements = pmtMap;
}

int PMTParser::makeKey(ts_pid_t pid, prog_num_t progNum)
{
	return ((pid << 16) | progNum);
}
