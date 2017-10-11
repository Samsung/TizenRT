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

static int media_player_tash_cb(int argc, char **args)
{
	media_t *music;

	if (argc < 3) {
		printf("Usage: %s [option] [filename]\n", args[0]);
		printf(" -p: Playing\n");
		printf(" -r: Recording\n");
		printf("\n");
		return 0;
	}

	if (args[1][1] == 'p') {
		media_init(NULL);
		music = media_open(args[2], MEDIA_OP_PLAYBACK, MEDIA_TYPE_PCM);
		media_play(music, false);
		printf("Playing started(with Pause/Resume) [%s]\n", args[2]);
		sleep(5);
		printf("Pause!\n");
		media_pause(music);
		sleep(3);
		printf("Resume!\n");
		media_resume(music);
		sleep(7);
		printf("Done\n");
		media_stop(music);
	} else {
		media_init((struct media_ops_s *)1);
		music = media_open(args[2], MEDIA_OP_RECORD, MEDIA_TYPE_PCM);
		media_record(music);
		printf("Recording 10secs started [%s]\n", args[2]);
		sleep(10);
		printf("Done\n");
		media_stop(music);
	}

	printf("Done\n");
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
int mediaplayer_tash_main(int argc, char **args)
#endif
{
	tash_cmd_install("mediaplayer_tash", media_player_tash_cb, TASH_EXECMD_ASYNC);

	return 0;
}
