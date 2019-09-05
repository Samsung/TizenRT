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
#include "PESPacket.h"
#include "Mpeg2TsTypes.h"

#define PACKET_LENGTH(buffer)   ((buffer[4] << 8) | buffer[5])
#define PES_PACKET_HEAD_BYTES   (6) // packet_start_code_prefix + stream_id + PES_packet_length

std::shared_ptr<PESPacket> PESPacket::create(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size)
{
	auto instance = std::make_shared<PESPacket>();
	if (instance && instance->initialize(pid, continuityCounter, pData, size)) {
		return instance;
	}

	meddbg("create PESPacket instance failed!\n");
	return nullptr;
}

uint16_t PESPacket::parseLengthField(uint8_t *pData, uint16_t size)
{
	return (PES_PACKET_HEAD_BYTES + PACKET_LENGTH(pData));
}
