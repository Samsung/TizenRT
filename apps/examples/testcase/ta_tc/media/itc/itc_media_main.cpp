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

#ifdef CONFIG_MEDIA_PLAYER
int itc_media_MediaPlayer_main(void);
int itc_media_FileInputDataSource_main(void);
#endif
#ifdef CONFIG_MEDIA_RECORDER
int itc_media_mediarecorder_main(void);
int itc_media_fileoutputdatasource_main(void);
int itc_media_bufferoutputdatasource_main(void);
int itc_media_FocusRequest_main(void);
int itc_media_FocusManager_main(void);
#endif

extern "C"
{
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_media_main(int argc, char *argv[])
#endif
{
	up_cxxinitialize();
	
	if (tc_handler(TC_START, "Media ITC") == -1) {
		return -1;
	}

#ifdef CONFIG_MEDIA_PLAYER
	itc_media_MediaPlayer_main();
	itc_media_FileInputDataSource_main();
#endif
#ifdef CONFIG_MEDIA_RECORDER
	itc_media_mediarecorder_main();
	itc_media_fileoutputdatasource_main();
	itc_media_FocusRequest_main();
	itc_media_FocusManager_main();
	itc_media_bufferoutputdatasource_main();
#endif

	(void)tc_handler(TC_END, "Media ITC");
	return 0;
}
}
