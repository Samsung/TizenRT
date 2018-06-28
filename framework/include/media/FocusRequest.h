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

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/FocusRequest.h
 * @brief Media FocusRequest APIs
 */

#ifndef __MEDIA_FOCUSREQUEST_H
#define __MEDIA_FOCUSREQUEST_H

#include <memory>
#include <string>
#include <media/FocusChangeListener.h>

namespace media {
class FocusRequest
{
public:
	class Builder
	{
	public:
		Builder();
		Builder &setFocusChangeListener(std::shared_ptr<FocusChangeListener> listener);
		std::shared_ptr<FocusRequest> build();

	private:
		std::string mId;
		std::shared_ptr<FocusChangeListener> mListener;
	};

	FocusRequest(std::string id, std::shared_ptr<FocusChangeListener> listener);
	std::string getId();
	std::shared_ptr<FocusChangeListener> getListener();

private:
	std::string mId;
	std::shared_ptr<FocusChangeListener> mListener;
};
} // namespace media
#endif
/** @} */ // end of MEDIA group
