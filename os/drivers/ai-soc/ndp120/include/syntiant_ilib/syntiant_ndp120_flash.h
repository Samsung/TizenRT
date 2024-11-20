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
#ifndef _SYNTIANT_NDP120_FLASH_H_
#define _SYNTIANT_NDP120_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief syntiant_get_flash_id
 *
 * Print the flash manufacturer id and device id..
 *
 * @param ndp pointer to NDP state object
 *
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_get_flash_id(struct syntiant_ndp_device_s *ndp);

/**
 * @brief syntiant_stream_flash_read
 *
 * Stream flash to buffer
 *
 * @param ndp pointer to NDP state object
 * @param buffer reading to buffer
 * @param length length of buffer
 *
 * @return a @c SYNTIANT_NDP_ERROR_* code
 *
 * The call sequence of is as follows:
 * 1. stream initialize (buffer == NULL, length == 0)
 * 2. stream iterating (buffer != NULL, length != 0)
 * 3. stream terminate (buffer != NULL, length == 0)
 */
int syntiant_stream_flash_read(struct syntiant_ndp_device_s *ndp,
    uint8_t *buffer, unsigned length);

/**
 * @brief syntiant_stream_flash_write
 *
 * Stream buffer to flash
 *
 * @param ndp pointer to NDP state object
 * @param buffer reading to buffer
 * @param length length of buffer
 *
 * @return a @c SYNTIANT_NDP_ERROR_* code
 *
 * The call sequence of is as follows:
 * 1. stream initialize (buffer == NULL, length == 0)
 * 2. stream iterating (buffer != NULL, length != 0)
 * 3. stream terminate (buffer != NULL, length == 0)
 */
int syntiant_stream_flash_write(struct syntiant_ndp_device_s *ndp,
    const uint8_t *buffer, unsigned length);

/**
 * @brief syntiant_stream_to_flash
 *
 * Stream named file to flash
 *
 * @param ndp pointer to NDP state object
 * @param filename file containing flashable image (i.e. combined synpkg)
 * @param verify set true to compare verify
 *
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_stream_to_flash(struct syntiant_ndp_device_s *ndp,
    char *filename, int verify);

#ifdef __cplusplus
}
#endif

#endif

