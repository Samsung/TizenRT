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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <apps/shell/tash.h>
#include <fcntl.h>

#include <media/media_player.h>
#include <media/media_recorder.h>
#include <media/media_utils.h>

#include <dm/dm_connectivity.h>
#include <dm/dm_error.h>
#include <sys/socket.h>

#define DISCONNECT_WIFI_AND_CLOSE_SOCKET() do { dm_conn_wifi_disconnect(); disconnect_socket(&fd); } while (0)

int isWiFiConnected = 0;

void WiFi_Link_Up(void)
{
	isWiFiConnected = 1;
	printf("\n[WiFi] Connected!\n");
}

void WiFi_Link_Down(void)
{
	isWiFiConnected = 0;
	printf("\n[WiFi] Disconnected!\n");
}

int network_setup(void)
{
	isWiFiConnected = 0;

	dm_cb_register_init();
	dm_conn_wifi_connect(WiFi_Link_Up, WiFi_Link_Down);

	while (true) {
		if (isWiFiConnected == 1) {
			break;
		}
		printf("[WiFi] Waiting Connect......\n");
		sleep(1);
	}
	dm_conn_dhcp_init();

	if (isWiFiConnected == 1) {
		return 0;
	} else {
		return -1;
	}
}

int connect_socket(const char *ip, int port)
{
	int fd = -1;
	struct sockaddr_in dest;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("[WiFi] Open Socket Failed!\n");
		return -1;
	}

	memset(&dest, 0, sizeof(struct sockaddr_in));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr(ip);
	dest.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&dest, sizeof(struct sockaddr)) < 0) {
		printf("[TCPCLIENT] connect fail: %d\n", errno);
		return -1;
	}

	return fd;
}

void disconnect_socket(int *fd)
{
	if (*fd >= 0) {
		closesocket(*fd);
		*fd = -1;
	}
}

