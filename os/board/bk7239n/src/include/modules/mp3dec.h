/* ***** BEGIN LICENSE BLOCK *****
 * Version: RCSL 1.0/RPSL 1.0
 *
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc. All Rights Reserved.
 *
 * The contents of this file, and the files included with this file, are
 * subject to the current version of the RealNetworks Public Source License
 * Version 1.0 (the "RPSL") available at
 * http://www.helixcommunity.org/content/rpsl unless you have licensed
 * the file under the RealNetworks Community Source License Version 1.0
 * (the "RCSL") available at http://www.helixcommunity.org/content/rcsl,
 * in which case the RCSL will apply. You may also obtain the license terms
 * directly from RealNetworks.  You may not use this file except in
 * compliance with the RPSL or, if you have a valid RCSL with RealNetworks
 * applicable to this file, the RCSL.  Please see the applicable RPSL or
 * RCSL for the rights, obligations and limitations governing use of the
 * contents of the file.
 *
 * This file is part of the Helix DNA Technology. RealNetworks is the
 * developer of the Original Code and owns the copyrights in the portions
 * it created.
 *
 * This file, and the files included with this file, is distributed and made
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *
 * Technology Compatibility Kit Test Suite(s) Location:
 *    http://www.helixcommunity.org/content/tck
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/**************************************************************************************
 * Fixed-point MP3 decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * June 2003
 *
 * mp3dec.h - public C API for MP3 decoder
 **************************************************************************************/

#ifndef _MP3DEC_H
#define _MP3DEC_H


#if defined(_WIN32) && !defined(_WIN32_WCE)
#
#elif defined(_WIN32) && defined(_WIN32_WCE) && defined(ARM)
#
#elif defined(_WIN32) && defined(WINCE_EMULATOR)
#
#elif defined(ARM_ADS)
#
#elif defined(_SYMBIAN) && defined(__WINS__)	/* Symbian emulator for Ix86 */
#
#elif defined(__GNUC__) && defined(ARM)
#
#elif defined(__GNUC__) && defined(MIPS)
#
#elif defined(__GNUC__) && defined(__i386__)
#
#elif defined(_OPENWAVE_SIMULATOR) || defined(_OPENWAVE_ARMULATOR)
#
#elif defined(__GNUC__)
#else
#error No platform defined. See valid options in mp3dec.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/* determining MAINBUF_SIZE:
 *   max mainDataBegin = (2^9 - 1) bytes (since 9-bit offset) = 511
 *   max nSlots (concatenated with mainDataBegin bytes from before) = 1440 - 9 - 4 + 1 = 1428
 *   511 + 1428 = 1939, round up to 1940 (4-byte align)
 */
#define MAINBUF_SIZE	1940

#define MAX_NGRAN		2		/* max granules */
#define MAX_NCHAN		2		/* max channels */
#define MAX_NSAMP		576		/* max samples per channel, per granule */

/* map to 0,1,2 to make table indexing easier */
typedef enum {
	MPEG1 =  0,
	MPEG2 =  1,
	MPEG25 = 2
} MPEGVersion;

typedef void *HMP3Decoder;

typedef enum {
	ERR_MP3_NONE =                  0,
	ERR_MP3_INDATA_UNDERFLOW =     -1,
	ERR_MP3_MAINDATA_UNDERFLOW =   -2,
	ERR_MP3_FREE_BITRATE_SYNC =    -3,
	ERR_MP3_OUT_OF_MEMORY =	       -4,
	ERR_MP3_NULL_POINTER =         -5,
	ERR_MP3_INVALID_FRAMEHEADER =  -6,
	ERR_MP3_INVALID_SIDEINFO =     -7,
	ERR_MP3_INVALID_SCALEFACT =    -8,
	ERR_MP3_INVALID_HUFFCODES =    -9,
	ERR_MP3_INVALID_DEQUANTIZE =   -10,
	ERR_MP3_INVALID_IMDCT =        -11,
	ERR_MP3_INVALID_SUBBAND =      -12,

	ERR_UNKNOWN =                  -9999
} err_mp3_t;

