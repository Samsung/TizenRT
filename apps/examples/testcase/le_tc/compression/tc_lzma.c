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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <crc32.h>
#include <tc_common.h>
#include <tinyara/lzma/LzmaLib.h>

#define SIZE_2KB 2048
#define SIZE_4KB 4096

unsigned int propsSize = LZMA_PROPS_SIZE;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tc_compression_lzma_2kb
 *
 * Description:
 *   Check and verify the checksum of a buffer of size 2KB after compression and decompression
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void tc_compression_lzma_2kb(void)
{

	int i;
	int ret;
	unsigned int ini_t;
	unsigned int after_t;
	unsigned int writesize = SIZE_2KB;
	unsigned int destLen = SIZE_2KB;
	unsigned char arr[SIZE_2KB];
	unsigned char out_buf[SIZE_2KB + LZMA_PROPS_SIZE];

	for (i = 0; i < SIZE_2KB; i++) {
		arr[i] = 'A' + ((rand()) % 26);
	}

	/* CRC of the buffer initially */
	ini_t = crc32(arr, SIZE_2KB);

	ret = LzmaCompress(&out_buf[LZMA_PROPS_SIZE], &writesize, arr, SIZE_2KB, out_buf, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);

	TC_ASSERT_EQ("LzmaCompress", ret, SZ_OK);
	/* If FAIL: Dynamic RAM size needed by Lzma is not enough */

	ret = LzmaUncompress(&arr[0], &destLen, &out_buf[LZMA_PROPS_SIZE], &writesize, &out_buf[0], LZMA_PROPS_SIZE);

	TC_ASSERT_NEQ("LzmaUncompress", ret, SZ_ERROR_FAIL);

	/* CRC of the buffer after compression-decompression */
	after_t = crc32(arr, SIZE_2KB);

	/* To check if the CRC of buffer is same after compression-decompression */
	TC_ASSERT_EQ("crc32", after_t, ini_t);

	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: tc_compression_lzma_4kb
 *
 * Description:
 *   Check and verify the checksum of a buffer of size 4KB after compression and decompression
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void tc_compression_lzma_4kb(void)
{
	int i;
	int ret;
	unsigned int ini_t;
	unsigned int after_t;
	unsigned int writesize = SIZE_4KB;
	unsigned int destLen = SIZE_4KB;
	unsigned char arr[SIZE_4KB];
	unsigned char out_buf[SIZE_4KB + LZMA_PROPS_SIZE];

	for (i = 0; i < SIZE_4KB; i++) {
		arr[i] = 'A' + ((rand()) % 26);
	}

	/* CRC of the buffer initially */
	ini_t = crc32(arr, SIZE_4KB);

	ret = LzmaCompress(&out_buf[LZMA_PROPS_SIZE], &writesize, arr, SIZE_4KB, out_buf, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);

	/* If FAIL: Dynamic RAM size needed by Lzma is not enough */
	TC_ASSERT_EQ("LzmaCompress", ret, SZ_OK);

	ret = LzmaUncompress(&arr[0], &destLen, &out_buf[LZMA_PROPS_SIZE], &writesize, &out_buf[0], LZMA_PROPS_SIZE);

	TC_ASSERT_NEQ("LzmaUncompress", ret, SZ_ERROR_FAIL);

	/* CRC of the buffer after compression-decompression */
	after_t = crc32(arr, SIZE_4KB);

	/* To check if the CRC of buffer is same after compression-decompression */
	TC_ASSERT_EQ("crc32", after_t, ini_t);

	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Public Function
 ****************************************************************************/
int lzma_main(void)				//Main defined
{
	tc_compression_lzma_2kb();
	tc_compression_lzma_4kb();

	return 0;
}
