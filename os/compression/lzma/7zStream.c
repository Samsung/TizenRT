/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*****************************************************************************
 *
 * LZMA SDK is written and placed in the public domain by Igor Pavlov.
 *
 * Some code in LZMA SDK is based on public domain code from another developers:
 *   1) PPMd var.H (2001): Dmitry Shkarin
 *   2) SHA-256: Wei Dai (Crypto++ library)
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute the
 * original LZMA SDK code, either in source code form or as a compiled binary, for
 * any purpose, commercial or non-commercial, and by any means.
 *
 * LZMA SDK code is compatible with open source licenses, for example, you can
 * include it to GNU GPL or GNU LGPL code.
 *
 *****************************************************************************/

/* 7zStream.c -- 7z Stream functions
2017-04-03 : Igor Pavlov : Public domain */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "Precomp.h"
#include <string.h>
#include "7zTypes.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define GET_LookToRead2 CLookToRead2 *p = CONTAINER_FROM_VTBL(pp, CLookToRead2, vt);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

SRes SeqInStream_Read2(const ISeqInStream *stream, void *buf, size_t size, SRes errorType)
{
	while (size != 0) {
		size_t processed = size;
		RINOK(ISeqInStream_Read(stream, buf, &processed));
		if (processed == 0) {
			return errorType;
		}
		buf = (void *)((Byte *) buf + processed);
		size -= processed;
	}
	return SZ_OK;
}

SRes SeqInStream_Read(const ISeqInStream *stream, void *buf, size_t size)
{
	return SeqInStream_Read2(stream, buf, size, SZ_ERROR_INPUT_EOF);
}

SRes SeqInStream_ReadByte(const ISeqInStream *stream, Byte *buf)
{
	size_t processed = 1;
	RINOK(ISeqInStream_Read(stream, buf, &processed));
	return (processed == 1) ? SZ_OK : SZ_ERROR_INPUT_EOF;
}

SRes LookInStream_SeekTo(const ILookInStream *stream, UInt64 offset)
{
	Int64 t = offset;
	return ILookInStream_Seek(stream, &t, SZ_SEEK_SET);
}

SRes LookInStream_LookRead(const ILookInStream *stream, void *buf, size_t *size)
{
	const void *lookBuf;
	if (*size == 0) {
		return SZ_OK;
	}
	RINOK(ILookInStream_Look(stream, &lookBuf, size));
	memcpy(buf, lookBuf, *size);
	return ILookInStream_Skip(stream, *size);
}

SRes LookInStream_Read2(const ILookInStream *stream, void *buf, size_t size, SRes errorType)
{
	while (size != 0) {
		size_t processed = size;
		RINOK(ILookInStream_Read(stream, buf, &processed));
		if (processed == 0) {
			return errorType;
		}
		buf = (void *)((Byte *) buf + processed);
		size -= processed;
	}
	return SZ_OK;
}

SRes LookInStream_Read(const ILookInStream *stream, void *buf, size_t size)
{
	return LookInStream_Read2(stream, buf, size, SZ_ERROR_INPUT_EOF);
}

static SRes LookToRead2_Look_Lookahead(const ILookInStream *pp, const void **buf, size_t *size)
{
	SRes res = SZ_OK;
	GET_LookToRead2 size_t size2 = p->size - p->pos;
	if (size2 == 0 && *size != 0) {
		p->pos = 0;
		p->size = 0;
		size2 = p->bufSize;
		res = ISeekInStream_Read(p->realStream, p->buf, &size2);
		p->size = size2;
	}
	if (*size > size2) {
		*size = size2;
	}
	*buf = p->buf + p->pos;
	return res;
}

static SRes LookToRead2_Look_Exact(const ILookInStream *pp, const void **buf, size_t *size)
{
	SRes res = SZ_OK;
	GET_LookToRead2 size_t size2 = p->size - p->pos;
	if (size2 == 0 && *size != 0) {
		p->pos = 0;
		p->size = 0;
		if (*size > p->bufSize) {
			*size = p->bufSize;
		}
		res = ISeekInStream_Read(p->realStream, p->buf, size);
		size2 = p->size = *size;
	}
	if (*size > size2) {
		*size = size2;
	}
	*buf = p->buf + p->pos;
	return res;
}

static SRes LookToRead2_Skip(const ILookInStream *pp, size_t offset)
{
	GET_LookToRead2 p->pos += offset;
	return SZ_OK;
}

static SRes LookToRead2_Read(const ILookInStream *pp, void *buf, size_t *size)
{
	GET_LookToRead2 size_t rem = p->size - p->pos;
	if (rem == 0) {
		return ISeekInStream_Read(p->realStream, buf, size);
	}
	if (rem > *size) {
		rem = *size;
	}
	memcpy(buf, p->buf + p->pos, rem);
	p->pos += rem;
	*size = rem;
	return SZ_OK;
}

static SRes LookToRead2_Seek(const ILookInStream *pp, Int64 *pos, ESzSeek origin)
{
	GET_LookToRead2 p->pos = p->size = 0;
	return ISeekInStream_Seek(p->realStream, pos, origin);
}

void LookToRead2_CreateVTable(CLookToRead2 *p, int lookahead)
{
	p->vt.Look = lookahead ? LookToRead2_Look_Lookahead : LookToRead2_Look_Exact;
	p->vt.Skip = LookToRead2_Skip;
	p->vt.Read = LookToRead2_Read;
	p->vt.Seek = LookToRead2_Seek;
}

static SRes SecToLook_Read(const ISeqInStream *pp, void *buf, size_t *size)
{
	CSecToLook *p = CONTAINER_FROM_VTBL(pp, CSecToLook, vt);
	return LookInStream_LookRead(p->realStream, buf, size);
}

void SecToLook_CreateVTable(CSecToLook *p)
{
	p->vt.Read = SecToLook_Read;
}

static SRes SecToRead_Read(const ISeqInStream *pp, void *buf, size_t *size)
{
	CSecToRead *p = CONTAINER_FROM_VTBL(pp, CSecToRead, vt);
	return ILookInStream_Read(p->realStream, buf, size);
}

void SecToRead_CreateVTable(CSecToRead *p)
{
	p->vt.Read = SecToRead_Read;
}