typedef struct _MP3FrameInfo {
	int bitrate;
	int nChans;
	int samprate;
	int bitsPerSample;
	int outputSamps;
	int layer;
	int version;
} MP3FrameInfo;

/* public API */
/**
 * @brief     Init the decoder of MP3
 *
 * This API init the mp3 decoder module:
 *  - allocate memory for platform-specific data
 *  - clear all the user-accessible fields
 *
 * @param None
 *
 * @return None
 *
 */
HMP3Decoder MP3InitDecoder(void);

/**
 * @brief     Free the decoder of MP3
 *
 * This API free the mp3 decoder module:
 *  - free platform-specific data allocated by InitMP3Decoder
 *  - zero out the contents of MP3DecInfo struct
 *
 * @param HMP3Decoder valid MP3 decoder instance pointer
 *
 * @return None
 *
 */
void MP3FreeDecoder(HMP3Decoder hMP3Decoder);

/**
 * @brief     Decode one frame of MP3 data
 *
 * @param hMP3Decoder valid MP3 decoder instance pointer
 * @param inbuf double pointer to buffer of MP3 data (containing headers + mainData)
 * @param bytesLeft number of valid bytes remaining in inbuf
 * @param outbuf pointer to outbuf, big enough to hold one frame of decoded PCM samples
 * @param useSize flag indicating whether MP3 data is normal MPEG format (useSize = 0) or reformatted as "self-contained" frames (useSize = 1)
 *
 * @return error code, defined in mp3dec.h (0 means no error, < 0 means error)
 *
 */
int MP3Decode(HMP3Decoder hMP3Decoder, unsigned char **inbuf, int *bytesLeft, short *outbuf, int useSize);

/**
 * @brief     Get info about last MP3 frame decoded
 *
 * This API get the info about last mp3 decoded:
 *  - number of sampled decoded, sample rate, bitrate, etc.
 *
 * @param hMP3Decoder valid MP3 decoder instance pointer
 * @param mp3FrameInfo pointer to MP3FrameInfo struct
 *
 * @return None
 *
 */
void MP3GetLastFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo);

/**
 * @brief     Parse MP3 frame header
 *
 * @param hMP3Decoder valid MP3 decoder instance pointer
 * @param mp3FrameInfo pointer to MP3FrameInfo struct
 * @param buf pointer to buffer containing valid MP3 frame header
 *
 * @return error code, defined in mp3dec.h (0 means no error, < 0 means error)
 *
 */
int MP3GetNextFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo, unsigned char *buf);

/**
 * @brief     locate the next byte-alinged sync word in the raw mp3 stream
 *
 * @param buf buffer to search for sync word
 * @param nBytes max number of bytes to search in buffer
 *
 * @return
 *    - offset to first sync word (bytes from start of buf)
 *    - -1 if sync not found after searching nBytes
 */
int MP3FindSyncWord(unsigned char *buf, int nBytes);


/**
 * @brief     indicate decoder which function to alloc free and memset
 *            This function must call before MP3InitDecoder or after MP3FreeDecoder.
 *
 * @param alloc alloc function
 * @param free free function
 * @param mset memset function
 *
 * @return
 *    - 0: success
 *    - other: err
 */
int MP3SetBuffMethod(void *(*alloc)(size_t size), void (*free)(void *buff), void *(*mset)(void *s, unsigned char c, size_t n));

/**
 * @brief     indicate decoder which function to alloc free and memset
 *            This function must call before MP3InitDecoder or after MP3FreeDecoder.
 *            This function will be used always 4 bytes access.
 *            This function is high priority than MP3SetBuffMethod
 *
 * @param alloc alloc function
 * @param free free function
 * @param mset memset function
 *
 * @return
 *    - 0: success
 *    - other: err
 */
int MP3SetBuffMethodAlwaysFourAlignedAccess(void *(*alloc)(size_t size), void (*free)(void *buff), void *(*mset)(void *s, unsigned char c, size_t n));



#ifdef __cplusplus
}
#endif

#endif	/* _MP3DEC_H */
