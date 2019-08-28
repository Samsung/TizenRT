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

#ifndef __PARSE_MANAGER_H
#define __PARSE_MANAGER_H

#include <map>
#include <vector>
#include <memory>
#include "Mpeg2TsTypes.h"

class Section;
class SectionParser;
class ParserManager
{
public:
	ParserManager();
	virtual ~ParserManager();
	// parse section with the corresponding parser
	bool processSection(std::shared_ptr<Section> pSection);
	// check if PAT has been received
	bool isPATReceived(void);
	// check if the PMT of the given program number has been received
	bool isPMTReceived(prog_num_t progNum);
	// check if all PMTs have been received
	bool isPMTReceived(void);
	// get audio stream information of the given program number
	bool getAudioStreamInfo(prog_num_t progNum, uint8_t &streamType, ts_pid_t &pid);
	// get all program numbers and save into vector
	bool getPrograms(std::vector<prog_num_t> &programs);
	// check if the given PID is a PMT PID
	bool isPMTPid(ts_pid_t pid);

protected:
	// add new section parser
	bool addParser(std::shared_ptr<SectionParser> pParser);
	// remove section parser of the given table id
	void removeParser(table_id_t tableId);
	// get section parser of the given table id
	std::shared_ptr<SectionParser> getParser(table_id_t tableId);
	// sync program information from PAT parser when PAT received,
	// then it's possible to filter and parse PMT sections for each program.
	bool syncProgramInfoFromPAT(void);

private:
	// table parsers
	std::map<table_id_t, std::shared_ptr<SectionParser>> mTableParsers;
	// Pids of PMT for section filtering
	std::vector<ts_pid_t> mPMTPids;
};

#endif /* __PARSE_MANAGER_H */
