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
#include "tc_common.h"

class EmptyFocusChangeListener : public media::FocusChangeListener
{
public:
	void onFocusChange(int focusChange) override;
};

void EmptyFocusChangeListener::onFocusChange(int focusChange)
{
}

static void utc_media_FocusRequest_Builder_setFocusChangeListener_p(void)
{
	{
		auto focusRequestBuilder = media::FocusRequest::Builder();
		auto ret1 = &focusRequestBuilder;
		auto ret2 = &focusRequestBuilder.setFocusChangeListener(nullptr);
		TC_ASSERT_EQ("utc_media_FocusRequest_Builder_setFocusChangeListener", ret1, ret2);
	}
	{
		auto listener = std::make_shared<EmptyFocusChangeListener>();
		auto focusRequestBuilder = media::FocusRequest::Builder();
		auto ret1 = &focusRequestBuilder;
		auto ret2 = &focusRequestBuilder.setFocusChangeListener(listener);
		TC_ASSERT_EQ("utc_media_FocusRequest_Builder_setFocusChangeListener", ret1, ret2);
	}
	TC_SUCCESS_RESULT();
}

static void utc_media_FocusRequest_Builder_setStreamInfo_p(void)
{
	stream_info_t *info;
	stream_info_create(STREAM_TYPE_MEDIA, &info);
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);

	auto focusRequestBuilder = media::FocusRequest::Builder();
	auto ret1 = &focusRequestBuilder;
	auto ret2 = &focusRequestBuilder.setStreamInfo(stream_info);
	TC_ASSERT_EQ("utc_media_FocusRequest_Builder_setStreamInfo", ret1, ret2);
	TC_SUCCESS_RESULT();
}

static void utc_media_FocusRequest_Builder_build_p(void)
{
	stream_info_t *info;
	stream_info_create(STREAM_TYPE_MEDIA, &info);
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	auto listener = std::make_shared<EmptyFocusChangeListener>();
	auto ret = media::FocusRequest::Builder()
				   .setStreamInfo(stream_info)
				   .setFocusChangeListener(listener)
				   .build();
	TC_ASSERT_NEQ("utc_media_FocusRequest_Builder_build", ret.get(), nullptr);
	TC_SUCCESS_RESULT();
}

static void utc_media_FocusRequest_Builder_build_n(void)
{
	auto ret = media::FocusRequest::Builder().build();
	TC_ASSERT_EQ("utc_media_FocusRequest_Builder_build", ret.get(), nullptr);
	TC_SUCCESS_RESULT();
}

int utc_media_FocusRequest_main(void)
{
	utc_media_FocusRequest_Builder_setStreamInfo_p();
	utc_media_FocusRequest_Builder_setFocusChangeListener_p();
	utc_media_FocusRequest_Builder_build_p();
	utc_media_FocusRequest_Builder_build_n();
	return 0;
}
