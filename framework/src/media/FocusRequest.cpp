/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <sstream>
#include <media/FocusRequest.h>

namespace media {
std::string FocusRequest::getId()
{
	return mId;
}

std::shared_ptr<FocusChangeListener> FocusRequest::getListener()
{
	return mListener;
}

FocusRequest::Builder::Builder() : mId(""), mListener(nullptr)
{
}

FocusRequest::Builder &FocusRequest::Builder::setFocusChangeListener(std::shared_ptr<FocusChangeListener> listener)
{
	mListener = listener;
	return *this;
}

std::shared_ptr<FocusRequest> FocusRequest::Builder::build()
{
	auto focusRequest = std::make_shared<FocusRequest>();
	std::stringstream ss;
	ss << static_cast<const void *>(focusRequest.get());
	ss << static_cast<const void *>(mListener.get());
	mId = ss.str();
	focusRequest.get()->mId = mId;
	focusRequest.get()->mListener = mListener;
	return focusRequest;
}
} // namespace media
