// Copyright 2023-2024 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef _AACDEC_H
#define _AACDEC_H

#if defined(_WIN32) && !defined(_WIN32_WCE)
#
#elif defined(_WIN32) && defined(_WIN32_WCE) && defined(ARM)
#
#elif defined(_WIN32) && defined(WINCE_EMULATOR)
#
#elif defined (__arm) && defined (__ARMCC_VERSION)
#
#elif defined(_SYMBIAN) && defined(__WINS__)
#
#elif defined(__GNUC__) && defined(__arm__)
#
#elif defined(__GNUC__) && defined(__i386__)
#
#elif defined(__GNUC__) && defined(__amd64__)
#
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__POWERPC__))
#
#elif defined(_OPENWAVE_SIMULATOR) || defined(_OPENWAVE_ARMULATOR)
#
#elif defined(_SOLARIS) && !defined(__GNUC__)
#
#elif defined(ARDUINO)
#
#else
//#error No platform defined. See valid options in aacdec.h
#endif

#ifndef USE_DEFAULT_STDLIB
#define USE_DEFAULT_STDLIB
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about jpeg dec API header
 *
 */

/* according to spec (13818-7 section 8.2.2, 14496-3 section 4.5.3)
 * max size of input buffer = 
 *    6144 bits =  768 bytes per SCE or CCE-I
 *   12288 bits = 1536 bytes per CPE
 *       0 bits =    0 bytes per CCE-D (uses bits from the SCE/CPE/CCE-I it is coupled to)
 */
#ifndef AAC_MAX_NCHANS				/* if max channels isn't set in makefile, */
#define AAC_MAX_NCHANS		2		/* set to default max number of channels  */
#endif
#define AAC_MAX_NSAMPS		1024
#define AAC_MAINBUF_SIZE	(768 * AAC_MAX_NCHANS)

#define AAC_NUM_PROFILES	3
#define AAC_PROFILE_MP		0
#define AAC_PROFILE_LC		1
#define AAC_PROFILE_SSR		2

/* define these to enable decoder features */
#if defined(HELIX_FEATURE_AUDIO_CODEC_AAC_SBR)
#define AAC_ENABLE_SBR
#endif //  HELIX_FEATURE_AUDIO_CODEC_AAC_SBR.
#define AAC_ENABLE_MPEG4

enum {
	ERR_AAC_NONE                          =   0,
	ERR_AAC_INDATA_UNDERFLOW              =  -1,
	ERR_AAC_NULL_POINTER                  =  -2,
	ERR_AAC_INVALID_ADTS_HEADER           =  -3,
	ERR_AAC_INVALID_ADIF_HEADER           =  -4,
	ERR_AAC_INVALID_FRAME                 =  -5,
	ERR_AAC_MPEG4_UNSUPPORTED             =  -6,
	ERR_AAC_CHANNEL_MAP                   =  -7,
	ERR_AAC_SYNTAX_ELEMENT                =  -8,

	ERR_AAC_DEQUANT                       =  -9,
	ERR_AAC_STEREO_PROCESS                = -10,
	ERR_AAC_PNS                           = -11,
	ERR_AAC_SHORT_BLOCK_DEINT             = -12,
	ERR_AAC_TNS                           = -13,
	ERR_AAC_IMDCT                         = -14,
	ERR_AAC_NCHANS_TOO_HIGH               = -15,

	ERR_AAC_SBR_INIT                      = -16,
	ERR_AAC_SBR_BITSTREAM                 = -17,
	ERR_AAC_SBR_DATA                      = -18,
	ERR_AAC_SBR_PCM_FORMAT                = -19,
	ERR_AAC_SBR_NCHANS_TOO_HIGH           = -20,
	ERR_AAC_SBR_SINGLERATE_UNSUPPORTED    = -21,

	ERR_AAC_RAWBLOCK_PARAMS               = -22,

	ERR_AAC_UNKNOWN						= -9999
};

typedef struct _AACFrameInfo {
	int bitRate;
	int nChans;
	int sampRateCore;
	int sampRateOut;
	int bitsPerSample;
	int outputSamps;
	int profile;
	int tnsUsed;
	int pnsUsed;
} AACFrameInfo;

typedef void *HAACDecoder;

/**
 * @brief HW JPEG DEC API
 * @{
 */

/**
 * @brief     This function is called to init aac decoder.
 *            - Allocate memory for platform-specific data
 *            - Clear all the user-accessible fields.
 *            - Initialize SBR decoder if enabled.
 *
 * @param     none
 *
 * @return
 *    - Not NULL: handle to AAC decoder instance
 *    - NULL: err
 */
