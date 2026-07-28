/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include "../aft/aft_main.h"
#include <debug.h>
#include <tinyara/init.h>
#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileInputDataSource.h>
#include <media/FileOutputDataSource.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>
#include <audio/SoundManager.h>
#include <media/voice/SpeechDetector.h>
#include <media/voice/SpeechDetectorListenerInterface.h>
#include <media/FocusManager.h>
#include <media/stream_info.h>
#include <stdio.h>
#include <memory>
#include <tinyara/pm/pm.h>
#include <semaphore.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <cstdio>
#include <iostream>

#include <fcntl.h>
#include <functional>
#include <tinyara/rtc.h>
#include <sys/ioctl.h>

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

static media::voice::SpeechDetector *sd;

#define RTC_DEVPATH "/dev/rtc0"



extern "C" {
static void stream_multi_channels(int duration, int verbose)
{
	time_t start_time, current_time;
	struct rtc_time start_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time current_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	unsigned int elapsed_time_rtc = 0;
	uint32_t total_bytes = 0;
	uint32_t audio_data_len = 0;
	uint32_t returned_extract_size = 0;
	uint32_t data_capacity = 0;
	int rtc_fd = -1;
	int n_extractions = 0;
	bool first = true;
	bool audio_stream_started = false;
	bool aft_stream_started = false;
	bool stream_error = false;
	uint8_t *data = NULL;

	rtc_fd = open(RTC_DEVPATH, O_RDWR);
	if (rtc_fd < 0) {
		printf("ERROR: Failed to open RTC\n");
		stream_error = true;
		goto cleanup;
	}

	if (send_stream_start() != 0) {
		printf("ERROR: AFT stream is not ready\n");
		stream_error = true;
		goto cleanup;
	}
	aft_stream_started = true;

	printf("Streaming for %ds...\n", duration);
	if (!startAudioDebugDumpStream(duration, verbose, &returned_extract_size)) {
		printf("ERROR: Failed to start multi-channel stream\n");
		stream_error = true;
		goto cleanup;
	}
	audio_stream_started = true;
	if (returned_extract_size == 0 || returned_extract_size > UINT32_MAX / 5) {
		printf("ERROR: Invalid extracted audio size\n");
		stream_error = true;
		goto cleanup;
	}

	data_capacity = returned_extract_size * 5;
	data = (uint8_t *)malloc(data_capacity);
	if (!data) {
		printf("ERROR: Failed to allocate audio buffer\n");
		stream_error = true;
		goto cleanup;
	}

	while ((duration == -1) || elapsed_time_rtc < duration) {
		if (!readAudioDebugDumpStream(data, &audio_data_len)) {
			printf("ERROR: Failed to read multi-channel stream\n");
			stream_error = true;
			break;
		}

		if (first) {
			if (ioctl(rtc_fd, RTC_RD_TIME, (unsigned long)&start_time_rtc) < 0) {
				printf("ERROR: Failed to read RTC time (errno %d)\n", get_errno());
				stream_error = true;
				break;
			}
			first = false;
		}

		n_extractions++;
		if (audio_data_len) {
			if (audio_data_len > data_capacity) {
				printf("ERROR: Invalid audio data length %u\n", audio_data_len);
				stream_error = true;
				break;
			}
			if (send_stream_buf(data, audio_data_len) != 0) {
				printf("ERROR: Failed to send AFT stream data\n");
				stream_error = true;
				break;
			}

			total_bytes += audio_data_len;
			if ((verbose && audio_data_len > 1932) || (verbose > 2)) {
				printf("%d - elapsed %u Extracted: %u total %u\n",
					   n_extractions, elapsed_time_rtc, audio_data_len, total_bytes);
			}
		}

		if (ioctl(rtc_fd, RTC_RD_TIME, (unsigned long)&current_time_rtc) < 0) {
			printf("ERROR: Failed to read RTC time (errno %d)\n", get_errno());
			stream_error = true;
			break;
		}
		start_time = mktime((FAR struct tm *)&start_time_rtc);
		current_time = mktime((FAR struct tm *)&current_time_rtc);
		elapsed_time_rtc = difftime(current_time, start_time);
	}

cleanup:
	if (audio_stream_started && !stopAudioDebugDumpStream()) {
		printf("ERROR: Failed to stop multi-channel stream\n");
		stream_error = true;
	}
	if (aft_stream_started && send_stream_end() != 0) {
		printf("ERROR: Failed to close AFT stream\n");
		stream_error = true;
	}
	free(data);
	if (rtc_fd >= 0) {
		close(rtc_fd);
	}
	printf("\nTotal sent: %u\n", total_bytes);
	printf(stream_error ? "Streaming stopped due to an error.\n" : "Successfully done.\n");
}

int dump4ch_main(int argc, char *argv[])
{
	long parsed_duration;
	long parsed_verbose;
	char *duration_end;
	char *verbose_end;

	printf("dump4ch_main Entry\n");
	if (argc != 3) {
		printf("Usage: dump4ch [duration] [verbose]\n");
		return -1;
	}

	parsed_duration = strtol(argv[1], &duration_end, 10);
	parsed_verbose = strtol(argv[2], &verbose_end, 10);
	if (argv[1][0] == '\0' || *duration_end != '\0' ||
		argv[2][0] == '\0' || *verbose_end != '\0' ||
		parsed_duration < -1 || parsed_duration > 2147483647L ||
		parsed_verbose < 0 || parsed_verbose > 2147483647L) {
		printf("Invalid duration or verbose value\n");
		return -1;
	}

	sd = media::voice::SpeechDetector::instance();
	if (!sd->initKeywordDetect(16000, 1)) {
		printf("#### [SD] init failed.\n");
		return 0;
	}
	if (!sd->changeKeywordModel(0)) {
		printf("#### [SD] model load failed.\n");
		sd->deinitKeywordDetect();
		return 0;
	}

	stream_multi_channels((int)parsed_duration, (int)parsed_verbose);
	if (!sd->deinitKeywordDetect()) {
		printf("WARNING: Failed to deinitialize keyword detector\n");
	}
	return 0;
}
}
