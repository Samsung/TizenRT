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

static void utc_media_FocusRequest_Builder_build_p(void)
{
	{
		auto ret = media::FocusRequest::Builder()
					   .setFocusChangeListener(nullptr)
					   .build();
		TC_ASSERT_NEQ("utc_media_FocusRequest_Builder_build", ret.get(), nullptr);
	}
	{
		auto listener = std::make_shared<EmptyFocusChangeListener>();
		auto ret = media::FocusRequest::Builder()
					   .setFocusChangeListener(listener)
					   .build();
		TC_ASSERT_NEQ("utc_media_FocusRequest_Builder_build", ret.get(), nullptr);
	}
	TC_SUCCESS_RESULT();
}

static void utc_media_FocusRequest_getId_p(void)
{
	{
		auto focusRequest = media::FocusRequest::Builder()
								.setFocusChangeListener(nullptr)
								.build();
		auto ret1 = focusRequest->getId();
		auto ret2 = focusRequest->getId();
		TC_ASSERT_EQ("utc_media_FocusRequest_getId", ret1.compare(ret2), 0);
	}
	{
		auto focusRequest1 = media::FocusRequest::Builder()
								 .setFocusChangeListener(nullptr)
								 .build();
		auto focusRequest2 = media::FocusRequest::Builder()
								 .setFocusChangeListener(nullptr)
								 .build();
		auto ret1 = focusRequest1->getId();
		auto ret2 = focusRequest2->getId();
		TC_ASSERT_NEQ("utc_media_FocusRequest_getId", ret1.compare(ret2), 0);
	}
	{
		auto listener = std::make_shared<EmptyFocusChangeListener>();
		auto focusRequest1 = media::FocusRequest::Builder()
								 .setFocusChangeListener(listener)
								 .build();
		auto focusRequest2 = media::FocusRequest::Builder()
								 .setFocusChangeListener(listener)
								 .build();
		auto ret1 = focusRequest1->getId();
		auto ret2 = focusRequest2->getId();
		TC_ASSERT_NEQ("utc_media_FocusRequest_getId", ret1.compare(ret2), 0);
	}
	TC_SUCCESS_RESULT();
}

static void utc_media_FocusRequest_getListener_p(void)
{
	auto listener = std::make_shared<EmptyFocusChangeListener>();
	auto focusRequest = media::FocusRequest::Builder()
							.setFocusChangeListener(listener)
							.build();
	auto ret = focusRequest->getListener();
	TC_ASSERT_EQ("utc_media_FocusRequest_getListener", ret.get(), listener.get());
	TC_SUCCESS_RESULT();
}

int utc_media_FocusRequest_main(void)
{
	utc_media_FocusRequest_Builder_setFocusChangeListener_p();
	utc_media_FocusRequest_Builder_build_p();
	utc_media_FocusRequest_getId_p();
	utc_media_FocusRequest_getListener_p();
	return 0;
}
