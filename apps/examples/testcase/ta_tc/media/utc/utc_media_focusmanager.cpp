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

#include <media/FocusRequest.h>
#include <media/FocusManager.h>
#include "tc_common.h"

static void utc_media_FocusManager_getFocusManager_p(void)
{
	auto focusMangerAddr1 = &media::FocusManager::getFocusManager();
	auto focusMangerAddr2 = &media::FocusManager::getFocusManager();
	TC_ASSERT_EQ("utc_media_FocusManager_getFocusManager", focusMangerAddr1, focusMangerAddr2);

	TC_SUCCESS_RESULT();
}

static void utc_media_FocusManager_requestFocus_p(void)
{
	auto focusRequest = media::FocusRequest::Builder().build();
	auto &focusManger = media::FocusManager::getFocusManager();
	auto ret = focusManger.requestFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_SUCCESS);

	focusManger.abandonFocus(focusRequest);

	TC_SUCCESS_RESULT();
}

static void utc_media_FocusManager_requestFocus_n(void)
{
	auto &focusManger = media::FocusManager::getFocusManager();
	auto ret = focusManger.requestFocus(nullptr);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_FAIL);

	TC_SUCCESS_RESULT();
}

static void utc_media_FocusManager_abandonFocus_p(void)
{
	auto focusRequest = media::FocusRequest::Builder().build();
	auto &focusManger = media::FocusManager::getFocusManager();
	focusManger.requestFocus(focusRequest);
	auto ret = focusManger.abandonFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusRequest_abandonFocus", ret, media::FOCUS_REQUEST_SUCCESS);

	TC_SUCCESS_RESULT();
}

static void utc_media_FocusManager_abandonFocus_n(void)
{
	auto &focusManger = media::FocusManager::getFocusManager();
	auto ret = focusManger.abandonFocus(nullptr);
	TC_ASSERT_EQ("utc_media_FocusRequest_abandonFocus", ret, media::FOCUS_REQUEST_FAIL);

	TC_SUCCESS_RESULT();
}

int utc_media_FocusManager_main(void)
{
	utc_media_FocusManager_getFocusManager_p();
	utc_media_FocusManager_requestFocus_p();
	utc_media_FocusManager_requestFocus_n();
	utc_media_FocusManager_abandonFocus_p();
	utc_media_FocusManager_abandonFocus_n();
	return 0;
}
