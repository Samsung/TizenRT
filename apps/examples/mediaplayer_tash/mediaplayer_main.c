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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <apps/shell/tash.h>
#include "mediaplayer.h"

static void usage(char *cmd)
{
	printf("Usage: %s [option] [filename1] [filename2]\n", cmd);
	printf("\t-p: Playing\n");
	printf("\t-r: Recording\n");
	printf("\t-x: Playing 2 files(Mixing)\n");
	printf("\n");
}

static int media_player_tash_cb(int argc, char **argv)
{
	media_t *music = NULL;
	media_t *music_ = NULL;
	int option;

	if (argc < 3) {
		usage(argv[0]);
		return 0;
	}

	media_init(NULL);

	while ((option = getopt(argc, argv, "prx")) != ERROR) {
		switch (option) {
		case 'p':
			music = media_open(argv[2], MEDIA_OP_PLAYBACK, MEDIA_TYPE_PCM);
			media_play(music, false);
			printf("Playing started(with Pause/Resume) [%s]\n", argv[2]);
			sleep(5);
			printf("Pause!\n");
			media_pause(music);
			sleep(3);
			printf("Resume!\n");
			media_resume(music);
			sleep(7);
			printf("Done\n");
			media_stop(music);
			break;

		case 'r':
			music = media_open(argv[2], MEDIA_OP_RECORD, MEDIA_TYPE_PCM);
			media_record(music);
			printf("Recording 7secs started [%s]\n", argv[2]);
			sleep(7);
			printf("Done\n");
			media_stop(music);
			break;

		case 'x':
			music = media_open(argv[2], MEDIA_OP_PLAYBACK, MEDIA_TYPE_PCM);
			media_play(music, false);
			music_ = media_open(argv[3], MEDIA_OP_PLAYBACK, MEDIA_TYPE_PCM);
			media_play(music_, false);
			sleep(6);
			printf("Done\n");
			media_stop(music);
			media_stop(music_);
			media_close(music_);
			break;

		default:
			usage(argv[0]);
			return 0;
			break;
		}
	}

	media_close(music);
	media_shutdown();

	return 0;
}

/****************************************************************************
 * mediaplayer_tash_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mediaplayer_tash_main(int argc, char **argv)
#endif
{
	tash_cmd_install("mediaplayer_tash", media_player_tash_cb, TASH_EXECMD_ASYNC);

	return 0;
}
