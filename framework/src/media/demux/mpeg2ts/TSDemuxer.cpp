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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>

#include "Mpeg2TsTypes.h"
#include "TSPacket.h"
#include "Section.h"
#include "PATParser.h"
#include "ParseManager.h"
#include "PMTElementary.h"
#include "PESPacket.h"
#include "PESParser.h"
#include "TSDemuxer.h"

#include "../../StreamBuffer.h"
#include "../../StreamBufferReader.h"
#include "../../StreamBufferWriter.h"

// times to match sync byte for resync
#define TS_SYNC_COUNT               (3)
// threshold is not used, we don't have any buffer observer now.
#define TS_DEMUX_BUFFER_THRESHOLD   (CONFIG_DEMUX_BUFFER_SIZE / 2)

namespace media {

TSDemuxer::TSDemuxer()
	: Demuxer(AUDIO_TYPE_MP2T)
	, mPESPid(INVALID_PID)
	, mPESDataUsed(0)
{
}

TSDemuxer::~TSDemuxer()
{
}

std::shared_ptr<TSDemuxer> TSDemuxer::create(void)
{
	auto instance = std::make_shared<TSDemuxer>();
	if (instance && instance->initialize()) {
		return instance;
	}

	meddbg("create TSDemuxer instance failed!\n");
	return nullptr;
}

bool TSDemuxer::initialize(void)
{
	auto streamBuffer = stream::StreamBuffer::Builder()
							.setBufferSize(CONFIG_DEMUX_BUFFER_SIZE)
							.setThreshold(TS_DEMUX_BUFFER_THRESHOLD)
							.build();
	if (!streamBuffer) {
		meddbg("streamBuffer is nullptr!\n");
		return false;
	}

	mStreamBuffer = streamBuffer;
	mBufferReader = std::make_shared<stream::StreamBufferReader>(mStreamBuffer);
	mBufferWriter = std::make_shared<stream::StreamBufferWriter>(mStreamBuffer);

	if (!mBufferReader || !mBufferWriter) {
		meddbg("mBufferReader/Writer is nullptr!\n");
		return false;
	}

	mParserManager = std::make_shared<ParserManager>();
	if (!mParserManager) {
		meddbg("mParserManager is nullptr!\n");
		return false;
	}

	mPESParser = std::make_shared<PESParser>();
	if (!mPESParser) {
		meddbg("mPESParser is nullptr!\n");
		return false;
	}

	mTSPacket = std::make_shared<TSPacket>();
	if (!mTSPacket) {
		meddbg("mTSPacket is nullptr!\n");
		return false;
	}

	return true;
}

size_t TSDemuxer::getAvailSpace(void)
{
	return mBufferWriter->sizeOfSpace();
}

ssize_t TSDemuxer::pushData(uint8_t *buf, size_t size)
{
	size_t written = 0;
	if (mBufferWriter) {
		written += mBufferWriter->write(buf, size);
	}
	return (ssize_t)written;
}

ssize_t TSDemuxer::pullData(uint8_t *buf, size_t size, void *param)
{
	if (mPESPid == INVALID_PID) {
		// setup PES pid
		prog_num_t progNum;
		if (param) {
			progNum = *((prog_num_t *)param);
		} else {
			// use default 1st program
			std::vector<prog_num_t> programs;
			mParserManager->getPrograms(programs);
			progNum = programs[0];
		}
		uint8_t streamType;
		if (!mParserManager->getAudioStreamInfo(progNum, streamType, mPESPid)) {
			meddbg("get audio PES PID failed\n");
			return DEMUXER_ERROR_NOT_READY;
		}
		medvdbg("setup audio PES PID: 0x%x\n", mPESPid);
	}

	int ret = DEMUXER_ERROR_NONE;
	size_t fill = 0;
	size_t need;
	while (fill < size) {
		need = size - fill;
		if (mPESParser->getESData() != nullptr) {
			// get remaining payload in last PES packet
			if (need > mPESParser->getESDataLen() - mPESDataUsed) {
				need = mPESParser->getESDataLen() - mPESDataUsed;
			}

			memcpy(&buf[fill], mPESParser->getESData() + mPESDataUsed, need);
			mPESDataUsed += need;
			fill += need;
			medvdbg("Got ES data %u(%u)/%u\n", fill, need, size);

			if (mPESDataUsed == mPESParser->getESDataLen()) {
				// all ES data in PES parser have been read.
				medvdbg("All ES data (%u) in PES parser have been read!\n", mPESParser->getESDataLen());
				mPESParser->reset();
				mPESDataUsed = 0;
			}
			continue;
		} else {
			medvdbg("Need to get new PES packet!\n");
		}

		// get new PES packet
		std::shared_ptr<PESPacket> pPESPacket = nullptr;
		ret = getPESPacket(pPESPacket);
		if (ret == DEMUXER_ERROR_WANT_DATA) {
			medvdbg("Push more data to get PES packet\n");
			break;
		}

		if (ret != DEMUXER_ERROR_NONE) {
			meddbg("Get PES packet failed! error: %d\n", ret);
			break;
		}

		// parse PES packet
		if (mPESParser->parse(pPESPacket)) {
			mPESDataUsed = 0;
		} else {
			meddbg("PES parse failed!\n");
			continue;
		}
	} // end while

	if (fill == 0) {
		medvdbg("Got nothing, please check error: %d\n", ret);
		return (ssize_t)ret;
	}

	return (ssize_t)fill;
}

bool TSDemuxer::getPrograms(std::vector<prog_num_t> &progs)
{
	return mParserManager->getPrograms(progs);
}

audio_type_t TSDemuxer::getAudioType(void *param)
{
	uint8_t streamType;
	ts_pid_t streamPid;
	prog_num_t progNum;

	if (param) {
		progNum = *((prog_num_t *)param);
	} else {
		std::vector<prog_num_t> programs;
		mParserManager->getPrograms(programs);
		progNum = programs[0];
	}

	if (mParserManager->getAudioStreamInfo(progNum, streamType, streamPid)) {
		switch (streamType) {
		case PMTElementary::STREAM_TYPE_AUDIO_AAC:
		case PMTElementary::STREAM_TYPE_AUDIO_HE_AAC:
			return AUDIO_TYPE_AAC;
		case PMTElementary::STREAM_TYPE_AUDIO_MPEG1:
			return AUDIO_TYPE_MP3;
		default:
			meddbg("unsupported audio type 0x%x\n", streamType);
			break;
		}
	} else {
		meddbg("didn't get any audio stream info\n");
	}

	return AUDIO_TYPE_INVALID;
}

// return value
// on success, [0, TSPacket::PACKET_SIZE)
// on failure, demuxer_error_e
int TSDemuxer::resync(uint8_t *pPacketData, size_t readOffset)
{
	uint8_t buffer[TSPacket::PACKET_SIZE];
	size_t szRead;
	int syncOffset;
	int count;

	for (syncOffset = 0; syncOffset < TSPacket::PACKET_SIZE; syncOffset++) {
		if (pPacketData[syncOffset] != TSPacket::SYNC_BYTE) {
			continue;
		}

		// found sync byte, now do sync verification!
		for (count = 1; count < TS_SYNC_COUNT; count++) {
			szRead = mBufferReader->copy(buffer, TSPacket::PACKET_SIZE, readOffset + syncOffset + count * TSPacket::PACKET_SIZE);
			if (szRead != TSPacket::PACKET_SIZE) {
				// data in buffer is not enough for sync verification
				return DEMUXER_ERROR_WANT_DATA;
			}

			if (buffer[0] != TSPacket::SYNC_BYTE) {
				// sync not match
				break;
			}
		}

		if (count == TS_SYNC_COUNT) {
			// sync verification succeed
			return syncOffset;
		}
	}

	// Do not find sync byte in one ts packet (188 bytes),
	// maybe it's not a transport stream.
	return DEMUXER_ERROR_SYNC_FAILED;
}

std::shared_ptr<Section> TSDemuxer::PSIUnpack(std::shared_ptr<TSPacket> pTSPacket)
{
	std::shared_ptr<Section> pSection = nullptr;
	uint8_t  lenPayload = 0;
	uint8_t *ptrPayload = pTSPacket->getPayloadData(&lenPayload);

	if (!ptrPayload) {
		// no payload
		return pSection;
	}

	if (pTSPacket->payloadUnitStartIndicator()) {
		// new section start
		// first byte in payload is the pointer field in case of unit start indicator is 1
		uint8_t u8PointerField = ptrPayload[0];
		if (u8PointerField != 0) {
			// prev section tail and next section head in this packet,
			// firstly, handle prev section data
			auto it = mPidSectionMap.find(pTSPacket->getPid());
			if (it != mPidSectionMap.end()) {
				auto preSection = it->second;
				preSection->appendData(pTSPacket->getPid(), pTSPacket->continuityCounter(), ptrPayload + 1, u8PointerField);
				if (preSection->isCompleted()) {
					pSection = preSection;
				} else {
					meddbg("Drop incomplete section!\n");
				}
				// remove section from map anyway
				mPidSectionMap.erase(it);
			}
		}

		// and then handle new section
		auto newSection = Section::create(pTSPacket->getPid(), pTSPacket->continuityCounter(), ptrPayload + 1 + u8PointerField, lenPayload - 1 - u8PointerField);
		if (newSection->isCompleted()) {
			if (pSection != nullptr) {
				meddbg("It should be unreachable! Fixme if it happen!\n");
			}
			pSection = newSection;
		} else {
			mPidSectionMap.insert(std::pair<uint16_t, std::shared_ptr<Section>>(pTSPacket->getPid(), newSection));
		}
	} else {
		// section appending
		auto it = mPidSectionMap.find(pTSPacket->getPid());
		if (it != mPidSectionMap.end()) {
			auto preSection = it->second;
			preSection->appendData(pTSPacket->getPid(), pTSPacket->continuityCounter(), ptrPayload, lenPayload); // no point filed
			if (preSection->isCompleted()) {
				pSection = preSection;
				// remove section from map
				mPidSectionMap.erase(it);
			}
		}
	}

	return pSection;
}

std::shared_ptr<PESPacket> TSDemuxer::PESUnpack(std::shared_ptr<TSPacket> pTSPacket)
{
	std::shared_ptr<PESPacket> pPESPacket = nullptr;
	uint8_t  lenPayload = 0;
	uint8_t *ptrPayload = pTSPacket->getPayloadData(&lenPayload);

	if (!ptrPayload) {
		// no payload
		return pPESPacket;
	}

	if (pTSPacket->payloadUnitStartIndicator()) {
		// new PES packet start
		medvdbg("new PES packet (PID:%u) start...\n", pTSPacket->getPid());
		auto newPacket = PESPacket::create(pTSPacket->getPid(), pTSPacket->continuityCounter(), ptrPayload, lenPayload);
		if (newPacket->isCompleted()) {
			medvdbg("PES packet (PID:%u) complete\n", pTSPacket->getPid());
			pPESPacket = newPacket;
		} else {
			auto it = mPidPESPacketMap.find(pTSPacket->getPid());
			if (it != mPidPESPacketMap.end()) {
				// incomplete PES packet with same PID exist, remove it!
				mPidPESPacketMap.erase(it);
			}
			mPidPESPacketMap.insert(std::pair<uint16_t, std::shared_ptr<PESPacket>>(pTSPacket->getPid(), newPacket));
		}
	} else {
		// PES packet appending
		auto it = mPidPESPacketMap.find(pTSPacket->getPid());
		if (it != mPidPESPacketMap.end()) {
			auto prePacket = it->second;
			prePacket->appendData(pTSPacket->getPid(), pTSPacket->continuityCounter(), ptrPayload, lenPayload);
			if (prePacket->isCompleted()) {
				medvdbg("PES packet (PID:%u) complete\n", pTSPacket->getPid());
				pPESPacket = prePacket;
				mPidPESPacketMap.erase(it);
			}
		}
	}

	return pPESPacket;
}

bool TSDemuxer::isPsiPid(uint16_t pid)
{
	switch (pid) {
	case PATParser::PAT_PID:
		return true;
	default:
		return mParserManager->isPMTPid(pid);
	}
}

bool TSDemuxer::isPESPid(uint16_t pid)
{
	return (pid == mPESPid);
}

int TSDemuxer::loadTSPacket(std::shared_ptr<TSPacket> pTSPacket, bool sync, size_t *offset)
{
	int syncOffset = 0;
	uint8_t buffLen; // TSPacket::PACKET_SIZE
	uint8_t *pBuffer = pTSPacket->getPacketBuffer(&buffLen);
	size_t readOffset = (offset == nullptr) ? 0 : *offset;

	// try to copy 188 bytes of packet data from stream buffer
	size_t size = mBufferReader->copy(pBuffer, buffLen, readOffset);
	if (size != buffLen) {
		// data in buffer is not enough!
		return DEMUXER_ERROR_WANT_DATA;
	}

	// check if resync is required
	if (sync || !pTSPacket->parse()) {
		syncOffset = resync(pBuffer, readOffset);
		if (syncOffset < 0) {
			// sync failed, negative value means error code.
			return syncOffset;
		}
		if (syncOffset != 0) {
			mBufferReader->copy(pBuffer, buffLen, readOffset + (size_t)syncOffset);
			readOffset += syncOffset;
		}
		// parse packet again after resync
		pTSPacket->parse();
	}

	// 188 bytes ts packet has been loaded to output buffer
	readOffset += buffLen;
	if (offset == nullptr) {
		mBufferReader->read(NULL, readOffset, false);
	} else {
		*offset = readOffset;
	}

	return DEMUXER_ERROR_NONE;
}

// return demuxer_error_e
int TSDemuxer::getPESPacket(std::shared_ptr<PESPacket> &pPESPacket)
{
	int ret;

	while ((ret = loadTSPacket(mTSPacket)) == DEMUXER_ERROR_NONE) {
		if (isPESPid(mTSPacket->getPid())) {
			pPESPacket = PESUnpack(mTSPacket);
			if (pPESPacket) {
				medvdbg("got new PES packet\n");
				return DEMUXER_ERROR_NONE;
			}
		}
	}

	return ret;
}

bool TSDemuxer::isReady(void)
{
	return (mParserManager->isPATReceived() && mParserManager->isPMTReceived());
}

// preparse ts stream buffer, get PAT and PMT info.
// return demuxer_error_t
int TSDemuxer::prepare(void)
{
	ssize_t ret;
	size_t readOffset = 0;

	medvdbg("called!\n");

	if (isReady()) {
		// parser is ready, do not need to reparse
		return DEMUXER_ERROR_NONE;
	}

	// Load 1st ts packet with force sync
	ret = loadTSPacket(mTSPacket, true, &readOffset);
	while (ret == DEMUXER_ERROR_NONE) {
		if (isPsiPid(mTSPacket->getPid())) {
			// packet of PSI table section
			auto pSection = PSIUnpack(mTSPacket);
			if (pSection) {
				// got PSI section
				if (mParserManager->processSection(pSection)) {
					if (isReady()) {
						// pre parse succeed
						medvdbg("preparse succeed!\n");
						return DEMUXER_ERROR_NONE;
					}
				}
			}
		}
		ret = loadTSPacket(mTSPacket, false, &readOffset);
	}

	// return error code
	meddbg("failed! error: %d\n", ret);
	return ret;
}

} // namespace media
