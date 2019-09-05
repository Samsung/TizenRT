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

#ifndef __PAT_PARSER_H
#define __PAT_PARSER_H

#include <map>
#include "Mpeg2TsTypes.h"
#include "SectionParser.h"
#include "TableBase.h"

class PATParser : public SectionParser, public TableBase
{
public:
	enum {
		TABLE_ID = 0x00,         // table id (0x00) of PAT
		NETWORK_PID_PN = 0x0000, // program number 0 specifies the network PID
	};

	PATParser();
	virtual ~PATParser();

	// get nubmer of programs in PAT table
	size_t sizeOfProgram(void);
	// get program number by index
	prog_num_t getProgramNumber(uint32_t index);
	// get program map PID by program number
	ts_pid_t getProgramMapPID(prog_num_t programNumber);
	// get transport stream id
	uint16_t getTansportStreamId(void);
	// get NIT table PID
	ts_pid_t getNetworkPID(void);
	// check if PAT is received
	bool isRecv(void);

protected:
	// parse specific information (program number and PMT Pid) in PAT
	virtual bool parseInfo(uint8_t *pData, uint32_t size) override;
	// delete specific information parsed by parseInfo()
	virtual void deleteInfo(void) override;
	// clear up parser, release all resources
	virtual void clearParser(void) override;
	// add new program information
	void addProgram(prog_num_t programNumber, ts_pid_t programPID);

private:
	// program information <program number, program map table PID>
	std::map<prog_num_t, ts_pid_t> mProgramMap;
	// tansport stream id
	uint16_t mTransportStreamId;
	// PID of network information table
	ts_pid_t mNetworkPID;
};

#endif /* __PAT_PARSER_H */
