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

#ifndef __PMT_PARSER_H
#define __PMT_PARSER_H

#include <map>
#include "Mpeg2TsTypes.h"
#include "SectionParser.h"

class PMTInstance;
class PMTParser : public SectionParser
{
public:
	enum {
		TABLE_ID = 0x02, // table id (0x02) of PMT
	};

	PMTParser();
	virtual ~PMTParser();

	// number of PMT instances
	size_t numOfPMTInstance(void);
	// get PMT instance by program number
	std::shared_ptr<PMTInstance> getPMTInstance(prog_num_t programNumber);
	// update PMT elememts
	void updatePMTElements(std::map<int, ts_pid_t> &pmtMap);
	// make key with PMT pid and program number
	static int makeKey(ts_pid_t pid, prog_num_t progNum);

protected:
	// clear up parser, release all resources
	virtual void clearParser(void) override;
	// parse specific information (program info and elementary streams) in PMT
	virtual bool parseInfo(uint8_t *pData, uint32_t size) override;

private:
	// PMT elements
	std::map<int, ts_pid_t> mPMTElements;  // <key = (pid<<16 | prog_num), data = pid>
	// PMT instances with program number in map
	std::map<prog_num_t, std::shared_ptr<PMTInstance>> mPMTInstances;
};

#endif /* __PMT_PARSER_H */
