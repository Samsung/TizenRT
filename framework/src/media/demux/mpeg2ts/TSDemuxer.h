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

#ifndef __TS_DEMUX_H
#define __TS_DEMUX_H

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <media/MediaTypes.h>
#include "../../Demuxer.h"

class ParserManager;
class Section;
class TSPacket;
class PESParser;
class PESPacket;

namespace media {
namespace stream {
class StreamBuffer;
class StreamBufferReader;
class StreamBufferWriter;
} // namespace stream

class TSDemuxer : public Demuxer
{
public:
	// create new TSDemuxer object
	static std::shared_ptr<TSDemuxer> create(void);

	// constructor & destructor
	TSDemuxer();
	virtual ~TSDemuxer();

	// initialize TSDemuxer, malloc buffer and initialize table parser modules
	virtual bool initialize(void) override;
	// space size available for push
	virtual size_t getAvailSpace(void) override;
	// push TS data into demux buffer
	virtual ssize_t pushData(uint8_t *buf, size_t size) override;
	// pull audio elementary stream data of the given program number
	// param, pointer to program nubmer of uint16, nullptr means first program as default
	virtual ssize_t pullData(uint8_t *buf, size_t size, void *param = nullptr) override;
	// prepare TSDemuxer, preparse TS data in stream buffer to get program information ahead
	virtual int prepare(void) override;
	// check if TSDemuxer is ready (prepare succeed)
	virtual bool isReady(void) override;
	// get audio type of elementary stream of the given program number
	// param, pointer of program number of uint16, nullptr means first program as default
	virtual audio_type_t getAudioType(void *param = nullptr) override;

	// get programs list after pre parsing
	bool getPrograms(std::vector<uint16_t> &progs);

private:
	// check if the given PID is PSI table's PID in TS
	bool isPsiPid(uint16_t pid);
	// check if the given PID is PES packet's PID we need
	bool isPESPid(uint16_t pid);
	// extract a PES packet from the input transport stream
	// on success, return 0
	// on failure, return negative value (see demuxer_error_e)
	int getPESPacket(std::shared_ptr<PESPacket> &pPESPacket);
	// load a valid TS packet from the input data stream
	// sync, request to do force resync
	// offset, if not null, just copy data from stream buffer
	// return value:
	// on success, return 0
	// on failure, return negative value (see demuxer_error_e)
	int loadTSPacket(std::shared_ptr<TSPacket> pTSPacket, bool sync = false, size_t *offset = nullptr);
	// Unpack a TS packet and return a section if get a completed one
	std::shared_ptr<Section> PSIUnpack(std::shared_ptr<TSPacket> pTSPacket);
	// Unpack a TS packet and return a PES packet if get a completed one
	std::shared_ptr<PESPacket> PESUnpack(std::shared_ptr<TSPacket> pTSPacket);
	// resync TS packet by TSPacket::SYNC_BYTE
	int resync(uint8_t *pPacketData, size_t offset);

private:
	// <pid, section_ptr> pairs in map to take incomplete sections
	std::map<uint16_t, std::shared_ptr<Section>> mPidSectionMap;
	// <pid, pespacket_ptr> pairs in map to take incomplete PES packets
	std::map<uint16_t, std::shared_ptr<PESPacket>> mPidPESPacketMap;
	// PSI table pasers manager
	std::shared_ptr<ParserManager> mParserManager;
	// stream buffer to held inputing TS stream data
	std::shared_ptr<stream::StreamBuffer> mStreamBuffer;
	// reader handler of the stream buffer
	std::shared_ptr<stream::StreamBufferReader> mBufferReader;
	// writer handler of the stream buffer
	std::shared_ptr<stream::StreamBufferWriter> mBufferWriter;
	// PES parser
	std::shared_ptr<PESParser> mPESParser;
	// TS packet
	std::shared_ptr<TSPacket> mTSPacket;
	uint16_t mPESPid;
	size_t mPESDataUsed;
};

} // namespace media

#endif /* __TS_DEMUX_H */
