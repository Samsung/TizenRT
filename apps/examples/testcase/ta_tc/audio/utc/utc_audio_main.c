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
#define AUDIO_TEST_FILE "/mnt/pcm"

#define AUDIO_DEFAULT_CHANNELS 2

#define AUDIO_DEFAULT_RATE 16000

#define AUDIO_DEFAULT_FORMAT PCM_FORMAT_S16_LE

#define AUDIO_RECORD_DURATION 3	//3sec
/****************************************************************************
 * Global Variables
 ****************************************************************************/

extern sem_t tc_sem;
extern int working_tc;

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

	buffer = malloc(pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm)));
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, clean_all_data(0, 0));

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT);
	TC_ASSERT_GT_CLEANUP("pcm_readi", fd, 0, clean_all_data(0, buffer));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
	frames_read = 0;
	remain = AUDIO_DEFAULT_RATE * AUDIO_RECORD_DURATION;

	printf("Record will be start for 3s, press any key to start(Total frame: %d)\n", remain);
	fflush(stdout);
	str = gets(input_str);
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", str, NULL, clean_all_data(fd, buffer));

	while (remain > 0) {
		frames_read = pcm_readi(g_pcm, buffer, remain);
		if (frames_read < 0) {
			break;
		}
		remain -= frames_read;
		ret = write(fd, buffer, bytes_per_frame * frames_read);
		TC_ASSERT_EQ_CLEANUP("pcm_readi", ret, (bytes_per_frame * frames_read), clean_all_data(fd, buffer));
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
	TC_ASSERT_GT_CLEANUP("pcm_readi", buffer, 0, clean_all_data(0, NULL));

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
* @testcase         audio_pcm_write_p
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    NA
*/
static void utc_audio_pcm_writei_p(void)
{
	int fd;
	int ret;

	char *buffer;
	int num_read;
	int total_frames;
	unsigned int size;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_writei", pcm_is_ready(g_pcm), 0);

	size = pcm_frames_to_bytes(g_pcm, pcm_get_buffer_size(g_pcm));
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("pcm_writei", fd, 0, clean_all_data(0, buffer));

	printf("Playback start!!\n");
	total_frames = 0;
	do {
		num_read = read(fd, buffer, size);
		if (num_read > 0) {
			total_frames += num_read;
			ret = pcm_writei(g_pcm, buffer, pcm_bytes_to_frames(g_pcm, num_read));
			TC_ASSERT_GEQ_CLEANUP("pcm_writei", ret, 0, clean_all_data(fd, buffer));
		}
	} while (num_read > 0);

	sleep(2);
	printf("Playback done! Total Frames: %d\n", pcm_bytes_to_frames(g_pcm, total_frames));

	clean_all_data(fd, buffer);
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

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
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

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_audio_main(int argc, char *argv[])
#endif
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	printf("########## Audio UTC Start ##########\n");	

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
	utc_audio_pcm_readi_p();
	utc_audio_pcm_readi_n();
	utc_audio_pcm_writei_p();
	utc_audio_pcm_writei_n();
	/* after test, unlink the file */
	unlink(AUDIO_TEST_FILE);

	printf("########## Audio UTC End [PASS : %d, FAIL : %d] ##########\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}
