/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/

#include <syntiant_ilib/syntiant_portability.h>

#include <syntiant_packager/syntiant_package.h>

#define CRC_POLYNOMIAL 0xEDB88320U

uint32_t
crc32_no_lib(uint8_t *bytes, size_t len)
{
    size_t i, j;
    uint32_t byte, crc, mask;

    i = 0;
    crc = 0xFFFFFFFF;
    for (i = 0; i < len; i++) {
        byte = bytes[i];    /* Get next byte */
        crc = crc ^ byte;
        for(j = 0; j < 8; ++j) {
            mask = (unsigned int) -(((int) crc) & 1);
            crc = (crc >> 1) ^ (CRC_POLYNOMIAL & mask);
        }
    }
    return ~crc;
}

uint32_t
crc32_no_lib_init(void)
{
    return 0xFFFFFFFF;
}

uint32_t
crc32_no_lib_update(uint32_t crc, uint8_t *bytes, size_t len)
{
    size_t i, j;
    unsigned int byte, mask;

    i = 0;
    for (i = 0; i < len; i++) {
        byte = bytes[i];    /* Get next byte */
        crc = crc ^ byte;
        for(j = 0; j < 8; ++j) {
            mask = (unsigned int) -(((int) crc) & 1);
            crc = (crc >> 1) ^ (CRC_POLYNOMIAL & mask);
        }
    }
    return crc;
}

uint32_t
crc32_no_lib_finalize(uint32_t crc)
{
    return ~crc;
}