/****************************************************************************
 * media_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int media_test_main(int argc, char **argv)
#endif
{
	char ch;
	unsigned char vol = 125;

	int fd = -1;
	int ret = 0;
	int pos = 0;
	int media_len = 0;

	while (true) {
		printf("[Media Test] v0.01\n");
		printf("-----------------\n");
		printf("0) network_setup\n");
		printf("1) media_play\n");
		printf("2) media_pause_play\n");
		printf("3) media_resume_play\n");
		printf("4) media_stop_play\n");
		printf("5) playing volume up\n");
		printf("6) playing volume down\n");
		printf("7) Forward\n");
		printf("8) Backward\n");
		printf("-----------------\n");
		printf("a) media_record\n");
		printf("b) media_record_stream\n");
		printf("c) media_pause_record\n");
		printf("d) media_resume_record\n");
		printf("e) media_stop_record\n");
		printf("f) before record(a and d), play_file_\n");
		printf("-----------------\n");
		printf("x) Exit\n");
		printf(">> ");

		ch = getchar();
		printf("Instruction: %c\n", ch);
		switch (ch) {
		case '0': {
			ret = network_setup();

			if (ret < 0) {
				printf("Network Error!!\n");
				return 0;
			}
		}
		break;
		case '1': {
			if (media_is_recording()) {
				media_stop_record();
				disconnect_socket(&fd);
			}

			if (media_play_init() != PLAY_OK) {
				printf("Error: media_play_init()\n");
				return 0;
			} else {
				printf("MediaPlay Init Okay..\n");
			}

			printf("Socket connecting...\n");
			fd = connect_socket("192.168.1.213", 8080);
			if (fd < 0) {
				printf("Connection failed...\n");
				dm_conn_wifi_disconnect();
				return 0;
			}

			printf("Socket connected successfully.\n");

			ret = media_play(fd, MEDIA_FORMAT_WAV);
			if (ret != MEDIA_OK) {
				printf("Error: media_play\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}
		}
		break;
		case '2': {
			ret = media_pause_play();
			if (ret != MEDIA_OK) {
				printf("Error: media_pause_play\n");
			}
		}
		break;
		case '3': {
			ret = media_resume_play();
			if (ret != MEDIA_OK) {
				printf("Error: media_pause_play\n");
			}
		}
		break;
		case '4': {
			ret = media_stop_play();
			if (ret != MEDIA_OK) {
				printf("Error: media_stop_play\n");
			}
		}
		break;
		case '5': {
			if (vol < 255) {
				vol += 10;
			}

			ret = media_set_vol(vol);
			if (ret != MEDIA_OK) {
				printf("Error: media_set_vol\n");
			} else {
				printf("Current Volume: %d\n", vol);
			}
		}
		break;
		case '6': {
			if (vol > 15) {
				vol -= 10;
			}

			ret = media_set_vol(vol);
			if (ret != MEDIA_OK) {
				printf("Error: media_set_vol\n");
			} else {
				printf("Current Volume: %d\n", vol);
			}
		}
		break;
		case '7': {
			pos += 1000;
			if (pos > media_len - 1) {
				pos = media_len - 1;
			}

			ret = media_set_position(pos);
			if (ret != MEDIA_OK) {
				printf("Error: media_set_position\n");
			}
		}
		break;
		case '8': {
			pos -= 1000;
			if (pos < 0) {
				pos = 0;
			}

			ret = media_set_position(pos);
			if (ret != MEDIA_OK) {
				printf("Error: media_set_position\n");
			}
		}
		break;
		case 'a': {
			if (media_is_playing()) {
				media_stop_play();
				disconnect_socket(&fd);
			}

			if (media_record_init() != RECORD_OK) {
				printf("Error: media_record_init()\n");
				return 0;
			} else {
				printf("MediaRecord Init Okay..\n");
			}

			fd = open("/mnt/record", O_TRUNC | O_CREAT | O_RDWR);
			if (fd < 0) {
				printf("Cannot create file!\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}

			if (media_record_set_config(2, 16000, 0, MEDIA_FORMAT_PCM) != RECORD_OK) {
				printf("Error: media_record_set_config()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}

			if (media_record_prepare() != RECORD_OK) {
				printf("Error: media_record_prepare()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}

			if (media_record(fd) != RECORD_OK) {
				printf("Error: media_record()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}
		}
		break;
		case 'b': {
			if (media_is_playing()) {
				media_stop_play();
				disconnect_socket(&fd);
			}

			if (media_record_init() != RECORD_OK) {
				printf("Error: media_record_init()\n");
				return 0;
			} else {
				printf("MediaRecord Init Okay..\n");
			}

			printf("Socket connecting...\n");
			fd = connect_socket("192.168.1.213", 8080);
			if (fd < 0) {
				printf("Connection failed...\n");
				dm_conn_wifi_disconnect();
				return 0;
			}
			printf("Socket connected successfully.\n");

			if (media_record_set_config(2, 16000, 0, MEDIA_FORMAT_PCM) != RECORD_OK) {
				printf("Error: media_record_set_config()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}

			if (media_record_prepare() != RECORD_OK) {
				printf("Error: media_record_prepare()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}

			if (media_record(fd) != RECORD_OK) {
				printf("Error: media_record()\n");
				DISCONNECT_WIFI_AND_CLOSE_SOCKET();
				return 0;
			}
		}
		break;
		case 'c': {
			if (media_pause_record() != RECORD_OK) {
				printf("Error: media_pause_record()\n");
			}
		}
		break;
		case 'd': {
			if (media_resume_record() != RECORD_OK) {
				printf("Error: media_resume_record()\n");
			}
		}
		break;
		case 'e': {
			if (media_stop_record() != RECORD_OK) {
				printf("Error: media_stop_record()\n");
			}
		}
		break;
		case 'f': {
			char *buffer;
			int num_read;
			int total_frames;
			unsigned int size;

			struct pcm *p_pcm = NULL;
			p_pcm = pcm_open(0, 0, PCM_OUT, NULL);
			size = pcm_frames_to_bytes(p_pcm, pcm_get_buffer_size(p_pcm));
			buffer = malloc(size);

			fd = open("/mnt/record", O_RDONLY);

			printf("Playback start!!\n");
			total_frames = 0;
			do {
				num_read = read(fd, buffer, size);
				if (num_read > 0) {
					total_frames += num_read;
					ret = pcm_writei(p_pcm, buffer, pcm_bytes_to_frames(p_pcm, num_read));
				}
			} while (num_read > 0);

			sleep(2);
			printf("Playback done! Total Frames: %d\n", pcm_bytes_to_frames(p_pcm, total_frames));

			free(buffer);
			pcm_close(p_pcm);
			close(fd);
			break;
		}
		break;
		case 'x': {
			char instruction = 0x06;
			send(fd, &instruction, 1, 0);

			if (media_is_playing()) {
				media_stop_play();
			}

			if (media_is_recording()) {
				media_stop_record();
			}

			DISCONNECT_WIFI_AND_CLOSE_SOCKET();

			return 0;
		}
		break;
		}

	}

	return 0;
}
