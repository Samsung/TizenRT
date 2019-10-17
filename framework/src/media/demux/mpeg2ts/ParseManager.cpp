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
#include "Section.h"
#include "SectionParser.h"
#include "PATParser.h"
#include "PMTParser.h"
#include "PMTInstance.h"
#include "PMTElementary.h"
#include "ParseManager.h"

#define TABILE_ID(buffer)   (buffer[0])

ParserManager::ParserManager()
{
	// Add PAT parser
	addParser(std::make_shared<PATParser>());
	// Add PMT parser
	addParser(std::make_shared<PMTParser>());
}

ParserManager::~ParserManager()
{
	mTableParsers.clear();
}

bool ParserManager::isPATReceived(void)
{
	auto pPATParser = std::static_pointer_cast<PATParser>(getParser(PATParser::TABLE_ID));
	if (!pPATParser) {
		meddbg("PAT parser is not found!\n");
		return false;
	}

	return pPATParser->isRecv();
}

bool ParserManager::isPMTReceived(prog_num_t progNum)
{
	auto pPMTParser = std::static_pointer_cast<PMTParser>(getParser(PMTParser::TABLE_ID));
	if (!pPMTParser) {
		meddbg("PMT parser is not found!\n");
		return false;
	}

	auto pPMTInstance = pPMTParser->getPMTInstance(progNum);
	if (!pPMTInstance) {
		medwdbg("PMT of program %d has not been received yet!\n", progNum);
		return false;
	}

	return pPMTInstance->isCompleted();
}

bool ParserManager::isPMTReceived(void)
{
	auto pPATParser = std::static_pointer_cast<PATParser>(getParser(PATParser::TABLE_ID));
	if (!pPATParser) {
		meddbg("PAT parser is not found!\n");
		return false;
	}

	if (pPATParser->isRecv()) {
		size_t i;
		size_t progs = pPATParser->sizeOfProgram();
		for (i = 0; i < progs; i++) {
			if (!isPMTReceived(pPATParser->getProgramNumber(i))) {
				return false;
			}
		}
		return true;
	}

	return false;
}

bool ParserManager::getAudioStreamInfo(prog_num_t progNum, uint8_t &streamType, ts_pid_t &pid)
{
	auto pPMTParser = std::static_pointer_cast<PMTParser>(getParser(PMTParser::TABLE_ID));
	auto pPMTInstance = pPMTParser->getPMTInstance(progNum);

	if (pPMTInstance && pPMTInstance->isCompleted()) {
		size_t i;
		size_t num = pPMTInstance->numOfElementary();
		for (i = 0; i < num; i++) {
			auto pStream = pPMTInstance->getPMTElementary(i);
			if (!pStream) {
				meddbg("Run out of memory!\n");
				return false;
			}

			switch (pStream->getStreamType()) {
				case PMTElementary::STREAM_TYPE_AUDIO_AAC:   // fall through
				case PMTElementary::STREAM_TYPE_AUDIO_MPEG2: // fall through
				case PMTElementary::STREAM_TYPE_AUDIO_AC3:   // fall through
				case PMTElementary::STREAM_TYPE_AUDIO_MPEG1: // fall through
				case PMTElementary::STREAM_TYPE_AUDIO_HE_AAC:
					streamType = pStream->getStreamType();
					pid = pStream->getElementaryPID();
					medvdbg("stream type 0x%02x, pid 0x%x\n", streamType, pid);
					return true;
				default:
					break;
			}
		}
		meddbg("Can not find audio stream in PMT of program number %d\n", progNum);
		return false;
	}

	meddbg("PMT of program number %d has not be received!\n", progNum);
	return false;
}

