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

#ifndef __PES_PARSER_H
#define __PES_PARSER_H

#include <memory>
#include "Mpeg2TsTypes.h"

class PESPacket;
class PESParser
{
public:
	enum {
		PES_PACKET_START_CODE_PREFIX = 0x000001,
	};

	PESParser();
	virtual ~PESParser();
	// parse PES packet, and the parser will add reference to the packet.
	bool parse(std::shared_ptr<PESPacket> pPESPacket);
	// get ES data in PES
	uint8_t *getESData(void);
	// get ES data length
	uint16_t getESDataLen(void);
	// reset PES parser, to remove reference of the PES packet
	void reset(void);

protected:
	// parse stream data in PES
	bool parseStream(uint8_t *pData, uint32_t size);

private:
	// PES packet reference
	std::shared_ptr<PESPacket> mPESPacket;
	// packet start code prefix
	uint32_t mPacketStartCodePrefix;
	// stream id
	uint8_t mStreamId;
	// PES packet length
	uint16_t mPacketLength;
	/// for audio streams
	//{ optional PES header
	uint8_t mPESScramblingControl : 2;
	uint8_t mPESPriority : 1;
	uint8_t mDataAlignmentIndicator : 1;
	uint8_t mCopyright : 1;
	uint8_t mOriginalOrCopy : 1;
	// 7 flags
	uint8_t mPtsDtsFlags : 2;
	uint8_t mESCRFlag : 1;
	uint8_t mESRateFlag : 1;
	uint8_t mDSMTrickModeFlag : 1;
	uint8_t mAdditionalCopyInfoFlag : 1;
	uint8_t mPESCrcFlag : 1;
	uint8_t mPESExtensionFlag : 1;
	// following header length
	uint8_t mPESHeaderDataLength;
	//optional fields ...
	//} optional PES header
};

#endif /* __PES_PARSER_H */
