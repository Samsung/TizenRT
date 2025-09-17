/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <cstdio>
#include <debug.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tinyara/init.h>
#include <tinyara/pm/pm.h>
#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileInputDataSource.h>
#include <media/FileOutputDataSource.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>
#include <media/voice/SpeechDetector.h>
#include <media/voice/SpeechDetectorListenerInterface.h>
#include <media/FocusManager.h>
#include <media/stream_info.h>
#include <iostream>
#include <memory>
#include <functional>

#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../apps/include/inifile.h"
#include <wifi_manager/wifi_manager.h>
#include <tinyara/rtc.h>
#include <sys/ioctl.h>

#define RTC_DEVPATH "/dev/rtc0"
#define WIFI_INI_FILE_NAME "/res/kernel/audio/dumptool.ini"
#define WIFI_CONNECT_ATTEMPTS 10

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

static media::voice::SpeechDetector *sd;

static const char *filePath = "/tmp/record.pcm";
static uint8_t *gBuffer = NULL;
static uint32_t bufferSize = 0;

extern "C" {

static wifi_manager_cb_s g_wifi_callbacks = {NULL, NULL, NULL, NULL, NULL};
static wifi_manager_ap_config_s ap_config;

int wifi_start(char* ssid, char *psk)
{
	int repeat = 0;
	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	while(repeat++ < 3){
		wres = wifi_manager_init(&g_wifi_callbacks);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("fail to initialize wifi manager, ret: %d\n", wres);
			printf("Will try again in a sec..\n");
			wifi_manager_deinit();
			usleep(1000000);
			continue;
		}
		strncpy(ap_config.ssid, ssid, strlen(ssid) + 1);
		ap_config.ssid_length = strlen(ssid);
		strncpy(ap_config.passphrase, psk, strlen(psk) + 1);
		ap_config.passphrase_length = strlen(psk);
		ap_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
		ap_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_NONE;
		wres = wifi_manager_connect_ap(&ap_config);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("failed to connect to %s, ret: %d\n", ssid, wres);
			printf("Will try again in a sec..\n");
			wifi_manager_deinit();
			usleep(1000000);
			continue;
		} else {
			printf("Connected to %s, ip addr can be checked using ifconfig\n", ssid);
			break;
		}
	}
	return wres;
}

static char *server = NULL;
static int port = 12345;

void ndp120_stream_wifi_setup(void)
{
	/* read ini file to set up wifi */
	INIHANDLE handle = inifile_initialize(WIFI_INI_FILE_NAME);
	if (handle) {
		char *ssid;
		char *passwd;
		int i;
		ssid = inifile_read_string(handle, "config", "ssid", "ssid");
		passwd = inifile_read_string(handle, "config", "psk", "psk");

		server = inifile_read_string(handle, "config", "server", "server");
		port = inifile_read_integer(handle, "config", "port", 12345);

		wifi_manager_info_s wifi_info;
		wifi_manager_get_info(&wifi_info);
		if (wifi_info.status != AP_CONNECTED) {
			wifi_start(ssid, passwd);
		}
		inifile_free_string(ssid);
		inifile_free_string(passwd);

		for (i = 0; i < WIFI_CONNECT_ATTEMPTS; i++) {
			wifi_manager_get_info(&wifi_info);
			if (wifi_info.status == AP_CONNECTED) {
				printf("AP connected!\n");
				break;
			}
			usleep(250000);
		}
	}
}

#define STREAM_CHUNK_SIZE 1000000
#define NUM_STREAM_CHUNKS 4

struct tcp_stream_data_s {
	int socket_s;
	uint8_t *data;
	uint32_t len;
	int tid_idx;
	int verbose;
};

static pthread_mutex_t stream_mutex;
static int tid_idx_to_send = 0;

int get_ms_time(uint32_t *ms_time)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);

	*ms_time = (uint32_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return 0;
}

static void stream_file(int socket_s, uint8_t * data, uint32_t len, int my_tid_idx, int verbose)
{
	int s = 0;

	if (verbose) {
		printf("I am supposed to send from %p -- my tid_idx is %d\n", data, my_tid_idx);
	}

	for (;;) {
		pthread_mutex_lock(&stream_mutex);
		if ((my_tid_idx >= 0) && (tid_idx_to_send != my_tid_idx)) {
			if (verbose) {
				auddbg("The tid_idx for send is %d\n", tid_idx_to_send);
			}
			pthread_mutex_unlock(&stream_mutex);
			usleep(100000);
		} else {
			/* nothing here, but we can continue now, with the mutex locked */
			break;
		}
	}

	int remaining = len;
	unsigned int sent = 0;
	if (verbose) {
		printf("tid_idx %d Sending from address %p\n", my_tid_idx, data);
	}

	while (sent < len) {
		s = send(socket_s, &data[sent], remaining, 0);
		if (s <= 0) {
			printf("Error sending\n");
		}
		remaining -= s;
		sent += s;
	}
	if (verbose) {
		printf("sent %d bytes\n", sent);
	}

	/* now we are done, we can set next tid to be sent
	*/
	tid_idx_to_send++;
	if (tid_idx_to_send == NUM_STREAM_CHUNKS) {
		tid_idx_to_send = 0;
	}

	pthread_mutex_unlock(&stream_mutex);
}

static void *stream_file_task(void *arg)
{
	struct tcp_stream_data_s tsf = *(struct tcp_stream_data_s *)arg;
	stream_file(tsf.socket_s, tsf.data, tsf.len, tsf.tid_idx, tsf.verbose);
	return NULL;
}

