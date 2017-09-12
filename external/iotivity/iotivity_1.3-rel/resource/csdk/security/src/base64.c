//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "base64.h"

/** base character of Base64. */
static const char g_b64TransTbl[] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"\
                "ghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64 block encode function.
 *
 * @param in is the octet stream, max 3 byte.
 * @param out is the Base64 encoded stream, 4 byte.
 * @param len is the  byte-length of octet stream.
 *
 * @return ::B64_OK for Success, otherwise some error value.
 */
static B64Result b64EncodeBlk(const uint8_t* in, char* out, size_t len)
{
    if (NULL == in || NULL ==  out || 0 == len )
    {
        return B64_INVALID_PARAM;
    }

    out[0] = g_b64TransTbl[in[0] >> 2];

    if (1 == len)
    {
        out[1] = g_b64TransTbl[((in[0] & 0x03) << 4)];
    }
    else
    {
        out[1] = g_b64TransTbl[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
    }

    if (2 == len)
    {
        out[2] = g_b64TransTbl[((in[1] & 0x0f) << 2)];
    }
    else if (1 < len)
    {
        out[2] = g_b64TransTbl[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)];
    }
    else
    {
        out[2] = '=';
    }

    if (2 < len)
    {
        out[3] = g_b64TransTbl[in[2] & 0x3f];
    }
    else
    {
        out[3] = '=';
    }

    return B64_OK;
}

B64Result b64Encode(const uint8_t* in, const size_t inLen,
               char* outBuf, const size_t outBufSize, size_t* outLen)
{
    if (NULL == in || NULL == outBuf || NULL == outLen )
    {
        return B64_INVALID_PARAM;
    }

    *outLen = ((inLen / 3) * 3 == inLen) ?
              ((inLen / 3) * 4) :
              (((inLen / 3) + 1) * 4);
    size_t minBufSize = (*outLen + 1);
    if (outBufSize < minBufSize)
    {
        return B64_OUTPUT_BUFFER_TOO_SMALL;
    }

    size_t i;
    for (i = 0; i < inLen / 3; i++)
    {
        if(B64_OK != b64EncodeBlk(in + i * 3, outBuf + i * 4, 3))
        {
            return B64_INVALID_PARAM;
        }
    }

    if ((i * 3) != inLen)
    {
        if (B64_OK != b64EncodeBlk(in + i * 3, outBuf + i * 4, inLen - i * 3))
        {
            return B64_INVALID_PARAM;
        }
    }

    outBuf[*outLen] = '\0';

    return B64_OK;
}

/**
 * Get decoded value.
 *
 * @param c is the Base64 encoded character.
 *
 * @return decoded value, 6-bit.
 */
static uint32_t b64GetVal(char c)
{
    if (('A' <= c) && ('Z' >= c))
    {
        return c - 'A';
    }
    else if (('a' <= c) && ('z' >= c))
    {
        return c - 'a' + 26;
    }
    else if (('0' <= c) && ('9' >= c))
    {
        return c - '0' + 52;
    }
    else if ('+' == c)
    {
        return 62;
    }
    else if ('/' == c)
    {
        return 63;
    }
    else if ('=' == c)
    {
        return 0;
    }

    return 0;
}

/**
 * Check if the input string is a valid Base64 encoded string.
 * The string must have groups of 4 characters and only contain
 * the US-ASCII characters `[A-Z][a-z][0-9]+/` and (pad) `=`
 *
 * @param[in] in is the Base64 encoded message to validate.
 * @param[in] inLen is the byte length of the encoded message.
 *
 * @return
 *    - B64_OK    if it is valid Base64 encoding string.
 *    - B64_ERROR if string is not a valid Base64 encoded string.
 */
static B64Result isValidB64EncodedString(const char* in, const size_t inLen)
{
    // All valid Base64 encoded strings will be multiples of 4
    if (0 != (inLen % 4))
    {
        return B64_ERROR;
    }
    for (size_t i = 0; i < inLen; ++i)
    {
        // "[A-Z][a-z][0-9]+/=" are the only valid
        // letters in a Base64 encoded string
        if ((('A' > in[i]) || ('Z' < in[i])) &&
            (('a' > in[i]) || ('z' < in[i])) &&
            (('0' > in[i]) || ('9' < in[i])) &&
            ('+' != in[i]) &&
            ('/' != in[i]) &&
            ('=' != in[i]))
        {
            return B64_ERROR;
        }
        // Padding character "=" can only show up as last 2 characters
        if ('=' == in[i])
        {
            if (i < inLen - 2)
            {
                return B64_ERROR;
            }
            if (i == inLen - 2 && '=' != in[i+1])
            {
                return B64_ERROR;
            }
        }
    }
    return B64_OK;
}

/**
 * Base64 block decode function.
 *
 * @param in is the Base64 encoded stream, 4 bytes.
 * @param out is the Octet stream, 3 bytes.
 *
 * @return ::B64_OK for Success, otherwise some error value.
 */
static B64Result b64DecodeBlk(const char* in, uint8_t* out)
{
    if (NULL == in || NULL == out)
    {
        return B64_INVALID_PARAM;
    }

    uint32_t val = (b64GetVal(in[0]) << 18) | (b64GetVal(in[1]) << 12) |
          (b64GetVal(in[2]) << 6) | (b64GetVal(in[3]));

    out[0] = (val >> 16) & 0xff;

    if ('=' != in[2])
    {
        out[1] = (val >> 8) & 0xff;
    }
    if ('=' != in[3])
    {
        out[2] = val & 0xff;
    }

    return B64_OK;
}

B64Result b64Decode(const char* in, const size_t inLen,
               uint8_t* outBuf, size_t outBufSize, size_t* outLen)
{
    if (NULL == in || NULL == outBuf || NULL == outLen)
    {
        return B64_INVALID_PARAM;
    }

    if (B64_OK != isValidB64EncodedString(in, inLen))
    {
        return B64_INVALID_PARAM;
    }

    *outLen = (inLen / 4) * 3;
    size_t minBufSize = (inLen / 4) * 3;
    if (inLen != 0)
    {
        if ('=' == in[inLen - 1])
        {
            minBufSize--;
            (*outLen)--;
        }
        if ('=' == in[inLen - 2])
        {
            minBufSize--;
            (*outLen)--;
        }
    }
    if (outBufSize < minBufSize)
    {
        return B64_OUTPUT_BUFFER_TOO_SMALL;
    }

    for (size_t i = 0; i < inLen / 4; i++)
    {
        if (B64_OK != b64DecodeBlk(in + i * 4, outBuf + i * 3))
        {
            return B64_INVALID_PARAM;
        }
    }

    return B64_OK;
}
