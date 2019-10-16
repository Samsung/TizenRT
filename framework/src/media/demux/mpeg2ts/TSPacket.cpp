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
#include "TSPacket.h"
#include "Mpeg2TsTypes.h"

/*
Each TS Packet consists of 188 bytes.
|   syntax                                      | bits
transport_packet() {
    sync_byte                                   | 8
    transport_error_indicator                   | 1
    payload_unit_start_indicator                | 1
    transport_priority                          | 1
    PID                                         | 13
    transport_scrambling_control                | 2
    adaptation_field_control                    | 2
    continuity_counter                          | 4
    if (adaptation_field_control == '10' ||
        adaptation_field_control == '11') {
        adaptation_field()                      | see adaptation_field()
    }
    if (adaptation_field_control == '01' ||
        adaptation_field_control == '11') {
        for (i = 0; i < N; i++){
            data_byte                           | 8
        }
    }
}

adaptation_field() {
    adaptation_field_length                     | 8
    if (adaptation_field_length > 0) {
        discontinuity_indicator                 | 1
        random_access_indicator                 | 1
        elementary_stream_priority_indicator    | 1
        PCR_flag                                | 1
        OPCR_flag                               | 1
        splicing_point_flag                     | 1
        transport_private_data_flag             | 1
        adaptation_field_extension_flag         | 1
        if (PCR_flag) {
            ...
        }
        if (OPCR_flag) {
            ...
        }
        ...
        for (i = 0; i < N; i++){
            stuffing_byte                       | 8
        }
    }
}
*/


#define BITS_MASK(bits)     ((1 << (bits)) - 1)
#define PID_MASK            BITS_MASK(13)
#define LENGTH_BYTES        (1) // adatation-field-length takes 1 byte

TSPacket::AdaptationField::AdaptationField()
	: mAdaptationFieldLength(0)
{
}

TSPacket::AdaptationField::~AdaptationField()
{
}

bool TSPacket::AdaptationField::parse(const uint8_t *pData)
{
	mAdaptationFieldLength = pData[0];
	if (mAdaptationFieldLength > 0) {
		mDiscontinuityIndicator            = (pData[1] >> 7) & BITS_MASK(1);
		mRandomAccessIndicator             = (pData[1] >> 6) & BITS_MASK(1);
		mElementaryStreamPriorityIndicator = (pData[1] >> 5) & BITS_MASK(1);
		mPCRFlag                           = (pData[1] >> 4) & BITS_MASK(1);
		mOPCRFlag                          = (pData[1] >> 3) & BITS_MASK(1);
		mSplicingPointFlag                 = (pData[1] >> 2) & BITS_MASK(1);
		mTransportPrivateDataFlag          = (pData[1] >> 1) & BITS_MASK(1);
		mAdaptationFieldExtensionFlag      = (pData[1]) & BITS_MASK(1);
		// parse more if necessary...
	}

	return true;
}

TSPacket::TSPacket()
	: mSyncByte(0)
	, mTransportErrorIndicator(0)
	, mPayloadUnitStartIndicator(0)
	, mTransportPriority(0)
	, mPid(INVALID_PID)
	, mTransportScramblingControl(0)
	, mAdaptationFieldControl(0)
	, mContinuityCounter(0)
{
}

TSPacket::~TSPacket()
{
}

bool TSPacket::parse(void)
{
	const uint8_t *pData = mData;

	mSyncByte = pData[0];
	if (mSyncByte != SYNC_BYTE) {
		return false;
	}

	mTransportErrorIndicator    = (pData[1] >> 7) & BITS_MASK(1);
	mPayloadUnitStartIndicator  = (pData[1] >> 6) & BITS_MASK(1);
	mTransportPriority          = (pData[1] >> 5) & BITS_MASK(1);
	mPid                        = ((pData[1] << 8) + pData[2]) & BITS_MASK(13);
	mTransportScramblingControl = (pData[3] >> 6) & BITS_MASK(2);
	mAdaptationFieldControl     = (pData[3] >> 4) & BITS_MASK(2);
	mContinuityCounter          = (pData[3]) & BITS_MASK(4);

	if (mAdaptationFieldControl == CONTROL_ADAPTATION_ONLY ||
		mAdaptationFieldControl == CONTROL_ADAPTATION_PLAYLOAD) {
		mAdaptationField.parse(pData + HEAD_BYTES);
	}

	return true;
}

uint8_t *TSPacket::getPacketBuffer(uint8_t *packetBuffLen)
{
	if (packetBuffLen) {
		*packetBuffLen = PACKET_SIZE;
	}
	return mData;
}

uint8_t *TSPacket::getPayloadData(uint8_t *payloadDataLen)
{
	uint8_t lenPayload = PACKET_SIZE - HEAD_BYTES;
	uint8_t *ptrPayload = mData + HEAD_BYTES;

	if (mSyncByte != SYNC_BYTE) {
		meddbg("Invalid packet\n");
		return nullptr;
	}

	if (transportErrorIndicator()) {
		meddbg("Transport Error\n");
		return nullptr;
	}

	if (adaptationFieldControl() == CONTROL_RESERVED) {
		medwdbg("adaptationFieldControl 0 is reserved\n");
		return nullptr;
	}

	if (adaptationFieldControl() == CONTROL_ADAPTATION_ONLY) {
		// no playload, 183 bytes adaption field only
		return nullptr;
	}

	if (adaptationFieldControl() == CONTROL_ADAPTATION_PLAYLOAD) {
		// 0~182 bytes adaption field + playload
		lenPayload = PACKET_SIZE - HEAD_BYTES - (LENGTH_BYTES + adaptationField().adaptationFieldLength());
		ptrPayload = mData + (PACKET_SIZE - lenPayload);
	}

	if (payloadDataLen) {
		*payloadDataLen = lenPayload;
	}

	return ptrPayload;
}
