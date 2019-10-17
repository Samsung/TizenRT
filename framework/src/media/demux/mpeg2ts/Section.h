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

#ifndef __SECTION_H
#define __SECTION_H

#include <memory>
#include "Mpeg2TsTypes.h"

class Section
{
public:
	// should always use this static method to create a new section instance
	static std::shared_ptr<Section> create(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size);
	// constructor and destructor
	Section();
	virtual ~Section();
	// initialize section member and allocate data buffer
	bool initialize(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size);
	// append new section data from ts packet payload
	bool appendData(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size);
	// verify mpeg2 crc32
	bool verifyCrc32(void);
	// check if section is completed
	bool isCompleted(void);
	// get pointer of section data buffer
	uint8_t *getDataPtr(void) { return mSectionData; }
	// get length in bytes of section data
	uint16_t getDataLen(void) { return mSectionDataLen; }
	// get PID
	ts_pid_t getPid(void) { return mPid; }

protected:
	// parse length field from the given data
	// return length value of the object, in this class it's section_length
	// derived class can override this method to get it's own length field.
	virtual uint16_t parseLengthField(uint8_t *pData, uint16_t size);
	// calculates the MPEG2 32 bit CRC
	uint32_t crc32(uint8_t *data, uint32_t length);

private:
	// PID of transport stream this section from
	ts_pid_t mPid;
	// continuity counter of last ts packet accepted
	uint8_t mContinuityCounter;
	// section data buffer allocated
	uint8_t *mSectionData;
	// total data length in bytes of a completed section
	uint16_t mSectionDataLen;
	// present data length in section data buffer
	uint16_t mPresentDataLen;
};

#endif /* __SECTION_H */
