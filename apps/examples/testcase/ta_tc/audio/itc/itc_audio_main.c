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
#include <fcntl.h>
#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>
#include <tinyara/fs/ioctl.h>
#include <apps/shell/tash.h>
#include "tc_common.h"
#include <tinyara/config.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define AUDIO_TEST_FILE "/mnt/pcm"

#define AUDIO_DEFAULT_CHANNELS 2

#define AUDIO_DEFAULT_RATE 16000

#define AUDIO_DEFAULT_FORMAT PCM_FORMAT_S16_LE

#define AUDIO_RECORD_DURATION 3	//3sec

#define CONFIG_RATE 4000

#define INPUT_STR_LEN 16
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
	}

	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
}

/**
* @testcase         audio_pcm_open_close_tc_p
* @brief            open and initlaize pcm structure
* @scenario         Open pcm and check its validation
* @apicovered       pcm_open, pcm_close
* @precondition     NA
* @postcondition    NA
*/
static void itc_audio_pcm_open_close_tc_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_is_ready_tc_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	TC_ASSERT_GT_CLEANUP("pcm_is_ready", pcm_is_ready(g_pcm), 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_prepare_tc_p(void)
{
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	TC_ASSERT_EQ_CLEANUP("pcm_prepare", pcm_prepare(g_pcm), 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_open_by_name_tc_p(void)
{
	g_pcm = pcm_open_by_name("hw:0,0", PCM_IN, NULL);
	TC_ASSERT_GT_CLEANUP("pcm_open_by_name", pcm_get_file_descriptor(g_pcm), 0, pcm_close(g_pcm))
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_config_tc_p(void)
{
	const struct pcm_config *config = NULL;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	config = pcm_get_config(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->channels, AUDIO_DEFAULT_CHANNELS, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->format, AUDIO_DEFAULT_FORMAT, pcm_close(g_pcm));
	TC_ASSERT_EQ_CLEANUP("pcm_get_config", config->rate, AUDIO_DEFAULT_RATE, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_channels_tc_p(void)
{
	unsigned int ch = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	ch = pcm_get_channels(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_channels", ch, AUDIO_DEFAULT_CHANNELS, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_rate_tc_p(void)
{
	int rate = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	rate = pcm_get_rate(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_rate", rate, AUDIO_DEFAULT_RATE, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_format_tc_p(void)
{
	enum pcm_format format = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	format = pcm_get_format(g_pcm);
	TC_ASSERT_EQ_CLEANUP("pcm_get_format", format, AUDIO_DEFAULT_FORMAT, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_file_descriptor_tc_p(void)
{
	int fd = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	fd = pcm_get_file_descriptor(g_pcm);
	/* Now we tested all config values of g_pcm, close here */
	TC_ASSERT_GT_CLEANUP("pcm_get_file_descriptor", fd, 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_error_tc_p(void)
{
	struct pcm *pcm = NULL;
	pcm = pcm_open(0, 0, PCM_IN, NULL);
	const char *errors = pcm_get_error(pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_error", errors, NULL, pcm_close(pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(pcm), 0);
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
static void itc_audio_pcm_get_buffer_size_tc_p(void)
{
	ssize_t size = 0;
	/* Open again to test APIs regarding buffering & recoridng & playing */
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_get_subdevice_tc_p(void)
{
	unsigned int device = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	device = pcm_get_subdevice(g_pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_subdevice", device, 1, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_frames_to_bytes_tc_p(void)
{
	ssize_t size = 0;
	unsigned int bytes = 0;
	unsigned int frame_size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);

	/* set basic configuration values for next test */
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
* @testcase         audio_pcm_bytes_to_frames_p
* @brief            convert bytes to frame
* @scenario         get configuration value of pcm and calculate byte to frame
* @apicovered       pcm_bytes_to_frames
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_bytes_to_frames_tc_p(void)
{
	unsigned int frame_size = 0;
	ssize_t size = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);

	size = pcm_get_buffer_size(g_pcm);
	TC_ASSERT_GT_CLEANUP("pcm_get_buffer_size", size, 0, pcm_close(g_pcm));

	frame_size = pcm_bytes_to_frames(g_pcm, size);
	TC_ASSERT_GT_CLEANUP("pcm_bytes_to_frames", frame_size, 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_audio_pcm_format_to_bits_tc_p
* @brief            get bits information based on pcm's format
* @scenario         get bits of format value of pcm
* @apicovered       pcm_format_to_bits
* @precondition     pcm should be opened before.
* @postcondition    NA
*/
static void itc_audio_pcm_format_to_bits_tc_p(void)
{
	enum pcm_format format = 0;
	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);
	format = pcm_get_format(g_pcm);
	TC_ASSERT_NEQ_CLEANUP("pcm_get_format", format, PCM_FORMAT_NONE, pcm_close(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_format_to_bits", pcm_format_to_bits(format), 0, pcm_close(g_pcm));
	TC_ASSERT_EQ("pcm_close", pcm_close(g_pcm), 0);
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
static void itc_audio_pcm_readi_tc_p(void)
{
	int fd = 0;
	char input_str[INPUT_STR_LEN];
	int ret = 0;
	char *str = NULL;
	char *buffer = NULL;
	unsigned int bytes_per_frame = 0;
	int frames_read = 0;
	int remain = 0;

	g_pcm = pcm_open(0, 0, PCM_IN, NULL);
	TC_ASSERT_GT("pcm_open", pcm_get_file_descriptor(g_pcm), 0);

	buffer = malloc(pcm_get_buffer_size(g_pcm));
	TC_ASSERT_NEQ_CLEANUP("pcm_readi", buffer, NULL, clean_all_data(0, 0));

	fd = open(AUDIO_TEST_FILE, O_RDWR | O_CREAT);
	TC_ASSERT_GT_CLEANUP("pcm_readi", fd, 0, clean_all_data(0, buffer));

	bytes_per_frame = pcm_frames_to_bytes(g_pcm, 1);
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
* @testcase         audio_pcm_write_p
* @brief            play captured data from filesystem
* @scenario         play recored file previously
* @apicovered       pcm_writei
* @precondition     pcm_readi should be opened before
* @postcondition    NA
*/
static void itc_audio_pcm_writei_tc_p(void)
{
	int fd = 0;
	int ret = 0;

	char *buffer = NULL;
	int num_read = 0;
	int total_frames = 0;
	unsigned int size = 0;

	/* use default config here */
	g_pcm = pcm_open(0, 0, PCM_OUT, NULL);
	TC_ASSERT_GT("pcm_writei", pcm_is_ready(g_pcm), 0);

	size = pcm_get_buffer_size(g_pcm);
	buffer = malloc(size);
	TC_ASSERT_NEQ_CLEANUP("pcm_writei", buffer, NULL, clean_all_data(0, NULL));

	fd = open(AUDIO_TEST_FILE, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("pcm_writei", fd, 0, clean_all_data(0, buffer));

	printf("Playback start!!\n");

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

static int audio_tc_launcher(int argc, char **args)
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	itc_audio_pcm_open_close_tc_p();
	itc_audio_pcm_is_ready_tc_p();
	itc_audio_pcm_prepare_tc_p();
	itc_audio_pcm_open_by_name_tc_p();
	itc_audio_pcm_get_config_tc_p();
	itc_audio_pcm_get_channels_tc_p();
	itc_audio_pcm_get_rate_tc_p();
	itc_audio_pcm_get_format_tc_p();
	itc_audio_pcm_get_file_descriptor_tc_p();
	itc_audio_pcm_get_error_tc_p();
	itc_audio_pcm_get_buffer_size_tc_p();
	itc_audio_pcm_get_subdevice_tc_p();
	itc_audio_pcm_frames_to_bytes_tc_p();
	itc_audio_pcm_bytes_to_frames_tc_p();
	itc_audio_pcm_format_to_bits_tc_p();
	itc_audio_pcm_readi_tc_p();
	itc_audio_pcm_writei_tc_p();
	/* after test, unlink the file */
	unlink(AUDIO_TEST_FILE);

	printf("#########################################\n");
	printf("           Audio ITC Result               \n");
	printf("           PASS : %d FAIL : %d        \n", total_pass, total_fail);
	printf("#########################################\n");

	working_tc--;
	sem_post(&tc_sem);

	return total_pass;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_audio_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("audio_itc", audio_tc_launcher, TASH_EXECMD_SYNC);
#else
	audio_tc_launcher(argc, argv);
#endif

	return 0;
}