void ndp120_stream_multi_channels(char* server_ip, unsigned int destport, int duration, int verbose)
{
	time_t start_time, current_time;
	struct rtc_time start_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time current_time_rtc = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	unsigned int elapsed_time_rtc = 0;
	
	uint32_t total_bytes = 0;
	int socket_send;
	int n_extractions = 0;
	int first = 1;
	int tid_idx = 0;
	int written = 0;
	pthread_t tid;
	int i;
	uint32_t audio_data_len;

	uint8_t * recorded_data[NUM_STREAM_CHUNKS] = {NULL};
	struct tcp_stream_data_s stream_file_args = {};

	pthread_attr_t attr;
	struct sched_param sparam;
	struct sockaddr_in address;

	/* restart from 0 */
	tid_idx_to_send = 0;

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setstacksize(&attr, 4096);
	sparam.sched_priority = 150;
	pthread_attr_setschedparam(&attr, &sparam);

	pthread_mutex_init(&stream_mutex, NULL);

	int rtc_fd = open(RTC_DEVPATH, O_RDWR);
	if (rtc_fd < 0) {
		printf("ERROR : Fail to open rtc.\n");
	}

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server_ip);
	address.sin_port = htons(destport);

	socket_send = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(socket_send, (struct sockaddr *)&address, sizeof(address))) {
		printf("Could not connect to server\n");
		return;
	}

	stream_file_args.socket_s = socket_send;
	stream_file_args.verbose = verbose;

	printf("Streaming for %ds...\n", duration);

	uint32_t returned_extract_size;
	sd->startMultiChStream(duration, verbose, &returned_extract_size);

	uint8_t *data = (uint8_t *) malloc(returned_extract_size*5);
	if (!data) {
		printf("Error allocating memory for audio data\n");
		return;
	}
	for (i = 0; i < NUM_STREAM_CHUNKS; i++) {
		int alloc_len = STREAM_CHUNK_SIZE + returned_extract_size * 5; /* + some overhead */
		recorded_data[i] = (uint8_t*)malloc(alloc_len);
		if (!recorded_data[i]) {
			printf("Error Allocating Memory!!!\n");
		}
		printf("Data allocated at %p (len %d)\n",recorded_data[i], alloc_len);
	}

	while ((duration == -1) || elapsed_time_rtc < duration) {
		sd->readMultiChStream(data, &audio_data_len);
		if (first) {
			if (ioctl(rtc_fd, RTC_RD_TIME, (unsigned long)&start_time_rtc) < 0) {
				printf("Fail to call RTC READ TIME(errno %d)", get_errno());
			}
			first = 0;
		}
		n_extractions++;

		if (audio_data_len) {
			memcpy(recorded_data[tid_idx] + written, data, audio_data_len);
			written += audio_data_len;
			total_bytes += audio_data_len;

			if ((verbose && audio_data_len > 1932) || (verbose > 2))  {
				uint32_t tnow;
				get_ms_time(&tnow);
				printf("%d - elapsed %d Extracted: %d @ %u	 written %d   total %u \n",
					   n_extractions, elapsed_time_rtc, audio_data_len, tnow, written, total_bytes);
			}

			if (written >= STREAM_CHUNK_SIZE) {
				stream_file_args.data = recorded_data[tid_idx];
				stream_file_args.len = written;
				stream_file_args.tid_idx = tid_idx;

				pthread_create(&tid, &attr, stream_file_task, &stream_file_args);
				tid_idx++;
				if (tid_idx == NUM_STREAM_CHUNKS) {
					tid_idx = 0;
				}
				written = 0;
			}
		}

		if (ioctl(rtc_fd, RTC_RD_TIME, (unsigned long)&current_time_rtc) < 0) {
			printf("Fail to call RTC READ TIME(errno %d)", get_errno());
		}
		
		start_time = mktime((FAR struct tm *)&start_time_rtc);
		current_time = mktime((FAR struct tm *)&current_time_rtc);
		elapsed_time_rtc = difftime(current_time, start_time);
	}

	sleep(1);

	/* sending inline with -1 to allow sending last chunk regardless of check */
	stream_file(stream_file_args.socket_s, recorded_data[tid_idx], written, -1, verbose);

	printf("\nTotal written: %d\n", total_bytes);

	close(socket_send);

	printf("Turning off streaming  ...\n");
	sd->stopMultiChStream();

	for (i = 0; i < NUM_STREAM_CHUNKS; i++) {
		if (recorded_data[i]) free(recorded_data[i]);
	}
	if (data) free(data);

	close(rtc_fd);

	printf("\nSuccessfully Done.\n\nPLEASE REBOOT PRIOR TO DOING ANOTHER RECORDING\n");
}

int dump4ch_main(int argc, char *argv[])
{
	int streamDuration = -1; /* -1 = infinite, otherwise in secs */
	int streamVerbose = 0;

	printf("dump4ch_main Entry\n");

	if (argc <= 3) {
		ndp120_stream_wifi_setup();

		if (argc >= 2) {
			streamDuration = atoi(argv[1]);
		}
		if (argc >= 3) {
			streamVerbose = atoi(argv[2]);
		}
	}

	if (argc > 3) {
		printf("invalid input\n");
		printf("Usage : dump4ch [mode]\n");
		printf("Alternative Usage : dump4ch [duration] [verbose]\n");
		return -1;
	}
	sd = media::voice::SpeechDetector::instance();

	if (!sd->initKeywordDetect(16000, 1)) {
		printf("#### [SD] init failed.\n");
		return 0;
	}

	ndp120_stream_multi_channels(server, port, streamDuration, streamVerbose);
	return 0;
}
}

