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

#include <media/MediaPlayer.h>
#include "tc_common.h"

static void utc_media_mediaplayer_create_p(void)
{
	media::MediaPlayer mp;
	TC_ASSERT_EQ("utc_media_mediaplayer_create", mp.create(), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_mediaplayer_create_n(void)
{
	media::MediaPlayer mp;
	mp.create();

	TC_ASSERT_EQ("utc_media_mediaplayer_create", mp.create(), media::PLAYER_ERROR);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

int utc_media_mediaplayer_main(void)
{
	utc_media_mediaplayer_create_p();
	utc_media_mediaplayer_create_n();
	return 0;
}
