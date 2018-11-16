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

static void itc_media_FocusManager_getFocusManager_p(void)
{
	auto focusRequest = media::FocusRequest::Builder().build();
	auto focusMangerAddr1 = &media::FocusManager::getFocusManager();

	auto ret1 = focusMangerAddr1->requestFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusManager_requestFocus", ret1, media::FOCUS_REQUEST_SUCCESS);

	auto focusMangerAddr2 = &media::FocusManager::getFocusManager();
	TC_ASSERT_EQ("itc_media_FocusManager_getFocusManager", focusMangerAddr1, focusMangerAddr2);

	auto ret2 = focusMangerAddr1->abandonFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusRequest_abandonFocus", ret2, media::FOCUS_REQUEST_SUCCESS);

	auto focusMangerAddr3 = &media::FocusManager::getFocusManager();
	TC_ASSERT_EQ("itc_media_FocusManager_getFocusManager", focusMangerAddr1, focusMangerAddr3);

	auto ret3 = focusMangerAddr1->requestFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusManager_requestFocus", ret3, media::FOCUS_REQUEST_SUCCESS);

	auto focusMangerAddr4 = &media::FocusManager::getFocusManager();
	TC_ASSERT_EQ("itc_media_FocusManager_getFocusManager", focusMangerAddr1, focusMangerAddr4);

	TC_SUCCESS_RESULT();
}

static void itc_media_FocusManager_requestFocus_p(void)
{
	auto focusRequest = media::FocusRequest::Builder().build();
	auto &focusManger = media::FocusManager::getFocusManager();

	auto ret1 = focusManger.abandonFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusRequest_abandonFocus", ret1, 0);

	auto ret2 = focusManger.requestFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusManager_requestFocus", ret2, media::FOCUS_REQUEST_SUCCESS);

	auto ret3 = focusManger.abandonFocus(focusRequest);
	TC_ASSERT_EQ("itc_media_FocusRequest_abandonFocus", ret3, media::FOCUS_REQUEST_SUCCESS);

	TC_SUCCESS_RESULT();
}

int itc_media_FocusManager_main(void)
{
	itc_media_FocusManager_getFocusManager_p();
	itc_media_FocusManager_requestFocus_p();

	return 0;
}
