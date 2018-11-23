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
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>
#include <tinyara/fs/ioctl.h>
#include "tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define AUDIO_TEST_FILE "/tmp/pcm"

#define AUDIO_DEFAULT_CHANNELS 2

#define AUDIO_DEFAULT_RATE 16000

#define AUDIO_DEFAULT_FORMAT PCM_FORMAT_S16_LE

#define AUDIO_RECORD_DURATION 3	//3sec

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
		g_pcm = NULL;
	}

	if (buffer) {
		free(buffer);
	}
}

/**
* @testcase         audio_pcm_open_p
* @brief            open and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_open_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_open_n
* @brief            open and initlaize pcm structure with invalid values
* @scenario         Open pcm with invalid device & card value and check its validation
* @apicovered       pcm_open
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_open_n(void)
{
	struct pcm *pcm;
	pcm = pcm_open(999, 999, PCM_IN, NULL);
	TC_ASSERT_LEQ_CLEANUP("pcm_open", pcm_get_file_descriptor(pcm), 0, pcm_close(pcm));
	pcm_close(pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_is_ready_p
* @brief            open and check pcm is ready or not
* @scenario         Open pcm and check its state
* @apicovered       pcm_is_ready
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void utc_audio_pcm_is_ready_p(void)
{
	TC_ASSERT_GT("pcm_is_ready", pcm_is_ready(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_is_ready_n
* @brief            open and check pcm is ready or not
* @scenario         Open pcm and check its state with NULL pcm
* @apicovered       pcm_is_ready
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void utc_audio_pcm_is_ready_n(void)
{
	TC_ASSERT_LEQ("pcm_is_ready", pcm_is_ready(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_prepare_p
* @brief            open and prepare pcm
* @scenario         Open pcm and prepare
* @apicovered       pcm_prepare
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void utc_audio_pcm_prepare_p(void)
{
	TC_ASSERT_EQ("pcm_prepare", pcm_prepare(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_prepare_n
* @brief            open and prepare pcm
* @scenario         Open pcm and prepare with NULL pcm
* @apicovered       pcm_prepare
* @precondition     pcm_open should be called
* @postcondition    NA
*/
static void utc_audio_pcm_prepare_n(void)
{
	TC_ASSERT_LT("pcm_prepare", pcm_prepare(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_close_p
* @brief            close opened pcm
* @scenario         close opened pcm
* @apicovered       pcm_close
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_close_p(void)
{
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_close_n
* @brief            close opened pcm
* @scenario         close opened pcm with null pcm
* @apicovered       pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_close_n(void)
{
	TC_ASSERT_NEQ("pcm_close", pcm_close(NULL), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_open_by_name_p
* @brief            open with name and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open_by_name
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_open_by_name_p(void)
{
	g_pcm = pcm_open_by_name("hw:0,0", PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open_by_name", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm))
	pcm_close(g_pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_open_by_name_n
* @brief            open with name and initlaize pcm structure
* @scenario         Open pcm and with invalid name, check its validation
* @apicovered       pcm_open_by_name
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_open_by_name_n(void)
{
	struct pcm *pcm;
	pcm = pcm_open_by_name(NULL, PCM_IN, NULL);
	TC_ASSERT_LEQ_CLEANUP("pcm_open", pcm_get_file_descriptor(pcm), 0, pcm_close(pcm))
	pcm_close(pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_config_p
* @brief            get configuration values of pcm
* @scenario         get configuration values with pcm structure
* @apicovered       pcm_get_config
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_config_p(void)
{
	const struct pcm_config *config;
	/* open g_pcm again to check config test */
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	config = pcm_get_config(g_pcm);
	TC_ASSERT_EQ("pcm_get_config", config->channels, AUDIO_DEFAULT_CHANNELS);
	TC_ASSERT_EQ("pcm_get_config", config->format, AUDIO_DEFAULT_FORMAT);
	TC_ASSERT_EQ("pcm_get_config", config->rate, AUDIO_DEFAULT_RATE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_config_n
* @brief            get configuration values of pcm
* @scenario         get configuration values of NULL pcm data
* @apicovered       pcm_get_config
* @precondition     NA.
* @postcondition    NA
*/
static void utc_audio_pcm_get_config_n(void)
{
	const struct pcm_config *config = pcm_get_config(NULL);
	TC_ASSERT_EQ("pcm_get_config", config, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_channels_p
* @brief            get channels values of pcm
* @scenario         get channels values after pcm_open
* @apicovered       pcm_get_channels
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_channels_p(void)
{
	unsigned int ch;
	ch = pcm_get_channels(g_pcm);
	TC_ASSERT_EQ("pcm_get_channels", ch, AUDIO_DEFAULT_CHANNELS);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_channels_n
* @brief            get channels values of pcm
* @scenario         get channels values with NULL pcm
* @apicovered       pcm_get_channels
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_channels_n(void)
{
	int ch;
	ch = pcm_get_channels(NULL);
	TC_ASSERT_EQ("pcm_get_channels", ch, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_rate_p
* @brief            get bit-rate values of pcm
* @scenario         get bit-rate values after pcm_open
* @apicovered       pcm_get_rate
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_rate_p(void)
{
	int rate;
	rate = pcm_get_rate(g_pcm);
	TC_ASSERT_EQ("pcm_get_rate", rate, AUDIO_DEFAULT_RATE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_rate_n
* @brief            get rate values of pcm
* @scenario         get rate values with NULL pcm
* @apicovered       pcm_get_rate
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_rate_n(void)
{
	int rate;
	rate = pcm_get_rate(NULL);
	TC_ASSERT_EQ("pcm_get_channels", rate, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_format_p
* @brief            get format values of pcm
* @scenario         get format values after pcm_open
* @apicovered       pcm_get_format
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_format_p(void)
{
	int format;
	format = pcm_get_format(g_pcm);
	TC_ASSERT_EQ("pcm_get_format", format, AUDIO_DEFAULT_FORMAT);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_format_n
* @brief            get format values of pcm
* @scenario         get format values with NULL pcm
* @apicovered       pcm_get_format
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_format_n(void)
{
	int pcm_format;
	pcm_format = pcm_get_format(NULL);
	TC_ASSERT_EQ("pcm_get_format", pcm_format, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_file_descriptor_p
* @brief            get file descriptor values of pcm
* @scenario         get file descriptor after pcm_open
* @apicovered       pcm_get_file_descriptor
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_file_descriptor_p(void)
{
	int fd;

	fd = pcm_get_file_descriptor(g_pcm);
	/* Now we tested all config values of g_pcm, close here */
	TC_ASSERT_GT_CLEANUP("pcm_get_file_descriptor", fd, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_file_descriptor_n
* @brief            get file descriptor values of pcm
* @scenario         get file descriptor with NULL pcm
* @apicovered       pcm_get_file_descriptor
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_file_descriptor_n(void)
{
	int fd;
	fd = pcm_get_file_descriptor(NULL);
	TC_ASSERT_LEQ("pcm_get_file_descriptor", fd, -1)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_error_p
* @brief            get last error of pcm
* @scenario         file open with invalid file path and call pcm_get_error
* @apicovered       pcm_get_error
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_error_p(void)
{
	struct pcm *pcm;
	pcm = pcm_open(0, 0, PCM_IN, NULL);
	const char *errors = pcm_get_error(pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_error", errors, NULL, pcm_close(pcm));
	pcm_close(pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_error_n
* @brief            get last error of pcm
* @scenario         call pcm_get_error with NULL pcm
* @apicovered       pcm_get_error
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_error_n(void)
{
	TC_ASSERT_EQ("pcm_get_error", pcm_get_error(NULL), NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_buffer_size_p
* @brief            get buffer size of pcm
* @scenario         get buffer size after pcm_open
* @apicovered       pcm_get_buffer_size
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_buffer_size_p(void)
{
	ssize_t size;
	/* Open again to test APIs regarding buffering & recoridng & playing */
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT("pcm_get_buffer_size", size, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_buffer_size_n
* @brief            get buffer size of pcm
* @scenario         get buffer size with NULL pcm
* @apicovered       pcm_get_buffer_size
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_get_buffer_size_n(void)
{
	ssize_t size;
	size = pcm_get_buffer_size(NULL);
	TC_ASSERT_EQ("pcm_get_buffer_size", size, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_subdevice_p
* @brief            get subdevice of pcm
* @scenario         get subdevice of pcm
* @apicovered       pcm_get_subdevice
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_get_subdevice_p(void)
{
	unsigned int device;
	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_NEQ("pcm_get_subdevice", device, 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_get_subdevice_n
* @brief            get subdevice of pcm
* @scenario         get subdevice with NULL pcm
* @apicovered       pcm_get_subdevice
* @precondition     NA.
* @postcondition    NA
*/
static void utc_audio_pcm_get_subdevice_n(void)
{
	unsigned int device;
	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_EQ("pcm_get_subdevice", device, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_frame_to_bytes_p
* @brief            convert frames to bytes
* @scenario         get configuration value of pcm and calculate frame to byte
* @apicovered       pcm_frames_to_bytes
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_frames_to_bytes_p(void)
{
	ssize_t size;
	unsigned int bytes;
	unsigned int frame_size;

	/* set basic configuration values for next test */
	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT("pcm_get_buffer_size", size, 0);

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT("pcm_bytes_to_frames", frame_size, 0);

	bytes = pcm_frames_to_bytes(g_pcm, frame_size);
	TC_ASSERT_GT("pcm_frames_to_bytes", bytes, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_frame_to_bytes_n
* @brief            convert frames to bytes
* @scenario         get configuration value of NULL pcm and calculate frame to byte
* @apicovered       pcm_frames_to_bytes
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_frames_to_bytes_n(void)
{
	unsigned int bytes;
	bytes = pcm_frames_to_bytes(NULL, pcm_get_buffer_size(g_pcm));
	TC_ASSERT_EQ("pcm_frames_to_bytes", bytes, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_bytes_to_frames_p
* @brief            convert bytes to frame
* @scenario         get configuration value of pcm and calculate byte to frame
* @apicovered       pcm_bytes_to_frames
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_bytes_to_frames_p(void)
{
	ssize_t size;
	unsigned int frame_size;

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT("pcm_get_buffer_size", size, 0);

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT("pcm_bytes_to_frame", frame_size, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_bytes_to_frames_n
* @brief            convert bytes to frame
* @scenario         get configuration value of NULL pcm and calculate byte to frame
* @apicovered       pcm_bytes_to_frames
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_bytes_to_frames_n(void)
{
	unsigned int frame;
	frame = pcm_bytes_to_frames(NULL, 0);
	TC_ASSERT_LEQ("pcm_bytes_to_frames", frame, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         utc_audio_pcm_format_to_bits_p
* @brief            get bits information based on pcm's format
* @scenario         get bits of format value of pcm
* @apicovered       pcm_format_to_bits
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void utc_audio_pcm_format_to_bits_p(void)
{
	enum pcm_format format;
	format = pcm_get_format(g_pcm);
	TC_ASSERT_NEQ("pcm_format_to_bits", format, PCM_FORMAT_NONE);
	TC_ASSERT_NEQ("pcm_format_to_bits", pcm_format_to_bits(format), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         utc_audio_pcm_format_to_bits_n
* @brief            get bits information based on pcm's format
* @scenario        get bits of format value with invalid format
* @apicovered       pcm_format_to_bits
* @precondition     NA
* @postcondition    NA
*/
static void utc_audio_pcm_format_to_bits_n(void)
{
	TC_ASSERT_EQ("pcm_format_to_bits", pcm_format_to_bits(PCM_FORMAT_NONE), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_readi_p
* @brief            read captured data from chip
* @scenario         record voice and write data in specific file
* @apicovered       pcm_readi
* @precondition     pcm should be opened before
* @postcondition    NA
*/
static void utc_audio_pcm_readi_p(void)
{
	int fd;
	char input_str[16];
	int ret;
	char *str;
	char *buffer;
	unsigned int bytes_per_frame;
	int frames_read;
	int remain;
	int retry = PREPARE_RETRY_COUNT;

	buffer = malloc(pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm)));
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("pcm_readi", fd, 0, clean_all_data(0, buffer));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
	frames_read = 0;
	remain = AUDIO_DEFAULT_RATE * AUDIO_RECORD_DURATION;

	printf("Sample will be recorded for 3s, press any key to start(Total frame: %d)\n", remain);
	fflush(stdout);
	str = gets(input_str);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", str, NULL, clean_all_data(fd, buffer));

	while (remain > 0) {
		frames_read = pcm_readi(g_pcm, buffer, pcm_get_buffer_size(g_pcm));
		if (frames_read == -EPIPE) {
			if (retry--) {
				pcm_prepare(g_pcm);
				continue;
			} else {
				printf("ERROR: read failed due to multiple xrun \n");
				TC_ASSERT_CLEANUP("pcm_readi", 0, clean_all_data(fd, buffer));
			}
		}

		if (frames_read < 0) {
			break;
		}

		retry = PREPARE_RETRY_COUNT;
		remain -= frames_read;
		ret = write(fd, buffer, bytes_per_frame * frames_read);
		TC_ASSERT_EQ_CLEANUP("pcm_readi", ret, (bytes_per_frame * frames_read), clean_all_data(fd, buffer));
		if (remain < pcm_get_buffer_size(g_pcm)) {
			break;
		}
	}

	printf("Record done.\n");
	sleep(2);
	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_readi_n
* @brief            read captured data from chip
* @scenario         call readi with invalid values
* @apicovered       pcm_readi
* @precondition     pcm should be opened before
* @postcondition    NA
*/
static void utc_audio_pcm_readi_n(void)
{
	int ret;
	ssize_t size;
	char *buffer;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_readi", pcm_get_file_descriptor(g_pcm), 0);

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = (char *)malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, clean_all_data(0, NULL));

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
* @testcase         audio_pcm_drop_p
* @brief            drop all the buffers which are being processed and stop the device
* @scenario         call pcm_drop after readi and check that audio until pcm_drop print is recorded.
* @apicovered       pcm_drop
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_drop_p(void)
{
	int fd;
	char input_str[16];
	int ret;
	char *str;
	char *buffer;
	unsigned int bytes_per_frame;
	int frames_read;
	int remain;
	int size;
	int total_frames;
	int num_read;
	int retry = PREPARE_RETRY_COUNT;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_drop", pcm_is_ready(g_pcm), 0);

	buffer = malloc(pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm)));
	TC_ASSERT_NEQ_CLEANUP("pcm_drop", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("pcm_drop", fd, 0, clean_all_data(0, buffer));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
	frames_read = 0;
	remain = AUDIO_DEFAULT_RATE * AUDIO_RECORD_DURATION;

	printf("Sample will be recorded for 3s, press any key to start(Total frame: %d)\n", remain);
	fflush(stdout);
	str = gets(input_str);
	TC_ASSERT_NEQ_CLEANUP("pcm_drop", str, NULL, clean_all_data(fd, buffer));

	while (remain > 0) {
		frames_read = pcm_readi(g_pcm, buffer, pcm_get_buffer_size(g_pcm));
		if (frames_read == -EPIPE) {
			if (retry--) {
				pcm_prepare(g_pcm);
				continue;
			} else {
				printf("ERROR: readi failed due to multiple xrun \n");
				TC_ASSERT_CLEANUP("pcm_drop", 0, clean_all_data(fd, buffer));
			}
		}
		if (frames_read < 0) {
			break;
		}

		retry = PREPARE_RETRY_COUNT;
		remain -= frames_read;
		ret = write(fd, buffer, bytes_per_frame * frames_read);
		TC_ASSERT_EQ_CLEANUP("pcm_drop", ret, (bytes_per_frame * frames_read), clean_all_data(fd, buffer));
	}

	printf("Nothing after this statement should be recorded\n");
	ret = pcm_drop(g_pcm);
	TC_ASSERT_GEQ_CLEANUP("pcm_drop", ret, 0, clean_all_data(fd, buffer));

	printf("Record dropped.\n");

	clean_all_data(fd, buffer);

	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_drop", pcm_is_ready(g_pcm), 0);

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_drop", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("pcm_drop", fd, 0, clean_all_data(0, buffer));

	printf("Check now for recorded part. Nothing after drop should be heard\n");
	total_frames = 0;
	retry = PREPARE_RETRY_COUNT;
	do {
		if (retry == PREPARE_RETRY_COUNT) {
			num_read = read(fd, buffer, size);
		}
		if (num_read > 0) {
			ret = pcm_writei(g_pcm, buffer, pcm_bytes_to_frames(g_pcm, num_read));
			if (ret == -EPIPE) {
				if (retry--) {
					pcm_prepare(g_pcm);
					continue;
				} else {
					printf("ERROR: writei failed due to multiple xrun \n");
					TC_ASSERT_CLEANUP("pcm_drop", 0, clean_all_data(fd, buffer));
				}
			}

			retry = PREPARE_RETRY_COUNT;
			total_frames += num_read;
			TC_ASSERT_GEQ_CLEANUP("pcm_drop", ret, 0, clean_all_data(fd, buffer));
		}
	} while (num_read > 0);

	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_drop_n
* @brief            drop all the buffers which are being processed and stop the device
* @scenario         call drop when device is stopped (not started)
* @apicovered       pcm_drop
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_drop_n(void)
{
	int ret;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_drop", pcm_is_ready(g_pcm), 0);

	ret = pcm_drop(g_pcm);
	TC_ASSERT_LT_CLEANUP("pcm_drop", ret, 0, clean_all_data(0, NULL));

	clean_all_data(0, NULL);
	TC_SUCCESS_RESULT();
}


/**
* @testcase         audio_pcm_write_p
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    audio_pcm_drain_p should be called to ensure clean exit 
*/
static void utc_audio_pcm_writei_p(void)
{
	int fd;
	int ret;

	char *buffer;
	int num_read;
	unsigned int size;
	int retry = PREPARE_RETRY_COUNT;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_writei", pcm_is_ready(g_pcm), 0);

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("pcm_writei", fd, 0, clean_all_data(0, buffer));

	printf("Playback start!!\n");

	for (;;) {
		if (retry == PREPARE_RETRY_COUNT) {
			num_read = read(fd, buffer, size);
		}

		if (num_read <= 0) {
			break;
		}
		ret = pcm_writei(g_pcm, buffer, pcm_bytes_to_frames(g_pcm, num_read));
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
		TC_ASSERT_GEQ_CLEANUP("pcm_writei", ret, 0, clean_all_data(fd, buffer));
	}

	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}
	if (fd > 0) {
		close(fd);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_write_n
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    NA
*/
static void utc_audio_pcm_writei_n(void)
{
	int ret;
	ssize_t size;
	char *buffer;

	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_writei", pcm_get_file_descriptor(g_pcm), 0);

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = (char *)malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, 0, clean_all_data(0, NULL));

	ret = pcm_writei(NULL, buffer, size);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	ret = pcm_writei(g_pcm, NULL, size);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	ret = pcm_writei(g_pcm, buffer, 0);
	TC_ASSERT_LT_CLEANUP("pcm_writei", ret, 0, clean_all_data(0, buffer));

	clean_all_data(0, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_drain_p
* @brief            play/record all enqueued buffers and stop the device
* @scenario         play recored file previously completely
* @apicovered       pcm_drain
* @precondition     audio_pcm_writei_p should be executed just before this function
* @postcondition    NA
*/

static void utc_audio_pcm_drain_p(void)
{
	/* Executed after writei positive tc */
	int ret;

	printf("Draining buffers to complete playback and close device\n");
	ret = pcm_drain(g_pcm);
	TC_ASSERT_CLEANUP("pcm_drain", ret >= 0 || ret == -EPIPE, clean_all_data(0, NULL));
	printf("Playback done!\n");

	clean_all_data(0, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase         audio_pcm_drain_n
* @brief            play/record all enqueued buffers and stop the device
* @scenario         drain when device is stopped (not started)
* @apicovered       pcm_drain
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_drain_n(void)
{
	int ret;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_drain", pcm_is_ready(g_pcm), 0);

	ret = pcm_start(g_pcm);
	TC_ASSERT_GEQ_CLEANUP("pcm_drain", ret, 0, clean_all_data(0, NULL));

	ret = pcm_stop(g_pcm);
	TC_ASSERT_GEQ_CLEANUP("pcm_drain", ret, 0, clean_all_data(0, NULL));

	ret = pcm_drain(g_pcm);
	TC_ASSERT_LT_CLEANUP("pcm_drain", ret, 0, clean_all_data(0, NULL));

	clean_all_data(0, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_avail_update_n
* @brief            get the number of frames available for read or write operation
* @scenario         get avail frames when devices is stopped or not opened or not opened with MMAP flag
* @apicovered       pcm_avail_update
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_avail_update_n(void)
{
	int ret;

	ret = pcm_avail_update(NULL);
	TC_ASSERT_LT("pcm_avail_update", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_avail_update", pcm_is_ready(g_pcm), 0);
	ret = pcm_avail_update(g_pcm);
	TC_ASSERT_LT_CLEANUP("pcm_avail_update", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_avail_update", pcm_is_ready(g_pcm), 0);
	ret = pcm_avail_update(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_avail_update", ret, 0, pcm_close(g_pcm));

	pcm_close(g_pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_avail_update_p
* @brief            get the number of frames available for read or write operation
* @scenario         get avail frames when devices is stopped or not opened or not opened with MMAP flag
* @apicovered       pcm_avail_update
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_avail_update_p(void)
{
	int ret;
	unsigned int retry = PREPARE_RETRY_COUNT;

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_avail_update", pcm_is_ready(g_pcm), 0);

	do {
		ret = pcm_wait(g_pcm, -1);
		TC_ASSERT_CLEANUP("pcm_avail_update", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));
	} while (ret == -EPIPE && retry--);

	TC_ASSERT_GT_CLEANUP("pcm_avail_update", ret, 0, pcm_close(g_pcm));

	ret = pcm_avail_update(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_avail_update", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_avail_update", pcm_is_ready(g_pcm), 0);

	ret = pcm_avail_update(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_avail_update", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_wait_n
* @brief            wait for data buffer to become available in MMAP mode
* @scenario         wait for data when pcm is not opened or not opened in MMAP mode or not started
* @apicovered       pcm_wait
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_wait_n(void)
{
	int ret;

	ret = pcm_wait(NULL, 0);
	TC_ASSERT_LT("pcm_wait", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_wait", pcm_is_ready(g_pcm), 0);
	ret = pcm_wait(g_pcm, 0);
	TC_ASSERT_LT_CLEANUP("pcm_wait", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_wait_p
* @brief            wait for data buffer to become available in MMAP mode
* @scenario         wait for data when pcm is not opened or not opened in MMAP mode or not started
* @apicovered       pcm_wait
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_wait_p(void)
{
	int ret;

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_wait", pcm_is_ready(g_pcm), 0);

	ret = pcm_wait(g_pcm, -1);
	TC_ASSERT_CLEANUP("pcm_wait", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));

	sleep(1);
	ret = pcm_wait(g_pcm, -1);
	TC_ASSERT_CLEANUP("pcm_wait", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));

	ret = pcm_drain(g_pcm);
	TC_ASSERT_GEQ_CLEANUP("pcm_drain", ret, 0, pcm_close(g_pcm));

	ret = pcm_wait(g_pcm, -1);
	TC_ASSERT_CLEANUP("pcm_wait", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));

	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_begin_n
* @brief            get the next mmap buffer along with its size and offset
* @scenario         get next buffer when pcm is not opened or opened without MMAP flag or passing null pointers for output params
* @apicovered       pcm_mmap_begin
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_begin_n(void)
{
	int ret;
	char *areas;
	unsigned int offset;
	unsigned int frames;

	ret = pcm_mmap_begin(NULL, (void **)&areas, &offset, &frames);
	TC_ASSERT_LT("pcm_mmap_begin", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_mmap_begin", pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_begin", pcm_is_ready(g_pcm), 0);

	ret = pcm_mmap_begin(g_pcm, NULL, &offset, &frames);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, NULL, &frames);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, NULL);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));

	pcm_close(g_pcm);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_begin_p
* @brief            get the next mmap buffer along with its size and offset
* @scenario         try to get next buffer
* @apicovered       pcm_mmap_begin
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_begin_p(void)
{
	int ret;
	char *areas = NULL;
	unsigned int offset;
	unsigned int frames;
	unsigned int retry = PREPARE_RETRY_COUNT;

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_begin",  pcm_is_ready(g_pcm), 0);

	do {
		ret = pcm_wait(g_pcm, -1);
		TC_ASSERT_CLEANUP("pcm_mmap_begin", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));
	} while (ret == -EPIPE && retry--);

	TC_ASSERT_GT_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_begin", areas, NULL, pcm_close(g_pcm));
	TC_ASSERT_GT_CLEANUP("pcm_mmap_begin", frames, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_begin",  pcm_is_ready(g_pcm), 0);

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_begin", ret, 0, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_begin", areas, NULL, pcm_close(g_pcm));
	TC_ASSERT_GT_CLEANUP("pcm_mmap_begin", frames, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_commit_n
* @brief            return a mmap buffer after read / write
* @scenario         try to return a buffer when pcm is not open or opened without MMAP flag
* @apicovered       pcm_mmap_commit
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_commit_n(void)
{
	int ret;

	ret = pcm_mmap_commit(NULL, 0, 0);
	TC_ASSERT_LT("pcm_mmap_commit", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_mmap_commit", pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_commit(g_pcm, 0, 0);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));

	pcm_close(g_pcm);
	TC_SUCCESS_RESULT();
}


/**
* @testcase         audio_pcm_commit_p
* @brief            return a mmap buffer after read / write
* @scenario         try to return a buffer
* @apicovered       pcm_mmap_commit
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_commit_p(void)
{
	int ret;
	char *areas = NULL;
	unsigned int offset;
	unsigned int frames;
	unsigned int retry = PREPARE_RETRY_COUNT;

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_commit",  pcm_is_ready(g_pcm), 0);

	do {
		ret = pcm_wait(g_pcm, -1);
		TC_ASSERT_CLEANUP("pcm_mmap_commit", ret > 0 || ret == -EPIPE, pcm_close(g_pcm));
	} while (ret == -EPIPE && retry--);

	TC_ASSERT_GT_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_commit", areas, NULL, pcm_close(g_pcm));
	TC_ASSERT_GT_CLEANUP("pcm_mmap_commit", frames, 0, pcm_close(g_pcm));

	ret = pcm_mmap_commit(g_pcm, offset, 10);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_commit", areas, NULL, pcm_close(g_pcm));
	TC_ASSERT_GT_CLEANUP("pcm_mmap_commit", frames, 0, pcm_close(g_pcm));
	ret = pcm_mmap_commit(g_pcm, offset, frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));

	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_commit",  pcm_is_ready(g_pcm), 0);

	ret = pcm_mmap_begin(g_pcm, (void **)&areas, &offset, &frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_commit", areas, NULL, pcm_close(g_pcm));
	TC_ASSERT_GT_CLEANUP("pcm_mmap_commit", frames, 0, pcm_close(g_pcm));
	ret = pcm_mmap_commit(g_pcm, offset, frames);
	TC_ASSERT_EQ_CLEANUP("pcm_mmap_commit", ret, 0, pcm_close(g_pcm));

	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_read_n
* @brief            read predetermined bytes of data from pcm and store in a file
* @scenario         open pcm in mmap mode and read some data and store it in a file
* @apicovered       pcm_mmap_read
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_mmap_read_n(void)
{
	int ret;
	char buf[10];

	ret = pcm_mmap_read(NULL, buf, 10);
	TC_ASSERT_LT("pcm_mmap_read", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_read",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_read(g_pcm, NULL, 0);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_read", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_mmap_read",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_read(g_pcm, buf, 10);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_read", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_mmap_read",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_read(g_pcm, buf, 10);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_read", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_read_p
* @brief            read predetermined bytes of data from pcm and store in a file
* @scenario         open pcm in mmap mode and read some data and store it in a file
* @apicovered       pcm_mmap_read
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_mmap_read_p(void)
{
	int fd;
	char input_str[16];
	int ret;
	char *str;
	unsigned int bytes_per_frame;
	int remain;
	char *buffer;
	int numread;
	int buflen;

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ("pcm_mmap_read", fd, 0);

	g_pcm = pcm_open(0, 0, PCM_IN | PCM_MMAP, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_mmap_read", pcm_is_ready(g_pcm), 0, clean_all_data(fd, NULL));

	buflen = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(buflen);
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_read", buffer, NULL, clean_all_data(fd, NULL));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
	remain = AUDIO_DEFAULT_RATE * AUDIO_RECORD_DURATION;

	printf("Sample will be recorded for 3s, press any key to start(Total frame: %d)\n", remain);
	fflush(stdout);
	str = gets(input_str);
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_read", str, NULL, clean_all_data(fd, buffer));

	while (remain > 0) {
		numread = pcm_mmap_read(g_pcm, buffer, buflen);
		TC_ASSERT_CLEANUP("pcm_mmap_read", numread >= 0 || numread == -EPIPE, clean_all_data(fd, buffer));

		if (numread == -EPIPE) {
			continue;
		}

		remain -= numread;
		ret = write(fd, buffer, bytes_per_frame * numread);
		TC_ASSERT_EQ_CLEANUP("pcm_mmap_read", ret, (bytes_per_frame * numread), clean_all_data(fd, buffer));
	}

	printf("Record done.\n");

	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_write_n
* @brief            write some predetermined amout of data from file to pcm
* @scenario         open a pcm with MMAP flag and write data using mmap mode api's
* @apicovered       pcm_mmap_write
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_mmap_write_n(void)
{
	int ret;
	char buf[10];

	ret = pcm_mmap_write(NULL, buf, 10);
	TC_ASSERT_LT("pcm_mmap_write", ret, 0);

	g_pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, NULL);
	TC_ASSERT_GT("pcm_mmap_write",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_write(g_pcm, NULL, 0);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_write", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_mmap_write",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_write(g_pcm, buf, 10);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_write", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_mmap_write",  pcm_is_ready(g_pcm), 0);
	ret = pcm_mmap_write(g_pcm, buf, 10);
	TC_ASSERT_LT_CLEANUP("pcm_mmap_write", ret, 0, pcm_close(g_pcm));
	pcm_close(g_pcm);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         audio_pcm_write_p
* @brief            write some predetermined amout of data from file to pcm
* @scenario         open a pcm with MMAP flag and write data using mmap mode api's
* @apicovered       pcm_mmap_write
* @precondition     NA
* @postcondition    NA
*/

static void utc_audio_pcm_mmap_write_p(void)
{
	int fd;
	int ret;
	char *buffer;
	int num_read;
	unsigned int size;

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ("pcm_mmap_write", fd, 0);

	g_pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_mmap_write", pcm_is_ready(g_pcm), 0, clean_all_data(fd, NULL));

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_mmap_write", buffer, NULL, clean_all_data(0, NULL));

	printf("Playback start.\n");
	do {
		num_read = read(fd, buffer, size);
		if (num_read > 0) {
			ret = pcm_mmap_write(g_pcm, buffer, num_read);
			TC_ASSERT_CLEANUP("pcm_mmap_write", ret >= 0 || ret == -EPIPE, clean_all_data(fd, buffer));
		}
	} while (num_read > 0);

	printf("Playback done.\n");

	clean_all_data(fd, buffer);
	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_audio_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Audio UTC") == ERROR) {
		return ERROR;
	}

	utc_audio_pcm_open_p();
	utc_audio_pcm_open_n();
	utc_audio_pcm_is_ready_p();
	utc_audio_pcm_is_ready_n();
	utc_audio_pcm_prepare_p();
	utc_audio_pcm_prepare_n();
	utc_audio_pcm_close_p();
	utc_audio_pcm_close_n();
	utc_audio_pcm_open_by_name_p();
	utc_audio_pcm_open_by_name_n();
	utc_audio_pcm_get_config_p();
	utc_audio_pcm_get_config_n();
	utc_audio_pcm_get_channels_p();
	utc_audio_pcm_get_channels_n();
	utc_audio_pcm_get_rate_p();
	utc_audio_pcm_get_rate_n();
	utc_audio_pcm_get_format_p();
	utc_audio_pcm_get_format_n();
	utc_audio_pcm_get_file_descriptor_p();
	utc_audio_pcm_get_file_descriptor_n();
	utc_audio_pcm_get_error_p();
	utc_audio_pcm_get_error_n();
	utc_audio_pcm_get_buffer_size_p();
	utc_audio_pcm_get_buffer_size_n();
	utc_audio_pcm_get_subdevice_p();
	utc_audio_pcm_get_subdevice_n();
	utc_audio_pcm_frames_to_bytes_p();
	utc_audio_pcm_frames_to_bytes_n();
	utc_audio_pcm_bytes_to_frames_p();
	utc_audio_pcm_bytes_to_frames_n();
	utc_audio_pcm_format_to_bits_p();
	utc_audio_pcm_format_to_bits_n();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	utc_audio_pcm_readi_p();
	utc_audio_pcm_readi_n();

	/* writei_p and drain_p should be executed together since drain needs writei for testing.
	  drain_p includes the cleanup part needed by writei to exit cleanly 
	*/
	utc_audio_pcm_writei_p();
	utc_audio_pcm_drain_p();
	utc_audio_pcm_writei_n();
	utc_audio_pcm_drain_n();
	utc_audio_pcm_drop_p();
	utc_audio_pcm_drop_n();

	/* Testcases for mmap API's */
	utc_audio_pcm_avail_update_n();
	utc_audio_pcm_wait_n();
	utc_audio_pcm_begin_n();
	utc_audio_pcm_commit_n();
	utc_audio_pcm_mmap_read_n();
	utc_audio_pcm_mmap_write_n();
	utc_audio_pcm_avail_update_p();
	utc_audio_pcm_wait_p();
	utc_audio_pcm_begin_p();
	utc_audio_pcm_commit_p();
	utc_audio_pcm_mmap_read_p();
	utc_audio_pcm_mmap_write_p();
#endif
	clean_all_data(0, NULL);
	/* after test, unlink the file */
	unlink(AUDIO_TEST_FILE);

	(void)tc_handler(TC_END, "Audio UTC");

	return 0;
}