bool ParserManager::getPrograms(std::vector<prog_num_t> &programs)
{
	size_t i, num;

	auto pPATParser = std::static_pointer_cast<PATParser>(getParser(PATParser::TABLE_ID));
	if (!pPATParser) {
		meddbg("PAT parser is not found!\n");
		return false;
	}

	if (!pPATParser->isRecv()) {
		meddbg("PAT has not been received yet!\n");
		return false;
	}

	num = pPATParser->sizeOfProgram();
	for (i = 0; i < num; i++) {
		programs.push_back(pPATParser->getProgramNumber(i));
	}

	return true;
}

bool ParserManager::syncProgramInfoFromPAT(void)
{
	size_t i, num;
	ts_pid_t pmtPid;
	prog_num_t progNum;
	std::map<int, ts_pid_t> pmt_elements;

	auto pPATParser = std::static_pointer_cast<PATParser>(getParser(PATParser::TABLE_ID));
	if (!pPATParser) {
		meddbg("PAT parser is not found!\n");
		return false;
	}

	if (!pPATParser->isRecv()) {
		meddbg("PAT has not been received yet!\n");
		return false;
	}

	// Clear current PMT Pids
	mPMTPids.clear();

	// Update new informations from PAT
	num = pPATParser->sizeOfProgram();
	for (i = 0; i < num; ++i) {
		progNum = pPATParser->getProgramNumber(i);
		if (progNum == (prog_num_t)PATParser::NETWORK_PID_PN) {
			continue;
		}
		pmtPid = pPATParser->getProgramMapPID(progNum);
		if (pmtPid == (ts_pid_t)INVALID_PID) {
			continue;
		}
		pmt_elements[PMTParser::makeKey(pmtPid, progNum)] = pmtPid;
		mPMTPids.push_back(pmtPid);
		medvdbg("index %d, progNum %u ,pmtPid 0x%02x\n", i, progNum, pmtPid);
	}

	auto pPMTParser = std::static_pointer_cast<PMTParser>(getParser(PMTParser::TABLE_ID));
	if (!pPMTParser) {
		meddbg("PMT parser is not found!\n");
		return false;
	}
	// Reinitialize PMT parser and update new PMT elements to PMT parser
	pPMTParser->Initialize();
	pPMTParser->updatePMTElements(pmt_elements);
	return true;
}

bool ParserManager::isPMTPid(ts_pid_t pid)
{
	for (auto iter : mPMTPids) {
		if (iter == pid) {
			return true;
		}
	}

	return false;
}

bool ParserManager::processSection(std::shared_ptr<Section> pSection)
{
	if (!pSection) {
		meddbg("section is nullptr!\n");
		return false;
	}

	if (!pSection->verifyCrc32()) {
		meddbg("section invalid!\n");
		return false;
	}

	uint8_t *pData = pSection->getDataPtr();
	table_id_t tableId = TABILE_ID(pData);
	auto pTableParser = getParser(tableId);
	if (!pTableParser) {
		meddbg("table (0x%02x) parser is not found!\n", tableId);
		return false;
	}

	bool result = pTableParser->parseSection(pSection->getPid(), pData);
	if (result) {
		switch (tableId) {
		case PATParser::TABLE_ID: // PAT received
			if (!syncProgramInfoFromPAT()) {
				meddbg("Sync program information failed when PAT received!\n");
				return false;
			}
			break;
		default:
			break;
		}
	}

	return result;
}

bool ParserManager::addParser(std::shared_ptr<SectionParser> pParser)
{
	if (!pParser) {
		meddbg("pParser is nullptr!\n");
		return false;
	}

	if (getParser(pParser->getTableId())) {
		meddbg("parser for tableid (0x%02x) already exists!\n", pParser->getTableId());
		return false;
	}

	mTableParsers[pParser->getTableId()] = pParser;
	return true;
}

std::shared_ptr<SectionParser> ParserManager::getParser(table_id_t tableId)
{
	auto it = mTableParsers.find(tableId);
	if (it == mTableParsers.end()) {
		medwdbg("no parser for tableid (0x%02x)\n", tableId);
		return nullptr;
	}

	return it->second;
}