HAACDecoder AACInitDecoder(void);

/**
 * @brief     This function is called to free aac decoder.
 *            - Free platform-specific data allocated by AACInitDecoder
 *            - Free SBR decoder if enabled
 *
 * @param[in]    hAACDecoder: valid AAC decoder instance pointer (HAACDecoder)
 *
 * @return       none
 */
void AACFreeDecoder(HAACDecoder hAACDecoder);

/**
 * @brief     This function is called to decode aac frame data.
 *
 * @param[in]     hAACDecoder: valid AAC decoder instance pointer (HAACDecoder)
 * @param[in]     inbuf: double pointer to buffer of AAC data
 * @param[in]     bytesLeft: pointer to number of valid bytes remaining in inbuf
 * @param[in]     outbuf: pointer to outbuf, big enough to hold one frame of decoded PCM samples
 *
 * @param[out]    inbuf: updated inbuf pointer
 * @param[out]    bytesLeft: updated bytesLeft
 * @param[out]    outbuf: PCM data in outbuf, interleaved LRLRLR... if stereo number of output
 *                        samples = 1024 per channel.
 *
 * @return
 *    - 0: successful
 *    - others: error code (< 0) if error
 */
int AACDecode(HAACDecoder hAACDecoder, unsigned char **inbuf, int *bytesLeft, short *outbuf);

/**
 * @brief     This function is called to find aac sync word.
 *            - Locate the next byte-alinged sync word in the raw AAC stream
 *
 * @param[in]     buf: buffer to search for sync word
 * @param[in]     nBytes: max number of bytes to search in buffer
 *
 * @return
 *    - >=0: offset to first sync word (bytes from start of buf)
 *    - -1: sync not found
 */
int AACFindSyncWord(unsigned char *buf, int nBytes);

/**
 * @brief     This function is called to get last frame information.
 *
 * @param[in]     hAACDecoder: valid AAC decoder instance pointer (HAACDecoder)
 * @param[in]     aacFrameInfo: pointer to AACFrameInfo struct
 *
 * @param[out]    aacFrameInfo: filled-in AACFrameInfo struct
 *
 * @return        none
 */
void AACGetLastFrameInfo(HAACDecoder hAACDecoder, AACFrameInfo *aacFrameInfo);

/**
 * @brief     This function is called to set aac raw block parameters.
 *
 * @param[in]     hAACDecoder: valid AAC decoder instance pointer (HAACDecoder)
 * @param[in]     copyLast: flag indicating source of parameters
 * @param[in]     aacFrameInfo: AACFrameInfo struct, with the members nChans, sampRate, and profile
 *                              optionally filled-in.
 *
 * @param[out]    inbuf: updated inbuf pointer
 * @param[out]    bytesLeft: updated bytesLeft
 * @param[out]    outbuf: PCM data in outbuf, interleaved LRLRLR... if stereo number of output
 *                        samples = 1024 per channel.
 *
 * @return
 *    - 0: successful
 *    - others: error code (< 0) if error
 *
 * Notes:       if copyLast == 1, then the codec sets up its internal state (for 
 *                decoding raw blocks) based on previously-decoded ADTS header info
 *              if copyLast == 0, then the codec uses the values passed in
 *                aacFrameInfo to configure its internal state (useful when the
 *                source is MP4 format, for example)
 */
int AACSetRawBlockParams(HAACDecoder hAACDecoder, int copyLast, AACFrameInfo *aacFrameInfo);

/**
 * @brief     This function is called to flush internal codec state.
 *
 * @param[in]     hAACDecoder: valid AAC decoder instance pointer (HAACDecoder)
 *
 * @return
 *    - 0: successful
 *    - others: error code (< 0) if error
 */
int AACFlushCodec(HAACDecoder hAACDecoder);

#ifdef BK_CONFIG_AAC_GENERATE_TRIGTABS_FLOAT
/**
 * @brief     This function is called to generate AAC decoder tables using floating-point math library.
 *
 * @param     none
 *
 * @return
 *    - 0: successful
 *    - others: error code (< 0) if error
 */
int AACInitTrigtabsFloat(void);

/**
 * @brief     This function is called to free any memory allocated by AACInitTrigtabsFloat().
 *
 * @param     none
 *
 * @return
 *    - 0: successful
 *    - others: error code (< 0) if error
 */
void AACFreeTrigtabsFloat(void);
#endif

#ifdef __cplusplus
}
#endif

#endif	/* _AACDEC_H */
