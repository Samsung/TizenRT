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

#ifndef __PES_PACKET_H
#define __PES_PACKET_H

#include <memory>
#include "Mpeg2TsTypes.h"
#include "Section.h"

class PESPacket : public Section
{
public:
	// should always use this static method to create a new PESPacket instance
	static std::shared_ptr<PESPacket> create(ts_pid_t pid, uint8_t continuityCounter, uint8_t *pData, uint16_t size);
	// constructor and destructor
	PESPacket() {}
	virtual ~PESPacket() {}

protected:
	// parse length field from the given data
	// return length value of the PES packet.
	virtual uint16_t parseLengthField(uint8_t *pData, uint16_t size) override;
};

#endif /* __PES_PACKET_H */
