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

#ifndef __TS_PACKET_H
#define __TS_PACKET_H

#include "Mpeg2TsTypes.h"


class TSPacket
{
public:
	class AdaptationField
	{
	public:
		AdaptationField();
		virtual ~AdaptationField();
		// parser adaptation field in transport packet
		bool parse(const uint8_t *data);
		// getters
		uint8_t adaptationFieldLength(void) { return mAdaptationFieldLength; }
		// add more getters if necessary...

	private:
		// adaptation field length
		uint8_t mAdaptationFieldLength;
		uint8_t mDiscontinuityIndicator : 1;
		uint8_t mRandomAccessIndicator : 1;
		uint8_t mElementaryStreamPriorityIndicator : 1;
		uint8_t mPCRFlag : 1;
		uint8_t mOPCRFlag : 1;
		uint8_t mSplicingPointFlag : 1;
		uint8_t mTransportPrivateDataFlag : 1;
		uint8_t mAdaptationFieldExtensionFlag : 1;
	};

public:
	enum {
		PACKET_SIZE = 188,  // number of bytes of each transport packet
		SYNC_BYTE = 0x47,   // transport packet sync byte (0x47)
		HEAD_BYTES = 4,     // 4 bytes header followed by payload
	};

	enum {
		CONTROL_RESERVED = 0,
		CONTROL_PAYLOAD_ONLY = 1,
		CONTROL_ADAPTATION_ONLY = 2,
		CONTROL_ADAPTATION_PLAYLOAD = 3,
	};

	TSPacket();
	virtual ~TSPacket();

	// parse transport packet stored in packet data buffer
	// get packet buffer and put data in the buffer firstly
	bool parse(void);

	// getters
	ts_pid_t getPid(void) { return mPid; }
	bool transportErrorIndicator(void) { return static_cast<bool>(mTransportErrorIndicator); }
	bool payloadUnitStartIndicator(void) { return static_cast<bool>(mPayloadUnitStartIndicator); }
	uint8_t adaptationFieldControl(void) { return mAdaptationFieldControl; }
	uint8_t continuityCounter(void) { return mContinuityCounter; }
	AdaptationField &adaptationField(void) { return mAdaptationField; }
	// get pointer to the packet data buffer (188 bytes)
	uint8_t *getPacketBuffer(uint8_t *packetBuffLen);
	// get pointer to the payload data start address
	// return nullptr if there's no payload
	uint8_t *getPayloadData(uint8_t *payloadDataLen);
	// add more getters if necessary...

private:
	// packet data array
	uint8_t mData[PACKET_SIZE];
	// sync byte
	uint8_t mSyncByte;
	// transport error indicator
	uint16_t mTransportErrorIndicator : 1;
	// payload unit start indicator
	uint16_t mPayloadUnitStartIndicator : 1;
	// transport priority
	uint16_t mTransportPriority : 1;
	// packet identifier (PID)
	uint16_t mPid : 13;
	// transport scrambling control
	uint8_t mTransportScramblingControl : 2;
	// adatation field control
	uint8_t mAdaptationFieldControl : 2;
	// continuity counter
	uint8_t mContinuityCounter : 4;
	// adaptation field object
	AdaptationField mAdaptationField;
};

#endif /* __TS_PACKET_H */
