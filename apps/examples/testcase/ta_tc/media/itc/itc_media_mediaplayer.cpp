/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <memory>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>
#include "tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define COUNT 10

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static const char *file_path = "/mnt/fileinputdatasource.raw";
static const char *test_data = "dummydata";
static int g_flag = 0;

static void SetUp(void)
{
	FILE *fp = fopen(file_path, "w");
	if (fp != NULL) {
		int ret = fputs("dummydata", fp);
		if (ret != (int)strlen("dummydata")) {
			printf("fail to fputs\n");
		}
		fclose(fp);
		g_flag = 1;
	} else {
		printf("fail to open %s, errno : %d\n", file_path, get_errno());
	}
}

static void TearDown()
{
	remove(file_path);
}

/**
* @testcase         itc_media_MediaPlayer_create_destroy_p
* @brief            create and destroy media player
* @scenario         create and destroy media player and check its validation
* @apicovered       create,destroy
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_MediaPlayer_create_destroy_p(void)
{
	media::MediaPlayer mp;
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_EQ("create", mp.create(), media::PLAYER_OK);
		TC_ASSERT_EQ("destroy", mp.destroy(), media::PLAYER_OK);
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_MediaPlayer_create_destroy_n
* @brief            create, destroy media player
* @scenario         create, recreate and destroy, re-destroy media player and check its validation
* @apicovered       create, destroy
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_MediaPlayer_create_destroy_n(void)
{
	media::MediaPlayer mp;
	TC_ASSERT_NEQ("destroy", mp.destroy(), media::PLAYER_OK);
	TC_ASSERT_EQ("create", mp.create(), media::PLAYER_OK);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_NEQ("recreate", mp.create(), media::PLAYER_OK);
	}
	TC_ASSERT_EQ("destroy", mp.destroy(), media::PLAYER_OK);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_NEQ("destroy", mp.destroy(), media::PLAYER_OK);
	}
	TC_SUCCESS_RESULT();
}

int itc_media_MediaPlayer_main(void)
{
	SetUp();
	if (g_flag) {
		itc_media_MediaPlayer_create_destroy_p();
		itc_media_MediaPlayer_create_destroy_n();
	}
	TearDown();
	return 0;
}
