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

#ifndef __TABLE_BASE_H
#define __TABLE_BASE_H

#include "Mpeg2TsTypes.h"

/* This class is used to manage all sections of a table */
class TableBase
{
public:
	enum {
		SECTION_PRESENT, // the section is present (already received).
		SECTION_INITIAL, // first time  to receive a section of one table, need to do initialization.
		SECTION_APPEND,  // receive more sections in multi section case.
		SECTION_CHANGE,  // table version changed.
		SECTION_IGNORE,  // ignore section, e.g. crc32 verify failed.
	};

private:
	// table version
	uint8_t mVersion;
	// last section number
	uint8_t mLastSectionNum;
	// CRC32 value of each section
	uint32_t *mMultiSectionCRC;
	// flag value (received or not) of each section
	bool *mMultiSectionFlag;

protected :
	TableBase();
	virtual ~TableBase();
	// Check the given section info and make the decision how to deal with it
	int checkSection(uint8_t version, uint8_t sectionNum, uint8_t lastSectionNum, uint32_t crc32 = 0);
	// Init the table with the first receieved section
	bool initTable(uint8_t version, uint8_t sectionNumber, uint8_t lastSectionNumber = 0, uint32_t crc32 = 0);
	// Check if table has been updated.
	bool isTableChanged(uint8_t version, uint8_t sectionNum, uint8_t lastSectionNum, uint32_t crc32);
	// Delete specific information (parsed from PAT/PMT tables), derived class should implement it
	virtual void deleteInfo(void) = 0;

public:
	// If all sections of the table have been received or not
	bool isCompleted(void);
	// Reset table to release all resources
	void resetTable(void);
};

#endif /* __TABLE_BASE_H */
