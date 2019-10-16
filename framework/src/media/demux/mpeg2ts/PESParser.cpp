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
#include "PESParser.h"
#include "PESPacket.h"

#define PES_PACKET_HEAD_BYTES               (6) // packet_start_code_prefix + stream_id + packet length fields
#define PES_STREAM_HEAD_BYTES               (3) // stream info + 7 flags + PES head data length fields
#define PACKET_START_CODE_PREFIX(buffer)    ((buffer[0] << 16) | (buffer[1] << 8) | buffer[2])
#define STREAM_ID(buffer)                   (buffer[3])
#define PACKET_LENGTH(buffer)               ((buffer[4] << 8) | buffer[5])

PESParser::PESParser()
	: mPacketStartCodePrefix(0)
	, mStreamId(0)
	, mPacketLength(0)
{
}

PESParser::~PESParser()
{
}

bool PESParser::parse(std::shared_ptr<PESPacket> pPESPacket)
{
	if (!pPESPacket) {
		meddbg("pPESPacket is null!\n");
		reset();
		return false;
	}

	mPESPacket = pPESPacket;

	uint8_t *pData = mPESPacket->getDataPtr();
	mPacketStartCodePrefix = PACKET_START_CODE_PREFIX(pData);
	mStreamId = STREAM_ID(pData);
	mPacketLength = PACKET_LENGTH(pData);

	if (mPacketStartCodePrefix != PES_PACKET_START_CODE_PREFIX){
		meddbg("Invalid PES packet, not match PES_PACKET_START_CODE_PREFIX!\n");
		reset();
		return false;
	}

	if ((size_t)PES_PACKET_HEAD_BYTES + mPacketLength > mPESPacket->getDataLen()) {
		meddbg("Packet length overflow!\n");
		reset();
		return false;
	}

	return parseStream(&pData[PES_PACKET_HEAD_BYTES], mPacketLength);
}

bool PESParser::parseStream(uint8_t *pData, uint32_t size)
{
	if (mStreamId >= 0xc0 && mStreamId <= 0xdf) {
		// stream id = 110xxxxx means audio streams
		mPESScramblingControl   = (pData[0] >> 4) & 0x3;
		mPESPriority            = (pData[0] >> 3) & 0x1;
		mDataAlignmentIndicator = (pData[0] >> 2) & 0x1;
		mCopyright              = (pData[0] >> 1) & 0x1;
		mOriginalOrCopy         = (pData[0]) & 0x1;
		mPtsDtsFlags            = (pData[1] >> 6) & 0x3;
		mESCRFlag               = (pData[1] >> 5) & 0x1;
		mESRateFlag             = (pData[1] >> 4) & 0x1;
		mDSMTrickModeFlag       = (pData[1] >> 3) & 0x1;
		mAdditionalCopyInfoFlag = (pData[1] >> 2) & 0x1;
		mPESCrcFlag             = (pData[1] >> 1) & 0x1;
		mPESExtensionFlag       = (pData[1]) & 0x1;
		mPESHeaderDataLength    = (pData[2]);
		medvdbg("stream_id: 0x%x for audio!\n", mStreamId);
		return true;
	}

	meddbg("stream_id: 0x%x is not supported!\n", mStreamId);
	reset();
	return false;
}

uint8_t *PESParser::getESData(void)
{
	if (!mPESPacket) {
		// no PES packet, it's normal case.
		return nullptr;
	}

	return mPESPacket->getDataPtr() + PES_PACKET_HEAD_BYTES + PES_STREAM_HEAD_BYTES + mPESHeaderDataLength;
}

uint16_t PESParser::getESDataLen(void)
{
	if (!mPESPacket) {
		meddbg("There's no valid PES packet!\n");
		return 0;
	}

	return mPacketLength - PES_STREAM_HEAD_BYTES - mPESHeaderDataLength;
}

void PESParser::reset(void)
{
	medvdbg("reset PES packet!\n");
	mPESPacket = nullptr;
	mPacketStartCodePrefix = 0;
	mStreamId = 0;
	mPacketLength = 0;
}
