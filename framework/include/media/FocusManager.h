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
 * @file media/FocusManager.h
 * @brief Media FocusManager APIs
 */

#ifndef __MEDIA_FOCUSMANAGER_H
#define __MEDIA_FOCUSMANAGER_H

#include <list>
#include <memory>
#include <mutex>
#include <string>

#include <media/FocusRequest.h>

namespace media {

static const int FOCUS_REQUEST_SUCCESS = 0;
static const int FOCUS_REQUEST_FAIL = -1;

class FocusManager
{
public:
	static FocusManager &getFocusManager();
	int abandonFocus(std::shared_ptr<FocusRequest> focusRequest);
	int requestFocus(std::shared_ptr<FocusRequest> focusRequest);

private:
	class FocusRequester
	{
	public:
		FocusRequester(std::string id, std::shared_ptr<FocusChangeListener> listener);
		bool hasSameId(std::string id);
		void notify(int focusChange);

	private:
		std::string mId;
		std::shared_ptr<FocusChangeListener> mListener;
	};

	FocusManager() = default;
	virtual ~FocusManager() = default;
	void removeFocusElement(std::string id);
	std::list<std::shared_ptr<FocusRequester>> mFocusList;
	std::mutex mFocusLock;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
