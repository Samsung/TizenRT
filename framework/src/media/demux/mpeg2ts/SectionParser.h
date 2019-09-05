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

#ifndef __SECTION_PARSER_H
#define __SECTION_PARSER_H

#include <vector>

class Section;

class SectionParser
{
public:
	enum {
		PAT_PID = 0x0000,            // PID of Program Association Table
	};

	enum {
		SECTION_HEADER_LENGTH = 3,   // 3 bytes: table_id + section_syntax_indicatior + section_length
		LONG_FORM_HEADER_LENGTH = 5, // 5 bytes: long form header length if section_syntax_indicatior is 1
		SECTION_MAX_LENGTH = 4096,   // max length of a section is 4K bytes
	};

public:
	SectionParser() = delete;
	// constructor with a table id
	SectionParser(table_id_t tableId);
	virtual ~SectionParser();
	// Initialize parser
	virtual void Initialize(void);
	// parse section data
	virtual bool parseSection(ts_pid_t pid, uint8_t *pData);
	// getters
	virtual bool isRecv(void) { return mIsRecv; }
	virtual ts_pid_t getPid(void) { return mPid; }
	virtual table_id_t getTableId(void) { return mTableId; }
	virtual uint16_t getTableIdExt(void) { return mTableIdExtension; }
	virtual uint8_t getSectionSyntaxIndicator(void) { return mSectionSyntaxIndicator; }
	virtual uint8_t getPrivateIndicator(void) { return mPrivateIndicator; }
	virtual uint16_t getSectionLength(void) { return mSectionLength; }
	virtual uint8_t getVersionNumber(void) { return mVersionNumber; }
	virtual bool getCurrentNextIndicator(void) { return mCurrentNextIndicator; }
	virtual uint8_t getSectionNumber(void) { return mSectionNumber; }
	virtual uint8_t getLastSectionNumber(void) { return mLastSectionNumber; }
	virtual uint32_t getSectionCRC(void) { return mCrc32; }

protected:
	// Specific information parsing method, derived class should implement it
	virtual bool parseInfo(uint8_t *pData, uint32_t size) = 0;
	// Parser cleaning method, derived class should implement it
	virtual void clearParser(void) = 0;

protected:
	bool mIsRecv;
	uint8_t *mSectionData;

	// section_pid
	ts_pid_t mPid;
	// table_id
	table_id_t mTableId;
	// section_syntax_indicatior
	bool mSectionSyntaxIndicator;
	// private_indicatior
	bool mPrivateIndicator;
	// section_length
	uint16_t mSectionLength;
	// table_id_extention
	uint16_t  mTableIdExtension;
	// version_number
	uint8_t mVersionNumber;
	// current_next_indicator
	bool mCurrentNextIndicator;
	// section_number
	uint8_t mSectionNumber;
	// last_section_number
	uint8_t mLastSectionNumber;
	// protocal_version
	uint8_t mProtocolVersion;
	// crc32
	uint32_t mCrc32;
};

#endif /* __SECTION_PARSER_H */
