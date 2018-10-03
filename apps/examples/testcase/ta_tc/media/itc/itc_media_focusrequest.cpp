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

bool focus_checker;

class EmptyFocusChangeListener : public media::FocusChangeListener
{
	public:
		void onFocusChange(int focusChange) override;
};

void EmptyFocusChangeListener::onFocusChange(int focusChange)
{
	focus_checker = true;
}

static void itc_media_FocusRequest_getId_p(void)
{
	auto listener1 = std::make_shared<EmptyFocusChangeListener>();
	auto listener2 = std::make_shared<EmptyFocusChangeListener>();

	auto focusRequest1 = media::FocusRequest::Builder().setFocusChangeListener(listener1).build();
	auto focusRequest2 = media::FocusRequest::Builder().setFocusChangeListener(listener2).build();

	auto ret1 = focusRequest1->getId();
	auto ret2 = focusRequest2->getId();

	TC_ASSERT_NEQ("itc_media_FocusRequest_getId", ret1.compare(ret2), 0);

	listener1 = std::make_shared<EmptyFocusChangeListener>();
	listener2 = std::make_shared<EmptyFocusChangeListener>();

	auto focusRequestBuilder1 = media::FocusRequest::Builder().setFocusChangeListener(listener1);
	focusRequestBuilder1 = focusRequestBuilder1.setFocusChangeListener(listener2);

	focusRequest1 = focusRequestBuilder1.build();
	focusRequest2 = media::FocusRequest::Builder().setFocusChangeListener(listener2).build();

	ret1 = focusRequest1->getId();
	ret2 = focusRequest2->getId();

	TC_ASSERT_NEQ("itc_media_FocusRequest_getId", ret1.compare(ret2), 0);

	TC_SUCCESS_RESULT();
}

static void itc_media_FocusRequest_getListener_p(void)
{
	auto focusRequestBuilder1 = media::FocusRequest::Builder();
	auto focusRequestBuilder2 = media::FocusRequest::Builder();

	auto listener1 = std::make_shared<EmptyFocusChangeListener>();
	auto listener2 = std::make_shared<EmptyFocusChangeListener>();

	focusRequestBuilder1 = focusRequestBuilder1.setFocusChangeListener(listener1);
	auto focusRequest1  = focusRequestBuilder1.build();
	focusRequestBuilder2 = focusRequestBuilder2.setFocusChangeListener(listener2);
	auto focusRequest2  = focusRequestBuilder2.build();
	focusRequestBuilder1 = focusRequestBuilder1.setFocusChangeListener(listener2);
	focusRequest1  = focusRequestBuilder1.build();

	auto ret1 = focusRequest1->getListener();
	auto ret2 = focusRequest2->getListener();
	TC_ASSERT_EQ("itc_media_FocusRequest_getListener", ret1.get(), ret2.get());

	focusRequestBuilder1 = media::FocusRequest::Builder();
	focusRequestBuilder2 = media::FocusRequest::Builder();
	listener1 = std::make_shared<EmptyFocusChangeListener>();
	listener2 = std::make_shared<EmptyFocusChangeListener>();
	focusRequestBuilder1 = focusRequestBuilder1.setFocusChangeListener(listener1);
	focusRequestBuilder2 = focusRequestBuilder2.setFocusChangeListener(listener2);

	focusRequest2  = focusRequestBuilder2.build();
	focusRequestBuilder1 = focusRequestBuilder1.setFocusChangeListener(listener2);
	focusRequest1  = focusRequestBuilder1.build();
	ret1 = focusRequest1->getListener();
	ret2 = focusRequest2->getListener();
	TC_ASSERT_EQ("itc_media_FocusRequest_getListener", ret1.get(), ret2.get());

	TC_SUCCESS_RESULT();
}

int itc_media_FocusRequest_main(void)
{
	focus_checker = false;
	itc_media_FocusRequest_getId_p();
	itc_media_FocusRequest_getListener_p();
	return 0;
}
