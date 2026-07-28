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
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
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
#include <wifi_manager/wifi_manager.h>
#include <tinyara/rtc.h>
#include <sys/ioctl.h>

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

static media::voice::SpeechDetector *sd;

#define RTC_DEVPATH "/dev/rtc0"


#define STREAM_CHUNK_SIZE 1000000
#define NUM_STREAM_CHUNKS 4

extern "C" {
static bool is_wifi_connected(void)
{
	wifi_manager_info_s wifi_info;
	wifi_manager_result_e result = wifi_manager_get_info(&wifi_info);

	return result == WIFI_MANAGER_SUCCESS && wifi_info.status == AP_CONNECTED;
}

static int get_ms_time(uint32_t *ms_time)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);

	*ms_time = (uint32_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return 0;
}

static bool stream_file(int socket_s, const uint8_t *data, uint32_t len, int verbose)
{
	uint32_t sent = 0;

	if (verbose) {
		printf("Sending %u bytes from address %p\n", len, data);
	}

	while (sent < len) {
		int result;

		if (!is_wifi_connected()) {
			printf("Wi-Fi AP disconnected while streaming\n");
			return false;
		}

		result = send(socket_s, &data[sent], len - sent, 0);
		if (result <= 0) {
			printf("Error sending: %d\n", get_errno());
			return false;
		}
		sent += (uint32_t)result;
	}
	if (verbose) {
		printf("Sent %u bytes\n", sent);
	}
	return true;
}


