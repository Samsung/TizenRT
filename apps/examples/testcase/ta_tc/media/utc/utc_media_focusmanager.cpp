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

static int gain_counter, loss_counter;
class TestFocusChangeListener : public media::FocusChangeListener
{
public:
	void onFocusChange(int focusChange) override;
};

void TestFocusChangeListener::onFocusChange(int focusChange)
{
	media::FocusChangeListener::onFocusChange(focusChange);
	if (focusChange == media::FOCUS_GAIN) {
		printf("GAIN\n");
		gain_counter++;
	} else if (focusChange == media::FOCUS_LOSS) {
		printf("LOSS\n");
		loss_counter++;
	}
}

static void utc_media_FocusManager_getFocusManager_p(void)
{
	auto focusMangerAddr1 = &media::FocusManager::getFocusManager();
	auto focusMangerAddr2 = &media::FocusManager::getFocusManager();
	TC_ASSERT_EQ("utc_media_FocusManager_getFocusManager", focusMangerAddr1, focusMangerAddr2);

	TC_SUCCESS_RESULT();
}

static void utc_media_FocusManager_requestFocus_p(void)
{
	auto &focusManger = media::FocusManager::getFocusManager();
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto listener = std::make_shared<TestFocusChangeListener>();

	stream_info_t *info;
	stream_info_create(STREAM_TYPE_MEDIA, &info);
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	auto focusRequest = media::FocusRequest::Builder()
							.setStreamInfo(stream_info)
							.setFocusChangeListener(listener)
							.build();

	stream_info_t *info2;
	stream_info_create(STREAM_TYPE_EMERGENCY, &info2);
	auto stream_info2 = std::shared_ptr<stream_info_t>(info2, deleter);
	auto focusRequest2 = media::FocusRequest::Builder()
							.setStreamInfo(stream_info2)
							.setFocusChangeListener(listener)
							.build();

	gain_counter = 0;
	auto ret = focusManger.requestFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_SUCCESS);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", gain_counter, 1);

	/* reqestFocus with same focusRequest. focus change callback will not be called */
	ret = focusManger.requestFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_SUCCESS);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", gain_counter, 1);

	/* reqestFocus with a high prio focusRequest. focus change callback will be called */
	ret = focusManger.requestFocus(focusRequest2);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_SUCCESS);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", gain_counter, 2);

	/* requestFocus with a low prio focusRequest. focus change callback will not be called */
	ret = focusManger.requestFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", ret, media::FOCUS_REQUEST_SUCCESS);
	TC_ASSERT_EQ("utc_media_FocusManager_requestFocus", gain_counter, 2);

	focusManger.abandonFocus(focusRequest);
	focusManger.abandonFocus(focusRequest2);

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
	stream_info_t *info;
	stream_info_create(STREAM_TYPE_MEDIA, &info);
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	auto listener = std::make_shared<TestFocusChangeListener>();
	auto focusRequest = media::FocusRequest::Builder()
							.setStreamInfo(stream_info)
							.setFocusChangeListener(listener)
							.build();
	auto &focusManger = media::FocusManager::getFocusManager();

	loss_counter = 0;
	focusManger.requestFocus(focusRequest);
	auto ret = focusManger.abandonFocus(focusRequest);
	TC_ASSERT_EQ("utc_media_FocusRequest_abandonFocus", ret, media::FOCUS_REQUEST_SUCCESS);
	TC_ASSERT_EQ("utc_media_FocusRequest_abandonFocus", loss_counter, 1);

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
