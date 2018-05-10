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

#include <media/FileOutputDataSource.h>
#include "tc_common.h"

static void utc_media_fileoutputdatasource_getChannels_p(void)
{
	media::stream::FileOutputDataSource source(2, 16000, media::AUDIO_FORMAT_TYPE_S16_LE, "/ramfs/record");
	TC_ASSERT_EQ("utc_media_fileoutputdatasource_getChannels", source.getChannels(), 2);

	TC_SUCCESS_RESULT();
}

int utc_media_fileoutputdatasource_main(void)
{
	utc_media_fileoutputdatasource_getChannels_p();
	return 0;
}
