/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file sfs.c
/// @brief Test Case Example for Secure File System Feature


/****************************************************************************
 * Included Files
 ****************************************************************************/


#include <stdio.h>
#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tc_internal.h"
#include "../../tc_common.h"

#define MAXBUFSIZE 256
#define CRYPTSIZE 16
#define SFS_TASK_STACKSIZE 8192
#define SFS_thread_name "SFS_TC"

char testdata[MAXBUFSIZE];
char data[MAXBUFSIZE] = "";
static const char *FILE_PATH = "/mnt/name.txt";

/**
* @fn                   :file_writetest_fwrite
* @brief                :writing into the specified file using fwrite function
* @return               :void
*/

static void file_writetest_fwrite(char *dat, size_t len)
{
	FILE *fp;
	/*If file already exists, delete the file */

	remove(FILE_PATH);
	fp = fopen(FILE_PATH, "w+");
	if (fp != NULL) {
		fwrite(dat, 1, len, fp);
	}
	fclose(fp);
}

/**
* @fn                   :file_writetest_fread
* @brief                :reading the specified file using fread function
* @return               :void
*/

static void file_readtest_fread(char *dat)
{
	FILE *fp;
	size_t len;

	fp = fopen(FILE_PATH, "r+");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (fp != NULL) {
		fread(dat, 1, len, fp);
	}
	fclose(fp);
}

/**
* @fn                   :file_writetest_fgets
* @brief                :reading the specified file using fgets function
* @return               :void
*/

static void file_readtest_fgets(char *dat)
{
	FILE *fp;
	size_t len;

	fp = fopen(FILE_PATH, "r+");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (fp != NULL) {
		fgets(dat, len+1, fp);
	}
	fclose(fp);
}

/**
* @fn                   :file_writetest_fprintf
* @brief                :writing into the specified file using fprintf function
* @return               :void
*/

static void file_writetest_fprintf(char *dat)
{
	FILE *fp;
	/*If file already exists, delete the file*/

	remove(FILE_PATH);
	fp = fopen(FILE_PATH, "w+");
	if (fp != NULL) {
		fprintf(fp, dat);
	}
	fclose(fp);
}

/**
* @fn                   :file_writetest_fputs
* @brief                :writing into the specified file using fputs function
* @return               :void
*/

static void file_writetest_fputs(char *dat)
{
	FILE *fp;
	/*If file already exists, delete the file*/

	remove(FILE_PATH);
	fp = fopen(FILE_PATH, "w+");
	if (fp != NULL) {
		fputs(dat, fp);
	}
	fclose(fp);
}

/**
* @fn                   :file_readtest_byte
* @brief                :reading only numbytes from the headbyte position
* @return               :void
*/

static void file_readtest_byte(char *dat, int headbyte, int numbyte)
{
	char temp_buf[MAXBUFSIZE] = "";
	FILE *fp;
	int temp_offset;

	fp = fopen(FILE_PATH, "r+");
	/* If headbyte starts from a new 16-byted data*/

	if (headbyte % CRYPTSIZE == 0) {
		/*Going to the head of 16byted data */

		temp_offset = (((headbyte / CRYPTSIZE) - 1) * CRYPTSIZE);
		fseek(fp, temp_offset, SEEK_SET);
		if (fp != NULL) {
			fread(temp_buf, numbyte + CRYPTSIZE-1, 1, fp);
		}
		strncpy(dat, temp_buf + CRYPTSIZE-1, numbyte);
		fclose(fp);
	} else {
		/* Headbyte doesn't start from a new 16-byted data */

		temp_offset = ((headbyte / CRYPTSIZE) * CRYPTSIZE);
		fseek(fp, temp_offset, SEEK_SET);
		if (fp != NULL) {
			fread(temp_buf, numbyte + (headbyte % CRYPTSIZE), 1, fp);
		}
		fclose(fp);
		strncpy(dat, temp_buf + (headbyte % CRYPTSIZE) - 1, numbyte);
	}
}

/****************************************************************************
 * Name: Test Cases
 ****************************************************************************/

