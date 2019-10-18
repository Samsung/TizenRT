/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <media/MediaUtils.h>
#include <debug.h>
#include <errno.h>
#include <tinyara/config.h>
#include <cstring>

namespace media {
namespace utils {

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef __GNUC__
#define POPCOUNT(x) __builtin_popcount(x)
#else
#define POPCOUNT(x) popcount(x)
#endif

#define IF_FAIL_RETURN_VAL(cond, val)           do { if (!(cond)) { return val; } } while (0)
#define MINIMUM(x, y)                           (((x) < (y)) ? (x) : (y))

#define TS_PACKET_SIZE                          188     // number of bytes of each transport packet
#define TS_SYNC_BYTE                            0x47    // transport packet sync byte (0x47)
#define TS_SYNC_COUNT                           3       // match Sync-Byte 3 times at least
#define TS_HEAD_BYTES                           4       // 4 bytes header followed by payload
#define TS_ADAPT_FIELD_LENGTH_BYTES             1       // adaptation-field-length takes 1 byte
#define TS_CONTINUITY_COUNTER_MOD               16      // continuity counter's module value

#define BITS_MASK(bits)                         ((1 << (bits)) - 1)
#define TS_TRANSPORT_ERROR_INDICATOR(buffer)    ((buffer[1] >> 7) & BITS_MASK(1))
#define TS_PAYLOAD_UNIT_START_INDICATOR(buffer) ((buffer[1] >> 6) & BITS_MASK(1))
#define TS_PID(buffer)                          (((buffer[1] << 8) + buffer[2]) & BITS_MASK(13))
#define TS_ADAPTION_FIELD_CONTROL(buffer)       ((buffer[3] >> 4) & BITS_MASK(2))
#define TS_CONTINUITY_COUNTER(buffer)           ((buffer[3]) & BITS_MASK(4))
#define TS_ADAPTION_FIELD_LENGTH(buffer)        (buffer[TS_HEAD_BYTES])

#define SECTION_LENGTH(buffer)                  (((buffer[1] & 0x0F) << 8) + buffer[2])
#define SECTION_HEAD_BYTES                      3       // table_id + ... + section_length
#define PAT_PID                                 0x0000  // PID of Program Association Table
#define NETWORK_PID_PN                          0x0000  // program number 0 specifies the network PID
#define PMT_STREAMS_MAXIMUM                     10      // maximum number of streams in PMT
#define PROGRAM_INFO_LENGTH(buffer)             (((buffer[10] << 8) + buffer[11]) & 0x0FFF)
#define STREAM_INFO_HEAD_BYTES                  5       // each stream info head takes 5bytes
#define ES_INFO_LENGTH(buffer)                  ((((buffer)[3] << 8) + (buffer)[4]) & 0x0FFF)
#define CRC32_BYTES                             4       // CRC32 takes 4bytes

// Supported audio stream types
#define STREAM_TYPE_AUDIO_MPEG1                 0x03 // MPEG-1 audio (ISO/IEC 11172-3 audio)
#define STREAM_TYPE_AUDIO_AAC                   0x0F // MPEG-2 lower bit-rate audio (ISO/IEC 13818-7 ADTS AAC)
#define STREAM_TYPE_AUDIO_HE_AAC                0x11 // MPEG-4 LOAS multi-format framed audio (ISO/IEC 14496-3)

#define PES_PACKET_HEAD_BYTES                   6   // packet_start_code_prefix + stream_id + PES_packet_length
#define PES_STREAM_HEAD_BYTES                   3   // stream info + 7 flags + PES head data length fields
#define PES_PACKET_START_CODE_PREFIX(buffer)    ((buffer[0] << 16) | (buffer[1] << 8) | buffer[2])
#define PES_STREAM_ID(buffer)                   (buffer[3])
#define PES_PACKET_LENGTH(buffer)               ((buffer[4] << 8) | buffer[5])
#define PES_HEADER_DATA_LENGTH(buffer)          (buffer[PES_PACKET_HEAD_BYTES + 2])

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
enum utils_error_e {
	UTILS_ERROR_WANT_MORE_DATA = -6,
	UTILS_ERROR_NOT_FOUND = -5,
	UTILS_ERROR_NOT_SUPPORTED = -4,
	UTILS_ERROR_NOT_AVAILABLE = -3,
	UTILS_ERROR_BAD_PARAMETER = -2,
	UTILS_ERROR_UNKNOWN = -1,
	UTILS_ERROR_NONE = 0 // No Error
};

typedef struct {
	uint16_t tsId;          // transport stream ID
	uint16_t numOfPrgs;     // number of programs
} psi_pat_info_t;

typedef struct {
	uint16_t prgNum;        // program number
	uint16_t numOfStreams;  // number of streams
} psi_pmt_info_t;

typedef struct {
	uint8_t type;           // stream type
	uint16_t pid;           // elementary PID
} psi_pmt_stream_info_t;

typedef struct {
	const uint8_t *data;    // section data buffer address
	uint16_t size;          // size in bytes of section data
} psi_section_t;

/**
 * Limitations:
 * Most of time there's only one section in a PSI table (PAT especially),
 * Therefore, now we simply support single section for both PAT and PMT,
 * and this single section must be packaged in only one TS packet.
 * If the limit is broken, ts_psi_process() returns UTILS_ERROR_NOT_SUPPORTED.
 */
typedef struct {
	psi_section_t section;  // single section of PSI table
} psi_table_t;

typedef struct {
	uint16_t pid;           // PID of PES
	uint16_t packetLen;     // total length bytes of the PES packet
	uint16_t present;       // present bytes unpacked from TS packets
	uint16_t esOffset;      // consumed ES data offset from PES packet head
	uint8_t  continuity;    // continuity counter of last unpacked TS packet
} pes_packet_t;

typedef struct {
	uint8_t *buffer;        // user given mpeg2-ts data buffer
	size_t buflen;          // length in bytes of data in the buffer
	size_t offset;          // offset in bytes indicate how many data used in buffer
	pes_packet_t pes;       // PES
	psi_table_t pat;        // PAT
	psi_table_t pmt;        // PMT
	uint16_t numOfStreams;  // number of streams in PMT components
	uint16_t streamOffset[PMT_STREAMS_MAXIMUM]; // stream info offset map
} ts_demux_t;

/****************************************************************************
 * Private Variables
 ****************************************************************************/
// Mine-Type for audio stream
static const std::string AAC_MIME_TYPE = "audio/aac";
static const std::string AACP_MIME_TYPE = "audio/aacp";
static const std::string MPEG_MIME_TYPE = "audio/mpeg";
static const std::string MP4_MIME_TYPE = "audio/mp4";
static const std::string OPUS_MIME_TYPE = "audio/opus";
static const std::string MP2T_MIME_TYPE = "video/MP2T";

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifndef __GNUC__
static int32_t popcount(uint32_t x)
{
	x -= (x >> 1) & 0x55555555;
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x += x >> 8;
	x += x >> 16;
	return x & 0x0000003F;
}
#endif

// Locate 1st TS packet in the given buffer, resync header if necessary or required.
static int findTsPacket(const uint8_t *buffer, size_t size, bool sync)
{
	size_t syncOffset = 0;
	IF_FAIL_RETURN_VAL(size >= TS_PACKET_SIZE, UTILS_ERROR_WANT_MORE_DATA);

	if (sync || (buffer[0] != TS_SYNC_BYTE)) {
		for (syncOffset = 0; syncOffset < TS_PACKET_SIZE; syncOffset++) {
			if (buffer[syncOffset] != TS_SYNC_BYTE) {
				continue;
			}
			IF_FAIL_RETURN_VAL(size >= syncOffset + TS_SYNC_COUNT * TS_PACKET_SIZE, UTILS_ERROR_WANT_MORE_DATA);

			int count;
			for (count = 1; count < TS_SYNC_COUNT; count++) {
				if (buffer[syncOffset + count * TS_PACKET_SIZE] != TS_SYNC_BYTE) {
					break;
				}
			}
			if (count == TS_SYNC_COUNT) {
				break; // sync succeed
			}
		}
		IF_FAIL_RETURN_VAL(syncOffset < TS_PACKET_SIZE, UTILS_ERROR_NOT_FOUND);
	}

	return (int)syncOffset;
}

// Abstract payload from the given TS packet
static const uint8_t *getPayloadData(const uint8_t *packet, uint8_t *payloadDataLen)
{
	uint8_t lenPayload = TS_PACKET_SIZE - TS_HEAD_BYTES;
	const uint8_t *payload = packet + TS_HEAD_BYTES;
	if (TS_TRANSPORT_ERROR_INDICATOR(packet)) {
		meddbg("Transport Error\n");
		return nullptr;
	}
	if (TS_ADAPTION_FIELD_CONTROL(packet) == 0) { // 0 - reserved
		medwdbg("adaptationFieldControl 0 is reserved\n");
		return nullptr;
	}
	if (TS_ADAPTION_FIELD_CONTROL(packet) == 2) { // 2 - 183bytes adaptation field only
		return nullptr; // no playload
	}
	if (TS_ADAPTION_FIELD_CONTROL(packet) == 3) { // 3 - 1~182bytes adaptation field + payload
		lenPayload = TS_PACKET_SIZE - TS_HEAD_BYTES - (TS_ADAPT_FIELD_LENGTH_BYTES + TS_ADAPTION_FIELD_LENGTH(packet));
		payload = packet + (TS_PACKET_SIZE - lenPayload);
	} // else 1 - 184bytes playload
	*payloadDataLen = lenPayload;
	return payload;
}

// Unpack TS packet to abstract PSI section
static int PSI_Unpack(const uint8_t *packet, const uint8_t **section, uint16_t *sectlen)
{
	uint8_t  lenPayload = 0;
	const uint8_t *ptrPayload = getPayloadData(packet, &lenPayload);
	if (!ptrPayload) {
		// no payload
		*section = NULL;
		return UTILS_ERROR_NONE;
	}

	if (TS_PAYLOAD_UNIT_START_INDICATOR(packet)) {
		// new section start, and the 1st byte in payload is the pointer field.
		uint8_t u8PointerField = ptrPayload[0];
		if (u8PointerField != 0) {
			// prev section tail and next section head in this packet!
			return UTILS_ERROR_NOT_SUPPORTED;
		}
		// handle new section
		const uint8_t *ptrSection = ptrPayload + 1 + u8PointerField;
		uint16_t lenSection = (SECTION_HEAD_BYTES + SECTION_LENGTH(ptrSection));
		if (lenSection > (lenPayload - 1 - u8PointerField)) {
			// portion of section in this packet
			return UTILS_ERROR_NOT_SUPPORTED;
		}
		// TODO: check CRC32
		// section complete (whole section contained in one TS packet)
		*section = ptrSection;
		*sectlen = lenSection;
		return UTILS_ERROR_NONE;
	}

	// section appending
	return UTILS_ERROR_NOT_SUPPORTED;
}

// Get the information (number of programs) in the PAT.
static int ts_psi_get_pat_info(ts_demux_t *demux, psi_pat_info_t *info)
{
	psi_section_t *patSect = &demux->pat.section;
	info->tsId = (patSect->data[3] << 8) + patSect->data[4];
	// take off 8bytes header and 4bytes CRC, one program consists of 4bytes.
	info->numOfPrgs = (patSect->size - 12) >> 2;
	return UTILS_ERROR_NONE;
}

// Get the program number and PID from PAT at the given program index.
static int ts_psi_get_program(ts_demux_t *demux, uint16_t index, uint16_t *prgNum, uint16_t *pmtPid)
{
	psi_section_t *patSect = &demux->pat.section;
	IF_FAIL_RETURN_VAL(index < ((patSect->size - 12) >> 2), UTILS_ERROR_BAD_PARAMETER);
	// 8bytes header + loop { 2bytes program number + 2bytes pmt pid }
	const uint8_t *prgData = &patSect->data[8 + index * 4];
	*prgNum = (prgData[0] << 8) + prgData[1];
	*pmtPid = ((prgData[2] << 8) + prgData[3]) & 0x1FFF;
	return UTILS_ERROR_NONE;
}

// Get the information (program number, number of streams, etc.) in the PMT.
static int ts_psi_get_pmt_info(ts_demux_t *demux, psi_pmt_info_t *info)
{
	psi_section_t *pmtSect = &demux->pmt.section;
	info->prgNum = (pmtSect->data[3] << 8) + pmtSect->data[4];
	info->numOfStreams = demux->numOfStreams;
	return UTILS_ERROR_NONE;
}

// Get the information (stream type, stream PID, etc.) of the PMT elementary stream.
static int ts_psi_get_stream_info(ts_demux_t *demux, uint16_t index, psi_pmt_stream_info_t *info)
{
	psi_section_t *pmtSect = &demux->pmt.section;
	IF_FAIL_RETURN_VAL(index < demux->numOfStreams, UTILS_ERROR_BAD_PARAMETER);
	uint16_t offset = demux->streamOffset[index];
	info->type = pmtSect->data[offset];
	info->pid = ((pmtSect->data[offset + 1] << 8) + pmtSect->data[offset + 2]) & 0x1FFF;
	return UTILS_ERROR_NONE;
}

// Calculate the number of streams in PMT and map each stream offset
static void map_pmt_streams(ts_demux_t *demux)
{
	psi_section_t *pmtSect = &demux->pmt.section;
	// 12bytes header + program info length + loop { stream info }
	uint16_t offset = 12 + PROGRAM_INFO_LENGTH(pmtSect->data);
	uint16_t next = offset + STREAM_INFO_HEAD_BYTES + ES_INFO_LENGTH(&pmtSect->data[offset]);
	int count = 0;
	// loop until next stream offset exceed the first CRC byte
	while (next <= pmtSect->size - CRC32_BYTES) {
		demux->streamOffset[count++] = offset;
		if (count == PMT_STREAMS_MAXIMUM) {
			// [rarely] there are too many streams, consider to increase PMT_STREAMS_MAXIMUM.
			break;
		}
		offset = next;
		next = offset + STREAM_INFO_HEAD_BYTES + ES_INFO_LENGTH(&pmtSect->data[offset]);
	}
	demux->numOfStreams = count;
}

// Initialize the TS demux
static int ts_demux_init(ts_demux_t *demux, uint8_t *buffer, uint32_t size)
{
	IF_FAIL_RETURN_VAL(demux != NULL, UTILS_ERROR_BAD_PARAMETER);
	IF_FAIL_RETURN_VAL(buffer != NULL && size >= TS_SYNC_COUNT * TS_PACKET_SIZE, UTILS_ERROR_BAD_PARAMETER);

	memset(demux, 0x00, sizeof(ts_demux_t));
	demux->buffer = buffer;
	demux->buflen = size;
	demux->offset = 0;
	demux->numOfStreams = 0;
	demux->pes.pid = 0x1FFF;
	demux->pes.continuity = 0xff;
	return UTILS_ERROR_NONE;
}

// Process demuxing to abstract PSI table (PAT or PMT) with the given PID.
static int ts_psi_process(ts_demux_t *demux, uint16_t pid)
{
	while (1) {
		// locate ts packet
		int ret = findTsPacket(demux->buffer + demux->offset, demux->buflen - demux->offset, false);
		IF_FAIL_RETURN_VAL(ret >= UTILS_ERROR_NONE, ret);
		demux->offset += ret;
		uint8_t *tsPacket = demux->buffer + demux->offset;
		demux->offset += TS_PACKET_SIZE;
		if (TS_PID(tsPacket) != pid) {
			continue; // PID not match
		}
		// unpack ts packet to get section
		uint16_t lenSection = 0;
		const uint8_t *ptrSection = NULL;
		ret = PSI_Unpack(tsPacket, &ptrSection, &lenSection);
		IF_FAIL_RETURN_VAL(ret == UTILS_ERROR_NONE, ret);
		if (ptrSection == NULL) {
			continue; // no section
		}
		if (ptrSection[6] != 0 || ptrSection[7] != 0) {
			// multi sections
			return UTILS_ERROR_NOT_SUPPORTED;
		}
		// table complete (support only one section in the PAT/PMT)
		psi_table_t *table = (pid == PAT_PID ? &demux->pat : &demux->pmt);
		table->section.data = ptrSection;
		table->section.size = lenSection;
		if (pid != PAT_PID) {
			// calculate streams offset once PMT received
			map_pmt_streams(demux);
		}
		return UTILS_ERROR_NONE;
	}
}

// Process demuxing to abstract PES/ES data with the given PID.
static int ts_pes_process(ts_demux_t *demux, uint16_t pid, uint8_t *buffer, size_t size)
{
	pes_packet_t *pes = &demux->pes;
	IF_FAIL_RETURN_VAL((pes->pid != pid), UTILS_ERROR_NOT_SUPPORTED);
	pes->pid = pid;
	pes->continuity = 0xff;

	size_t total = 0;
	while (total < size) {
		// locate ts packet
		int ret = findTsPacket(demux->buffer + demux->offset, demux->buflen - demux->offset, false);
		IF_FAIL_RETURN_VAL(ret >= UTILS_ERROR_NONE, ret);
		demux->offset += ret;
		uint8_t *tsPacket = demux->buffer + demux->offset;
		demux->offset += TS_PACKET_SIZE;
		if (TS_PID(tsPacket) != pid) {
			continue; // PID not match
		}
		// unpack ts packet to get ES data
		uint8_t lenPayload = 0;
		const uint8_t *ptrPayload = getPayloadData(tsPacket, &lenPayload);
		if (!ptrPayload) {
			continue; // no payload
		}
		if (TS_PAYLOAD_UNIT_START_INDICATOR(tsPacket)) {
			// new PES packet start
			if (PES_STREAM_ID(ptrPayload) < 0xc0 || PES_STREAM_ID(ptrPayload) > 0xdf) {
				// not audio stream (stream id = 110xxxxx)
				pes->continuity = 0xff;
				continue;
			}
			pes->present = 0;
			pes->packetLen = PES_PACKET_HEAD_BYTES + PES_PACKET_LENGTH(ptrPayload);
			pes->esOffset = PES_PACKET_HEAD_BYTES + PES_STREAM_HEAD_BYTES + PES_HEADER_DATA_LENGTH(ptrPayload);
		} else if (pes->continuity == 0xff) {
			// waiting for new start indicator
			continue;
		} else if (TS_CONTINUITY_COUNTER(tsPacket) != ((pes->continuity + 1) % TS_CONTINUITY_COUNTER_MOD)) {
			// continuity is broken
			pes->continuity = 0xff;
			total = 0; // drop copied ES data
			continue;
		}
		// copy incoming ES data to output buffer
		uint16_t header = (pes->present < pes->esOffset) ? pes->esOffset - pes->present : 0;
		if (lenPayload > header) {
			// ES data is available
			uint16_t esLen = MINIMUM(pes->present + lenPayload, pes->packetLen) - pes->esOffset;
			esLen = MINIMUM(size - total, esLen);
			memcpy((void *)&buffer[total], (const void *)&ptrPayload[header], esLen);
			total += esLen;
			pes->esOffset += esLen;
		} // else, no ES data in this ts packet
		pes->continuity = TS_CONTINUITY_COUNTER(tsPacket);
		pes->present += lenPayload;
	}
	return total;
}

/**
 * @brief Parsing the mpeg2 transport stream data in buffer, to get audio stream codec type and other informations
 * @param[in] buffer, buffer size, and audio type, channel, sample rate, pcm format adderss to receive.
 * @return true - parsing success. false - parsing fail.
 */
static bool ts_parsing(unsigned char *buffer, unsigned int bufferSize, audio_type_t *audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	int ret;
	ts_demux_t demux;

	// init parser
	ret = ts_demux_init(&demux, buffer, bufferSize);
	if (ret < UTILS_ERROR_NONE) {
		meddbg("Demux init failed, err %d\n", ret);
		return false;
	}

	// parse and get PAT info
	ret = ts_psi_process(&demux, PAT_PID);
	if (ret < UTILS_ERROR_NONE) {
		meddbg("PAT not found, err %d\n", ret);
		return false;
	}
	psi_pat_info_t patInfo = {0};
	ret = ts_psi_get_pat_info(&demux, &patInfo);
	if (ret < UTILS_ERROR_NONE || patInfo.numOfPrgs < 1) {
		meddbg("No program in PAT, err %d\n", ret);
		return false;
	}

	// select the 1st program
	uint16_t prgNum, pmtPid;
	ret = ts_psi_get_program(&demux, 0, &prgNum, &pmtPid);
	if (ret == UTILS_ERROR_NONE && prgNum == NETWORK_PID_PN) {
		// skip network pn-pid, get next program
		ret = ts_psi_get_program(&demux, 1, &prgNum, &pmtPid);
	}
	if (ret < UTILS_ERROR_NONE) {
		meddbg("Select program failed, err %d\n", ret);
		return false;
	}

	// parse and get PMT info
	ret = ts_psi_process(&demux, pmtPid);
	if (ret < UTILS_ERROR_NONE) {
		meddbg("PMT not found, err %d\n", ret);
		return false;
	}
	psi_pmt_info_t pmtInfo = {0};
	ret = ts_psi_get_pmt_info(&demux, &pmtInfo);
	if (ret < UTILS_ERROR_NONE || pmtInfo.numOfStreams < 1) {
		meddbg("No elementary stream in PMT, err %d\n", ret);
		return false;
	}

	// get supported audio stream info
	psi_pmt_stream_info_t streamInfo = {0};
	*audioType = AUDIO_TYPE_INVALID;
	for (int i = 0; i < pmtInfo.numOfStreams; i++) {
		ts_psi_get_stream_info(&demux, i, &streamInfo);
		switch (streamInfo.type) {
			case STREAM_TYPE_AUDIO_AAC:
			case STREAM_TYPE_AUDIO_HE_AAC:
				*audioType = AUDIO_TYPE_AAC;
				break;
			case STREAM_TYPE_AUDIO_MPEG1:
				*audioType = AUDIO_TYPE_MP3;
				break;
			default:
				medvdbg("Unsupported stream type %d\n", streamInfo.type);
				break;
		}
		if (*audioType != AUDIO_TYPE_INVALID) {
			break; // got supported audio stream
		}
	}
	if (*audioType == AUDIO_TYPE_INVALID) {
		meddbg("Audio stream (supported type) not found\n");
		return false;
	}

	// get ES data
	uint8_t audioES[64];
	size_t audioESLen = sizeof(audioES);
	ret = ts_pes_process(&demux, streamInfo.pid, audioES, audioESLen);
	if (ret < UTILS_ERROR_NONE) {
		meddbg("Get ES data failed, PID %u, err %d\n", streamInfo.pid, ret);
		return false;
	}
	audioESLen = (size_t)ret;

	// parse ES data and return result
	if (pcmFormat) {
		// set default value as same as the one in DataSource::DataSource()
		*pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
	}
	return buffer_header_parsing(audioES, (unsigned int)audioESLen, *audioType, channel, sampleRate, pcmFormat);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void toLowerString(std::string &str)
{
	for (char &c : str) {
		if ('A' <= c && c <= 'Z') {
			c += ('a' - 'A');
		}
	}
}

void toUpperString(std::string &str)
{
	for (char &c : str) {
		if ('a' <= c && c <= 'z') {
			c -= ('a' - 'A');
		}
	}
}

audio_type_t getAudioTypeFromPath(std::string datapath)
{
	std::string basename = datapath.substr(datapath.find_last_of("/") + 1);
	std::string extension;

	if (basename.find(".") == std::string::npos) {
		extension = "";
	} else {
		extension = basename.substr(basename.find_last_of(".") + 1);
	}

	toLowerString(extension);

	if (extension.compare("mp3") == 0) {
		medvdbg("audio type : mp3\n");
		return AUDIO_TYPE_MP3;
	} else if ((extension.compare("aac") == 0) || (extension.compare("mp4") == 0)) {
		medvdbg("audio type : aac\n");
		return AUDIO_TYPE_AAC;
	} else if ((extension.compare("opus") == 0)) {
		medvdbg("audio type : opus\n");
		return AUDIO_TYPE_OPUS;
	} else if ((extension.compare("flac") == 0)) {
		medvdbg("audio type : flac\n");
		return AUDIO_TYPE_FLAC;
	} else if ((extension.compare("") == 0) || (extension.compare("pcm") == 0) || (extension.compare("raw") == 0)) {
		medvdbg("audio type : pcm\n");
		return AUDIO_TYPE_PCM;
	} else if (extension.compare(AUDIO_EXT_TYPE_WAV) == 0) {
		medvdbg("audio type : wav\n");
		return AUDIO_TYPE_WAVE;
	} else if (extension.compare("ts") == 0) {
		medvdbg("audio type : ts\n");
		return AUDIO_TYPE_MP2T;
	} else {
		medvdbg("audio type : unknown\n");
		return AUDIO_TYPE_INVALID;
	}
}

audio_type_t getAudioTypeFromMimeType(std::string &mimeType)
{
	audio_type_t audioType;

	if (mimeType.empty()) {
		medwdbg("empty mime type!\n");
		audioType = AUDIO_TYPE_UNKNOWN;
	} else if ((mimeType.find(AAC_MIME_TYPE) != std::string::npos) ||
			(mimeType.find(AACP_MIME_TYPE) != std::string::npos)) {
		audioType = AUDIO_TYPE_AAC;
	} else if (mimeType.find(MPEG_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_MP3;
	} else if (mimeType.find(MP4_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_AAC;
	} else if (mimeType.find(OPUS_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_OPUS;
	} else if (mimeType.find(MP2T_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_MP2T;
	} else {
		meddbg("Unsupported mime type: %s\n", mimeType.c_str());
		audioType = AUDIO_TYPE_UNKNOWN;
	}

	return audioType;
}

bool mp3_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate)
{
/**
*mp3_header is AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
*A - Frame sync
*B - MPEG Audio version
*...
*F - Sampling rate frequency
*.
*I - Channel Mode
*...
*/
	unsigned char bit;
	unsigned int mpegVersion;

	bit = header[1];
	bit >>= 3;
	bit &= (unsigned char)0x03;
	/* we need B information so Shift header[1] three times to the right, and & 0x03 */
	switch (bit) {
	case 0:
		mpegVersion = 2;
		break;
	case 2:
		mpegVersion = 1;
		break;
	case 3:
		mpegVersion = 0;
		break;
	default:
		medvdbg("Not Supported Format mpeg version : %u\n", mpegVersion);
		return false;
	}

	bit = header[2];
	bit >>= 2;
	bit &= 0x03;
	/* we need F information so Shift header[1] two times to the right, and & 0x03 */
	switch (bit) {
	case 0:
		*sampleRate = AUDIO_SAMPLE_RATE_44100;
		break;
	case 1:
		*sampleRate = AUDIO_SAMPLE_RATE_48000;
		break;
	case 2:
		*sampleRate = AUDIO_SAMPLE_RATE_32000;
		break;
	default:
		medvdbg("Not Supported Format sample rate : %u\n", sampleRate);
		return false;
	}

	*sampleRate >>= mpegVersion;

	bit = header[3];
	bit >>= 6;
	/* we need I information so Shift header[3] six times to the right */
	if (bit <= 2) {
		*channel = 2;
	} else {
		*channel = 1;
	}
	return true;
}

bool aac_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate)
{
/**
*aac_header is AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP
*A - syncword 0xFFF, all bits must be 1
*....
*F - Sampling rate frequency
*..
*H - Channel Mode
*......
*/
	unsigned char bit;
	bit = header[2];
	bit >>= 2;
	bit &= 0x0F;
	/* we need F information so Shift header[2] two times to the right, and & 0x0F */
	switch (bit) {
	case 0:
		*sampleRate = AUDIO_SAMPLE_RATE_96000;
		break;
	case 1:
		*sampleRate = AUDIO_SAMPLE_RATE_88200;
		break;
	case 2:
		*sampleRate = AUDIO_SAMPLE_RATE_64000;
		break;
	case 3:
		*sampleRate = AUDIO_SAMPLE_RATE_48000;
		break;
	case 4:
		*sampleRate = AUDIO_SAMPLE_RATE_44100;
		break;
	case 5:
		*sampleRate = AUDIO_SAMPLE_RATE_32000;
		break;
	case 6:
		*sampleRate = AUDIO_SAMPLE_RATE_24000;
		break;
	case 7:
		*sampleRate = AUDIO_SAMPLE_RATE_22050;
		break;
	case 8:
		*sampleRate = AUDIO_SAMPLE_RATE_16000;
		break;
	case 9:
		*sampleRate = AUDIO_SAMPLE_RATE_12000;
		break;
	case 10:
		*sampleRate = AUDIO_SAMPLE_RATE_11025;
		break;
	case 11:
		*sampleRate = AUDIO_SAMPLE_RATE_8000;
		break;
	case 12:
		*sampleRate = AUDIO_SAMPLE_RATE_7350;
		break;
	default:
		meddbg("Not Supported Format sample rate : %u\n", sampleRate);
		return false;
	}
	bit = header[2];
	bit <<= 2;
	bit |= (header[3] >> 6);
	bit &= 0x07;
	/* we need H information so Shift header[3] six times to the right, and & 0x07 */
	if (bit <= 0 || bit >= 8) {
		meddbg("Invalid value bit : %d\n", bit);
		return false;
	}

	if (bit == 7) {
		*channel = 8;
	} else {
		*channel = (unsigned int)bit;
	}
	return true;
}

bool wave_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
/**
*wave header is
*Chunk ID    (4byte) / .... (4byte) /            .... (4byte)                /
*.....       (4byte) / .... (4byte) / .. (2byte)   / NumChannels     (2byte) /
*sample Rate (4byte) / .... (4byte) / .. (2byte)   / Bits Per sample (2byte) /
*.....       (4byte) / .... (4byte)
*/
	unsigned short bitPerSample;
	*channel = header[23];
	*channel <<= 8;
	*channel |= header[22];

	*sampleRate = header[27];
	*sampleRate <<= 8;
	*sampleRate |= header[26];
	*sampleRate <<= 8;
	*sampleRate |= header[25];
	*sampleRate <<= 8;
	*sampleRate |= header[24];

	bitPerSample = header[35];
	bitPerSample <<= 8;
	bitPerSample |= header[34];
	/* wave header is Little Endian, so read from right byte */
	switch (bitPerSample) {
	case 8:
		*pcmFormat = AUDIO_FORMAT_TYPE_S8;
		break;
	case 16:
		*pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
		break;
	case 32:
		*pcmFormat = AUDIO_FORMAT_TYPE_S32_LE;
		break;
	default:
		meddbg("Not Supported Format bit/sample : %u\n", bitPerSample);
		return false;
	}
	return true;
}

bool file_header_parsing(FILE *fp, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	unsigned char *header = NULL;
	unsigned char tag[2];
	bool isHeader;
	int ret;

	switch (audioType) {
	case AUDIO_TYPE_MP3:
		isHeader = false;
		while (fread(tag, sizeof(unsigned char), 2, fp) == 2) {
			/* 12 bits for MP3 Sync Word(the beginning of the frame) */
			if ((tag[0] == 0xFF) && ((tag[1] & 0xF0) == 0xF0)) {
				isHeader = true;
				break;
			} else {
				/* If isn't the header information, go back 1byte and then check 2bytes again */
				ret = fseek(fp, -1, SEEK_CUR);
				if (ret != OK) {
					meddbg("file seek failed errno : %d\n", errno);
					return false;
				}
			}
		}
		if (isHeader) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (MP3_HEADER_LENGTH + 1));

			if (header == NULL) {
				meddbg("malloc failed error\n");
				return false;
			}

			ret = fseek(fp, -2, SEEK_CUR);
			if (ret != OK) {
				meddbg("file seek failed errno : %d\n", errno);
				free(header);
				return false;
			}

			ret = fread(header, sizeof(unsigned char), MP3_HEADER_LENGTH, fp);
			if (ret != MP3_HEADER_LENGTH) {
				meddbg("read failed ret : %d\n", ret);
				free(header);
				return false;
			}

			if (!mp3_header_parsing(header, channel, sampleRate)) {
				meddbg("Header parsing failed\n");
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_AAC:
		isHeader = false;
		while (fread(tag, sizeof(unsigned char), 1, fp) > 0) {
			if (tag[0] == 0xFF) {
				isHeader = true;
				break;
			}
		}
		if (isHeader) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (AAC_HEADER_LENGTH + 1));
			if (header == NULL) {
				meddbg("malloc failed error\n");
				return false;
			}

			if (fseek(fp, -1, SEEK_CUR) != 0) {
				meddbg("file seek failed error\n");
				free(header);
				return false;
			}

			if ((fread(header, sizeof(unsigned char), AAC_HEADER_LENGTH, fp) != AAC_HEADER_LENGTH) || !aac_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			meddbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_WAVE:
		header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
		if (header == NULL) {
			meddbg("malloc failed error\n");
			return false;
		}

		if ((fread(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp) != WAVE_HEADER_LENGTH) || !wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
			free(header);
			return false;
		}
		break;
	case AUDIO_TYPE_MP2T: {
		// has container, demux and parse stream data to get audio type
		size_t bufferSize = CONFIG_DATASOURCE_PREPARSE_BUFFER_SIZE;
		header = (unsigned char *)malloc(sizeof(unsigned char) * bufferSize);
		if (!header) {
			meddbg("run out of memory! size %u\n", bufferSize);
			return false;
		}

		size_t readSize = fread(header, sizeof(unsigned char), bufferSize, fp);
		if (readSize != bufferSize) {
			free(header);
			meddbg("can not read enough data for preparsing! read:%u\n", readSize);
			return false;
		}

		if (!ts_parsing(header, readSize, &audioType, channel, sampleRate, pcmFormat)) {
			free(header);
			meddbg("ts_parsing failed, can not get audio information!\n");
			return false;
		}
	} break;
	default:
		medvdbg("does not support header parsing\n");
		return false;
	}

	if (header != NULL) {
		free(header);
	}

	if (fseek(fp, 0, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		return false;
	}
	return true;
}

bool buffer_header_parsing(unsigned char *buffer, unsigned int bufferSize, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	unsigned int headPoint;
	unsigned char *header = NULL;
	bool isHeader;
	switch (audioType) {
	case AUDIO_TYPE_MP3:
		if (MP3_HEADER_LENGTH > bufferSize) {
			medvdbg("no header\n");
			return false;
		}
		isHeader = false;
		headPoint = 0;
		while (headPoint < bufferSize) {
			/* 12 bits for MP3 Sync Word(the beginning of the frame) */
			if ((buffer[headPoint]) == 0xFF && ((buffer[headPoint + 1] & 0xF0) == 0xF0)) {
				isHeader = true;
				break;
			}
			headPoint++;
		}
		if (isHeader && MP3_HEADER_LENGTH <= bufferSize - headPoint) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (MP3_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer + headPoint, MP3_HEADER_LENGTH);
			if (!mp3_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_AAC:
		if (AAC_HEADER_LENGTH > bufferSize) {
			medvdbg("no header\n");
			return false;
		}
		isHeader = false;
		headPoint = 0;
		while (headPoint < bufferSize) {
			if (buffer[headPoint] == 0xFF) {
				isHeader = true;
				break;
			}
			headPoint++;
		}
		if (isHeader && AAC_HEADER_LENGTH <= bufferSize - headPoint) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (AAC_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer + headPoint, AAC_HEADER_LENGTH);
			if (!aac_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_WAVE:
		if (WAVE_HEADER_LENGTH <= bufferSize) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer, WAVE_HEADER_LENGTH);
			if (!wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_MP2T:
		if (!ts_parsing(buffer, bufferSize, &audioType, channel, sampleRate, pcmFormat)) {
			meddbg("ts_parsing failed, can not get audio information!\n");
			return false;
		}
		break;
	default:
		medvdbg("does not support header parsing\n");
		return false;
	}

	if (header != NULL) {
		free(header);
	}
	return true;
}

struct wav_header_s {
	char headerRiff[4]; //"RIFF"
	uint32_t riffSize;
	char headerWave[4]; //"wave"
	char headerFmt[4]; //"fmt "
	uint32_t fmtSize; //16 for pcm
	uint16_t format; //1 for pcm
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitPerSample;
	char headerData[4]; //"data"
	uint32_t dataSize;
};

bool createWavHeader(FILE *fp)
{
	struct wav_header_s *header;
	header = (struct wav_header_s *)malloc(sizeof(struct wav_header_s));
	if (!header) {
		meddbg("fail to malloc buffer\n");
		return false;
	}

	memset(header, 0xff, WAVE_HEADER_LENGTH);
	int ret;
	ret = fwrite(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp);
	if (ret != WAVE_HEADER_LENGTH) {
		meddbg("file write failed error %d\n", errno);
		free(header);
		return false;
	}
	if (fseek(fp, WAVE_HEADER_LENGTH, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		free(header);
		return false;
	}

	free(header);
	return true;
}

bool writeWavHeader(FILE *fp, unsigned int channel, unsigned int sampleRate, audio_format_type_t pcmFormat, unsigned int fileSize)
{
/**
*wave header is
*Chunk ID 'RIFF' (4byte) / Chunk Size (4byte) / Fomat 'WAVE' (4byte) /
*Chunk ID 'fmt ' (4byte) / Chunk Size (4byte) / Audio Format (2byte) / NumChannels     (2byte) /
*sample Rate     (4byte) / Byte Rate  (4byte) / Block Align  (2byte) / Bits Per sample (2byte) /
*Chunk ID 'data' (4byte) / Chunk Size (4byte)
*/
	if (fseek(fp, 0, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		return false;
	}

	uint32_t byteRate = 0;
	uint16_t bitPerSample = 0;
	uint16_t blockAlign = 0;

	switch (pcmFormat) {
	case AUDIO_FORMAT_TYPE_S16_LE:
		bitPerSample = 16;
		break;
	case AUDIO_FORMAT_TYPE_S32_LE:
		bitPerSample = 32;
		break;
	default:
		meddbg("does not support audio format.\n");
		return false;
	}

	blockAlign = channel * (bitPerSample >> 3);
	byteRate = sampleRate * blockAlign;

	struct wav_header_s *header;
	header = (struct wav_header_s *)malloc(sizeof(struct wav_header_s));
	if (header == NULL) {
		meddbg("malloc failed error\n");
		return false;
	}

	memcpy(header->headerRiff, "RIFF", 4);

	header->riffSize = fileSize - 8;

	memcpy(header->headerWave, "WAVE", 4);
	memcpy(header->headerFmt, "fmt ", 4);

	header->fmtSize = 16;
	header->format = 1;
	header->channels = channel;
	header->sampleRate = sampleRate;
	header->byteRate = byteRate;
	header->blockAlign = blockAlign;
	header->bitPerSample = bitPerSample;

	memcpy(header->headerData, "data", 4);

	header->dataSize = fileSize - WAVE_HEADER_LENGTH;

	int ret = 0;
	ret = fwrite(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp);

	if (ret != WAVE_HEADER_LENGTH) {
		meddbg("file write failed error %d\n", errno);
		free(header);
		return false;
	}

	free(header);
	return true;
}

unsigned int splitChannel(unsigned int layout, const signed short *stream, unsigned int frames, unsigned int channels, ...)
{
	uint32_t ret = 0;
	if (stream == NULL) {
		meddbg("invalid audio stream!\n");
		return ret;
	}

	uint32_t spf = POPCOUNT(layout); // samples per frame
	uint32_t mask, i, j;
	const int16_t *sdata;
	int16_t *buffer;

	va_list ap;
	va_start(ap, channels);

	for (i = 0; i < channels; i++) {
		mask = va_arg(ap, uint32_t);
		buffer = va_arg(ap, int16_t *);

		// Check params validation
		if (POPCOUNT(mask) != 1) {
			meddbg("specified channel must be a single channel! i:%u, mask:0x%x\n", i, mask);
			continue;
		}

		if ((layout & mask) == 0) {
			meddbg("specified channel does not exist! layout: 0x%x, i:%u, mask:0x%x\n", layout, i, mask);
			continue;
		}

		if (buffer == NULL) {
			meddbg("invalid output buffer! i:%u, mask:0x%x\n", i, mask);
			continue;
		}

		sdata = stream + POPCOUNT(layout & (mask - 1));
		for (j = 0; j < frames; j++) {
			*buffer++ = *sdata;
			sdata += spf;
		}

		ret |= mask;
	}

	va_end(ap);
	return ret;
}

float getSignalToNoiseRatio(const short *buffer, size_t size, int windows, int *index, ...)
{
	const int NOISE_MIN = 30;

	int i = 0;
	int frame_sample = 0;

	float sum = 0.0f;
	float noise = 0.0f;
	float speechAverage = 0.0f;
	float energyRatio = 0.0f;

	va_list ap;
	va_start(ap, index);

	float *speechSum = va_arg(ap, float *);
	float *noiseSum = va_arg(ap, float *);
	float *noiseAverage = va_arg(ap, float *);

	if (*index == 0) {
		*noiseAverage = 32767;
		*speechSum = 0;
		*noiseSum = 0;
		(*index)++;
	}

	sum = 0;
	for (i = 0; i < size; i++) {
		sum += buffer[i] > 0 ? buffer[i] : -buffer[i];
	}

	*noiseSum += sum;

	if (*index % windows == 0) {
		if ((noise = *noiseSum / (size * windows)) < NOISE_MIN) {
			noise = NOISE_MIN;
		}

		if (noise < *noiseAverage) {
			*noiseAverage = noise;
		}
		*noiseSum = 0;
	}

	if (*index > windows) {
		*speechSum += sum;
		frame_sample = size * (*index - windows);
	}

	medvdbg("frame_sample : %d\n", frame_sample);
	if (frame_sample != 0) {
		speechAverage = *speechSum / frame_sample;
		energyRatio = speechAverage / *noiseAverage;
	}

	(*index)++;

	medvdbg("SNR = %f, %f, %f\n", energyRatio, speechAverage, *noiseAverage);

	va_end(ap);
	return energyRatio;
}

bool isMpeg2Ts(const unsigned char *buffer, size_t size)
{
	if (!buffer) {
		meddbg("Invalid param, buffer is null!\n");
		return false;
	}

	int ret = findTsPacket(buffer, size, true);
	if (ret < UTILS_ERROR_NONE) {
		meddbg("Not found TS packets, err %d\n", ret);
		return false;
	}

	return true;
}

} // namespace util
} // namespace media
