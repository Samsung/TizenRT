/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_misc.c

/// @brief Test Case Example for Libc Misc API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <tinyara/regex.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <crc8.h>
#include <crc16.h>
#include <crc32.h>
#include "tc_internal.h"

#define BUFF_SIZE 256
#define USEC_100 100
#define ARR_NUM 2 // length of input/output array
#define SRC_LEN 1 // length of input data
#define VAL_50 50
#define VAL_71 71
#define VAL_100 100
#define VAL_192 192
#define VAL_213 213
#define VAL_255 255
#define VAL_4753 4753 // 0x1291
#define VAL_5649 5649 // 0x1611
#define VAL_9506 9506 // 0x2522
#define VAL_10842 10842 // 0x2A5A
#define VAL_11298 11298 // 0x2C22
#define VAL_52690 52690 // 0xCDD2
#define VAL_54050 54050 // 0XD322
#define VAL_55949 55949 // 0xDA8D
#define VAL_57534 57534 // 0xE0BE
#define VAL_63457 63457 // 0xF7E1
#define VAL_65380 65380
#define VAL_CRC32_1 2564639436UL
#define VAL_CRC32_2 450215437UL
#define VAL_CRC32_3 3051332929UL

/**
 * @fn                  :tc_libc_misc_crc8
 * @brief               :Return a 8-bit CRC of the contents of the 'src_arr' buffer, length 'len' using a
 *                       CRC with the polynomial x^8+x^6+x^3+x^2+1
 * @scenario            :Return a 8-bit CRC of the contents of the 'src_arr' buffer, length 'len' using a
 *                       CRC with the polynomial x^8+x^6+x^3+x^2+1
 * @API's covered       :crc8
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc8(void)
{
	uint8_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };

	/* Return value should be 213 as calculated by crc8 */

	ret_chk = crc8(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc8", ret_chk, VAL_213);

	/* Return value should be 71 as calculated by crc8 */

	src_arr[0] = VAL_50;
	ret_chk = crc8(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc8", ret_chk, VAL_71);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc8part
 * @brief               :Continue CRC calculation on a part of the buffer.
 * @scenario            :Continue CRC calculation on a part of the buffer.
 * @API's covered       :crc8part
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc8part(void)
{
	uint8_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };
	uint8_t crc_8val = 0;

	/* Return value should be 213 as calculated by crc8part */

	ret_chk = crc8part(src_arr, SRC_LEN, crc_8val);
	TC_ASSERT_EQ("crc8part", ret_chk, VAL_213);

	/* Return value should be 192 as calculated by crc8part */

	crc_8val = VAL_255;
	ret_chk = crc8part(src_arr, SRC_LEN, crc_8val);
	TC_ASSERT_EQ("crc8part", ret_chk, VAL_192);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16
 * @brief               :Return a 16-bit CRC of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 16-bit CRC of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc16
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };

	/* Return value should be 100 as calculated by crc16 */

	ret_chk = crc16(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc16", ret_chk, VAL_100);

	/* Return value should be 50 as calculated by crc16 */

	src_arr[0] = VAL_50;
	ret_chk = crc16(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc16", ret_chk, VAL_50);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16part
 * @brief               :Continue CRC calculation on a part of the buffer.
 * @scenario            :Continue CRC calculation on a part of the buffer.
 * @API's covered       :crc16part
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16part(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };
	uint16_t crc_16val = 0;

	/* Return value should be 100 as calculated by crc16part */

	ret_chk = crc16part(src_arr, SRC_LEN, crc_16val);
	TC_ASSERT_EQ("crc16part", ret_chk, VAL_100);

	/* Return value should be 65380 as calculated by crc16part */

	crc_16val = VAL_255;
	ret_chk = crc16part(src_arr, SRC_LEN, crc_16val);
	TC_ASSERT_EQ("crc16part", ret_chk, VAL_65380);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_xmodem
 * @brief               :Return a 16-bit CRC16-CCITT/Xmodem of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 16-bit CRC16-CCITT/Xmodem of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc16_ccitt_xmodem
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_xmodem(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[ARR_NUM] = { VAL_50, VAL_100 };
	uint16_t res_arr[ARR_NUM] = { VAL_5649, VAL_11298 };

	/* Check crc16_ccitt_xmodem results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_xmodem(&src_arr[arr_idx], SRC_LEN);
		TC_ASSERT_EQ("crc16_ccitt_xmodem", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_false
 * @brief               :Return a 16-bit CRC16-CCITT/False of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 16-bit CRC16-CCITT/False of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc16_ccitt_false
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_false(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[ARR_NUM] = { VAL_50, VAL_100 };
	uint16_t res_arr[ARR_NUM] = { VAL_63457, VAL_52690 };

	/* Check crc16_ccitt_false results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_false(&src_arr[arr_idx], SRC_LEN);
		TC_ASSERT_EQ("crc16_ccitt_false", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_aug
 * @brief               :Return a 16-bit CRC16-CCITT/Aug of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 16-bit CRC16-CCITT/Aug of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc16_ccitt_aug
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_aug(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[ARR_NUM] = { VAL_50, VAL_100 };
	uint16_t res_arr[ARR_NUM] = { VAL_55949, VAL_57534 };

	/* Check crc16_ccitt_aug results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_aug(&src_arr[arr_idx], SRC_LEN);
		TC_ASSERT_EQ("crc16_ccitt_aug", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_kermit
 * @brief               :Return a 16-bit CRC16-CCITT/Kermit of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 16-bit CRC16-CCITT/Kermit of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc16_ccitt_kermit
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_kermit(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[ARR_NUM] = { VAL_50, VAL_100 };
	uint16_t res_arr[ARR_NUM] = { VAL_4753, VAL_9506 };

	/* Check crc16_ccitt_kermit results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_kermit(&src_arr[arr_idx], SRC_LEN);
		TC_ASSERT_EQ("crc16_ccitt_kermit", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_part
 * @brief               :Continue CRC16-CCITT calculation on a part of the buffer.
 * @scenario            :Continue CRC16-CCITT calculation on a part of the buffer. (calculated by polynomial 0x1021)
 * @API's covered       :crc16_ccitt_part
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_part(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };
	uint16_t crc_16val[2] = { 0, VAL_255 };
	uint16_t res_arr[2] = { VAL_11298, VAL_54050 };

	/* Check crc16_ccitt_part results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_part(src_arr, SRC_LEN, crc_16val[arr_idx]);
		TC_ASSERT_EQ("crc16_ccitt_part", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc16_ccitt_rev_part
 * @brief               :Continue CRC16-CCITT calculation on a part of the buffer.
 * @scenario            :Continue CRC16-CCITT calculation on a part of the buffer. (calculated by polynomial 0x8408)
 * @API's covered       :crc16_ccitt_rev_part
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc16_ccitt_rev_part(void)
{
	uint16_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };
	uint16_t crc_16val[2] = { 0, VAL_255 };
	uint16_t res_arr[2] = { VAL_9506, VAL_10842 };

	/* Check crc16_ccitt_rev_part results according to input values */

	for (int arr_idx = 0; arr_idx < ARR_NUM; arr_idx++) {
		ret_chk = crc16_ccitt_rev_part(src_arr, SRC_LEN, crc_16val[arr_idx]);
		TC_ASSERT_EQ("crc16_ccitt_rev_part", ret_chk, res_arr[arr_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc32
 * @brief               :Return a 32-bit CRC of the contents of the 'src' buffer, length 'len'
 * @scenario            :Return a 32-bit CRC of the contents of the 'src' buffer, length 'len'
 * @API's covered       :crc32
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc32(void)
{
	uint32_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };

	/* Return value should be 0x4adfa541 as calculated by crc32 */

	ret_chk = crc32(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc32", ret_chk, VAL_CRC32_1);

	/* Return value should be VAL_CRC32_2 as calculated by crc32 */

	src_arr[0] = VAL_50;
	ret_chk = crc32(src_arr, SRC_LEN);
	TC_ASSERT_EQ("crc32", ret_chk, VAL_CRC32_2);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_crc32part
 * @brief               :Continue CRC calculation on a part of the buffer.
 * @scenario            :Continue CRC calculation on a part of the buffer.
 * @API's covered       :crc32part
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_crc32part(void)
{
	uint32_t ret_chk;
	uint8_t src_arr[1] = { VAL_100 };
	uint32_t crc_32val = 0;

	/* Return value should be 0x4adfa541 as calculated by crc32part */

	ret_chk = crc32part(src_arr, SRC_LEN, crc_32val);
	TC_ASSERT_EQ("crc32part", ret_chk, (uint32_t)VAL_CRC32_1);

	/* Return value should be 0x67dd4acc as calculated by crc32part */

	crc_32val = VAL_255;
	ret_chk = (uint32_t)crc32part(src_arr, SRC_LEN, crc_32val);
	TC_ASSERT_EQ("crc32part", ret_chk, (uint32_t)VAL_CRC32_3);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_DEBUG
#ifdef CONFIG_DEBUG_ERROR
/**
 * @fn                  :tc_libc_misc_dbg
 * @brief               :Outputs messages to the console similar to printf() except that the output is not buffered.
 * @scenario            :In general, error messages and output of importance use dbg()
 * @API's covered       :dbg
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_dbg(void)
{
	int ret_chk = ERROR;
	const char *msg_output = "tc_libc_misc_dbg: Examples";
	/*
	   in debug.h file dbg is defined as :
	   # define EXTRA_FMT "%s: "
	   # define EXTRA_ARG ,__FUNCTION__
	   # define dbg(format, ...) \
	   syslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
	 */

	/* Message displayed is:  "tc_libc_misc_dbg: Examples" */

	usleep(USEC_100);
	ret_chk = dbg("Examples");
	TC_ASSERT_EQ("dbg", ret_chk, strlen(msg_output));

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_misc_lib_dumpbuffer
 * @brief               :Do a pretty buffer dump
 * @scenario            :Do a pretty buffer dump
 * @API's covered       :lib_dumpbuffer
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_lib_dumpbuffer(void)
{
	const char *msg = "tc_libc_misc_lib_dumpbuffer";
	unsigned char buffer[] = {'S', 'A', 'M', 'S', 'U', 'N', 'G', '-', 'T', 'i', 'z', 'e', 'n', 'R', 'T'};
	unsigned char *buf = NULL;
	int idx;

	/* To guarantee flushing other printf messages before using lowsyslog */
	usleep(10);

	/**
	 * As lib_dumpbuffer returns void, there in no way to check the success or failure case
	 * we need to manually check the dump output
	 * Case-1: output: 53414d53554e472d54697a656e5254 SAMSUNG-TizenRT
	 * Case-2: output: 0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a ...............	(i.e, 256 "0a")
	 */

	/* Case-1 */
	lib_dumpbuffer(msg, buffer, sizeof(buffer));

	/* Case-2 */
	buf = (unsigned char *)malloc(BUFF_SIZE);
	TC_ASSERT_NEQ("malloc", buf, NULL);

	for (idx = 0; idx < BUFF_SIZE; idx++) {
		buf[idx] = 0xA;
	}
	lib_dumpbuffer(msg, buf, BUFF_SIZE);

	free(buf);
	buf = NULL;

	TC_SUCCESS_RESULT();
}

#if !defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_ARCH_LOWPUTC)
/**
 * @fn                  :tc_libc_misc_lldbg
 * @brief               :Identical to [a-z]dbg() except this is uses special interfaces provided by architecture-specific logic to \n
 *                       talk directly to the underlying console hardware.
 * @scenario            :lldbg() is particularly useful in low-level code  For example, only lldbg() should be used in interrupt handlers.
 * @API's covered       :lldbg
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_lldbg(void)
{
	int ret_chk = -1;
	const char *msg_output = "tc_libc_misc_lldbg: Examples";
	/*
	   in debug.h file lldbg is defined as :
	   # define EXTRA_FMT "%s: "
	   # define EXTRA_ARG ,__FUNCTION__
	   #  define lldbg(format, ...) \
	   lowsyslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
	 */

	/* Message displayed is:  "tc_libc_misc_lldbg: Examples" */

	usleep(USEC_100);
	ret_chk = lldbg("Examples");
	TC_ASSERT_EQ("lldbg", ret_chk, strlen(msg_output));

	TC_SUCCESS_RESULT();
}
#endif /* !CONFIG_BUILD_PROTECTED && CONFIG_ARCH_LOWPUTC */
#endif /* CONFIG_DEBUG_ERROR */

#ifdef CONFIG_DEBUG_VERBOSE
/**
 * @fn                  :tc_libc_misc_vdbg
 * @brief               :This is intended for general debug output that is normally suppressed.
 * @scenario            :When debug out is required with all details
 * @API's covered       :vdbg
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */

static void tc_libc_misc_vdbg(void)
{
	int ret_chk = -1;
	const char *msg_output = "tc_libc_misc_vdbg: Examples";
	/*
	   in debug.h file vdbg is defined as :
	   # define EXTRA_FMT "%s: "
	   # define EXTRA_ARG ,__FUNCTION__
	   #  define vdbg(format, ...) \
	   syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
	 */

	/* Message displayed is:  "tc_libc_misc_vdbg: Examples" */

	usleep(USEC_100);
	ret_chk = vdbg("Examples");
	TC_ASSERT_EQ("vdbg", ret_chk, strlen(msg_output));

	TC_SUCCESS_RESULT();
}

#if !defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_ARCH_LOWPUTC)
/**
 * @fn                  :tc_libc_misc_llvdbg
 * @brief               :This is intended for general debug output that is normally suppressed.
 * @scenario            :When debug out is required with all details
 * @API's covered       :llvdbg
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_llvdbg(void)
{
	int ret_chk = -1;
	const char *msg_output = "tc_libc_misc_llvdbg: Examples";
	/*
	   in debug.h file llvdbg is defined as :
	   # define EXTRA_FMT "%s: "
	   # define EXTRA_ARG ,__FUNCTION__
	   # define llvdbg(format, ...) \
	   lowsyslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
	 */

	/* Message displayed is:  "tc_libc_misc_llvdbg: Examples" */

	usleep(USEC_100);
	ret_chk = llvdbg("Examples");
	TC_ASSERT_EQ("llvdbg", ret_chk, strlen(msg_output));

	TC_SUCCESS_RESULT();
}
#endif /* !CONFIG_BUILD_PROTECTED && CONFIG_ARCH_LOWPUTC */
#endif /* CONFIG_DEBUG_VERBOSE */
#endif /* CONFIG_DEBUG */

/**
 * @fn                  :tc_libc_misc_match
 * @brief               :Simple shell-style filename pattern matcher
 * @scenario            :Returns 1 (match) or 0 (no-match)
 * @API's covered       :match
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_misc_match(void)
{
	const char *pattern = "?|";
	const char *string = "S";
	int ret;

	ret = match(pattern, string);
	TC_ASSERT_EQ("match", ret, 1);

	pattern = "somepattern";
	string = "nomatch";
	ret = match(pattern, string);
	TC_ASSERT_EQ("match", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_misc
 ****************************************************************************/

int libc_misc_main(void)
{
	tc_libc_misc_crc8();
	tc_libc_misc_crc8part();
	tc_libc_misc_crc16();
	tc_libc_misc_crc16part();
	tc_libc_misc_crc16_ccitt_xmodem();
	tc_libc_misc_crc16_ccitt_false();
	tc_libc_misc_crc16_ccitt_aug();
	tc_libc_misc_crc16_ccitt_kermit();
	tc_libc_misc_crc16_ccitt_part();
	tc_libc_misc_crc16_ccitt_rev_part();
	tc_libc_misc_crc32();
	tc_libc_misc_crc32part();
#ifdef CONFIG_DEBUG

#ifdef CONFIG_DEBUG_ERROR
	tc_libc_misc_dbg();
	tc_libc_misc_lib_dumpbuffer();
#if !defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_ARCH_LOWPUTC)
	tc_libc_misc_lldbg();
#endif /* !CONFIG_BUILD_PROTECTED && CONFIG_ARCH_LOWPUTC */
#endif /* CONFIG_DEBUG_ERROR */

#ifdef CONFIG_DEBUG_VERBOSE
	tc_libc_misc_vdbg();
#if !defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_ARCH_LOWPUTC)
	tc_libc_misc_llvdbg();
#endif /* !CONFIG_BUILD_PROTECTED && CONFIG_ARCH_LOWPUTC */
#endif /* CONFIG_DEBUG_VERBOSE */

#endif /* CONFIG_DEBUG */
	tc_libc_misc_match();

	return 0;
}