/**
 * @fn                   :tc_fs_sfs_fwrite_data2
 * @brief                :using fwrite on 2 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data2(void)
{
	char *input = "ab";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data2", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fwrite_data8
 * @brief                :using fwrite on 8 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data8(void)
{
	char *input = "abcdefgh";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data8", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fwrite_data16
 * @brief                :using fwrite on 16 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data16(void)
{
	char *input = "abcdefgh12345678";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data16", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fwrite_data24
 * @brief                :using fwrite on 24 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data24(void)
{
	char *input = "abcdefgh12345678abcdefgh";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data24", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fwrite_data121
 * @brief                :using fwrite on 121 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data121(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data121", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fwrite_data255
 * @brief                :using fwrite on 255 bytes data.
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_fwrite_data255(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1234567";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fwrite_data255", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();

}

/**
 * @fn                   :tc_fs_sfs_fprintf_data2
 * @brief                :using fprintf on 2 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data2(void)
{
	char *input = "ab";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data2", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fprintf_data8
 * @brief                :using fprintf on 8 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data8(void)
{
	char *input = "abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data8", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fprintf_data16
 * @brief                :using fprintf on 16 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data16(void)
{
	char *input = "abcdefgh12345678";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data16", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fprintf_data24
 * @brief                :using fprintf on 24 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data24(void)
{
	char *input = "abcdefgh12345678abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data24", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fprintf_data121
 * @brief                :using fprintf on 121 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data121(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data121", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fprintf_data255
 * @brief                :using fprintf on 255 bytes data.
 * @API's covered        :fprintf, fread
 * @return               :void
 */