static void stream_multi_channels(const char *server_ip, unsigned int destport, int duration, int verbose)
{
	time_t start_time, current_time;
	struct rtc_time start_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time current_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	unsigned int elapsed_time_rtc = 0;
	
	uint32_t total_bytes = 0;
	int socket_send = -1;
	int rtc_fd = -1;
	int n_extractions = 0;
	int first = 1;
	int tid_idx = 0;
	uint32_t written = 0;
	int i;
	uint32_t audio_data_len = 0;
	uint32_t returned_extract_size = 0;
	uint32_t data_capacity = 0;
	uint32_t chunk_capacity = 0;
	bool stream_started = false;
	bool stream_error = false;
	uint8_t *data = NULL;

	uint8_t * recorded_data[NUM_STREAM_CHUNKS] = {NULL};
	struct sockaddr_in address;
	struct timeval send_timeout = {5, 0};

	rtc_fd = open(RTC_DEVPATH, O_RDWR);
	if (rtc_fd < 0) {
		printf("ERROR: Failed to open RTC\n");
		stream_error = true;
		goto cleanup;
	}

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server_ip);
	address.sin_port = htons(destport);

	socket_send = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_send >= 0 &&
		setsockopt(socket_send, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout)) < 0) {
		printf("WARNING: Failed to set socket send timeout\n");
	}
	if (socket_send < 0) {
		printf("ERROR: Failed to create socket\n");
		stream_error = true;
		goto cleanup;
	}
	if (connect(socket_send, (struct sockaddr *)&address, sizeof(address)) != 0) {
		printf("ERROR: Could not connect to server\n");
		stream_error = true;
		goto cleanup;
	}


	printf("Streaming for %ds...\n", duration);

	if (!startAudioDebugDumpStream(duration, verbose, &returned_extract_size)) {
		printf("ERROR: Failed to start multi-channel stream\n");
		stream_error = true;
		goto cleanup;
	}
	stream_started = true;
	if (returned_extract_size == 0 || returned_extract_size > UINT32_MAX / 5) {
		printf("ERROR: Invalid extracted audio size\n");
		stream_error = true;
		goto cleanup;
	}
	data_capacity = returned_extract_size * 5;
	if (data_capacity > UINT32_MAX - STREAM_CHUNK_SIZE) {
		printf("ERROR: Invalid stream buffer size\n");
		stream_error = true;
		goto cleanup;
	}
	chunk_capacity = STREAM_CHUNK_SIZE + data_capacity;

	data = (uint8_t *)malloc(data_capacity);
	if (!data) {
		printf("ERROR: Failed to allocate audio buffer\n");
		stream_error = true;
		goto cleanup;
	}
	for (i = 0; i < NUM_STREAM_CHUNKS; i++) {
		recorded_data[i] = (uint8_t *)malloc(chunk_capacity);
		if (!recorded_data[i]) {
			printf("ERROR: Failed to allocate stream buffer %d\n", i);
			stream_error = true;
			goto cleanup;
		}
	}

	while ((duration == -1) || elapsed_time_rtc < duration) {
		if (!is_wifi_connected()) {
			printf("Wi-Fi AP disconnected while streaming\n");
			stream_error = true;
			break;
		}
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
			first = 0;
		}
		n_extractions++;
		if (audio_data_len) {
			if (audio_data_len > data_capacity || written > chunk_capacity - audio_data_len) {
				printf("ERROR: Invalid audio data length %u\n", audio_data_len);
				stream_error = true;
				break;
			}
			memcpy(recorded_data[tid_idx] + written, data, audio_data_len);
			written += audio_data_len;
			total_bytes += audio_data_len;
			if ((verbose && audio_data_len > 1932) || (verbose > 2)) {
				uint32_t tnow;
				get_ms_time(&tnow);
				printf("%d - elapsed %d Extracted: %u @ %u written %u total %u\n",
					   n_extractions, elapsed_time_rtc, audio_data_len, tnow, written, total_bytes);
			}
			if (written >= STREAM_CHUNK_SIZE) {
				if (!stream_file(socket_send, recorded_data[tid_idx], written, verbose)) {
					stream_error = true;
					break;
				}
				tid_idx = (tid_idx + 1) % NUM_STREAM_CHUNKS;
				written = 0;
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
	if (!stream_error && written > 0 &&
		!stream_file(socket_send, recorded_data[tid_idx], written, verbose)) {
		stream_error = true;
	}

cleanup:
	if (stream_started) {
		printf("Turning off streaming ...\n");
		if (!stopAudioDebugDumpStream()) {
			printf("ERROR: Failed to stop multi-channel stream\n");
			stream_error = true;
		}
	}
	if (socket_send >= 0) {
		close(socket_send);
	}
	for (i = 0; i < NUM_STREAM_CHUNKS; i++) {
		free(recorded_data[i]);
	}
	free(data);
	if (rtc_fd >= 0) {
		close(rtc_fd);
	}
	printf("\nTotal written: %u\n", total_bytes);
	if (stream_error) {
		printf("Streaming stopped due to an error.\n");
	} else {
		printf("Successfully done.\n");
	}
}

int dump4ch_main(int argc, char *argv[])
{
	const char *server_ip;
	unsigned int server_port;
	unsigned long parsed_port;
	long parsed_duration;
	long parsed_verbose;
	char *port_end;
	char *duration_end;
	char *verbose_end;
	struct in_addr parsed_address;
	int streamDuration;
	int streamVerbose;

	printf("dum4ch_main Entry\n");

	if (argc != 5) {
		printf("Usage: dump4ch [ip] [port] [duration] [verbose]\n");
		return -1;
	}

	server_ip = argv[1];
	if (inet_aton(server_ip, &parsed_address) == 0) {
		printf("Invalid server IP address: %s\n", server_ip);
		return -1;
	}

	parsed_port = strtoul(argv[2], &port_end, 10);
	if (argv[2][0] == '\0' || *port_end != '\0' ||
		parsed_port == 0 || parsed_port > 65535UL) {
		printf("Invalid server port: %s\n", argv[2]);
		return -1;
	}
	server_port = (unsigned int)parsed_port;

	parsed_duration = strtol(argv[3], &duration_end, 10);
	parsed_verbose = strtol(argv[4], &verbose_end, 10);
	if (argv[3][0] == '\0' || *duration_end != '\0' ||
		argv[4][0] == '\0' || *verbose_end != '\0' ||
		parsed_duration < -1 || parsed_duration > 2147483647L ||
		parsed_verbose < 0 || parsed_verbose > 2147483647L) {
		printf("Invalid duration or verbose value\n");
		return -1;
	}
	streamDuration = (int)parsed_duration;
	streamVerbose = (int)parsed_verbose;

	if (!is_wifi_connected()) {
		printf("Wi-Fi AP is not connected\n");
		return -1;
	}

	printf("dump4ch config - server: %s, port: %u, duration: %d, verbose: %d\n",
		   server_ip, server_port, streamDuration, streamVerbose);

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

	stream_multi_channels(server_ip, server_port, streamDuration, streamVerbose);
	if (!sd->deinitKeywordDetect()) {
		printf("WARNING: Failed to deinitialize keyword detector\n");
	}

	return 0;
}
}
