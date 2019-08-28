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

#ifndef __PMT_INSTANCE_H
#define __PMT_INSTANCE_H

#include <vector>
#include <memory>

#include "Mpeg2TsTypes.h"
#include "TableBase.h"

class PMTElementary;
class PMTInstance : public TableBase
{
public:
	PMTInstance() = delete;
	// constructor with a PMT PID
	PMTInstance(ts_pid_t pid);
	virtual ~PMTInstance();

	// parse PMT section
	bool parse(uint8_t *pData, uint32_t size, prog_num_t programNum, int8_t  versionNumber, uint8_t sectionNumber, uint8_t lastSectionNumber, uint32_t crc32, bool currentNextIndicator);
	// number of elementary streams
	size_t numOfElementary(void);
	// get elementary stream by index
	std::shared_ptr<PMTElementary> getPMTElementary(uint32_t index);
	// get PID of this PMT table
	ts_pid_t getPid(void) { return mPid; }
	// get program number
	prog_num_t getProgramNumber(void) { return mProgramNumber; }
	// get PCR PID of this program
	ts_pid_t getPcrPid(void) { return mPcrPid; }
	// get program info length field value
	uint16_t getProgramInfoLength(void) { return mProgramInfoLength; }

protected:
	// parse specific information in PMT
	bool parseInfo(uint8_t *pData, uint32_t size); // not override
	// parse program info descriptors
	bool parseProgramInfo(uint8_t *pData, uint32_t size);
	// delete specific information (program info and elementary streams info)
	void deleteInfo(void) override;

private:
	// PID of this PMT table
	ts_pid_t mPid;
	// program number associated
	prog_num_t mProgramNumber;
	// PID of PCR (program clock reference)
	ts_pid_t mPcrPid;
	// program info length
	uint16_t mProgramInfoLength;
	// Elementary streams
	std::vector<std::shared_ptr<PMTElementary>> mElementaryStreams;
};

#endif /* __PMT_INSTANCE_H */