static void tc_fs_sfs_fprintf_data255(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1234567";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fprintf_data255", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data2
 * @brief                :using fputs on 2 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data2(void)
{
	char *input = "ab";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data2", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data8
 * @brief                :using fputs on 8 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data8(void)
{
	char *input = "abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data8", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data16
 * @brief                :using fputs on 16 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data16(void)
{
	char *input = "abcdefgh12345678";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data16", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data24
 * @brief                :using fputs on 24 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data24(void)
{
	char *input = "abcdefgh12345678abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data24", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data121
 * @brief                :using fputs on 121 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data121(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data121", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fputs_data255
 * @brief                :using fputs on 255 bytes data.
 * @API's covered        :fputs, fread
 * @return               :void
 */

static void tc_fs_sfs_fputs_data255(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1234567";

	memset(data, '\0', sizeof(data));
	file_writetest_fputs(input);
	file_readtest_fread(data);
	TC_ASSERT_EQ("tc_fputs_data255", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();

}

/**
 * @fn                   :tc_fs_sfs_fgets_data2
 * @brief                :using fgets on 2 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data2(void)
{
	char *input = "ab";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data2", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_data8
 * @brief                :using fgets on 8 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data8(void)
{
	char *input = "abcdefgh";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data8", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_data16
 * @brief                :using fgets on 16 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data16(void)
{
	char *input = "abcdefgh12345678";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data16", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_data24
 * @brief                :using fgets on 24 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data24(void)
{
	char *input = "abcdefgh12345678abcdefgh";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data24", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_data121
 * @brief                :using fgets on 121 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data121(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data121", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_data255
 * @brief                :using fgets on 255 bytes data.
 * @API's covered        :fwrite, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_data255(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1234567";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_data255", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();

}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data2
 * @brief                :using fgets & fprintf on 2 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data2(void)
{
	char *input = "ab";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data2", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data8
 * @brief                :using fgets & fprintf on 8 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data8(void)
{
	char *input = "abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data8", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data16
 * @brief                :using fgets & fprintf on 16 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data16(void)
{
	char *input = "abcdefgh12345678";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data16", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data24
 * @brief                :using fgets & fprintf on 24 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data24(void)
{
	char *input = "abcdefgh12345678abcdefgh";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data24", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data121
 * @brief                :using fgets & fprintf on 121 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data121(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data121", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_fgets_fprintf_data255
 * @brief                :using fgets & fprintf on 255 bytes data.
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_fgets_fprintf_data255(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh12345678abcdefgh1234567";

	memset(data, '\0', sizeof(data));
	file_writetest_fprintf(input);
	file_readtest_fgets(data);
	TC_ASSERT_EQ("tc_fgets_fprintf_data255", strcmp(input, data), 0);
	TC_SUCCESS_RESULT();

}

/**
 * @fn                   :tc_fs_sfs_readtest_1byte
 * @brief                :reading only first byte of the input data
 * @API's covered        :fprintf, fgets
 * @return               :void
 */

static void tc_fs_sfs_readtest_1byte(void)
{
	char *input = "abcdefgh12345678abcd";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_byte(data, 1, 1);
	memset(testdata, '\0', sizeof(testdata));
	strncpy(testdata, input, 1);
	TC_ASSERT_EQ("tc_readtest_1byte", strcmp(testdata, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_readtest_5byte
 * @brief                :reading first few bytes of the input data
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_readtest_5byte(void)
{
	char *input = "abcdefgh12345678abcd";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_byte(data, 1, 5);
	memset(testdata, '\0', sizeof(testdata));
	strncpy(testdata, input, 5);
	TC_ASSERT_EQ("tc_readtest_5byte", strcmp(testdata, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_readtest_middlebyte
 * @brief                :reading a single byte from the middle of input data
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_readtest_middlebyte(void)
{
	char *input = "abcdefgh12345678abcdefgh";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_byte(data, 11, 1);
	memset(testdata, '\0', sizeof(testdata));
	strncpy(testdata, input + 10, 1);
	TC_ASSERT_EQ("tc_readtest_middlebyte", strcmp(testdata, data), 0);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_fs_sfs_readtest_middlebyte10
 * @brief                :reading a few bytes from the middle of input data
 * @API's covered        :fwrite, fread
 * @return               :void
 */

static void tc_fs_sfs_readtest_middlebyte10(void)
{
	char *input = "abcdefgh12345678abcdefgh12345678";
	size_t len;

	memset(data, '\0', sizeof(data));
	len = strlen(input);
	file_writetest_fwrite(input, len);
	file_readtest_byte(data, 20, 10);
	memset(testdata, '\0', sizeof(testdata));
	strncpy(testdata, input + 19, 10);
	TC_ASSERT_EQ("tc_readtest_middlebyte10", strcmp(testdata, data), 0);
	TC_SUCCESS_RESULT();
}


static void smart_sfs_main_util(void)
{
	tc_fs_sfs_fwrite_data2();
	tc_fs_sfs_fwrite_data8();
	tc_fs_sfs_fwrite_data16();
	tc_fs_sfs_fwrite_data24();
	tc_fs_sfs_fwrite_data121();
	tc_fs_sfs_fwrite_data255();
	tc_fs_sfs_fprintf_data2();
	tc_fs_sfs_fprintf_data8();
	tc_fs_sfs_fprintf_data16();
	tc_fs_sfs_fprintf_data24();
	tc_fs_sfs_fprintf_data121();
	tc_fs_sfs_fprintf_data255();
	tc_fs_sfs_fputs_data2();
	tc_fs_sfs_fputs_data8();
	tc_fs_sfs_fputs_data16();
	tc_fs_sfs_fputs_data24();
	tc_fs_sfs_fputs_data121();
	tc_fs_sfs_fputs_data255();
	tc_fs_sfs_fgets_data2();
	tc_fs_sfs_fgets_data8();
	tc_fs_sfs_fgets_data16();
	tc_fs_sfs_fgets_data24();
	tc_fs_sfs_fgets_data121();
	tc_fs_sfs_fgets_data255();
	tc_fs_sfs_fgets_fprintf_data2();
	tc_fs_sfs_fgets_fprintf_data8();
	tc_fs_sfs_fgets_fprintf_data16();
	tc_fs_sfs_fgets_fprintf_data24();
	tc_fs_sfs_fgets_fprintf_data121();
	tc_fs_sfs_fgets_fprintf_data255();
	tc_fs_sfs_readtest_1byte();
	tc_fs_sfs_readtest_5byte();
	tc_fs_sfs_readtest_middlebyte();
	tc_fs_sfs_readtest_middlebyte10();
}

int smart_sfs_main(void)
{
	pthread_t p;
	pthread_attr_t attr;

	pthread_setname_np(p, SFS_thread_name);
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, SFS_TASK_STACKSIZE);
	pthread_create(&p, &attr, (pthread_startroutine_t)smart_sfs_main_util, NULL);
	pthread_join(p, NULL);
	return 0;
}
