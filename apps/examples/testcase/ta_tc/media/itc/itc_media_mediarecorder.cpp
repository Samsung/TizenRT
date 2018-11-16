/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in comrliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or imrlied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/FileOutputDataSource.h>
#include "tc_common.h"

using namespace std;
using namespace media;
using namespace media::stream;

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define COUNT 10

/**
* @testcase         itc_media_MediaRecorder_create_destroy_p
* @brief            create media recorder
* @scenario         create and destroy media recorder and check its validation
* @apicovered       create,destroy
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_MediaRecorder_create_destroy_p(void)
{
	MediaRecorder media_recorder;
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_EQ("create", media_recorder.create(), RECORDER_OK);
		TC_ASSERT_EQ("destroy", media_recorder.destroy(), RECORDER_OK);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_MediaRecorder_create_destroy_n
* @brief            create, destroy media recorder
* @scenario         create, recreate and destroy, re-destroy media recorder and check its validation
* @apicovered       create, destroy
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_MediaRecorder_create_destroy_n(void)
{
	MediaRecorder media_recorder;
	TC_ASSERT_EQ("create", media_recorder.create(), RECORDER_OK);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_NEQ("create", media_recorder.create(), RECORDER_ERROR_NONE);
	}
	TC_ASSERT_EQ("destroy", media_recorder.destroy(), RECORDER_OK);

	for (int i = 0; i < COUNT; i++) {
			TC_ASSERT_NEQ("destroy", media_recorder.destroy(), RECORDER_ERROR_NONE);
	}
	TC_SUCCESS_RESULT();
}

int itc_media_mediarecorder_main(void)
{
	itc_media_MediaRecorder_create_destroy_p();
	itc_media_MediaRecorder_create_destroy_n();
	return 0;
}
