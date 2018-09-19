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

/// @file tc_audio_main.c
/// @brief Main Function for Audio TestCase Example

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyalsa/tinyalsa.h>
#include "tc_common.h"
#include <tinyara/config.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define AUDIO_TEST_FILE "/tmp/pcm"
#define AUDIO_DEFAULT_CHANNELS 2
#define AUDIO_DEFAULT_RATE 16000
#define AUDIO_DEFAULT_FORMAT PCM_FORMAT_S16_LE
#define AUDIO_RECORD_DURATION 3//3sec
#define INPUT_STR_LEN 16
#define SEC_2 2
#define PREPARE_RETRY_COUNT 10

/****************************************************************************
 * Global Variables
 ****************************************************************************/
struct pcm *g_pcm;

static void clean_all_data(int fd, char *buffer)
{
	if (fd > 0) {
		close(fd);
	}

	if (g_pcm) {
		pcm_close(g_pcm);
	}

	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
}

/**
* @testcase         itc_audio_pcm_open_close_p
* @brief            open and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open, pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_close_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_open_p_with_different_flag
* @brief            open and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open, pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_p_with_different_flag(void)
{
	int index = 0;
	int count = 0;
	int ret = 0;
	bool check = true;
	unsigned int flag_arr[] = { PCM_IN, PCM_OUT, PCM_MMAP, PCM_NOIRQ };
	const char *ptr_flag[] = { "PCM_IN", "PCM_OUT", "PCM_MMAP", "PCM_NOIRQ" };
	count = sizeof(flag_arr) / sizeof(flag_arr[0]);

	for (index = 0; index < count; index++) {
		g_pcm = pcm_open(0, 0, flag_arr[index], NULL);
		if (pcm_get_file_descriptor(g_pcm) <= 0) {
			printf("\nitc_audio_pcm_open_p_with_different_flag: pcm_open FAIL for flag : %s\n", ptr_flag[index]);
			check = false;
			continue;
		}
		ret = pcm_close(g_pcm);
		if (ret != 0) {
			printf("\nitc_audio_pcm_open_p_with_different_flag: pcm_close FAIL for flag : %s\n", ptr_flag[index]);
			check = false;
		}
	}

	TC_ASSERT_EQ("itc_audio_pcm_open_p_with_different_flag", check, true);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_open_n_reopen
* @brief            open and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open, pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_n_reopen(void)
{
	struct pcm *ptr_pcm = NULL;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	// re-open pcm stream, should not be able to open
	ptr_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_LEQ_CLEANUP("pcm_open", pcm_get_file_descriptor(ptr_pcm), 0, pcm_close(g_pcm); pcm_close(ptr_pcm));
	pcm_close(ptr_pcm);
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_open_n
* @brief            open and initlaize pcm structure with invalid values
* @scenario         Open pcm with invalid device & card value and check its validation
* @apicovered       pcm_open
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_n(void)
{
	struct pcm *ptr_pcm = NULL;

	ptr_pcm = pcm_open(999, 999, PCM_IN, NULL);
	TC_ASSERT_LEQ_CLEANUP("pcm_open", pcm_get_file_descriptor(ptr_pcm), 0, pcm_close(ptr_pcm));
	pcm_close(ptr_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_close_n
* @brief            close opened pcm
* @scenario         close opened pcm with null pcm
* @apicovered       pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_close_n(void)
{
	TC_ASSERT_NEQ("pcm_close", pcm_close(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_is_ready_p
* @brief            open and check pcm is ready or not
* @scenario         Open pcm and check its state
* @apicovered       pcm_is_ready
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void itc_audio_pcm_is_ready_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_is_ready", pcm_is_ready(g_pcm), 1, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_is_ready_n_after_close
* @brief            open and check pcm is ready or not after pcm close
* @scenario         Open and close pcm and check its state
* @apicovered       pcm_is_ready
* @precondition     pcm_open and pcm_close should be called
* @postcondition    NA
*/
static void itc_audio_pcm_is_ready_n_after_close(void)
{
	int ret = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	ret = pcm_is_ready(g_pcm);
	TC_ASSERT_EQ("pcm_is_ready", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_is_ready_n
* @brief            open and check pcm is ready or not
* @scenario         Open pcm and check its state with NULL pcm
* @apicovered       pcm_is_ready
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_is_ready_n(void)
{
	TC_ASSERT_EQ("pcm_is_ready", pcm_is_ready(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_prepare_p
* @brief            open and prepare pcm
* @scenario         Open pcm and prepare
* @apicovered       pcm_prepare
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void itc_audio_pcm_prepare_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_prepare", pcm_prepare(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_prepare_n_after_close
* @brief            open and prepare pcm
* @scenario         Open and close pcm and prepare
* @apicovered       pcm_prepare
* @precondition     pcm_open and pcm_close should be called
* @postcondition    NA
*/
static void itc_audio_pcm_prepare_n_after_close(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);

	TC_ASSERT_NEQ("pcm_prepare", pcm_prepare(g_pcm), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_prepare_n
* @brief            open and prepare pcm
* @scenario         Open pcm and prepare with NULL pcm
* @apicovered       pcm_prepare
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_prepare_n(void)
{
	TC_ASSERT_LT("pcm_prepare", pcm_prepare(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_open_by_name_p
* @brief            open with name and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open_by_name
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_by_name_p(void)
{
	g_pcm = pcm_open_by_name("hw:0,0", PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open_by_name", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm))

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_open_by_name_n
* @brief            open with name and initlaize pcm structure
* @scenario         Open pcm and with invalid name, check its validation
* @apicovered       pcm_open_by_name
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_by_name_n(void)
{
	struct pcm *ptr_pcm = NULL;
	ptr_pcm = pcm_open_by_name(NULL, PCM_IN, NULL);
	TC_ASSERT_EQ_CLEANUP("pcm_open", ptr_pcm, 0, pcm_close(ptr_pcm));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_config_p
* @brief            get configuration values of pcm
* @scenario         get configuration values with pcm structure
* @apicovered       pcm_get_config
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_config_p(void)
{
	const struct pcm_config *config = NULL;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	config = pcm_get_config(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->channels, AUDIO_DEFAULT_CHANNELS, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->format, AUDIO_DEFAULT_FORMAT, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->rate, AUDIO_DEFAULT_RATE, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_config_n_after_close
* @brief            get configuration values of pcm
* @scenario         get configuration values with pcm structure after close
* @apicovered       pcm_get_config
* @precondition     pcm should be opened and closed before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_config_n_after_close(void)
{
	const struct pcm_config *config = NULL;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	config = pcm_get_config(g_pcm);
	TC_ASSERT_EQ("pcm_get_config", config, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_config_n
* @brief            get configuration values of pcm
* @scenario         get configuration values of NULL pcm data
* @apicovered       pcm_get_config
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_config_n(void)
{
	const struct pcm_config *config = pcm_get_config(NULL);
	TC_ASSERT_EQ("pcm_get_config", config, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_channels_p
* @brief            get channels values of pcm
* @scenario         get channels values after pcm_open
* @apicovered       pcm_get_channels
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_channels_p(void)
{
	unsigned int ch = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	ch = pcm_get_channels(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_channels", ch, AUDIO_DEFAULT_CHANNELS, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_channels_n_after_close
* @brief            get channels values of pcm
* @scenario         get channels values after pcm_open and pcm_close
* @apicovered       pcm_get_channels
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_channels_n_after_close(void)
{
	unsigned int ch = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	ch = pcm_get_channels(g_pcm);
	TC_ASSERT_EQ("pcm_get_channels", ch, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_channels_n
* @brief            get channels values of pcm
* @scenario         get channels values with NULL pcm
* @apicovered       pcm_get_channels
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_channels_n(void)
{
	int ch = 0;
	ch = pcm_get_channels(NULL);
	TC_ASSERT_EQ("pcm_get_channels", ch, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_rate_p
* @brief            get bit-rate values of pcm
* @scenario         get bit-rate values after pcm_open
* @apicovered       pcm_get_rate
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_rate_p(void)
{
	int rate = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	rate = pcm_get_rate(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_rate", rate, AUDIO_DEFAULT_RATE, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_rate_n_after_close
* @brief            get bit-rate values of pcm
* @scenario         get bit-rate values after pcm_open and pcm_close
* @apicovered       pcm_get_rate
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_rate_n_after_close(void)
{
	int rate = -1;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	rate = pcm_get_rate(g_pcm);
	TC_ASSERT_EQ("pcm_get_rate", rate, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_rate_n
* @brief            get rate values of pcm
* @scenario         get rate values with NULL pcm
* @apicovered       pcm_get_rate
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_rate_n(void)
{
	int rate = -1;
	rate = pcm_get_rate(NULL);
	TC_ASSERT_EQ("pcm_get_channels", rate, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_format_p
* @brief            get format values of pcm
* @scenario         get format values after pcm_open
* @apicovered       pcm_get_format
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_format_p(void)
{
	enum pcm_format format = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	format = pcm_get_format(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_format", format, AUDIO_DEFAULT_FORMAT, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_format_n_after_close
* @brief            get format values of pcm
* @scenario         get format values after pcm_open and pcm_close
* @apicovered       pcm_get_format
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_format_n_after_close(void)
{
	enum pcm_format format = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	format = pcm_get_format(g_pcm);
	TC_ASSERT_EQ("pcm_get_format", format, PCM_FORMAT_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_format_n
* @brief            get format values of pcm
* @scenario         get format values with NULL pcm
* @apicovered       pcm_get_format
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_format_n(void)
{
	int pcm_format;
	pcm_format = pcm_get_format(NULL);
	TC_ASSERT_EQ("pcm_get_format", pcm_format, PCM_FORMAT_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_file_descriptor_p
* @brief            get file descriptor values of pcm
* @scenario         get file descriptor after pcm_open
* @apicovered       pcm_get_file_descriptor
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_file_descriptor_p(void)
{
	int fd = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);

	fd = pcm_get_file_descriptor(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_file_descriptor", fd, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_file_descriptor_n_after_close
* @brief            get file descriptor values of pcm
* @scenario         get file descriptor after pcm_open and pcm_close
* @apicovered       pcm_get_file_descriptor
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_file_descriptor_n_after_close(void)
{
	int fd = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	fd = pcm_get_file_descriptor(g_pcm);
	TC_ASSERT_LEQ("pcm_get_file_descriptor", fd, -1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_file_descriptor_n
* @brief            get file descriptor values of pcm
* @scenario         get file descriptor with NULL pcm
* @apicovered       pcm_get_file_descriptor
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_file_descriptor_n(void)
{
	int fd;
	fd = pcm_get_file_descriptor(NULL);
	TC_ASSERT_LEQ("pcm_get_file_descriptor", fd, -1)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_error_p
* @brief            get last error of pcm
* @scenario         file open with invalid file path and call pcm_get_error
* @apicovered       pcm_get_error
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_error_p(void)
{
	struct pcm *ptr_pcm = NULL;
	ptr_pcm = pcm_open(0, 0, PCM_IN, NULL);
	const char *errors = pcm_get_error(ptr_pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_error", errors, NULL, pcm_close(ptr_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(ptr_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_error_n
* @brief            get last error of pcm
* @scenario         call pcm_get_error with NULL pcm
* @apicovered       pcm_get_error
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_error_n(void)
{
	TC_ASSERT_EQ("pcm_get_error", pcm_get_error(NULL), NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_buffer_size_p
* @brief            get buffer size of pcm
* @scenario         get buffer size after pcm_open
* @apicovered       pcm_get_buffer_size
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_buffer_size_p(void)
{
	ssize_t size = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_buffer_size_n_after_close
* @brief            get buffer size of pcm
* @scenario         get buffer size after pcm_open and pcm_close
* @apicovered       pcm_get_buffer_size
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_buffer_size_n_after_close(void)
{
	ssize_t size = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_EQ("pcm_get_buffer_size", size, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_buffer_size_n
* @brief            get buffer size of pcm
* @scenario         get buffer size with NULL pcm
* @apicovered       pcm_get_buffer_size
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_get_buffer_size_n(void)
{
	ssize_t size;
	size = pcm_get_buffer_size(NULL);
	TC_ASSERT_EQ("pcm_get_buffer_size", size, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_subdevice_p
* @brief            get subdevice of pcm
* @scenario         get subdevice of pcm
* @apicovered       pcm_get_subdevice
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_get_subdevice_p(void)
{
	unsigned int device = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));
	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_subdevice", device, 1, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_subdevice_n_after_close
* @brief            get subdevice of pcm
* @scenario         get subdevice of pcm after pcm_open and pcm_close
* @apicovered       pcm_get_subdevice
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_get_subdevice_n_after_close(void)
{
	unsigned int device = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);

	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_EQ("pcm_get_subdevice", device, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_get_subdevice_n
* @brief            get subdevice of pcm
* @scenario         get subdevice with NULL pcm
* @apicovered       pcm_get_subdevice
* @precondition     NA.
* @postcondition    NA
*/
static void itc_audio_pcm_get_subdevice_n(void)
{
	unsigned int device;
	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_EQ("pcm_get_subdevice", device, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_frames_to_bytes_p
* @brief            convert frames to bytes
* @scenario         get configuration value of pcm and calculate frame to byte
* @apicovered       pcm_frames_to_bytes
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_frames_to_bytes_p(void)
{
	ssize_t size = 0;
	unsigned int bytes = 0;
	unsigned int frame_size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT_CLEANUP("pcm_bytes_to_frames", frame_size, 0, pcm_close(g_pcm));

	bytes = pcm_frames_to_bytes(g_pcm, frame_size);
	TC_ASSERT_GT_CLEANUP("pcm_frames_to_bytes", bytes, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_frames_to_bytes_n_after_close
* @brief            convert frames to bytes
* @scenario         get configuration value of pcm and calculate frame to byte after pcm close
* @apicovered       pcm_frames_to_bytes
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_frames_to_bytes_n_after_close(void)
{
	ssize_t size = 0;
	unsigned int bytes = 0;
	unsigned int frame_size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT_CLEANUP("pcm_bytes_to_frames", frame_size, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	bytes = pcm_frames_to_bytes(g_pcm, frame_size);
	TC_ASSERT_EQ("pcm_frames_to_bytes", bytes, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_frames_to_bytes_n
* @brief            convert frames to bytes
* @scenario         get configuration value of NULL pcm and calculate frame to byte
* @apicovered       pcm_frames_to_bytes
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_frames_to_bytes_n(void)
{
	unsigned int bytes = 1;
	bytes = pcm_frames_to_bytes(NULL, pcm_get_buffer_size(NULL));
	TC_ASSERT_EQ("pcm_frames_to_bytes", bytes, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_bytes_to_frames_p
* @brief            convert bytes to frame
* @scenario         get configuration value of pcm and calculate byte to frame
* @apicovered       pcm_bytes_to_frames
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_bytes_to_frames_p(void)
{
	unsigned int frame_size = 0;
	ssize_t size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT_CLEANUP("pcm_bytes_to_frames", frame_size, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_bytes_to_frames_n_after_close
* @brief            convert bytes to frame
* @scenario         get configuration value of pcm and calculate byte to frame after pcm close
* @apicovered       pcm_bytes_to_frames
* @precondition     pcm should be opened and closed before
* @postcondition    NA
*/
static void itc_audio_pcm_bytes_to_frames_n_after_close(void)
{
	unsigned int frame_size = 1;
	ssize_t size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	g_pcm = NULL;

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_EQ("pcm_bytes_to_frames", frame_size, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_bytes_to_frames_n
* @brief            convert bytes to frame
* @scenario         get configuration value of NULL pcm and calculate byte to frame
* @apicovered       pcm_bytes_to_frames
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_bytes_to_frames_n(void)
{
	unsigned int frame = 1;
	frame = pcm_bytes_to_frames(NULL, 0);
	TC_ASSERT_EQ("pcm_bytes_to_frames", frame, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_format_to_bits_p
* @brief            get bits information based on pcm's format
* @scenario         get bits of format value of pcm
* @apicovered       pcm_format_to_bits
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_format_to_bits_p(void)
{
	enum pcm_format format = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	format = pcm_get_format(g_pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_format", format, PCM_FORMAT_NONE, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_format_to_bits", pcm_format_to_bits(format), 0, pcm_close(g_pcm));

	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_format_to_bits_n
* @brief            get bits information based on pcm's format
* @scenario         get bits of format value with invalid format
* @apicovered       pcm_format_to_bits
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_format_to_bits_n(void)
{
	TC_ASSERT_EQ("pcm_format_to_bits", pcm_format_to_bits(PCM_FORMAT_NONE), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_readi_p
* @brief            read captured data from chip
* @scenario         record voice and write data in specific file
* @apicovered       pcm_readi
* @precondition     pcm should be opened before
* @postcondition    NA
*/
static void itc_audio_pcm_readi_p(void)
{
	int fd = 0;
	ssize_t size;
	char input_str[INPUT_STR_LEN];
	int ret = 0;
	char *str = NULL;
	char *buffer = NULL;
	unsigned int bytes_per_frame = 0;
	int frames_read = 0;
	int remain = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = (char *)malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, pcm_close(g_pcm));

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT);
	TC_ASSERT_GT_CLEANUP("pcm_readi", fd, 0, clean_all_data(0, buffer));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
	remain = AUDIO_DEFAULT_RATE * AUDIO_RECORD_DURATION;

	printf("Record will be start for 3s, press any key to start(Total frame: %d)\n", remain);
	fflush(stdout);
	str = fgets(input_str, INPUT_STR_LEN - 1, stdin);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", str, NULL, clean_all_data(fd, buffer));

	while (remain > 0) {
		frames_read = pcm_readi(g_pcm, buffer, pcm_get_buffer_size(g_pcm));
		printf("frames_read : %d\n", frames_read);
		if (frames_read < 0) {
			break;
		}
		remain -= frames_read;
		ret = write(fd, buffer, bytes_per_frame * frames_read);
		TC_ASSERT_EQ_CLEANUP("pcm_readi", ret, (bytes_per_frame * frames_read), clean_all_data(fd, buffer));
		if (remain < pcm_get_buffer_size(g_pcm)) {
			break;
		}
	}

	printf("Record done\n");
	sleep(SEC_2);

	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_readi_n
* @brief            read captured data from chip
* @scenario         call readi with invalid values
* @apicovered       pcm_readi
* @precondition     pcm should be opened before
* @postcondition    NA
*/
static void itc_audio_pcm_readi_n(void)
{
	int ret;
	ssize_t size;
	char *buffer = NULL;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_readi", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = (char *)malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, pcm_close(g_pcm));

	ret = pcm_readi(NULL, buffer, size);
	TC_ASSERT_LT_CLEANUP("pcm_readi", ret, 0, clean_all_data(0, buffer));

	ret = pcm_readi(g_pcm, NULL, size);
	TC_ASSERT_LT_CLEANUP("pcm_readi", ret, 0, clean_all_data(0, buffer));

	ret = pcm_readi(g_pcm, buffer, 0);
	TC_ASSERT_LT_CLEANUP("pcm_readi", ret, 0, clean_all_data(0, buffer));

	clean_all_data(0, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_writei_p
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    NA
*/
static void itc_audio_pcm_writei_p(void)
{
	int fd = 0;
	int ret = 0;

	char *buffer = NULL;
	int num_read = 0;
	unsigned int total_frames = 0;
	unsigned int size = 0;
	int retry = PREPARE_RETRY_COUNT;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_writei", pcm_is_ready(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, NULL, pcm_close(g_pcm));

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("pcm_writei", fd, 0, clean_all_data(0, buffer));

	printf("Playback start!!\n");

	do {
		if (retry == PREPARE_RETRY_COUNT) {
			num_read = read(fd, buffer, size);
		}
		if (num_read <= 0) {
			break;
		}
		printf("num_read : %d\n", num_read);
		if (num_read > 0) {
			total_frames += num_read;
			ret = pcm_writei(g_pcm, buffer, pcm_bytes_to_frames(g_pcm, size));
			if (ret == -EPIPE) {
				if (retry--) {
					pcm_prepare(g_pcm);
					continue;
				} else {
					printf("ERROR: writei failed due to multiple xrun \n");
					TC_ASSERT_CLEANUP("pcm_writei", 0, clean_all_data(fd, buffer));
				}
			}
			retry = PREPARE_RETRY_COUNT;
			printf("ret : %d\n", ret);
			TC_ASSERT_GEQ_CLEANUP("pcm_writei", ret, 0, clean_all_data(fd, buffer));
		}
	} while (num_read > 0);

	printf("Playback done! Total Frames: %ul\n", pcm_bytes_to_frames(g_pcm, total_frames));

	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_writei_n
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    NA
*/
static void itc_audio_pcm_writei_n(void)
{
	int ret;
	ssize_t size;
	char *buffer = NULL;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_writei", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm));

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = (char *)malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, NULL, pcm_close(g_pcm));

	ret = pcm_writei(NULL, buffer, size);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	ret = pcm_writei(g_pcm, NULL, size);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	ret = pcm_writei(g_pcm, buffer, 0);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	clean_all_data(0, buffer);
	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_audio_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Audio ITC") == ERROR) {
		return ERROR;
	}

	itc_audio_pcm_open_close_p();
	itc_audio_pcm_open_p_with_different_flag();
	itc_audio_pcm_open_n_reopen();
	itc_audio_pcm_open_n();
	itc_audio_pcm_close_n();
	itc_audio_pcm_is_ready_p();
	itc_audio_pcm_is_ready_n_after_close();
	itc_audio_pcm_is_ready_n();
	itc_audio_pcm_prepare_p();
	itc_audio_pcm_prepare_n_after_close();
	itc_audio_pcm_prepare_n();
	itc_audio_pcm_open_by_name_p();
	itc_audio_pcm_open_by_name_n();
	itc_audio_pcm_get_config_p();
	itc_audio_pcm_get_config_n_after_close();
	itc_audio_pcm_get_config_n();
	itc_audio_pcm_get_channels_p();
	itc_audio_pcm_get_channels_n_after_close();
	itc_audio_pcm_get_channels_n();
	itc_audio_pcm_get_rate_p();
	itc_audio_pcm_get_rate_n_after_close();
	itc_audio_pcm_get_rate_n();
	itc_audio_pcm_get_format_p();
	itc_audio_pcm_get_format_n_after_close();
	itc_audio_pcm_get_format_n();
	itc_audio_pcm_get_file_descriptor_p();
	itc_audio_pcm_get_file_descriptor_n_after_close();
	itc_audio_pcm_get_file_descriptor_n();
	itc_audio_pcm_get_error_p();
	itc_audio_pcm_get_error_n();
	itc_audio_pcm_get_buffer_size_p();
	itc_audio_pcm_get_buffer_size_n_after_close();
	itc_audio_pcm_get_buffer_size_n();
	itc_audio_pcm_get_subdevice_p();
	itc_audio_pcm_get_subdevice_n_after_close();
	itc_audio_pcm_get_subdevice_n();
	itc_audio_pcm_frames_to_bytes_p();
	itc_audio_pcm_frames_to_bytes_n_after_close();
	itc_audio_pcm_frames_to_bytes_n();
	itc_audio_pcm_bytes_to_frames_p();
	itc_audio_pcm_bytes_to_frames_n_after_close();
	itc_audio_pcm_bytes_to_frames_n();
	itc_audio_pcm_format_to_bits_p();
	itc_audio_pcm_format_to_bits_n();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	itc_audio_pcm_readi_p();
	itc_audio_pcm_readi_n();
	itc_audio_pcm_writei_p();
	itc_audio_pcm_writei_n();
#endif
	/* after test, unlink the file */
	unlink(AUDIO_TEST_FILE);

	(void)tc_handler(TC_END, "Audio ITC");

	return 0;
}
