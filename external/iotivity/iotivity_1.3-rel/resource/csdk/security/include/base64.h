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

#ifndef _IOTVT_B64_H_
#define _IOTVT_B64_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * An implementation of Base 64 encoding. As defined in RFC4648 section 4.
 *
 * This enables encoding/decoding binary data into a 65-character subset
 * of US-ASCII (64 characters plus a padding character).
 *
 * @note Only Base 64 encoding is implemented. Other encoding types
 * defined in RFC4648 are not implemented. Base 64 encoding with URL
 * and filename safe alphabet, Bass 32, and Base 16 Encoding are not
 * implemented.
 */

/**
 * Macro to calculate the size of 'output' buffer required for
 * a 'input' buffer of length x during Base64 encoding operation.
 */
#define B64ENCODE_OUT_SAFESIZE(x) ((((x) + 3 - 1)/3) * 4 + 1)

/**
 * Macro to calculate the size of 'output' buffer required for
 * a 'input' buffer of length x during Base64 decoding operation.
 */
#define B64DECODE_OUT_SAFESIZE(x) (((x)*3)/4)

/**
 * Result code of base64 functions.
 */
typedef enum
{
    /// The encode, decode operation was successful
    B64_OK = 0,
    /// Invalid input parameter.
    B64_INVALID_PARAM,
    /// The output buffer is not large enough for the encoded or decoded data.
    B64_OUTPUT_BUFFER_TOO_SMALL,
    /// Indicates error encoding or decoding.
    B64_ERROR
} B64Result;

/**
 * Encode the plain message in base64.
 *
 * Example usage
 *
 *     const uint8_t* input = { 0x00, 0x11, 0x22, 0x33 };
 *     size_t b64Size = 0;
 *     size_t b64BufSize = B64ENCODE_OUT_SAFESIZE(sizeof(input) / sizeof(uint8_t*));
 *     char* b64Buf = (char*)OICCalloc(1, b64BufSize);
 *     if(B64_OK != b64Encode(input, sizeof(input) / sizeof(uint8_t*),
 *                            b64Buf, b64BufSize, &b64Size))
 *     {
 *         printf("Failure Base 64 encoding input\n");
 *     }
 *
 * @param[in]  in is the plain message to be converted.
 * @param[in]  inLen is the byte length of plain message.
 * @param[out] outBuf is the output buffer containing Base64 encoded message.
 * @param[in]  outBufSize is the size of output buffer.
 * @param[out] outLen is the byte length of encoded message.
 * @note `outBuf` adds a NULL to the string configuration.
 *
 * @return ::B64_OK for Success, otherwise some error value.
 */
B64Result b64Encode(const uint8_t *in, const size_t inLen,
                    char *outBuf, const size_t outBufSize, size_t *outLen);

/**
 * Decode the encoded message in base64.
 *
 * Example usage
 *
 *     const char* b64EncodedData = "Zm9vYmFy";
 *     size_t outLen;
 *     size_t outBufLen = B64DECODE_OUT_SAFESIZE(strlen(b64EncodedData));
 *     uint8_t* outBuf = (uint8_t*)OICCalloc(1, outBufSize);
 *     if (B64_OK != b64Decode(b64EncodedData, strlen(b64EncodedData), outBuf, outBufLen, &outLen));
 *     {
 *          printf("Failure decoding Base 64 encoded data.\n");
 *     }
 *
 * @param[in]  in is the Base64 encoded message to be converted.
 * @param[in]  inLen is the byte length of the encoded message.
 * @param[out] outBuf is the output buffer containing decoded message.
 * @param[in]  outBufSize is the size of output buffer.
 * @param[out] outLen is the byte length of decoded message.
 *
 * @return ::B64_OK for Success, otherwise some error value.
 */
B64Result b64Decode(const char *in, const size_t inLen,
                    uint8_t *outBuf, size_t outBufSize, size_t *outLen);

#ifdef __cplusplus
}
#endif

#endif  //IOTVT_B64_H
