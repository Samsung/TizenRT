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
//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <tinyara/init.h>
#include <stdio.h>
#include <apps/platform/cxxinitialize.h>
#include <errno.h>
#include <iostream>
#include "tc_common.h"

#ifdef CONFIG_GMOCK
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gmock/utc_media_fileoutputdatasource.cpp"
#include "gmock/utc_media_recorder.cpp"
#include "gmock/utc_media_fileinputdatasource.cpp"
#include "gmock/utc_media_player.cpp"

int gtest_run(int *argc, char **argv)
{
	testing::InitGoogleTest(argc, argv);	
	return RUN_ALL_TESTS();
}

#else
int utc_media_FocusRequest_main(void);
int utc_media_FocusManager_main(void);
#ifdef CONFIG_MEDIA_PLAYER
int utc_media_MediaPlayer_main(void);
int utc_media_FileInputDataSource_main(void);
#endif
#ifdef CONFIG_MEDIA_RECORDER
int utc_media_mediarecorder_main(void);
int utc_media_fileoutputdatasource_main(void);
int utc_media_bufferoutputdatasource_main(void);
#endif
#ifdef CONFIG_MEDIA_VOICE_SPEECH_DETECTOR
int utc_media_SpeechDetector_main(void);
#endif
#endif

extern "C"
{
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_media_main(int argc, char *argv[])
#endif
{
	up_cxxinitialize();
	
	if (tc_handler(TC_START, "Media UTC") == -1) {
		return -1;
	}

#ifdef CONFIG_GMOCK
	gtest_run(&argc, argv);
#else
	utc_media_FocusRequest_main();
	utc_media_FocusManager_main();
#ifdef CONFIG_MEDIA_PLAYER
	utc_media_MediaPlayer_main();
	utc_media_FileInputDataSource_main();
#endif
#ifdef CONFIG_MEDIA_RECORDER
	utc_media_mediarecorder_main();
	utc_media_fileoutputdatasource_main();
	utc_media_bufferoutputdatasource_main();
	unlink("/tmp/record");
#endif
#ifdef CONFIG_MEDIA_VOICE_SPEECH_DETECTOR
	utc_media_SpeechDetector_main();
#endif
#endif

	(void)tc_handler(TC_END, "Media UTC");
	return 0;
}
}
